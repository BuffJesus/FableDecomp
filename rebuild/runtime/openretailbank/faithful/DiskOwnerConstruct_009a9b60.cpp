struct CFileInfo;

extern "C" void* __cdecl operator_new_impl(unsigned int size);

struct CCountedBlock
{
    int refCount;
    void* vtable;
    CFileInfo* ptr;
};

struct CCountedPointer_CFileInfo
{
    CFileInfo* m_ptr;
    CCountedBlock* m_block;
};

extern "C" CCountedPointer_CFileInfo* __fastcall CCountedPointer_CFileInfo_construct_b(CCountedPointer_CFileInfo* self, void* edx, CFileInfo* p)
{
    self->m_ptr = p;
    if (p != 0)
    {
        CCountedBlock* block = (CCountedBlock*)operator_new_impl(0xc);
        if (block != 0)
        {
            CFileInfo* cur = self->m_ptr;
            block->refCount = 1;
            block->vtable = (void*)0x009a9030;
            block->ptr = cur;
            self->m_block = block;
            return self;
        }
        self->m_block = (CCountedBlock*)0;
        return self;
    }
    self->m_block = 0;
    return self;
}
