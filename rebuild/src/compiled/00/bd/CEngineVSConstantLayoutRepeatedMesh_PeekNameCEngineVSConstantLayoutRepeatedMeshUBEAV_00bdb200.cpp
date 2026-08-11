// CEngineVSConstantLayoutRepeatedMesh::PeekName  (retail 0x00bdb200)
// virtual CCharString PeekName() const;  returns a literal string by value.

class CCharString {
public:
    CCharString(const char* s, int n);
private:
    char* m_data;
    int   m_len;
};

class CEngineVSConstantLayoutRepeatedMesh {
public:
    virtual CCharString PeekName() const;
    void* vfp;
};

CCharString CEngineVSConstantLayoutRepeatedMesh::PeekName() const {
    return CCharString("VSConstantLayoutRepeatedMesh", -1);
}