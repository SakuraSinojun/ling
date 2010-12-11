




#include "input.h"
#include "mod.h"
#include "game.h"

#include <windows.h>


namespace Input
{

        static HWND     m_hWnd = NULL;

        void    AtachInput(HWND hWnd)
        {
                m_hWnd = hWnd;
        }

        void    GetAsyncInput(INPUT_STRUCT& wi)
        {
                POINT   point;
                int     i;
                short   k;
                short   test;
                bool    flag;
                

                if(!IsIconic(m_hWnd))
                {
                        memset(&wi, 0, sizeof(wi));
                        wi.input = INPUT_NONE;
                        return;
                }

                GetCursorPos(&point);
                ScreenToClient(m_hWnd, &point);

                wi.x = point.x;
                wi.y = point.y;
                
                flag = false;
                for(i=1; i<255; i++)
                {
                        k = GetAsyncKeyState(i);
                        test = k >> 15;
                        test = test & 0x1;
                        k = k & 0x1;
                        if(k)
                        {
                                flag = true;
                        }
                        wi.key[i] = test;
                }
                if(!flag)
                {
                        wi.input = INPUT_NONE;
                }else{
                        wi.input = INPUT_NEWINPUT;
                }
        }

        
        
        void    ParseInput(INPUT_STRUCT& input)
        {
                GetAsyncInput(input);

        }
        
        
        

}





