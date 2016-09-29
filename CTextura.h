#ifndef __CTEXTURA_H_
#define __CTEXTURA_H_

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>
#include <stdio.h>

class CTextura
{
   public:
       CTextura();
      ~CTextura();

      void Elimina();
      void Usa();
      bool CargarBMP(char* szNombreFichero);
      bool Crear(char* szNombreFichero);

      unsigned int m_nID;
      unsigned int m_nAncho, m_nAlto;
};

#endif
