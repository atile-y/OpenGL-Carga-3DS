#include <windows.h>
#include "CGL.h"
#include "CObjeto.h"
#include "CTextura.h"
#include "CEscena.h"
#include "C3DS.h"
#include "Extras.h"

CGL      *ObjetoGL;
CEscena  *Escena;

bool bTeclas[256];
bool bActiva;

float fDesz = -3.0;
float fRotx, fRoty;

LRESULT CALLBACK GestorMensajes(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
   switch(msg)
   {
      case WM_ACTIVATE:
         if( !HIWORD(wParam) )
            bActiva = TRUE;
         else
            bActiva = FALSE;
         return 0;

      case WM_SYSCOMMAND:
         switch(wParam)
         {
            case SC_SCREENSAVE:
            case SC_MONITORPOWER:
               return 0;
         }
         break;

      case WM_CLOSE:
         PostQuitMessage(0);
         return 0;

      case WM_KEYDOWN:
         bTeclas[wParam] = TRUE;
         return 0;

      case WM_KEYUP:
         bTeclas[wParam] = FALSE;
         return 0;

      case WM_SIZE:
         ObjetoGL->InicializaEscenaGL( LOWORD(lParam), HIWORD(lParam) );
   }

   return(DefWindowProc(hwnd, msg, wParam, lParam));
}

int Renderiza(GLvoid)
{
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   glLoadIdentity();

   glTranslatef( 0.0, 0.0, fDesz-260 );
   glRotatef( fRotx, 1.0, 0.0, 0.0 );
   glRotatef( fRoty, 0.0, 1.0, 0.0 );

   for(int i=0;i<Escena->m_pListaObjetos->m_nNumElementos;i++)
   {
      if( Escena->m_pListaMateriales->m_nNumElementos > 0
       && Escena->m_pListaObjetos->Obtener(i)->m_nIDMaterial != Escena->m_nNumMateriales+10
       && Escena->m_pListaMateriales->Obtener(Escena->m_pListaObjetos->Obtener(i)->m_nIDMaterial)->m_bTex )
         Escena->m_pListaMateriales->Obtener(Escena->m_pListaObjetos->Obtener(i)->m_nIDMaterial)->m_pTextura->Usa();
      glBegin(GL_TRIANGLES);
         for(int j=0;j<Escena->m_pListaObjetos->Obtener(i)->m_pListaCaras->m_nNumElementos;j++)
         {
            CCara *cara = Escena->m_pListaObjetos->Obtener(i)->m_pListaCaras->Obtener(j);
            for(int k=0;k<3;k++)
            {
               CVertice *vertice = Escena->m_pListaObjetos->Obtener(i)->m_pListaVertices->Obtener(cara->m_Vertices[k]);

               glTexCoord2f( vertice->m_fU, vertice->m_fV );
               glVertex3f(vertice->m_cPunto.m_fX, vertice->m_cPunto.m_fY, vertice->m_cPunto.m_fZ);
            }
         }
      glEnd();
   }

   FPS(ObjetoGL->ObtHWnd());
   return TRUE;
}

void Control_Entrada()
{
   if( bTeclas[VK_LEFT] )
      fRoty -= 0.5;

   if( bTeclas[VK_RIGHT] )
      fRoty += 0.5;

   if( bTeclas[VK_UP] )
      fRotx += 0.5;

   if( bTeclas[VK_DOWN] )
      fRotx -= 0.5;

   if( bTeclas[VK_PRIOR] )
      fDesz += 0.5;

   if( bTeclas[VK_NEXT] )
      fDesz -= 0.5;

   if( bTeclas['A'] )
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   if( bTeclas['S'] )
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

bool Inicializa()
{
   bool bPCompleta = true;

   if( MessageBox(NULL, "¿Ejecutar en modo pantalla completa?", "Fullscreen?", MB_YESNO) == IDNO )
      bPCompleta = false;

   ObjetoGL = new CGL;
   if( !ObjetoGL->CreaVentanaGL("Carga archivos 3DS", 640, 480, 16, bPCompleta) )
      return 0;

   Escena = new CEscena;
   C3DS *f3DSaux = new C3DS;
   bool ret = f3DSaux->Cargar("mapa.3ds", Escena);
   delete f3DSaux;

   return ret;
}

void Destruir()
{
   ObjetoGL->EliminaVentanaGL();
   delete ObjetoGL;

   delete Escena;
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow)
{
   MSG  msg;
   BOOL bAplicacionFinalizada = false;

   if( !Inicializa() )
      return 0;

   while( !bAplicacionFinalizada )
   {
      if( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
         if( msg.message == WM_QUIT )
            bAplicacionFinalizada = TRUE;
         else
         {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
         }
      else
         if( bActiva && bTeclas[VK_ESCAPE] )
            bAplicacionFinalizada = TRUE;
         else
         {
            Renderiza();
            SwapBuffers( ObjetoGL->ObtHDC() );
            Control_Entrada();
         }
   }

   Destruir();
   return (msg.wParam);
}
