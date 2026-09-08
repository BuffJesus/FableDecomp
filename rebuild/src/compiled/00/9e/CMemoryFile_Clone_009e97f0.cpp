struct CAFile { void* VTable; };

struct CMemoryFileRetail : CAFile {
    unsigned long AccessType;
    long Position;
    long Length;
    unsigned char DataBuffer[0x0c]; // retail CArray; donor representation is 0x10
    bool Opened;
    unsigned char _pad_0x1d[3];
    void* Pathname;

    CMemoryFileRetail(const CMemoryFileRetail& source);
    CAFile* Clone() const;
};

CAFile* CMemoryFileRetail::Clone() const {
    return new CMemoryFileRetail(*this);
}