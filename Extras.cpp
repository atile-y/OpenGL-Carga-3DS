#include "Extras.h"

#include <stdio.h>

void FPS(HWND hWnd)
{
   static float fps = 0.0f;
   static float previousTime = 0.0f;
   static char strFPS[20] = {0};

   float currentTime = (GetTickCount()*0.001f);

   fps++;

   if( currentTime - previousTime > 1.0f )
   {
      previousTime = currentTime;
      sprintf(strFPS, "FPS: %d", int(fps));
      SetWindowText(hWnd, strFPS);
      fps = 0.0f;
   }
}
