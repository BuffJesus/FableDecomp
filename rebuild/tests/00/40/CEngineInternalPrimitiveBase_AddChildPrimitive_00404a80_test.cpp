#include <cstdio>

int g_primitiveVTableOrData = 0;

unsigned long CEngineInternalPrimitiveBase_AddChildPrimitive()
{
    return (unsigned long)(&g_primitiveVTableOrData);
}

int main()
{
    unsigned long r = CEngineInternalPrimitiveBase_AddChildPrimitive();
    if (r == (unsigned long)(&g_primitiveVTableOrData)) {
        std::printf("CEngineInternalPrimitiveBase_00404a80_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}