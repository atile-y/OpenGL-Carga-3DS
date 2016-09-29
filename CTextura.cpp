#include "CTextura.h"

#include <windows.h>

CTextura::CTextura()
{
   m_nID = 0;
}

CTextura::~CTextura()
{
   Elimina();
}

bool CTextura::Crear(char* szNombreFichero)
{
   Elimina();

   unsigned long int nszLon = strlen(szNombreFichero);

   glGenTextures(1, &m_nID);
   m_nID++;

   glBindTexture(GL_TEXTURE_2D, m_nID-1);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   return true;
}

void CTextura::Elimina()
{
   if( m_nID )
   {
      m_nID--;
      glDeleteTextures(1, &m_nID);
      m_nID = 0;
   }
}

void CTextura::Usa()
{
   glBindTexture(GL_TEXTURE_2D, m_nID-1);
}

bool CTextura::CargarBMP(char *szNombreFichero)
{
   FILE *hFichero;
   AUX_RGBImageRec *Imagen;
   bool bResultado = false;

   if(szNombreFichero)
   {
      hFichero = fopen(szNombreFichero, "r");
      if(hFichero)
      {
         fclose(hFichero);
         Crear(szNombreFichero);

         Imagen = auxDIBImageLoad(szNombreFichero);
         m_nAncho = Imagen->sizeX;
         m_nAlto = Imagen->sizeY;

         glTexImage2D(GL_TEXTURE_2D, 0, 3, m_nAncho, m_nAlto, 0, GL_RGB, GL_UNSIGNED_BYTE, Imagen->data);

         bResultado = true;
      }
   }

   delete Imagen->data;
   delete Imagen;
   return bResultado;
}
