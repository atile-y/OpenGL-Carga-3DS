#include "CEscena.h"

CEscena::CEscena()
{
   m_pListaMateriales = new CListaEnlazada <CMaterial>;
   m_pListaObjetos = new CListaEnlazada <CObjeto>;
   m_nNumMateriales = m_nNumObjetos = 0;
}

CEscena::~CEscena()
{
   m_pListaMateriales->LiberarTodo();
   m_pListaObjetos->LiberarTodo();
   delete m_pListaMateriales;
   delete m_pListaObjetos;
}

CObjeto *CEscena::NuevoObjeto(char *szNombre)
{
   CObjeto *pNuevoObjeto = new CObjeto();
   pNuevoObjeto->m_pEscena = this;
   strcpy(pNuevoObjeto->m_szNombre, szNombre);
   m_pListaObjetos->Anadir(pNuevoObjeto);
   return pNuevoObjeto;
}
