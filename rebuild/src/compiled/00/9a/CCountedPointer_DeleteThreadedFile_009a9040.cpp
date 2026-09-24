#include "fable_ui_bank_open.h"
void __fastcall FableUiDeleteThreadedFile(void* file)
{
    if(file)
    {
        typedef void (__fastcall *Delete)(void*,void*,unsigned);
        Delete* table=*static_cast<Delete**>(file);
        table[0](file,0,1);
    }
}
