// Standalone behaviour test for CEngineInternalPrimitiveMeshBase::GetMorphInfo @ 0x00ba7fe0
#include <stdio.h>

class CEngineInternalPrimitiveMorphInfo;

class CEngineInternalPrimitiveMeshBase { public: int filler; };

CEngineInternalPrimitiveMorphInfo * __fastcall
CEngineInternalPrimitiveMeshBase_GetMorphInfo(CEngineInternalPrimitiveMeshBase * /*this_ in ecx*/)
{
    return 0;
}

int main(void)
{
    CEngineInternalPrimitiveMeshBase mesh;
    mesh.filler = 0x11223344;

    CEngineInternalPrimitiveMorphInfo * mi =
        CEngineInternalPrimitiveMeshBase_GetMorphInfo(&mesh);

    if (mi == 0)
        printf("CENGINEINTERNALPRIMITIVEMESHBASE_00ba7fe0_TEST PASS\n");
    else
        printf("FAIL: expected null morph info, got %p\n", (void*)mi);

    return 0;
}