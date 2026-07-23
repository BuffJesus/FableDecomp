struct CNavQuadTreeNodeInitialiseOverlay
{
    void** vftable;
    float centreX;
    float centreY;
    std::uint8_t level;
    std::uint8_t flags;
    std::uint8_t stateByte;
    std::uint8_t pad0F;
    CANavQuadTreeNode* TLNode;
    CANavQuadTreeNode* TRNode;
    CANavQuadTreeNode* BLNode;
    CANavQuadTreeNode* BRNode;
};

struct UInt64VectorRaw
{
    void* first;
    void* last;
    void* end;
};

static_assert(sizeof(CNavQuadTreeNodeInitialiseOverlay) == 0x20);
static_assert(offsetof(CNavQuadTreeNodeInitialiseOverlay, centreX) == 0x04);
static_assert(offsetof(CNavQuadTreeNodeInitialiseOverlay, centreY) == 0x08);
static_assert(offsetof(CNavQuadTreeNodeInitialiseOverlay, level) == 0x0C);
static_assert(offsetof(CNavQuadTreeNodeInitialiseOverlay, flags) == 0x0D);
static_assert(offsetof(CNavQuadTreeNodeInitialiseOverlay, stateByte) == 0x0E);
static_assert(offsetof(CNavQuadTreeNodeInitialiseOverlay, TLNode) == 0x10);
static_assert(offsetof(CNavQuadTreeNodeInitialiseOverlay, TRNode) == 0x14);
static_assert(offsetof(CNavQuadTreeNodeInitialiseOverlay, BLNode) == 0x18);
static_assert(offsetof(CNavQuadTreeNodeInitialiseOverlay, BRNode) == 0x1C);
static_assert(sizeof(UInt64VectorRaw) == 0x0C);

