struct CAFile { unsigned char pad[4]; };

bool __fastcall CAFile_IsIOFinished(const CAFile* self)
{
    (void)self;
    return true;
}