#include "CGL.h"

CGL::CGL()
{
   m_hDC = NULL;
   m_hRC = NULL;
   m_hWnd = NULL;
}

CGL::~CGL()
{
}

GLvoid CGL::InicializaEscenaGL(GLsizei ancho, GLsizei alto)
{
   if( alto == 0 )
      alto = 1;

   glViewport( 0, 0, ancho, alto );

   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();

   gluPerspective( 45.0f, (GLfloat)ancho/(GLfloat)alto, 0.1f, 2000.0f );

   glMatrixMode( GL_MODELVIEW );
   glLoadIdentity();
}

int CGL::InicializaGL(GLvoid)
{
   glEnable(GL_COLOR_MATERIAL);
   glShadeModel(GL_SMOOTH);
   glClearDepth(1.0f);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL);
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
   glEnable(GL_TEXTURE_2D);
   glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   return TRUE;
}

GLvoid CGL::EliminaVentanaGL(GLvoid)
{
   if( m_bPantallaCompleta )
   {
      ChangeDisplaySettings( NULL, 0 );
      ShowCursor(TRUE);
   }

   if( m_hRC )
   {
      if( !wglMakeCurrent( NULL, NULL ) )
         MessageBox(NULL, "Fallo al eliminar el DC y el RC.", "Error", MB_OK);
      if( !wglDeleteContext(m_hRC) )
         MessageBox(NULL, "Fallo al eliminar el RC.", "Error", MB_OK);
      m_hRC = NULL;
   }

   if( m_hDC && !ReleaseDC(m_hWnd, m_hDC) )
   {
      MessageBox(NULL, "Fallo al eliminar el DC.", "Error", MB_OK);
      m_hDC = NULL;
   }

   if( m_hWnd && !DestroyWindow(m_hWnd) )
   {
      MessageBox(NULL, "No podemos eliminar la ventana.", "Error", MB_OK);
      m_hWnd = NULL;
   }

   if( !UnregisterClass("OpenGL", m_hInstance) )
   {
      MessageBox(NULL, "No podemos quitar del registro a la clase.", "Error", MB_OK);
      m_hInstance = NULL;
   }
}

BOOL CGL::CreaVentanaGL(char* Titulo, int ancho, int alto, int bits, bool fullscreen)
{
   GLuint   PixelFormat;
   WNDCLASS wc;
   DWORD    dwExStyle;
   DWORD    dwStyle;
   RECT     WindowRect;

   WindowRect.left = (long)0;
   WindowRect.right = (long)ancho;
   WindowRect.top = (long)0;
   WindowRect.bottom = (long)alto;

   m_bPantallaCompleta = fullscreen;
   m_hInstance = GetModuleHandle(NULL);

   wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
   wc.lpfnWndProc   = (WNDPROC) GestorMensajes;
   wc.cbClsExtra    = 0;
   wc.cbWndExtra    = 0;
   wc.hInstance     = m_hInstance;
   wc.hIcon         = LoadIcon(NULL, IDI_WINLOGO);
   wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
   wc.hbrBackground = NULL;
   wc.lpszMenuName  = NULL;
   wc.lpszClassName = "OpenGL";

   if( !RegisterClass(&wc) )
   {
      MessageBox(NULL, "Fallo al registrar la clase de la ventana.", "Error", MB_OK);
      return FALSE;
   }

   if( fullscreen )
   {
      DEVMODE dmScreenSettings;
      memset( &dmScreenSettings, 0, sizeof(dmScreenSettings) );
      dmScreenSettings.dmSize = sizeof(dmScreenSettings);
      dmScreenSettings.dmPelsWidth = ancho;
      dmScreenSettings.dmPelsHeight = alto;
      dmScreenSettings.dmBitsPerPel = bits;
      dmScreenSettings.dmFields = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

      if( ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL )
      {
         if( MessageBox(NULL, "El modo pantalla completa no es soportado por\n\
                               Su tarjeta gráfica. ¿Desea usar modo ventana?",
                               "¿Ventana?", MB_YESNO) == IDYES )
            fullscreen = FALSE;
         else
         {
            MessageBox(NULL, "El programa se cerrará.", "Error", MB_OK);
            return FALSE;
         }
      }
   }

   if( fullscreen )
   {
      dwExStyle = WS_EX_APPWINDOW;
      dwStyle = WS_POPUP;
      ShowCursor(FALSE);
   }
   else
   {
      dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
      dwStyle = WS_OVERLAPPEDWINDOW;
   }

   AdjustWindowRectEx( &WindowRect, dwStyle, FALSE, dwExStyle );

   if( !(m_hWnd = CreateWindowEx( dwExStyle,
                                  "OpenGL",
                                  Titulo,
                                  dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                                  fullscreen?0:100, fullscreen?0:100,
                                  WindowRect.right-WindowRect.left,
                                  WindowRect.bottom-WindowRect.top,
                                  NULL,
                                  NULL,
                                  m_hInstance,
                                  NULL )) )
   {
      EliminaVentanaGL();
      MessageBox(NULL, "Error en la creación de la ventana.", "Error", MB_OK);
      return FALSE;
   }

   static PIXELFORMATDESCRIPTOR pfd=
   {
      sizeof(PIXELFORMATDESCRIPTOR),
      1,
      PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
      PFD_TYPE_RGBA,
      bits,
      0, 0, 0, 0, 0, 0,
      0,
      0,
      0,
      0, 0, 0, 0,
      16,
      0,
      0,
      PFD_MAIN_PLANE,
      0,
      0, 0, 0
   };

   if( !(m_hDC = GetDC(m_hWnd)) )
   {
      EliminaVentanaGL();
      MessageBox(NULL, "No podemos crear el GL DC.", "Error", MB_OK);
      return FALSE;
   }

   if( !(PixelFormat = ChoosePixelFormat(m_hDC, &pfd)) )
   {
      EliminaVentanaGL();
      MessageBox(NULL, "No podemos encontrar un formato de pixel aceptable.", "Error", MB_OK);
      return FALSE;
   }

   if( !SetPixelFormat(m_hDC, PixelFormat, &pfd) )
   {
      EliminaVentanaGL();
      MessageBox(NULL, "No se puede establecer el formate del pixel.", "Error", MB_OK);
      return FALSE;
   }

   if( !(m_hRC = wglCreateContext(m_hDC)) )
   {
      EliminaVentanaGL();
      MessageBox(NULL, "No se puede crear el GL RC.", "Error", MB_OK);
      return FALSE;
   }

   if( !wglMakeCurrent(m_hDC, m_hRC) )
   {
      EliminaVentanaGL();
      MessageBox(NULL, "No se puede activar el GL RC.", "Error", MB_OK);
      return FALSE;
   }

   ShowWindow( m_hWnd, SW_SHOW );
   SetForegroundWindow( m_hWnd );
   SetFocus( m_hWnd );
   InicializaEscenaGL( ancho, alto );

   if( !InicializaGL() )
   {
      EliminaVentanaGL();
      MessageBox(NULL, "Fallo en la inicializacion.", "Error", MB_OK);
      return FALSE;
   }

   return TRUE;
}
