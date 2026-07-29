#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/49/CFrontendGameComponent_ClearTextInput_004944c0.cpp"

int main()
{
    CFrontendGameComponent_004944c0 component;
    unsigned char* bytes = reinterpret_cast<unsigned char*>(&component);
    std::memset(bytes, 0x5a, sizeof(component));

    component.ClearTextInput();

    for (unsigned long index = 0; index < 0x168; ++index)
    {
        if (bytes[index] != 0x5a)
        {
            std::printf("CFrontendGameComponent_ClearTextInput_004944c0_test FAIL\n");
            return 1;
        }
    }

    for (unsigned long index = 0x168; index < 0x1ec; ++index)
    {
        if (bytes[index] != 0)
        {
            std::printf("CFrontendGameComponent_ClearTextInput_004944c0_test FAIL\n");
            return 1;
        }
    }

    std::printf("CFrontendGameComponent_ClearTextInput_004944c0_test PASS\n");
    return 0;
}