void CNavQuadTreeNode::Initialise(
    const CNavQuadTree& quadTree,
    ENavQuadTreeNodeType& outNodeType,
    std::vector<unsigned __int64>& switchNodeIds)
{
    extern float DAT_0129dcac[];
    extern float _DAT_0123078c;
    extern void* PTR__vector_deleting_destructor__0129dcd4[];
    extern CANavQuadTreeNode DAT_013d2d2c;

    auto* const self = reinterpret_cast<CNavQuadTreeNodeInitialiseOverlay*>(this);
    auto* const quadTreePtr = const_cast<CNavQuadTree*>(&quadTree);

    bool bVar2;
    char cVar3;
    bool bVar4;
    std::uint8_t uVar5;
    CANavQuadTreeNode* pCVar6;
    CNavSwitchableLeafNode* pCVar7;
    CNavNavigableLeafNode* pCVar8;
    std::vector<unsigned __int64>* pvVar9;
    ENavQuadTreeNodeType local_44;
    ENavQuadTreeNodeType local_40;
    ENavQuadTreeNodeType local_3c;
    float local_38;
    float local_34;
    UInt64VectorRaw local_30;
    UInt64VectorRaw local_24;
    UInt64VectorRaw local_18;
    UInt64VectorRaw local_c;

    auto* param_3 = &switchNodeIds;

    self->stateByte = 0;
    outNodeType = static_cast<ENavQuadTreeNodeType>(1);

    if ((self->level < 5) ||
        ((self->level < 6) &&
         GoToHigherDetail(
             this,
             reinterpret_cast<std::vector<C2DBoxF>*>(
                 reinterpret_cast<std::byte*>(quadTreePtr) + 0x5C)))) {
        bVar2 = true;
    } else {
        bVar2 = false;
    }

    cVar3 = CNavQuadTree_InitializeLeafNode(
        this,
        reinterpret_cast<ENavQuadTreeNodeType*>(&param_3),
        param_3);

    if (cVar3 != '\0') {
        if (!bVar2) {
            return;
        }

        bVar4 = CNavQuadTree::IsAreaAllSamePreferability(
            quadTreePtr,
            reinterpret_cast<CANavQuadTreeNode*>(this));
        if (bVar4) {
            return;
        }
    }

    if (!bVar2) {
        outNodeType = static_cast<ENavQuadTreeNodeType>(
            (static_cast<char>(reinterpret_cast<std::uintptr_t>(param_3)) != '\0') + 2);
        return;
    }

    outNodeType = static_cast<ENavQuadTreeNodeType>(0);
    param_3 = nullptr;
    local_c.first = nullptr;
    local_c.last = nullptr;
    local_c.end = nullptr;

    const float fVar1 =
        DAT_0129dcac[static_cast<unsigned int>(self->level)] * _DAT_0123078c;

    local_38 = self->centreX - fVar1;
    local_34 = self->centreY - fVar1;

    pCVar6 = static_cast<CANavQuadTreeNode*>(::operator_new(0x20));
    if (pCVar6 == nullptr) {
        pCVar6 = nullptr;
    } else {
        const C2DVector centre{local_38, local_34};
        ::new (pCVar6) CANavQuadTreeNode(
            centre,
            static_cast<char>(self->level) + '\x01',
            self->flags);
        *reinterpret_cast<void***>(pCVar6) = PTR__vector_deleting_destructor__0129dcd4;
        reinterpret_cast<CNavQuadTreeNodeInitialiseOverlay*>(pCVar6)->TLNode = nullptr;
        reinterpret_cast<CNavQuadTreeNodeInitialiseOverlay*>(pCVar6)->TRNode = nullptr;
        reinterpret_cast<CNavQuadTreeNodeInitialiseOverlay*>(pCVar6)->BLNode = nullptr;
        reinterpret_cast<CNavQuadTreeNodeInitialiseOverlay*>(pCVar6)->BRNode = nullptr;
        static_cast<CNavQuadTreeNode*>(pCVar6)->Initialise(
            quadTree,
            *reinterpret_cast<ENavQuadTreeNodeType*>(&param_3),
            *reinterpret_cast<std::vector<unsigned __int64>*>(&local_c));
    }
    self->TLNode = pCVar6;

    if (param_3 == reinterpret_cast<std::vector<unsigned __int64>*>(0x1)) {
        pCVar8 = static_cast<CNavNavigableLeafNode*>(::operator_new(0x20));
        if (pCVar8 == nullptr) {
            pCVar6 = nullptr;
        } else {
            uVar5 = CNavQuadTree::GetPreferabilityCostForNode(quadTreePtr, self->TLNode);
            const C2DVector centre{local_38, local_34};
            pCVar6 = reinterpret_cast<CANavQuadTreeNode*>(
                ::new (pCVar8) CNavNavigableLeafNode(
                    centre,
                    static_cast<char>(self->level) + '\x01',
                    self->flags,
                    uVar5));
        }
        SetBLNode(pCVar6);
    } else {
        if (param_3 == reinterpret_cast<std::vector<unsigned __int64>*>(0x2)) {
            pCVar6 = &DAT_013d2d2c;
            SetBLNode(pCVar6);
        } else {
            if (param_3 == reinterpret_cast<std::vector<unsigned __int64>*>(0x3)) {
                pCVar7 = static_cast<CNavSwitchableLeafNode*>(::operator_new(0x30));
                if (pCVar7 == nullptr) {
                    pCVar6 = nullptr;
                } else {
                    pvVar9 = reinterpret_cast<std::vector<unsigned __int64>*>(&local_c);
                    uVar5 = CNavQuadTree::GetPreferabilityCostForNode(quadTreePtr, self->TLNode);
                    const C2DVector centre{local_38, local_34};
                    pCVar6 = reinterpret_cast<CANavQuadTreeNode*>(
                        ::new (pCVar7) CNavSwitchableLeafNode(
                            centre,
                            static_cast<char>(self->level) + '\x01',
                            self->flags,
                            uVar5,
                            pvVar9));
                }
                SetBLNode(pCVar6);
            }
        }
    }

    local_38 = fVar1 + self->centreX;
    local_44 = static_cast<ENavQuadTreeNodeType>(0);
    local_18.first = nullptr;
    local_18.last = nullptr;
    local_18.end = nullptr;
    local_34 = self->centreY - fVar1;

    pCVar6 = static_cast<CANavQuadTreeNode*>(::operator_new(0x20));
    if (pCVar6 == nullptr) {
        pCVar6 = nullptr;
    } else {
        const C2DVector centre{local_38, local_34};
        ::new (pCVar6) CANavQuadTreeNode(
            centre,
            static_cast<char>(self->level) + '\x01',
            self->flags);
        *reinterpret_cast<void***>(pCVar6) = PTR__vector_deleting_destructor__0129dcd4;
        reinterpret_cast<CNavQuadTreeNodeInitialiseOverlay*>(pCVar6)->TLNode = nullptr;
        reinterpret_cast<CNavQuadTreeNodeInitialiseOverlay*>(pCVar6)->TRNode = nullptr;
        reinterpret_cast<CNavQuadTreeNodeInitialiseOverlay*>(pCVar6)->BLNode = nullptr;
        reinterpret_cast<CNavQuadTreeNodeInitialiseOverlay*>(pCVar6)->BRNode = nullptr;
        static_cast<CNavQuadTreeNode*>(pCVar6)->Initialise(
            quadTree,
            local_44,
            *reinterpret_cast<std::vector<unsigned __int64>*>(&local_18));
    }
    self->TRNode = pCVar6;

    if (local_44 == static_cast<ENavQuadTreeNodeType>(1)) {
        pCVar8 = static_cast<CNavNavigableLeafNode*>(::operator_new(0x20));
        if (pCVar8 == nullptr) {
            pCVar6 = nullptr;
        } else {
            uVar5 = CNavQuadTree::GetPreferabilityCostForNode(quadTreePtr, self->TRNode);
            const C2DVector centre{local_38, local_34};
            pCVar6 = reinterpret_cast<CANavQuadTreeNode*>(
                ::new (pCVar8) CNavNavigableLeafNode(
                    centre,
                    static_cast<char>(self->level) + '\x01',
                    self->flags,
                    uVar5));
        }
        SetBLNode(pCVar6);
    } else {
        if (local_44 == static_cast<ENavQuadTreeNodeType>(2)) {
            pCVar6 = &DAT_013d2d2c;
            SetBLNode(pCVar6);
        } else {
            if (local_44 == static_cast<ENavQuadTreeNodeType>(3)) {
                pCVar7 = static_cast<CNavSwitchableLeafNode*>(::operator_new(0x30));
                if (pCVar7 == nullptr) {
                    pCVar6 = nullptr;
                } else {
                    pvVar9 = reinterpret_cast<std::vector<unsigned __int64>*>(&local_18);
                    uVar5 = CNavQuadTree::GetPreferabilityCostForNode(quadTreePtr, self->TRNode);
                    const C2DVector centre{local_38, local_34};
                    pCVar6 = reinterpret_cast<CANavQuadTreeNode*>(
                        ::new (pCVar7) CNavSwitchableLeafNode(
                            centre,
                            static_cast<char>(self->level) + '\x01',
                            self->flags,
                            uVar5,
                            pvVar9));
                }
                SetBLNode(pCVar6);
            }
        }
    }

    local_38 = self->centreX - fVar1;
    local_40 = static_cast<ENavQuadTreeNodeType>(0);
    local_24.first = nullptr;
    local_24.last = nullptr;
    local_24.end = nullptr;
    local_34 = fVar1 + self->centreY;

    pCVar6 = static_cast<CANavQuadTreeNode*>(::operator_new(0x20));
    if (pCVar6 == nullptr) {
        pCVar6 = nullptr;
    } else {
        const C2DVector centre{local_38, local_34};
        ::new (pCVar6) CANavQuadTreeNode(
            centre,
            static_cast<char>(self->level) + '\x01',
            self->flags);
        *reinterpret_cast<void***>(pCVar6) = PTR__vector_deleting_destructor__0129dcd4;
        reinterpret_cast<CNavQuadTreeNodeInitialiseOverlay*>(pCVar6)->TLNode = nullptr;
        reinterpret_cast<CNavQuadTreeNodeInitialiseOverlay*>(pCVar6)->TRNode = nullptr;
        reinterpret_cast<CNavQuadTreeNodeInitialiseOverlay*>(pCVar6)->BLNode = nullptr;
        reinterpret_cast<CNavQuadTreeNodeInitialiseOverlay*>(pCVar6)->BRNode = nullptr;
        static_cast<CNavQuadTreeNode*>(pCVar6)->Initialise(
            quadTree,
            local_40,
            *reinterpret_cast<std::vector<unsigned __int64>*>(&local_24));
    }
    self->BLNode = pCVar6;

    if (local_40 == static_cast<ENavQuadTreeNodeType>(1)) {
        pCVar8 = static_cast<CNavNavigableLeafNode*>(::operator_new(0x20));
        if (pCVar8 == nullptr) {
            pCVar6 = nullptr;
        } else {
            uVar5 = CNavQuadTree::GetPreferabilityCostForNode(quadTreePtr, self->BLNode);
            const C2DVector centre{local_38, local_34};
            pCVar6 = reinterpret_cast<CANavQuadTreeNode*>(
                ::new (pCVar8) CNavNavigableLeafNode(
                    centre,
                    static_cast<char>(self->level) + '\x01',
                    self->flags,
                    uVar5));
        }
        SetBLNode(pCVar6);
    } else {
        if (local_40 == static_cast<ENavQuadTreeNodeType>(2)) {
            pCVar6 = &DAT_013d2d2c;
            SetBLNode(pCVar6);
        } else {
            if (local_40 == static_cast<ENavQuadTreeNodeType>(3)) {
                pCVar7 = static_cast<CNavSwitchableLeafNode*>(::operator_new(0x30));
                if (pCVar7 == nullptr) {
                    pCVar6 = nullptr;
                } else {
                    pvVar9 = reinterpret_cast<std::vector<unsigned __int64>*>(&local_24);
                    uVar5 = CNavQuadTree::GetPreferabilityCostForNode(quadTreePtr, self->BLNode);
                    const C2DVector centre{local_38, local_34};
                    pCVar6 = reinterpret_cast<CANavQuadTreeNode*>(
                        ::new (pCVar7) CNavSwitchableLeafNode(
                            centre,
                            static_cast<char>(self->level) + '\x01',
                            self->flags,
                            uVar5,
                            pvVar9));
                }
                SetBLNode(pCVar6);
            }
        }
    }

    local_38 = fVar1 + self->centreX;
    local_3c = static_cast<ENavQuadTreeNodeType>(0);
    local_30.first = nullptr;
    local_30.last = nullptr;
    local_30.end = nullptr;
    local_34 = fVar1 + self->centreY;

    pCVar6 = static_cast<CANavQuadTreeNode*>(::operator_new(0x20));
    if (pCVar6 == nullptr) {
        pCVar6 = nullptr;
    } else {
        const C2DVector centre{local_38, local_34};
        ::new (pCVar6) CANavQuadTreeNode(
            centre,
            static_cast<char>(self->level) + '\x01',
            self->flags);
        *reinterpret_cast<void***>(pCVar6) = PTR__vector_deleting_destructor__0129dcd4;
        reinterpret_cast<CNavQuadTreeNodeInitialiseOverlay*>(pCVar6)->TLNode = nullptr;
        reinterpret_cast<CNavQuadTreeNodeInitialiseOverlay*>(pCVar6)->TRNode = nullptr;
        reinterpret_cast<CNavQuadTreeNodeInitialiseOverlay*>(pCVar6)->BLNode = nullptr;
        reinterpret_cast<CNavQuadTreeNodeInitialiseOverlay*>(pCVar6)->BRNode = nullptr;
        static_cast<CNavQuadTreeNode*>(pCVar6)->Initialise(
            quadTree,
            local_3c,
            *reinterpret_cast<std::vector<unsigned __int64>*>(&local_30));
    }
    self->BRNode = pCVar6;

    if (local_3c == static_cast<ENavQuadTreeNodeType>(1)) {
        pCVar8 = static_cast<CNavNavigableLeafNode*>(::operator_new(0x20));
        if (pCVar8 == nullptr) {
            pCVar6 = nullptr;
        } else {
            uVar5 = CNavQuadTree::GetPreferabilityCostForNode(quadTreePtr, self->BRNode);
            const C2DVector centre{local_38, local_34};
            pCVar6 = reinterpret_cast<CANavQuadTreeNode*>(
                ::new (pCVar8) CNavNavigableLeafNode(
                    centre,
                    static_cast<char>(self->level) + '\x01',
                    self->flags,
                    uVar5));
        }
    } else if (local_3c == static_cast<ENavQuadTreeNodeType>(2)) {
        pCVar6 = &DAT_013d2d2c;
    } else {
        if (local_3c != static_cast<ENavQuadTreeNodeType>(3)) {
            goto LAB_00a7a008;
        }
        pCVar7 = static_cast<CNavSwitchableLeafNode*>(::operator_new(0x30));
        if (pCVar7 == nullptr) {
            pCVar6 = nullptr;
        } else {
            pvVar9 = reinterpret_cast<std::vector<unsigned __int64>*>(&local_30);
            uVar5 = CNavQuadTree::GetPreferabilityCostForNode(quadTreePtr, self->BRNode);
            const C2DVector centre{local_38, local_34};
            pCVar6 = reinterpret_cast<CANavQuadTreeNode*>(
                ::new (pCVar7) CNavSwitchableLeafNode(
                    centre,
                    static_cast<char>(self->level) + '\x01',
                    self->flags,
                    uVar5,
                    pvVar9));
        }
    }
    SetBLNode(pCVar6);

LAB_00a7a008:
    if ((param_3 == reinterpret_cast<std::vector<unsigned __int64>*>(0x2)) &&
        (local_44 == static_cast<ENavQuadTreeNodeType>(2)) &&
        (local_40 == static_cast<ENavQuadTreeNodeType>(2)) &&
        (local_3c == static_cast<ENavQuadTreeNodeType>(2))) {
        outNodeType = static_cast<ENavQuadTreeNodeType>(2);
    }

    if (local_30.first != nullptr) {
        free(local_30.first);
    }
    if (local_24.first != nullptr) {
        free(local_24.first);
    }
    if (local_18.first != nullptr) {
        free(local_18.first);
    }
    if (local_c.first == nullptr) {
        return;
    }
    free(local_c.first);
}