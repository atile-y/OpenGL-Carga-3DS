#ifndef _CVECTOR_H_
#define _CVECTOR_H_

class CVector
{
   public:
      float m_fX, m_fY, m_fZ;
      float m_fPos[3];

      inline void Inicializa(float x, float y, float z)
      {
         m_fX = m_fPos[0] = x;
         m_fY = m_fPos[1] = y;
         m_fZ = m_fPos[2] = z;
      };
};

#endif
