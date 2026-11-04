#include "fable_string.h"
#include "fable_persist_context.h"

void CPersistContext::TransferSignedChar(const char* name, signed char& value, const signed char& defaultValue)
{
    CheckCRC(name);
    switch (Mode)
    {
    case MODE_DEFAULTS:
        value = defaultValue;
        break;
    case MODE_BINARY_SAVE:
        PSaveStream->WriteSLONG(static_cast<unsigned char>(value));
        break;
    case MODE_BINARY_LOAD:
        CPersistTraits<unsigned char>().TransferBinaryIn(*PLoadStream, reinterpret_cast<unsigned char&>(value));
        break;
    case MODE_TEXT_SAVE:
        {
            CPersistContext_TransferableComponent<unsigned char> component(reinterpret_cast<unsigned char&>(value));
            TransferObjectSaveText(name, component);
        }
        break;
    case MODE_TEXT_LOAD:
        {
            if (UseDefaults)
                value = defaultValue;
            CPersistContext_TransferableComponent<unsigned char> component(reinterpret_cast<unsigned char&>(value));
            TransferObjectLoadText(name, component);
        }
        break;
    default:
        __assume(0);
    }
}
