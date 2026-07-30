#include <cstdio>

unsigned char g_primitiveVTableOrData[0x54] = {};

unsigned long CEngineInternalPrimitiveBase_AddChildPrimitive()
;

int main()
{
    unsigned long r = CEngineInternalPrimitiveBase_AddChildPrimitive();
    if (r == (unsigned long)(&g_primitiveVTableOrData[0])) {
        std::printf("CEngineInternalPrimitiveBase_00404a80_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}
