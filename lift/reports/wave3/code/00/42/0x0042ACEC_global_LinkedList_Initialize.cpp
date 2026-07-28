void** __fastcall LinkedList_Initialize(void** out_list)
{
    *out_list = nullptr;

    void* const list = std::malloc(0x18);
    *out_list = list;

    return out_list;
}