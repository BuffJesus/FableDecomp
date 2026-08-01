class CWorld
{
public:
    void ResetToFrontEnd();
};

class CGameScriptInterface
{
public:
    void ResetToFrontEnd() const;
private:
    void* m_Pad00;
    CWorld* m_World;
};

void CGameScriptInterface::ResetToFrontEnd() const
{
    m_World->ResetToFrontEnd();
}