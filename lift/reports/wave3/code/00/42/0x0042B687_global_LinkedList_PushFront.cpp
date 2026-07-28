void __thiscall LinkedList_PushFront(std::uint32_t* self, std::uint32_t value)
{
    LinkedList_Insert_Before(&value, *self, value);
}