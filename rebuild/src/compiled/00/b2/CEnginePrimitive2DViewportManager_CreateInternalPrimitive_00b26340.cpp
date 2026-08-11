// CEnginePrimitive2DViewportManager::CreateInternalPrimitive
// p = new(0x178); if(p) return p->Construct(); else return 0;
// Construct() is a this-only ctor, tail-called (jmp); function pops no args (plain ret).

struct CEngineInternalPrimitiveBase;

void* operator_new_stub(unsigned int size);
#pragma comment(linker, "/alternatename:?operator_new_stub@@YAPAXI@Z=??2@YAPAXI@Z")

struct CEngineInternalPrimitiveBase
{
    CEngineInternalPrimitiveBase* Construct();
};

struct CEnginePrimitive2DViewportManager
{
    CEngineInternalPrimitiveBase* CreateInternalPrimitive();
};

CEngineInternalPrimitiveBase*
CEnginePrimitive2DViewportManager::CreateInternalPrimitive()
{
    CEngineInternalPrimitiveBase* p =
        (CEngineInternalPrimitiveBase*)operator_new_stub(0x178);
    if (p)
        return p->Construct();
    return 0;
}