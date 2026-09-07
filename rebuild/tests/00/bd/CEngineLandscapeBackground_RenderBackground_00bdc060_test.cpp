#include <stdio.h>
static int g_renders = 0;
static int g_lastFlags = -1;
struct CLandscapeBackgroundTreeNode {
    void Render(int flags);
};
void CLandscapeBackgroundTreeNode::Render(int flags) { g_renders++; g_lastFlags = flags; }

struct CEngineLandscapeBackground {
    void* m_pUnknown0;
    CLandscapeBackgroundTreeNode* m_pRootNode;
    void RenderBackground();
};
void CEngineLandscapeBackground::RenderBackground() {
    if (m_pRootNode != 0) {
        m_pRootNode->Render(0);
    }
}
int main() {
    CEngineLandscapeBackground background;
    background.m_pUnknown0 = 0;
    background.m_pRootNode = 0;
    background.RenderBackground();
    if (g_renders != 0) return 1;
    CLandscapeBackgroundTreeNode root;
    background.m_pRootNode = &root;
    background.RenderBackground();
    if (g_renders != 1) return 2;
    if (g_lastFlags != 0) return 3;
    printf("RENDER_BACKGROUND PASS\n");
    return 0;
}