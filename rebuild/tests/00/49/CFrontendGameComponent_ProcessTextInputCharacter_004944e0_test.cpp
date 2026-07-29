#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/49/CFrontendGameComponent_ProcessTextInputCharacter_004944e0.cpp"

static char* Text(CFrontendGameComponent_004944e0* component)
{
    return reinterpret_cast<char*>(component) + 0x168;
}

static long* Length(CFrontendGameComponent_004944e0* component)
{
    return reinterpret_cast<long*>(
        reinterpret_cast<unsigned char*>(component) + 0x1e8);
}

int main()
{
    CFrontendGameComponent_004944e0 component;
    unsigned char* bytes = reinterpret_cast<unsigned char*>(&component);
    std::memset(bytes, 0x5a, sizeof(component));
    std::memset(Text(&component), 0, 128);
    *Length(&component) = 0;

    component.ProcessTextInputCharacter('A');
    component.ProcessTextInputCharacter(0x142);
    if (*Length(&component) != 2 ||
        Text(&component)[0] != 'A' ||
        static_cast<unsigned char>(Text(&component)[1]) != 0x42)
    {
        std::printf(
            "CFrontendGameComponent_ProcessTextInputCharacter_004944e0_test FAIL\n");
        return 1;
    }

    component.ProcessTextInputCharacter(8);
    if (*Length(&component) != 1 || Text(&component)[1] != '\0')
    {
        std::printf(
            "CFrontendGameComponent_ProcessTextInputCharacter_004944e0_test FAIL\n");
        return 1;
    }

    component.ProcessTextInputCharacter(8);
    component.ProcessTextInputCharacter(8);
    if (*Length(&component) != 0 || Text(&component)[0] != '\0')
    {
        std::printf(
            "CFrontendGameComponent_ProcessTextInputCharacter_004944e0_test FAIL\n");
        return 1;
    }

    *Length(&component) = 127;
    Text(&component)[127] = static_cast<char>(0x6d);
    component.ProcessTextInputCharacter('Z');
    if (*Length(&component) != 127 ||
        static_cast<unsigned char>(Text(&component)[127]) != 0x6d)
    {
        std::printf(
            "CFrontendGameComponent_ProcessTextInputCharacter_004944e0_test FAIL\n");
        return 1;
    }

    for (unsigned long index = 0; index < 0x168; ++index)
    {
        if (bytes[index] != 0x5a)
        {
            std::printf(
                "CFrontendGameComponent_ProcessTextInputCharacter_004944e0_test FAIL\n");
            return 1;
        }
    }

    std::printf(
        "CFrontendGameComponent_ProcessTextInputCharacter_004944e0_test PASS\n");
    return 0;
}
