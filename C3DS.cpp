#include "C3DS.h"

#include <conio.h>
#include <string.h>
#include <stdlib.h>

//   Etiquetas Principales
#define MAIN3DS            0x4D4D
#define EDIT3DS            0x3D3D
#define KEYF3DS            0xB000

//   Sub-etiquetas de EDIT3DS
#define EDIT_MATERIAL      0xAFFF
#define EDIT_OBJECT        0x4000

//   Sub-etiquetas de EDIT_MATERIAL
#define MATERIAL_NAME      0xA000
#define MATERIAL_DIFFUSE   0xA020
#define MATERIAL_MAP       0xA200
#define MATERIAL_FILE      0xA300

//   Sub-etiquetas de EDIT_OBJECT
#define OBJECT_MESH        0x4100

//   Sub-etiquetas de OBJECT_MESH
#define OBJECT_VERTICES    0x4110
#define OBJECT_FACES       0x4120
#define OBJECT_MATERIAL    0x4130
#define OBJECT_UV          0x4140

/***********************************************/
/*** METODOS PARA LEER LOS DATOS DEL ARCHIVO ***/
/***********************************************/

unsigned char C3DS::ReadChar(unsigned long *bytesRead)
{
   (*bytesRead)++;
   return fgetc(FIn);
}

unsigned int C3DS::ReadInt(unsigned long *bytesRead)
{
   unsigned int tmp = ReadChar(bytesRead);
   return (tmp | (ReadChar(bytesRead)<<8));
}

unsigned long C3DS::ReadLong(unsigned long *bytesRead)
{
   unsigned long temp1, temp2;

   temp1 = ReadInt(bytesRead);
   temp2 = ReadInt(bytesRead);

   return (temp1+(temp2*0x10000L));
}

unsigned long C3DS::ReadString(char *s, unsigned long *bytesRead)
{
   int i=0;

   do
   {
      s[i] = ReadChar(bytesRead);
   }
   while( s[i++] != 0 );

   return strlen(s)+1;
}

DataChunk C3DS::ReadDataChunk()
{
   DataChunk ret;

   ret.bytesRead = 0L;
   ret.ID = ReadInt(&ret.bytesRead);
   ret.length = ReadLong(&ret.bytesRead);

   return ret;
}

/************************************************/
/*** INICIA EL PROCESO PARA CARGAR EL ARCHIVO ***/
/************************************************/

int C3DS::Cargar(char *szNombreFichero, CEscena *pEscena)
{
   unsigned char version;
   DataChunk CurrentChunk;

   FIn = fopen(szNombreFichero, "rb");
   if( FIn == NULL )
      return 0;

   fseek(FIn, 28L, SEEK_SET);
   version = ReadChar(&CurrentChunk.bytesRead);
   if( version < 3 )
   {
      MessageBox(NULL, "Esta libreria solo lee arhivos 3DS con version 3 o mayor\n", "ERROR", MB_OK);
      return 0; 
   }
   fseek(FIn, 0, SEEK_SET);

   CurrentChunk = ReadDataChunk();
   if( CurrentChunk.ID != MAIN3DS )
   {
      MessageBox(NULL, "Error en la cabecera del archivo", "ERROR", MB_OK);
      return 0;
   }

   ReadNextChunk(&CurrentChunk, pEscena);

   fclose(FIn);
   return 1;
}

