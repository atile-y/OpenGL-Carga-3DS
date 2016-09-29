#ifndef _CGL_H_
#define _CGL_H_

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

extern LRESULT CALLBACK GestorMensajes(HWND, UINT,	WPARAM, LPARAM);

class CGL
{
   public:
              CGL();
      virtual ~CGL();
      BOOL    CreaVentanaGL(char* szTitulo, int nAncho, int nAlto, int nBits, bool bPantallaCompleta);
      GLvoid  InicializaEscenaGL(GLsizei ancho, GLsizei alto);
      GLvoid  EliminaVentanaGL(GLvoid);

      void      PonPantallaCompleta(bool bPCompleta){ m_bPantallaCompleta = bPCompleta; };
      HDC       ObtHDC(){ return m_hDC; };
      HGLRC     ObtHRC(){ return m_hRC; };
      HWND      ObtHWnd(){ return m_hWnd; };
      HINSTANCE ObtHInstance(){ return m_hInstance; };
      bool      ObtPantallaCompleta(){ return m_bPantallaCompleta; };

   private:
      int InicializaGL(GLvoid);

      bool      m_bPantallaCompleta;
      HDC       m_hDC;
      HGLRC     m_hRC;
      HWND      m_hWnd;
      HINSTANCE m_hInstance;
};

#endif
