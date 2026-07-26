#include <cstdint>

template <typename T>
static inline T& GlobalAt(std::uintptr_t address)
{
    return *reinterpret_cast<T*>(address);
}

void __cdecl $E2()
{
    GlobalAt<std::uint32_t>(0x013B7CEC) = 0xFF000000;
    GlobalAt<std::uint32_t>(0x013B7CFC) = 0xFF000000;
    GlobalAt<std::uint32_t>(0x013B7CE4) = 0x00000000;
    GlobalAt<std::uint32_t>(0x013B7CE8) = 0x00404040;
    GlobalAt<std::uint8_t>(0x013B7CF0) = 0x18;
    GlobalAt<std::uint8_t>(0x013B7CF1) = 0xA0;
    GlobalAt<std::uint8_t>(0x013B7CF2) = 0x00;
    GlobalAt<std::uint32_t>(0x013B7CF4) = 0x00000000;
    GlobalAt<std::uint32_t>(0x013B7CF8) = 0xFFFFFFFF;
    GlobalAt<std::uint32_t>(0x013B7D00) = 0x00000000;
    GlobalAt<std::uint32_t>(0x013B7D04) = 0x00FFFF00;
    GlobalAt<std::uint32_t>(0x013B7D08) = 0x000000FF;
    GlobalAt<std::uint32_t>(0x013B7D0C) = 0x0000FF00;
    GlobalAt<std::uint32_t>(0x013B7D10) = 0x00FF0000;
    GlobalAt<std::uint32_t>(0x013B7D14) = 0x00FF0000;
    GlobalAt<std::uint8_t>(0x013B7D18) = 0x80;
    GlobalAt<std::uint32_t>(0x013B7D1C) = 0x00000000;
    GlobalAt<std::uint8_t>(0x013B7D20) = 0x02;
    GlobalAt<std::uint8_t>(0x013B7D21) = 0x01;
}