struct CSmallVector_NavNode {
    void** m_pData;
};

void* __fastcall CSmallVector_NavNode_index_a(CSmallVector_NavNode* self, void* /*edx*/, unsigned long i)
{
    return self->m_pData[i];
}