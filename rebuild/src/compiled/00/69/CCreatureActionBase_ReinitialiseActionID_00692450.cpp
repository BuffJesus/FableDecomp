extern unsigned long g_actionid;
struct S { char pad[0x28]; unsigned long f28; };
void __fastcall ReinitialiseActionID(S* self){ self->f28 = g_actionid; ++g_actionid; }