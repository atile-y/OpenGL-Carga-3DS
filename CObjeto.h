#ifndef _COBJETO_H_
#define _COBJETO_H_

#include "CVertice.h"
#include "CCara.h"
#include "CListaEnlazada.h"

class CEscena;

class CObjeto
{
   public:
       CObjeto();
      ~CObjeto();
      
      CListaEnlazada <CVertice> *m_pListaVertices;
      unsigned int               m_nNumVertices;
      CListaEnlazada <CCara>    *m_pListaCaras;
      unsigned int               m_nNumCaras;
      
      float        m_fColor[3];
      char         m_szNombre[12];
      unsigned int m_nIDMaterial;
      CEscena     *m_pEscena;
      
      void AnadirCara(int nNumeroCaras);
      void AnadirVertice(int nNumeroVertices);
      void CambiarCara(int nNumeroCara, CCara *pCara);
      void CambiarCara(int nNumCara, long Vertices[3]);
      void CambiarVertice(int nNumeroVertice, CVertice *pVertice);
};

#endif
