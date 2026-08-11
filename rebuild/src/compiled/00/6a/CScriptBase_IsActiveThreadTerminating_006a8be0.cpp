struct SubThread {
    char pad[0xc];
    bool terminating; // +0xc
};

struct CScriptBase {
    char pad0[0x2c];
    unsigned int flags;        // +0x2c
    char pad1[0x5c - 0x2c - 4];
    SubThread* activeThread;    // +0x5c
};

bool __stdcall IsActiveThreadTerminating(CScriptBase* self)
{
    if (self->flags & 8)
        return self->activeThread->terminating;
    return false;
}