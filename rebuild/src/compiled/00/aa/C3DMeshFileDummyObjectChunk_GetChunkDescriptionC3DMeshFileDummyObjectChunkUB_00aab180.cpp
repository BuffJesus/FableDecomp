// C3DMeshFileDummyObjectChunk::GetChunkDescription
// Retail 0x00aab180 : virtual const CCharString GetChunkDescription() const
//
// push esi ; mov esi,[esp+8] (hidden ret buf) ; push -1 ; push 0x129e620 ;
// mov ecx,esi ; call CCharString::CCharString(const char*,int) ; mov eax,esi ; ret 4

class CCharString
{
public:
    // ctor at 0x0099ebf0 : CCharString(const char* s, int len)
    CCharString(const char* s, int len);
private:
    char* m_data;
};

class C3DMeshFileDummyObjectChunk
{
public:
    virtual const CCharString GetChunkDescription() const;
};

const CCharString C3DMeshFileDummyObjectChunk::GetChunkDescription() const
{
    return CCharString("DummyObject", -1);
}