struct CTexture; enum ETextureType {};
struct CGraphicDataBank { CTexture* GetBlankTexture(ETextureType t); };
CTexture* CGraphicDataBank::GetBlankTexture(ETextureType t){ return (CTexture*)((char*)this + (int)t*40 + 8); }