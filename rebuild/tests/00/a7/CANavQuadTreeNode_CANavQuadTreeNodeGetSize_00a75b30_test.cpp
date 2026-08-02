#include <cstdio>

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

float g_ANavQuadTreeNodeSizeTable[256];

int main()
{
    for (int i = 0; i < 256; ++i)
        g_ANavQuadTreeNodeSizeTable[i] = (float)(i * 2 + 1);

    CANavQuadTreeNode node;
    node.m_depth = 5;

    float result = node.GetSize();

    if (result == g_ANavQuadTreeNodeSizeTable[5])
    {
        std::printf("PASS_GETSIZE_00A75B30 result=%f\n", result);
    }
    else
    {
        std::printf("FAIL result=%f expected=%f\n", result, g_ANavQuadTreeNodeSizeTable[5]);
    }

    return 0;
}