#include "fable_ui_strings.h"
void __fastcall FableUiUnassignString(FableUiStringValue* value,void*)
{
    if(value->Storage)
    {
        if(--value->Storage->owners<=0)
        {
            CCharStringData* data=value->Storage;
            if(data)
            {
                if(data->text) { FableUiFreeStringBuffer(data->text); data->text=0; }
                data->unknown04=0; data->unknown08&=0x80000000u;
                FableUiFreeStringRecord(data);
            }
        }
        value->Storage=0;
    }
}
