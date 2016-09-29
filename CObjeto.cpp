#include "CObjeto.h"

CObjeto::CObjeto()
{
   m_nNumCaras = m_nNumVertices = 0;
   m_fColor[0] = m_fColor[1] = m_fColor[2] = 0.0f;
   m_nIDMaterial = 0;
   m_pEscena = NULL;
   m_szNombre[0] = '\0';
   m_pListaCaras = new CListaEnlazada<CCara>;
   m_pListaVertices = new CListaEnlazada<CVertice>;
}

CObjeto::~CObjeto()
{
   m_pEscena = NULL;
   m_pListaCaras->LiberarTodo();
   m_pListaVertices->LiberarTodo();
}

void CObjeto::AnadirCara(int nNumeroCaras)
{
   CCara *nueva_cara;
   int i;
   int total = m_pListaCaras->m_nNumElementos;
   m_pListaCaras->Insertar(nNumeroCaras);
   for(i=0;i<nNumeroCaras;i++)
   {
      nueva_cara = new CCara;
      m_pListaCaras->Poner(total+i, nueva_cara);
   }
}

void CObjeto::CambiarCara(int nNumeroCara, CCara *pCara)
{
   if( nNumeroCara < 0 || nNumeroCara >= m_pListaCaras->Contar() )
      return;

   m_pListaCaras->Obtener(nNumeroCara)->Pon(pCara);
}

void CObjeto::CambiarCara(int nNumCara, long Vertices[3])
{
   m_pListaCaras->Obtener(nNumCara)->PonVert(0,Vertices[0]);
   m_pListaCaras->Obtener(nNumCara)->PonVert(1,Vertices[1]);
   m_pListaCaras->Obtener(nNumCara)->PonVert(2,Vertices[2]);
}

void CObjeto::AnadirVertice(int nNumeroVertices)
{
   CVertice *nuevo_vert;
   int i;
   int total = m_pListaVertices->Contar();
   m_pListaVertices->Insertar(nNumeroVertices);
   for(i=0;i<nNumeroVertices;i++)
   {
      nuevo_vert = new CVertice;
      m_pListaVertices->Poner(total+i, nuevo_vert);
   }
}

void CObjeto::CambiarVertice(int nNumeroVertice, CVertice *pVertice)
{
   if( nNumeroVertice < 0 || nNumeroVertice >= m_pListaVertices->Contar() )
      return;

   m_pListaVertices->Obtener(nNumeroVertice)->Pon(pVertice);
}
