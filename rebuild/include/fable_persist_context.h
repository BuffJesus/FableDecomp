#pragma once

#include "rebuild_abi.h"

class CPersistContext
{
public:
    template <class T>
    void Transfer(const char* name, T& value, const T& defaultValue);

    void TransferSignedChar(
        const char* name,
        signed char& value,
        const signed char& defaultValue);
};
