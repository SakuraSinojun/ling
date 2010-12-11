




#include "input.h"

#include <windows.h>


#ifdef __cplusplus
namespace Input
{
#endif

        typedef enum
        {
                WI_NONE         = 0,
                WI_LBUTTONDOWN,
                WI_RBUTTONDOWN,
                WI_WHEELUP,
                WI_WHEELDOWN,
                WI_KEYUP,
                WI_KEYDOWN,
        }WI_INPUT;
       
        static struct
        {
                int             x;
                int             y;
                int             key[256];
        }wi;



        void    GetAsyncInput()
        {
                POINT   point;
                int     i;
                short   k;
                short   test;
                
                GetCursorPos(&point);
                wi.x = point.x;
                wi.y = point.y;
                
                for(i=1; i<255; i++)
                {
                        k = GetAsyncKeyState(i);
                        test = k >> 15;
                        test = test & 0x1;
                        wi.key[i] = test;                        
                }
        }

        
        
        void    ParseInput(INPUT_STRUCT& input)
        {
        
        }
        
        
        

#ifdef __cplusplus
}
#endif




