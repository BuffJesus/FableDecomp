struct CSimpleMemoryPool {
    void* vtbl;
    char pad[0x21c - 4];
    unsigned long capacity;
};

extern void* const CSimpleMemoryPool_vtbl;

CSimpleMemoryPool* __fastcall SetCapacity(CSimpleMemoryPool* self) {
    self->vtbl = (void*)&CSimpleMemoryPool_vtbl;
    self->capacity = (((unsigned long)self) + 0x1b) & 0xfffffff0;
    return self;
}