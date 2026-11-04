#include "fable_string_parser.h"
#include "fable_persist_context.h"
#include <string.h>

void CPersistContext::TransferObjectLoadText(const char* name, CPersistContext_TransferableComponentGenericType& object)
{
    CStringParser parser(*PParser);
    unsigned long nameLength = strlen(name);
    while (!Finished && parser.SkipPastWholeString(name))
    {
        unsigned long start = parser.StringPos - nameLength;
        object.TransferIn(*this, parser);
        if (SingleTransfer)
            Finished = true;
        if (parser.ReadAsSymbol() == ";")
        {
            if (PStringToClean)
            {
                char* text = const_cast<char*>(PStringToClean->GetConstCharPtr());
                for (unsigned long i = start; i < parser.StringPos; ++i)
                    if (text[i] != '\n')
                        text[i] = ' ';
            }
        }
        else
            PersistError(parser, "Unable to find terminating semicolon.");
    }
}
