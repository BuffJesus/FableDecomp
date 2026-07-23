struct CDiskFileWin32 {
    char pad[0xc];
    long length;
};

long __fastcall CDiskFileWin32_GetLength(const CDiskFileWin32* self) {
    return self->length;
}