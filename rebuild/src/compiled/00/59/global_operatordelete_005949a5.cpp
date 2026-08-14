#pragma optimize("s",on)
// operator delete forwarder @ 0x005949a5
extern "C" void __stdcall inner_free(void *block, void *heap, int flags);
extern "C" void __stdcall operator_delete(void *block)
{
    inner_free(block, 0, 0);
}