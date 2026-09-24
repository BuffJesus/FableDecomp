#include "fable_ui_bank_factory.h"
int __fastcall FableUiCompareStringBytes(const char* left,const char* right)
{
    for(;;++left,++right)
    {
        signed char a=*left,b=*right;
        if(a==0 && b==0) return 0;
        if(a<b) return -1;
        if(a>b) return 1;
    }
}
