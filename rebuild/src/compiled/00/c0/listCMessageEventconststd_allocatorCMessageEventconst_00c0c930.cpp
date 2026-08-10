struct MsgEventList { void* p0; };

/* helper at 0xc0be20 (base-dtor), called this-in-ecx */
void __fastcall MsgEventList_SubDtor(MsgEventList* self);

/* global free function pointer at 0x1440030 (cdecl, one arg) */
typedef void (__cdecl *FreeFn)(void*);
static FreeFn* const g_freeSlot = (FreeFn*)0x1440030;

void __fastcall DtorMsgEventList_B(MsgEventList* self)
{
    MsgEventList_SubDtor(self);
    (*g_freeSlot)(self->p0);
}