void C3DS::ReadNextChunk(DataChunk *AptPreviousChunk, CEscena *pEscena)
{
   int buffer[65536];
   char Nombre[256];
   unsigned char version;
   DataChunk CurrentChunk;
   DataChunk TempChunk;
   CObjeto *objeto;
   CMaterial *material;

   while( AptPreviousChunk->bytesRead < AptPreviousChunk->length )
   {
      CurrentChunk = ReadDataChunk();
      switch(CurrentChunk.ID)
      {
         case EDIT3DS:
            TempChunk = ReadDataChunk();
            // Version de la malla?? o.O "Get the version of the mesh" wtf!
            TempChunk.bytesRead += fread(&version, 1, TempChunk.length-TempChunk.bytesRead, FIn);
            CurrentChunk.bytesRead += TempChunk.bytesRead;
            
            ReadNextChunk(&CurrentChunk, pEscena);
            break;

         case EDIT_MATERIAL:
            material = new CMaterial;
            pEscena->m_pListaMateriales->Anadir(material);
            pEscena->m_nNumMateriales++;
            ReadNextMaterialChunk(&CurrentChunk, material);
            break;

         case EDIT_OBJECT:
            pEscena->m_nNumObjetos++;
            ReadString(Nombre, &CurrentChunk.bytesRead);
            objeto = pEscena->NuevoObjeto(Nombre);
            ReadNextObjectChunk(&CurrentChunk, objeto);
            break;

         case KEYF3DS:
            // ReadNextKeyFrameChunk(&Currentchunk);
            CurrentChunk.bytesRead += fread(buffer, 1, CurrentChunk.length - CurrentChunk.bytesRead, FIn);
            break;

         default:
            CurrentChunk.bytesRead += fread(buffer, 1, CurrentChunk.length - CurrentChunk.bytesRead, FIn);
            break;
      }
      AptPreviousChunk->bytesRead += CurrentChunk.bytesRead;
   }
}

void C3DS::ReadNextMaterialChunk(DataChunk *AptPreviousChunk, CMaterial *pMaterial)
{
   int buffer[65536];
   char Nombre[256];
   DataChunk CurrentChunk;

   while( AptPreviousChunk->bytesRead < AptPreviousChunk->length )
   {
      CurrentChunk = ReadDataChunk();
      switch(CurrentChunk.ID)
      {
         case MATERIAL_MAP:
            ReadNextMaterialChunk(&CurrentChunk, pMaterial);
            break;

         case MATERIAL_NAME:
            ReadString(Nombre, &CurrentChunk.bytesRead);
            strcpy(pMaterial->m_szNombre, Nombre);
            break;

         case MATERIAL_DIFFUSE:
            ReadColorChunk(&CurrentChunk);
            break;

         case MATERIAL_FILE:
            ReadString(Nombre, &CurrentChunk.bytesRead);
            pMaterial->m_bTex = true;
            pMaterial->m_pTextura = new CTextura;
            if( !pMaterial->m_pTextura->CargarBMP(Nombre) )
            {
               char t[128];
               sprintf(t, "Error cargando el archivo %s", pMaterial->m_szNombre);
               MessageBox(NULL, t, "ERROR", MB_OK);
            }
            break;

         default:
            CurrentChunk.bytesRead += fread(buffer, 1, CurrentChunk.length - CurrentChunk.bytesRead, FIn);
            break;
      }
      AptPreviousChunk->bytesRead += CurrentChunk.bytesRead;
   }
}

void C3DS::ReadNextObjectChunk(DataChunk *AptPreviousChunk, CObjeto *pObjeto)
{
   int buffer[65536];
   DataChunk CurrentChunk;

   while( AptPreviousChunk->bytesRead < AptPreviousChunk->length )
   {
      CurrentChunk = ReadDataChunk();
      switch(CurrentChunk.ID)
      {
         case OBJECT_MESH:
            ReadNextObjectChunk(&CurrentChunk, pObjeto);
            break;

         case OBJECT_VERTICES:
            ReadVerticesChunk(&CurrentChunk, pObjeto);
            break;

         case OBJECT_FACES:
            ReadFacesChunk(&CurrentChunk, pObjeto);
            break;

         case OBJECT_MATERIAL:
            ReadObjMatChunk(&CurrentChunk, pObjeto);
            break;

         case OBJECT_UV:
            ReadUVCoordChunk(&CurrentChunk, pObjeto);
            break;

         default:
            CurrentChunk.bytesRead += fread(buffer, 1, CurrentChunk.length - CurrentChunk.bytesRead, FIn);
            break;
      }
      AptPreviousChunk->bytesRead += CurrentChunk.bytesRead;
   }
}

