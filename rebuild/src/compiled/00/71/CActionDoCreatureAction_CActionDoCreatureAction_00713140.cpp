#include "engine/CActionDoCreatureAction.h"

extern void __fastcall Base_ctor(void* self);
extern void* const g_vtbl_1265838;

struct CCreatureActionCountedPointerMethods {
    CCreatureActionBase* Data;
    CCPPointerInfo* Info;
    void Init(void* action);
};

typedef void* (__fastcall *CCreatureActionFactory)(void* self);

CActionDoCreatureAction* __fastcall CActionDoCreatureAction_CActionDoCreatureAction(
    CActionDoCreatureAction* self,
    int,
    void* actionProvider)
{
    Base_ctor(self);
    *(long*)&self->_pad_0x04[0] = 0;
    self->_pad_0x08[0] = 0;
    self->_pad_0x08[1] = 0;
    self->__vftable = (void*)&g_vtbl_1265838;
    int* actionPointer = (int*)&self->Action_Data;
    actionPointer[0] = 0;
    actionPointer[1] = 0;
    self->ActionID = 0;
    self->ActionSet = false;
    void* action = (*(CCreatureActionFactory*)((char*)*(void**)actionProvider + 0x2c))(
        actionProvider);
    ((CCreatureActionCountedPointerMethods*)&self->Action_Data)->Init(action);
    return self;
}
