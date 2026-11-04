#include "fable_string_parser.h"

void CParsedItem::InitAsFloat(const char* text, unsigned long length)
{
    float whole = 0.0f;
    float fraction = 0.0f;
    float divisor = 1.0f;
    Value.Float = 0.0f;
    bool negative = false;
    bool beforePoint = true;
    for (unsigned long i = 0; i < length; ++i)
    {
        if (text[i] == '-') negative = true;
        else if (text[i] == '.') beforePoint = false;
        else
        {
            long digit = static_cast<signed char>(text[i]) - '0';
            if (beforePoint) whole = whole * 10.0f + digit;
            else
            {
                fraction = fraction * 10.0 + digit;
                divisor = divisor * 10.0;
            }
        }
    }
    Type = FLOAT;
    float result = whole + fraction / divisor;
    Value.Float = result;
    if (negative) Value.Float = result * -1.0;
}
