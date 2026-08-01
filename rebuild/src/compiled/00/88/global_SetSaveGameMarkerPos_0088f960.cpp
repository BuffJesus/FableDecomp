struct C3DVector
{
    float x;
    float y;
    float z;
};

class CWorld
{
public:
    void SetSaveGameMarkerPos(const C3DVector* pos);
};

class CGameScriptInterface
{
public:
    void SetSaveGameMarkerPos(const C3DVector& pos);
    char m_Pad[0x4];
    CWorld* m_World;
};

void CGameScriptInterface::SetSaveGameMarkerPos(const C3DVector& pos)
{
    m_World->SetSaveGameMarkerPos(&pos);
}