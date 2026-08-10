struct MsgEventList { void* m0; };
extern "C" void __fastcall SubDtor_c0be20(MsgEventList* self);
typedef void (__cdecl *FreeFn)(void*);
#define g_free (*(FreeFn*)0x144002c)
extern "C" void __fastcall DtorMsgEventList_A(MsgEventList* self)
{
    SubDtor_c0be20(self);
    g_free(self->m0);
}