// CActiveFile::OnReadFinished @ 00bf3160
struct CActiveFile {
    void* buffer;
};

// callee at 0xbf29e0: takes pointer arg on stack, cleans up itself (no add esp,4 in caller)
void __stdcall ReleaseBuffer(void* p);

void __fastcall OnReadFinished(CActiveFile* self)
{
    if (self->buffer)
    {
        ReleaseBuffer(self->buffer);
        self->buffer = 0;
    }
}