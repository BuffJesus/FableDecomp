extern float g_ANavQuadTreeNodeSizeTable[256];

class CANavQuadTreeNode
{
public:
    unsigned char m_pad0[0xc];
    unsigned char m_depth;

    float GetSize();
};

float CANavQuadTreeNode::GetSize()
{
    return g_ANavQuadTreeNodeSizeTable[m_depth];
}