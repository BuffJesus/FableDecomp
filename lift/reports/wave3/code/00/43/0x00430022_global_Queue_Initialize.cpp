void** __fastcall Queue_Initialize(void** out_queue)
{
    *out_queue = nullptr;

    void* const allocation = std::malloc(0x18);
    *out_queue = allocation;

    return out_queue;
}