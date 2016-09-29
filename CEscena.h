#ifndef _CESCENA_H_
#define _CESCENA_H_

#include "CObjeto.h"
#include "CMaterial.h"
#include "CListaEnlazada.h"

class CEscena
{
   public:
      CListaEnlazada <CMaterial> *m_pListaMateriales;
      unsigned int                m_nNumMateriales;
      CListaEnlazada <CObjeto>   *m_pListaObjetos;
      unsigned int                m_nNumObjetos;

       CEscena();
      ~CEscena();

      CObjeto *NuevoObjeto(char *szNombre);
};

#endif
