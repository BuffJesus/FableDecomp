#pragma once
#include "fable_intelligent_pointer.h"
#include "fable_stlport_list.h"
class CClassFactory;
class CCharString;
// Ego_r CClass and retail constructor 00A372D0: primary CBaseObject,
// two 4-byte STLport iterators, then Type at +10. Retail size is 11.
#pragma pack(push,1)
class CClass : public CBaseObject
{
public:
    CClass(CClassFactory& factory, unsigned long type, bool registerClass);
    virtual ~CClass();
    // FableWin ?Save@CClass@@UAEXAAVCCharString@@@Z: CClass vtable 0x0129CF60
    // slot 2 (retail 0x00A19AB0, inherited by CNavigatorBase/Flyer/AStar).
    // Inline in the original header: retail's only copy is a COMDAT inside the
    // navigator-flyer TU (0x00A19AB0), not in the CClass TU (0x00A372D0..).
    virtual void Save(CCharString&) {}
private:
    _STL::list<CClass*>::iterator ClassListPos;
    _STL::list<CClass*>::iterator TypeListPos;
    unsigned char Type;
};
#pragma pack(pop)
FABLE_STATIC_ASSERT(sizeof(CClass)==0x11);
