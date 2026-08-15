#include <cstdio>

struct ListNode { ListNode* next; ListNode* prev; };
struct ListImpl { ListNode* head; };
struct Self {
    char pad0[4];
    ListNode* tail;
    int count;
    char pad_c[4];
    char sentinel_area[64];
};

static bool g_erase_called = false;
static int g_erase_one = 0;
static ListNode* g_local_seen = (ListNode*)1;

void list_erase_range(ListImpl* impl, ListNode** local, int one) {
    g_erase_called = true;
    g_erase_one = one;
    g_local_seen = *local; // should be null (local initialized to 0)
    (void)impl;
}

void model_reset(Self* self) {
    ListNode* local = 0;
    ListImpl* impl = (ListImpl*)((char*)self + 0xc);
    self->count = -1;
    list_erase_range(impl, &local, 1);
    ListNode* sentinel = (ListNode*)((char*)self + 0x18);
    impl->head->next = sentinel;
    self->tail = sentinel;
}

int main() {
    Self s;
    s.count = 999;
    s.tail = (ListNode*)0xBADBEEF;
    ListImpl* impl = (ListImpl*)((char*)&s + 0xc);
    // point head at a real node so head->next is writable
    static ListNode firstNode;
    impl->head = &firstNode;

    model_reset(&s);

    ListNode* sentinel = (ListNode*)((char*)&s + 0x18);
    bool ok = true;
    if (s.count != -1) ok = false;
    if (s.tail != sentinel) ok = false;
    if (firstNode.next != sentinel) ok = false;
    if (!g_erase_called) ok = false;
    if (g_erase_one != 1) ok = false;
    if (g_local_seen != 0) ok = false;

    if (ok) printf("PARITY_OK_0098b280\n");
    else printf("FAIL count=%d\n", s.count);
    return 0;
}