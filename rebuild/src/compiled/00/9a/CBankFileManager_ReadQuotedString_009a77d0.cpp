#include "fable_retail_banks.h"
#include "fable_data_input_stream.h"

bool CBankFileManager::ReadQuotedString(CCharString& text, CDataInputStream& stream)
{
    bool found;
    {
        CCharString skipped = stream.ReadUntilCharacter('"', found);
    }
    if (found)
    {
        stream.ReadCBYTE();
        text = stream.ReadUntilCharacter('"', found);
        if (found)
            stream.ReadCBYTE();
        return true;
    }
    return false;
}
