extern const int CChunkedFileChunk_vftable;
void __fastcall BaseDtor(void* self);
__declspec(naked) void __fastcall CChunkedFileChunk_DtorChunk(void* self)
{
    (void)self;
    __asm {
        mov dword ptr [ecx], offset CChunkedFileChunk_vftable
        jmp BaseDtor
    }
}