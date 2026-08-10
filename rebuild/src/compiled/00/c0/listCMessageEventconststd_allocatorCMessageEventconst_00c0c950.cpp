struct MsgEventList { void* p; };

extern "C" void __fastcall BaseDtor_C0BE20(MsgEventList* self);

typedef void (__cdecl *FreeFn)(void*);
#define g_free (*(FreeFn*)0x14401b8)

extern "C" void __fastcall DtorMsgEventList_C(MsgEventList* self)
{
    BaseDtor_C0BE20(self);
    g_free(self->p);
}