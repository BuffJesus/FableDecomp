struct CRIT { int x; };
typedef void (__stdcall *LEAVEFN)(void*);
LEAVEFN g_leave;

struct CHandleBasedMemoryPool {
    char pad[0x24];
    char flag;
    char pad2[0x44c - 0x25];
    CRIT cs;
};

void __fastcall EndCritical(CHandleBasedMemoryPool* self)
{
    if (self->flag) {
        g_leave(&self->cs);
    }
}