#ifndef _CVERTICE_H_
#define _CVERTICE_H_

#include "CVector.h"

class CVertice
{
   public:
      CVector m_cPunto;
      CVector m_cNormal;
      float   m_fU, m_fV;
      float   m_fColor[3];

      void PonColor(float r, float g, float b)
      {
         m_fColor[0] = r;
         m_fColor[1] = g;
         m_fColor[2] = b;
      }

      void PonCoordText(float u, float v)
      {
         m_fU = u;
         m_fV = v;
      }

      void Pon(CVertice *pVertice)
      {
         m_cPunto = pVertice->m_cPunto;
         m_cNormal = pVertice->m_cNormal;
         m_fColor[0] = pVertice->m_fColor[0];
         m_fColor[1] = pVertice->m_fColor[1];
         m_fColor[2] = pVertice->m_fColor[2];
         m_fU = pVertice->m_fU;
         m_fV = pVertice->m_fV;
      }

      void Pon(CVertice pVertice)
      {
         m_cPunto = pVertice.m_cPunto;
         m_cNormal = pVertice.m_cNormal;
         m_fColor[0] = pVertice.m_fColor[0];
         m_fColor[1] = pVertice.m_fColor[1];
         m_fColor[2] = pVertice.m_fColor[2];
         m_fU = pVertice.m_fU;
         m_fV = pVertice.m_fV;
      }
};

#endif
