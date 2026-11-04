#include "fable_string_parser.h"

void CParsedItem::InitAsInteger(const char* text, unsigned long length)
{
    Value.Integer = 0;
    bool negative = false;
    for (unsigned long i = 0; i < length; ++i)
    {
        if (text[i] == '-') negative = true;
        else Value.Integer = static_cast<long>(static_cast<unsigned long>(Value.Integer) * 10u + static_cast<signed char>(text[i]) - '0');
    }
    Type = INTEGER;
    if (negative) Value.Integer = static_cast<long>(0u - static_cast<unsigned long>(Value.Integer));
}
