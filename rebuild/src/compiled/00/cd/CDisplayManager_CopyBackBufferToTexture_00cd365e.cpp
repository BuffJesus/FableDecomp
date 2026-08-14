struct CTexture;

struct CDisplayManager {
    int dummy;
    void Helper(char* a, char* b);
    CTexture* CopyBackBufferToTexture();
};

#pragma optimize("s", on)
#pragma optimize("y", off)
CTexture* CDisplayManager::CopyBackBufferToTexture()
{
    char local;
    this->Helper(&local, &local + 1 - 1);
    return (CTexture*)this;
}