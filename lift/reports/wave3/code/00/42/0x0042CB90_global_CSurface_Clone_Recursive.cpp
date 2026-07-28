struct CSurfaceCloneNodeOverlay
{
    std::uint8_t pad_00[0x04];
    CSurfaceCloneNodeOverlay* parent;      // 0x04
    CSurfaceCloneNodeOverlay* nextSibling; // 0x08
    CSurfaceCloneNodeOverlay* firstChild;  // 0x0C
};

static_assert(offsetof(CSurfaceCloneNodeOverlay, parent) == 0x04);
static_assert(offsetof(CSurfaceCloneNodeOverlay, nextSibling) == 0x08);
static_assert(offsetof(CSurfaceCloneNodeOverlay, firstChild) == 0x0C);

CSurfaceCloneNodeOverlay* __thiscall CSurface_Clone_Recursive(
    CSurface* self,
    CSurfaceCloneNodeOverlay* source,
    CSurfaceCloneNodeOverlay* parent)
{
    auto* const clonedRoot =
        reinterpret_cast<CSurfaceCloneNodeOverlay*>(self->CSurface::GetHeight());
    clonedRoot->parent = parent;

    if (source->firstChild != nullptr)
    {
        auto* const clonedChild =
            CSurface_Clone_Recursive(self, source->firstChild, clonedRoot);
        clonedRoot->firstChild = clonedChild;
    }

    CSurfaceCloneNodeOverlay* clonedTail = clonedRoot;
    for (auto* sibling = source->nextSibling; sibling != nullptr; sibling = sibling->nextSibling)
    {
        auto* const clonedSibling =
            reinterpret_cast<CSurfaceCloneNodeOverlay*>(self->CSurface::GetHeight());
        clonedTail->nextSibling = clonedSibling;
        clonedSibling->parent = clonedTail;

        if (sibling->firstChild != nullptr)
        {
            auto* const clonedSiblingChild =
                CSurface_Clone_Recursive(self, sibling->firstChild, clonedSibling);
            clonedSibling->firstChild = clonedSiblingChild;
        }

        clonedTail = clonedSibling;
    }

    return clonedRoot;
}