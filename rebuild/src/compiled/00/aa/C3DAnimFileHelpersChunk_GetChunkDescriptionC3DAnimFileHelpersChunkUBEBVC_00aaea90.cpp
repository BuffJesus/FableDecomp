// C3DAnimFileHelpersChunk::GetChunkDescription - retail 0x00aaea90
// const method returning a fixed CCharString by value (hidden NRV return slot).

class CCharString {
public:
    CCharString(const char* s, int n);   // extern ctor -> real call 0x99ebf0
    char* m_data;
};

class C3DAnimFileHelpersChunk {
public:
    CCharString GetChunkDescription() const;
    int m_dummy;
};

CCharString C3DAnimFileHelpersChunk::GetChunkDescription() const
{
    return CCharString((const char*)0x129e564, -1);
}