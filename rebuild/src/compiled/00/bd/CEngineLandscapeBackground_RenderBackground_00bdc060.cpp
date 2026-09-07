// Retail 0x00BDC060. Null-guarded forward to
// CLandscapeBackgroundTreeNode::Render (0x00BF71D0). Kept as `call; ret` rather
// than tail-called because the forwarder pushes an argument the callee cleans.
struct CLandscapeBackgroundTreeNode {
    void Render(int flags);
};

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