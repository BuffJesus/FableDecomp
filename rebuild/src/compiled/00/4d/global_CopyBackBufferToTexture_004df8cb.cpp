struct CTexture;

struct CDisplayManager {
    void Helper(char *a, char *b);
    CTexture * CopyBackBufferToTexture();
};

#pragma optimize("t",off)
CTexture * CDisplayManager::CopyBackBufferToTexture()
{
    char *pa, *pb;
    { char a; pa = &a; }
    { char b; pb = &b; }
    Helper(pa, pb);
    return (CTexture *)this;
}