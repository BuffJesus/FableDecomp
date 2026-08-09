#include <stdio.h>
struct CEnginePrimitive;
struct CEngineInternalPrimitiveBase {
    unsigned long AddChildPrimitive(CEnginePrimitive* prim, unsigned long flags) {
        (void)prim; (void)flags;
        return 0;
    }
};
int main() {
    CEngineInternalPrimitiveBase b;
    unsigned long r1 = b.AddChildPrimitive((CEnginePrimitive*)0, 0);
    unsigned long r2 = b.AddChildPrimitive((CEnginePrimitive*)0x1234, 99);
    if (r1 != 0) { printf("FAIL r1\n"); return 1; }
    if (r2 != 0) { printf("FAIL r2\n"); return 1; }
    printf("OK_00b8d0b0\n");
    return 0;
}