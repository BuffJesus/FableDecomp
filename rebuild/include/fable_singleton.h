#pragma once
#include "rebuild_abi.h"

template<typename T>
class CTBaseSingleton
{
public:
    static T& FABLE_FASTCALL Get();
    static void FABLE_FASTCALL Release();
};
