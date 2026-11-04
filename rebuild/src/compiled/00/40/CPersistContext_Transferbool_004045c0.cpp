#include "fable_persist_context.h"

// CPersistContext::Transfer<bool>(const char*, bool&, const bool&) @ 0x004045C0.
// The cases are written in reverse mode order and the switch is exhaustive
// (no range check before the jump table), as in retail.
template <class T>
void CPersistContext::Transfer(const char* name, T& value, const T& defaultValue)
{
    CheckCRC(name);
    switch (Mode)
    {
    case MODE_DEFAULTS:
        value = defaultValue;
        break;
    case MODE_BINARY_SAVE:
        CPersistTraits<T>().TransferBinaryOut(*PSaveStream, value);
        break;
    case MODE_BINARY_LOAD:
        CPersistTraits<T>().TransferBinaryIn(*PLoadStream, value);
        break;
    case MODE_TEXT_SAVE:
        {
            CPersistContext_TransferableComponent<T> component(value);
            TransferObjectSaveText(name, component);
        }
        break;
    case MODE_TEXT_LOAD:
        {
            if (UseDefaults)
            {
                value = defaultValue;
            }
            CPersistContext_TransferableComponent<T> component(value);
            TransferObjectLoadText(name, component);
        }
        break;
    default:
        __assume(0);
    }
}

template void CPersistContext::Transfer<bool>(const char*, bool&, const bool&);
