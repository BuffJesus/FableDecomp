void** __fastcall Allocator_Alloc_24bytes(void** outPtr)
{
    *outPtr = nullptr;
    *outPtr = std::malloc(0x18);
    return outPtr;
}