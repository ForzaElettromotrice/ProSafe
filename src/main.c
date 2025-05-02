#include "main.h"



int main()
{
#ifdef Debug
    D_Print("Launched in Debug Mode!\n");
#endif

    printf("Hello, World!\n");
    return 0;
}
