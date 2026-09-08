struct LHHttpDocumentParts {
    char* Buffer;
    unsigned long BufferLength;
    unsigned long CurrBufPos;
};
struct LHHttpDocumentPartVectorRetail {
    LHHttpDocumentParts** Begin;
    LHHttpDocumentParts** End;
    LHHttpDocumentParts** CapacityEnd;
};
// Retail places DocumentParts at +0x884; Ego_r places the same PDB member
// later because of debug-layout string/container expansion.
struct LHHttp2Retail {
    unsigned char Reserved[0x884];
    LHHttpDocumentPartVectorRetail DocumentParts;
    unsigned long GetDocumentSize() const;
};
unsigned long LHHttp2Retail::GetDocumentSize() const
{
    unsigned long size = 0;
    for (LHHttpDocumentParts** part = DocumentParts.Begin; part != DocumentParts.End; ++part) {
        if (*part != 0)
            size += (*part)->BufferLength;
    }
    return size;
}