void C3DS::ReadColorChunk(DataChunk *AptPreviousChunk)
{
   DataChunk tmp = ReadDataChunk();
   unsigned char color[3];

   printf("ID: %0X\n", tmp.ID);
   color[0] = ReadChar(&tmp.bytesRead);
   color[1] = ReadChar(&tmp.bytesRead);
   color[2] = ReadChar(&tmp.bytesRead);
   printf("Colores leidos = { %d, %d, %d }\n", color[0], color[1], color[2]);
   AptPreviousChunk->bytesRead += tmp.bytesRead;
}

void C3DS::ReadVerticesChunk(DataChunk *AptPreviousChunk, CObjeto *pObjeto)
{
   CVector vect;
   CVertice *vert = new CVertice;
   unsigned int NVertex;
   float Vx[3], tmp;

   NVertex = ReadInt(&AptPreviousChunk->bytesRead);
   pObjeto->AnadirVertice(NVertex);

   for(int i=0;i<NVertex;i++)
   {
      AptPreviousChunk->bytesRead += fread(Vx, 1, 3*sizeof(float), FIn);
      tmp = Vx[1];
      Vx[1] = Vx[2];
      Vx[2] = -tmp;

      vect.Inicializa(Vx[0],Vx[1],Vx[2]);

      vert->m_cPunto = vect;
      vert->m_fU = 0.0f;
      vert->m_fV = 0.0f;

      pObjeto->CambiarVertice(i, vert);
   }
}

void C3DS::ReadFacesChunk(DataChunk *AptPreviousChunk, CObjeto *pObjeto)
{
   unsigned int NFaces;
   long vert[3];

   NFaces = ReadInt(&AptPreviousChunk->bytesRead);
   pObjeto->AnadirCara(NFaces);
   pObjeto->m_nNumCaras = NFaces;

   for(int i=0;i<pObjeto->m_nNumCaras;i++)
   {
      vert[0] = ReadInt(&AptPreviousChunk->bytesRead);
      vert[1] = ReadInt(&AptPreviousChunk->bytesRead);
      vert[2] = ReadInt(&AptPreviousChunk->bytesRead);
      ReadInt(&AptPreviousChunk->bytesRead);

      pObjeto->CambiarCara(i, vert);
   }
}

void C3DS::ReadObjMatChunk(DataChunk *AptPreviousChunk, CObjeto *pObjeto)
{
   CEscena *pEscena = pObjeto->m_pEscena;
   unsigned int buffer[65536];
   char MatName[256];

   ReadString(MatName, &AptPreviousChunk->bytesRead);

   pObjeto->m_nIDMaterial = pEscena->m_nNumMateriales+10;
   for(int i=0;i<pEscena->m_nNumMateriales;i++)
      if( !strcmp(MatName, pEscena->m_pListaMateriales->Obtener(i)->m_szNombre) )
         pObjeto->m_nIDMaterial = i;

   AptPreviousChunk->bytesRead += fread(buffer, 1, AptPreviousChunk->length-AptPreviousChunk->bytesRead, FIn);
}

void C3DS::ReadUVCoordChunk(DataChunk *AptPreviousChunk, CObjeto *pObjeto)
{
   float u, v;

   pObjeto->m_nNumVertices = ReadInt(&AptPreviousChunk->bytesRead);
   char t[128];

   for(int i=0;i<pObjeto->m_nNumVertices;i++)
   {
      AptPreviousChunk->bytesRead += fread(&u, 1, sizeof(float), FIn);
      AptPreviousChunk->bytesRead += fread(&v, 1, sizeof(float), FIn);

      pObjeto->m_pListaVertices->Obtener(i)->m_fU = u;
      pObjeto->m_pListaVertices->Obtener(i)->m_fV = v;
   }
}
