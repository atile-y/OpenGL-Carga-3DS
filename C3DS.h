#ifndef _3DS_H_
#define _3DS_H_

#include <stdio.h>
#include "CEscena.h"

/*
//   Sub-etiquetas de KEYF3DS
#define KEYF_UNKNWN01 0xB009
#define KEYF_UNKNWN02 0xB00A
#define KEYF_FRAMES   0xB008
#define KEYF_OBJDES   0xB002

#define KEYF_OBJHIERARCH  0xB010
#define KEYF_OBJDUMMYNAME 0xB011
#define KEYF_OBJUNKNWN01  0xB013
#define KEYF_OBJUNKNWN02  0xB014
#define KEYF_OBJUNKNWN03  0xB015
#define KEYF_OBJPIVOT     0xB020
#define KEYF_OBJUNKNWN04  0xB021  
#define KEYF_OBJUNKNWN05  0xB022  

//   Sub-etiquetas de los colores
#define COL_RGB  0x0010
#define COL_TRU  0x0011
#define COL_UNK  0x0013
*/

struct DataChunk{
   unsigned int ID;
   unsigned long length;
   unsigned long bytesRead;
};

class C3DS
{
   private:
      FILE *FIn;
      CVertice *ListaVertices;

      unsigned char ReadChar(unsigned long *);
      unsigned int ReadInt(unsigned long *);
      unsigned long ReadLong(unsigned long *);
      unsigned long ReadString(char *, unsigned long *);
      DataChunk ReadDataChunk();

      void ReadNextChunk(DataChunk *, CEscena *);
      void ReadNextMaterialChunk(DataChunk *, CMaterial *);
      void ReadNextObjectChunk(DataChunk *, CObjeto *);
      void ReadColorChunk(DataChunk *);
      void ReadVerticesChunk(DataChunk *, CObjeto *);
      void ReadFacesChunk(DataChunk *, CObjeto *);
      void ReadObjMatChunk(DataChunk *, CObjeto *);
      void ReadUVCoordChunk(DataChunk *, CObjeto *);

   public:
      int Cargar(char *szNombreFichero, CEscena *pEscena);
};

#endif
