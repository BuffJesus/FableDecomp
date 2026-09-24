#include "fable_ui_wide_strings.h"
void __fastcall FableUiUnassignWideString(FableUiWideStringValue* value,void*)
{
    if(value->Storage)
    {
        if(--value->Storage->owners<=0)
        {
            CWideStringData* data=value->Storage;
            if(data)
            {
                if(data->text) FableUiFreeWideBuffer(data->text);
                FableUiFreeStringRecord(data);
            }
        }
        value->Storage=0;
    }
}
