// Reconstruction of CShaderRenderManager::UnregisterVertexShaderConstantLayouts @ 0x0098b280
// Resets an intrusive std::list-like member: count = -1, erase node range via helper,
// then splice the embedded sentinel node (self+0x18) as the list's terminator.

struct ListNode {
    ListNode* next;   // +0
    ListNode* prev;   // +4
};

struct ListImpl {
    ListNode* head;   // +0  ([edi] -> first node / _Myhead)
    // __fastcall erase helper at 0x98d870: ecx=this, stack args (one, local)
    void erase_range(int one, ListNode** local);
};

struct Self {
    char pad0[4];        // +0
    ListNode* tail;      // +4   ([esi+4])
    int        count;    // +8   ([esi+8] = -1)
    char pad_c[4];       // +0xc placeholder (ListImpl occupies +0xc)
    char sentinel[16];   // +0x18 embedded sentinel node
};

void __fastcall UnregisterVertexShaderConstantLayouts(Self* self)
{
    ListImpl* impl = (ListImpl*)((char*)self + 0xc);
    self->count = -1;
    ListNode* local = 0;
    impl->erase_range(1, &local);
    ListNode* sentinel = (ListNode*)((char*)self + 0x18);
    impl->head->next = sentinel;
    self->tail = sentinel;
}