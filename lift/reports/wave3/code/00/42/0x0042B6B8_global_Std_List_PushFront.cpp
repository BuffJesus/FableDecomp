struct StdListOverlay {
    void* first;
};

static_assert(offsetof(StdListOverlay, first) == 0x0);

void __thiscall Std_List_PushFront(StdListOverlay* self, void* value)
{
    Std_List_Insert(&value, self->first, value);
}