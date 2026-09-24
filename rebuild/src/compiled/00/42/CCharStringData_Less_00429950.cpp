#include "fable_ui_bank_registry.h"
bool __fastcall FableUiStringDataLess(const CCharStringData* left,void*,const CCharStringData* right)
{
    const signed char* a=reinterpret_cast<const signed char*>(left->text);
    const signed char* b=reinterpret_cast<const signed char*>(right->text);
    for(;;++a,++b)
    {
        if(*a!=*b) return *a<*b;
        if(!*a) return false;
    }
}
