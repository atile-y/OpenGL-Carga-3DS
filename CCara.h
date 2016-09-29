#ifndef _CCARA_H_
#define _CCARA_H_

#include "CVector.h"
#include "CVertice.h"

class CCara
{
   public:
      long    m_Vertices[3];
      CVector m_Normal;

      void Pon(CCara *pCara)
      {
         m_Vertices[0] = pCara->m_Vertices[0];
         m_Vertices[1] = pCara->m_Vertices[1];
         m_Vertices[2] = pCara->m_Vertices[2];
         m_Normal = pCara->m_Normal;
      }

      void PonVert(int nNum, long lVert)
      {
         m_Vertices[nNum] = lVert;
      }
};

#endif
