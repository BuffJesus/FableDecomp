// CWorld::DrawGetEnvironment  0x0049e350
// mov eax, [ecx+0x14]; ret  -> returns member pointer at offset 0x14
class CIDrawEnvironment;

class CWorld {
public:
    char pad_0x00[0x14];
    CIDrawEnvironment* m_drawEnvironment; // +0x14
    const CIDrawEnvironment* DrawGetEnvironment();
};

const CIDrawEnvironment* CWorld::DrawGetEnvironment()
{
    return m_drawEnvironment;
}