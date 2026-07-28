void** __fastcall Allocator_Alloc_56bytes(void** outPtr)
{
    *outPtr = nullptr;
    *outPtr = std::malloc(0x38);
    return outPtr;
}