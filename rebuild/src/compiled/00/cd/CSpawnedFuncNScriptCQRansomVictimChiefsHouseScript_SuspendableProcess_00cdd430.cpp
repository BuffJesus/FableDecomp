struct CSpawnedFunc {
    char pad0[5];
    unsigned char suspended;
    char pad6[0x2e];
    void (__fastcall *fn)(void*);
    void* arg;
};

void __fastcall SuspendableProcess(CSpawnedFunc* self)
{
    self->fn(self->arg);
    self->suspended = 1;
}