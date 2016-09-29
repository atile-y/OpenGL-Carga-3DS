#ifndef _CMATERIAL_H_
#define _CMATERIAL_H_

#include "CTextura.h"
class CMaterial
{
   public:
      bool      m_bTex;
      float     m_fDifuso[4];
      char      m_szNombre[17];
      CTextura *m_pTextura;

       CMaterial()
      {
         m_bTex = false;
      }
};

#endif
