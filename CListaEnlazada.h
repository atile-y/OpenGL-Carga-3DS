#ifndef _CLISTAENLAZADA_H_
#define _CLISTAENLAZADA_H_

#include <stdlib.h>
#include <string.h>

template <class tTipo>
class CListaEnlazada
{
   private:
      typedef struct ElementoTipo{ tTipo *puntero; };

      void ReservaMemoria();
      bool m_bTodoLiberado;

   public:
      ElementoTipo *m_Zona;
      int           m_nNumElementos;

       CListaEnlazada();
      ~CListaEnlazada();

      void LiberarTodo();
      void Insertar(const int &nNumero);
      void CopiarLista(CListaEnlazada<tTipo> *Lista);
      void Anadir(tTipo *tElemento);
      void AnadirUnico(tTipo *tElemento);
      void Poner(const int &nIndice, tTipo *tElemento);
      bool Buscar(tTipo *tElemento, int &nIndice);
      bool Buscar(tTipo *tElemento);
      void Borrar(tTipo *tElemento);
      void Borrar(const int &nIndice);
      void Limite(int m_nNumElementos);
      tTipo *Obtener(const int &nIndice);
      int Contar();
      void Resetea();
};

template <class tTipo>
CListaEnlazada <tTipo>::CListaEnlazada()
{
   m_bTodoLiberado = false;
   m_nNumElementos = 0;
   m_Zona = NULL;
}

template <class tTipo>
CListaEnlazada <tTipo>::~CListaEnlazada()
{
   free(m_Zona);
}

template <class tTipo>
void CListaEnlazada <tTipo>::LiberarTodo()
{
   int i;
   m_bTodoLiberado = true;
   for(i=0;i<m_nNumElementos;i++)
      delete Obtener(i);
   delete this;
}

template <class tTipo>
void CListaEnlazada <tTipo>::Resetea()
{
   m_nNumElementos = 0;
   free(m_Zona);
   m_Zona = NULL;
}

template <class tTipo>
void CListaEnlazada <tTipo>::ReservaMemoria()
{
   m_Zona = (ElementoTipo*)realloc(m_Zona,m_nNumElementos<<2);
}

template <class tTipo>
void CListaEnlazada <tTipo>::Insertar(const int &nNumero)
{
   m_nNumElementos += nNumero;
   ReservaMemoria();
}

template <class tTipo>
void CListaEnlazada <tTipo>::CopiarLista(CListaEnlazada<tTipo> *Lista)
{
   if( Lista->m_nNumElementos == 0 )
      return;

   int last = m_nNumElementos;
   Insertar(Lista->m_nNumElementos);
   memcpy(&m_Zona[last], Lista->m_Zona, Lista->m_nNumElementos<<2);
}

template <class tTipo>
void CListaEnlazada <tTipo>::Anadir(tTipo *tElemento)
{
   m_nNumElementos++;
   ReservaMemoria();
   m_Zona[m_nNumElementos-1].puntero = tElemento;
}

template <class tTipo>
void CListaEnlazada <tTipo>::AnadirUnico(tTipo *tElemento)
{
   if( !Buscar(tElemento) )
      Anadir(tElemento);
}

template <class tTipo>
void CListaEnlazada <tTipo>::Poner(const int &nIndice, tTipo *tElemento)
{
   m_Zona[nIndice].puntero = tElemento;
}

template <class tTipo>
int CListaEnlazada <tTipo>::Contar()
{
   return m_nNumElementos;
}

template <class tTipo>
tTipo *CListaEnlazada <tTipo>::Obtener(const int &nIndice)
{
   return m_Zona[nIndice].puntero;
}

template <class tTipo>
bool CListaEnlazada <tTipo>::Buscar(tTipo *tElemento, int &nIndice)
{
   if( m_nNumElementos == 0 )
      return false;

   int contador = 0;
   ElementoTipo *acceso;

   for(acceso=m_Zona;acceso<m_Zona+m_nNumElementos;acceso++)
   {
      if( acceso->puntero == tElemento)
      {
         nIndice = contador;
         return true;
      }
      contador++;
   }

   return false;
}

template <class tTipo>
bool CListaEnlazada <tTipo>::Buscar(tTipo *tElemento)
{
   if( m_nNumElementos == 0 )
      return false;

   ElementoTipo *acceso;

   for(acceso=m_Zona;acceso<m_Zona+m_nNumElementos;acceso++)
      if( acceso->puntero == tElemento )
         return true;

   return false;
}

template <class tTipo>
void CListaEnlazada <tTipo>::Borrar(tTipo *tElemento)
{
   if( m_bTodoLiberado )
      return;

   int nIndice;

   if( Buscar(tElemento, nIndice) )
      Borrar(nIndice);
}

template <class tTipo>
void CListaEnlazada <tTipo>::Borrar(const int &nIndice)
{
   if( m_bTodoLiberado )
      return;

   if( m_nNumElementos < 2 )
   {
      m_nNumElementos = 0;
      free(m_Zona);
      m_Zona = NULL;
      return;
   }

   if( nIndice < m_nNumElementos-1 )
      memmove(&m_Zona[nIndice], &m_Zona[nIndice+1], (m_nNumElementos-(nIndice+1))*4);

   --m_nNumElementos;

   ReservaMemoria();
}

template <class tTipo>
void CListaEnlazada <tTipo>::Limite(int nNumElementos)
{
   this->m_nNumElementos = nNumElementos;
   ReservaMemoria();
}

#endif
