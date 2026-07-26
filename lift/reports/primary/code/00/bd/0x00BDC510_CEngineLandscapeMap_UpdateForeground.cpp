namespace
{
struct CRefCountBlock
{
    int m_nRefCount;     // +0x00
    void* m_pDestroy;    // +0x04
    void* m_pObject;     // +0x08
};

template <typename T>
struct CCountedPointer
{
    T* m_pObject;                 // +0x00
    CRefCountBlock* m_pRefCount;  // +0x04

    CCountedPointer& operator=(const CCountedPointer& rhs);
};

struct CBackgroundTreeBounds
{
    std::byte m_pad00[0xA8];
    float m_fMinX;  // +0xA8
    float m_fMinY;  // +0xAC
    float m_fMinZ;  // +0xB0
    float m_fMaxX;  // +0xB4
    float m_fMaxY;  // +0xB8
    float m_fMaxZ;  // +0xBC
};

struct CEngineLandscapeMapLayout
{
    std::byte m_pad00[0x04];
    CBackgroundTreeBounds* m_pBackgroundTree;          // 0x04
    CEngineLandscapePatch* m_pForegroundPatches;       // 0x08
    int m_nPatchColumns;                               // 0x0C
    int m_nPatchRows;                                  // 0x10
    std::byte m_pad14[0x04];
    CBankFileAsyncEntry* m_pBankEntry;                 // 0x18
    CRefCountBlock* m_pBankEntryRefCount;              // 0x1C
    std::byte m_pad20[0x0C];
    unsigned long m_uForegroundBlockOffset;            // 0x2C
    std::byte m_pad30[0x08];
    CCountedPointer<CBankFileAsyncData> m_ForegroundAsync; // 0x38
    std::byte m_pad40[0x1C];
    bool m_bUpdateForegroundActive;                    // 0x5C
    bool m_bKeepForegroundLoaded;                      // 0x5D
};

static_assert(sizeof(CCountedPointer<CBankFileAsyncData>) == 8);
static_assert(sizeof(CEngineLandscapePatch) == 0x48);
static_assert(offsetof(CEngineLandscapeMapLayout, m_pBackgroundTree) == 0x04);
static_assert(offsetof(CEngineLandscapeMapLayout, m_pForegroundPatches) == 0x08);
static_assert(offsetof(CEngineLandscapeMapLayout, m_nPatchColumns) == 0x0C);
static_assert(offsetof(CEngineLandscapeMapLayout, m_nPatchRows) == 0x10);
static_assert(offsetof(CEngineLandscapeMapLayout, m_pBankEntry) == 0x18);
static_assert(offsetof(CEngineLandscapeMapLayout, m_pBankEntryRefCount) == 0x1C);
static_assert(offsetof(CEngineLandscapeMapLayout, m_uForegroundBlockOffset) == 0x2C);
static_assert(offsetof(CEngineLandscapeMapLayout, m_ForegroundAsync) == 0x38);
static_assert(offsetof(CEngineLandscapeMapLayout, m_bUpdateForegroundActive) == 0x5C);
static_assert(offsetof(CEngineLandscapeMapLayout, m_bKeepForegroundLoaded) == 0x5D);

using PatchCtorFn = void(__cdecl*)();
using RefReleaseFn = void(__thiscall*)(void);

extern int* operator_new__(int size);
extern void operator_delete__(void* p);
extern void operator_delete(void* p);
extern void _vector_constructor_iterator_(void* base, int elementSize, int count, PatchCtorFn ctor);
extern void NHeroInformationScreens_Initialize();

extern int* __cdecl std__Cons_val__foreground_patch_array(
    int sizeBytes,
    CEngineLandscapeMap* self,
    pair<unsigned long, CAnimationEntry>* unaff_EDI);

extern void __cdecl std__Dest_val__counted_ptr_async(
    void* storage,
    CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>* extraout_EDX);
}

LS_LOADING_STATUS CEngineLandscapeMap::UpdateForeground()
{
    auto& self = *reinterpret_cast<CEngineLandscapeMapLayout*>(this);

    float local_34;
    float local_30;
    float local_28;
    float local_24;
    CMemoryDataInputStream local_1c[28];

    float* pfVar12 = reinterpret_cast<float*>(DAT_01436ea0 + 0x54);
    int* piVar13 = nullptr;

    self.m_bUpdateForegroundActive = true;

    if (self.m_pBackgroundTree != nullptr)
    {
        CBackgroundTreeBounds* const pBounds = self.m_pBackgroundTree;
        float fVar17;

        if (DAT_013d2880 == '\0')
        {
            local_34 = *pfVar12;
            fVar17 = *reinterpret_cast<float*>(DAT_01436ea0 + 0x5C);
            local_30 = *reinterpret_cast<float*>(DAT_01436ea0 + 0x58);

            if (pBounds->m_fMinX <= local_34)
            {
                if (pBounds->m_fMaxX < local_34)
                {
                    local_34 = pBounds->m_fMaxX;
                }
            }
            else
            {
                local_34 = pBounds->m_fMinX;
            }

            if (pBounds->m_fMinY <= local_30)
            {
                if (pBounds->m_fMaxY < local_30)
                {
                    local_30 = pBounds->m_fMaxY;
                }
            }
            else
            {
                local_30 = pBounds->m_fMinY;
            }

            if (pBounds->m_fMinZ <= fVar17)
            {
                if (pBounds->m_fMaxZ < fVar17)
                {
                    fVar17 = pBounds->m_fMaxZ;
                }
            }
            else
            {
                fVar17 = pBounds->m_fMinZ;
            }

            local_28 = *pfVar12 - local_34;
            local_24 = *reinterpret_cast<float*>(DAT_01436ea0 + 0x58) - local_30;
            fVar17 = *reinterpret_cast<float*>(DAT_01436ea0 + 0x5C) - fVar17;
            fVar17 = local_28 * local_28 + local_24 * local_24 + fVar17 * fVar17;
        }
        else
        {
            const std::uint64_t uVar2 = *reinterpret_cast<const std::uint64_t*>(pfVar12);

            alignas(16) float clampBaseRaw[4] = {
                *reinterpret_cast<float*>(DAT_01436ea0 + 0x5C),
                0.0f,
                *reinterpret_cast<const float*>(&uVar2),
                *reinterpret_cast<const float*>(reinterpret_cast<const std::byte*>(&uVar2) + 4),
            };
            alignas(16) float minsRaw[4] = {
                pBounds->m_fMinZ,
                0.0f,
                pBounds->m_fMinX,
                pBounds->m_fMinY,
            };
            alignas(16) float maxsRaw[4] = {
                pBounds->m_fMaxZ,
                0.0f,
                pBounds->m_fMaxX,
                pBounds->m_fMaxY,
            };

            __m128 auVar18 = _mm_load_ps(clampBaseRaw);
            __m128 auVar19 = _mm_load_ps(minsRaw);
            auVar19 = _mm_max_ps(auVar18, auVar19);

            const __m128 auVar5 = _mm_load_ps(maxsRaw);
            auVar19 = _mm_min_ps(auVar19, auVar5);

            alignas(16) float clamped[4];
            _mm_store_ps(clamped, auVar19);

            fVar17 = clamped[0] - *reinterpret_cast<float*>(DAT_01436ea0 + 0x5C);
            const float fVar20 = clamped[2] - *reinterpret_cast<const float*>(&uVar2);
            const float fVar21 =
                clamped[3] - *reinterpret_cast<const float*>(reinterpret_cast<const std::byte*>(&uVar2) + 4);
            fVar17 = fVar20 * fVar20 + fVar21 * fVar21 + fVar17 * fVar17;
            local_34 = fVar17;
        }

        const float fRange = *reinterpret_cast<float*>(DAT_01436ea8 + 0x618);
        if (fVar17 < fRange * fRange)
        {
            if ((self.m_pForegroundPatches == nullptr) && (self.m_pBankEntry != nullptr))
            {
                if (self.m_ForegroundAsync.m_pObject == nullptr)
                {
                    if (self.m_uForegroundBlockOffset != 0)
                    {
                        CCountedPointer<CDiskFileWin32>* pCVar7 =
                            reinterpret_cast<CCountedPointer<CDiskFileWin32>*>(
                                CBankFileAsyncEntry::BeginReadDataAutoAlloc(
                                    self.m_pBankEntry,
                                    static_cast<unsigned long>(&local_34),
                                    self.m_uForegroundBlockOffset,
                                    self.m_nPatchRows * self.m_nPatchColumns * 0x24));

                        self.m_ForegroundAsync.operator=(
                            *reinterpret_cast<const CCountedPointer<CBankFileAsyncData>*>(pCVar7));

                        std__Dest_val__counted_ptr_async(
                            &local_34,
                            reinterpret_cast<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>*>(pCVar7));

                        goto LAB_00bdc6c6;
                    }

                    goto LAB_00bdc96d;
                }

                if (!CBankFileAsyncData::IsFinished(self.m_ForegroundAsync.m_pObject))
                {
LAB_00bdc6c6:
                    self.m_bUpdateForegroundActive = false;
                    return LS_LOADING_STATUS(2);
                }

                CBankFileAsyncData* const pAsyncData = self.m_ForegroundAsync.m_pObject;
                const unsigned long uVar22 =
                    *reinterpret_cast<unsigned long*>(reinterpret_cast<std::byte*>(pAsyncData) + 0x10);
                void* const pvVar8 = CBankFileAsyncData::GetData(pAsyncData);
                CMemoryDataInputStream::CMemoryDataInputStream(local_1c, pvVar8, uVar22);

                int iVar15 = self.m_nPatchRows * self.m_nPatchColumns;
                int* piVar9;

                if (iVar15 == 0)
                {
                    piVar9 = operator_new__(4);
                }
                else
                {
                    piVar9 = std__Cons_val__foreground_patch_array(iVar15 * 0x48 + 4, this, nullptr);
                }

                if (piVar9 != nullptr)
                {
                    piVar13 = piVar9 + 1;
                    *piVar9 = iVar15;
                    _vector_constructor_iterator_(piVar13, 0x48, iVar15, NHeroInformationScreens_Initialize);
                }

                self.m_pForegroundPatches = reinterpret_cast<CEngineLandscapePatch*>(piVar13);

                if (piVar13 == nullptr)
                {
LAB_00bdc886:
                    self.m_bUpdateForegroundActive = false;
                    CChunkedFileChunk::~CChunkedFileChunk(reinterpret_cast<CChunkedFileChunk*>(local_1c));
                    return LS_LOADING_STATUS(3);
                }

                {
                    const unsigned int uVar10 =
                        static_cast<unsigned int>(self.m_nPatchColumns * self.m_nPatchRows);
                    const int iVar15Sign = static_cast<int>(uVar10) >> 0x1F;

                    if (((iVar15Sign < 0) || ((static_cast<int>(uVar10) < 0) && (-1 < iVar15Sign))) ||
                        ((-1 < iVar15Sign) && (0x7fffffff < uVar10)))
                    {
                        goto LAB_00bdc886;
                    }
                }

                {
                    int iVar15 = 0;
                    if (0 < self.m_nPatchRows)
                    {
                        do
                        {
                            if (0 < self.m_nPatchColumns)
                            {
                                int iVar16 = 0;
                                do
                                {
                                    auto* const pPatch = reinterpret_cast<CEngineLandscapePatch*>(
                                        reinterpret_cast<std::byte*>(self.m_pForegroundPatches) +
                                        (self.m_nPatchColumns * iVar15 + iVar16) * 0x48);

                                    CEngineLandscapePatch::Initialise(pPatch, this, iVar16 << 4, iVar15 << 4);
                                    CEngineLandscapePatch::LoadHeader(pPatch, local_1c);

                                    iVar16 = iVar16 + 1;
                                } while (iVar16 < self.m_nPatchColumns);
                            }

                            iVar15 = iVar15 + 1;
                        } while (iVar15 < self.m_nPatchRows);
                    }
                }

                {
                    CRefCountBlock* const pRef = self.m_ForegroundAsync.m_pRefCount;
                    if (pRef != nullptr)
                    {
                        pRef->m_nRefCount = pRef->m_nRefCount - 1;
                        if (self.m_ForegroundAsync.m_pRefCount->m_nRefCount == 0)
                        {
                            void* const pDestroy = *reinterpret_cast<void**>(
                                reinterpret_cast<std::byte*>(self.m_ForegroundAsync.m_pRefCount) + 0x04);
                            void* const pObject = *reinterpret_cast<void**>(
                                reinterpret_cast<std::byte*>(self.m_ForegroundAsync.m_pRefCount) + 0x08);
                            reinterpret_cast<RefReleaseFn>(pDestroy)
                                ->operator()(reinterpret_cast<void*>(pObject));
                            operator_delete(self.m_ForegroundAsync.m_pRefCount);
                        }
                    }
                }

                self.m_ForegroundAsync.m_pRefCount = nullptr;
                self.m_ForegroundAsync.m_pObject = nullptr;
                CChunkedFileChunk::~CChunkedFileChunk(reinterpret_cast<CChunkedFileChunk*>(local_1c));
            }

            if (self.m_pForegroundPatches != nullptr)
            {
                int iVar15 = 0;
                LS_LOADING_STATUS LVar14 = LS_LOADING_STATUS(1);

                if (0 < self.m_nPatchRows)
                {
                    do
                    {
                        if (0 < self.m_nPatchColumns)
                        {
                            int iVar16 = 0;
                            do
                            {
                                CBankFileAsyncEntry* const uVar3 = self.m_pBankEntry;
                                CRefCountBlock* const piVar13Ref = self.m_pBankEntryRefCount;

                                if (piVar13Ref != nullptr)
                                {
                                    piVar13Ref->m_nRefCount = piVar13Ref->m_nRefCount + 1;
                                }

                                const LS_LOADING_STATUS LVar11 =
                                    CEngineLandscapePatch::UpdateForeground(
                                        reinterpret_cast<CEngineLandscapePatch*>(
                                            reinterpret_cast<std::byte*>(self.m_pForegroundPatches) +
                                            (self.m_nPatchColumns * iVar15 + iVar16) * 0x48),
                                        uVar3,
                                        reinterpret_cast<int*>(piVar13Ref));

                                if (LVar11 == LS_LOADING_STATUS(3))
                                {
                                    self.m_bUpdateForegroundActive = false;
                                    return LS_LOADING_STATUS(3);
                                }

                                if (LVar11 == LS_LOADING_STATUS(2))
                                {
                                    LVar14 = LS_LOADING_STATUS(2);
                                }

                                iVar16 = iVar16 + 1;
                            } while (iVar16 < self.m_nPatchColumns);
                        }

                        iVar15 = iVar15 + 1;
                    } while (iVar15 < self.m_nPatchRows);
                }

                self.m_bUpdateForegroundActive = false;
                return LVar14;
            }

            goto LAB_00bdc96d;
        }
    }

    if (self.m_ForegroundAsync.m_pObject != nullptr)
    {
        CBankFileAsyncData::DiscardAsyncData(&self.m_ForegroundAsync);
    }

    {
        int* const puVar4 = reinterpret_cast<int*>(self.m_pForegroundPatches);
        if (puVar4 != nullptr)
        {
            if (!self.m_bKeepForegroundLoaded)
            {
                if (puVar4[-1] != 0)
                {
                    (**reinterpret_cast<void(__thiscall***)(void*, int)>(puVar4))(puVar4, 3);
                    self.m_pForegroundPatches = nullptr;
                    self.m_bUpdateForegroundActive = false;
                    return LS_LOADING_STATUS(1);
                }

                operator_delete__(puVar4 - 1);
                self.m_pForegroundPatches = nullptr;
            }
            else if (0 < self.m_nPatchRows)
            {
                int iRow = 0;
                do
                {
                    int iCol = 0;
                    if (0 < self.m_nPatchColumns)
                    {
                        do
                        {
                            CBankFileAsyncEntry* const uVar3 = self.m_pBankEntry;
                            CRefCountBlock* const piVar9 = self.m_pBankEntryRefCount;

                            if (piVar9 != nullptr)
                            {
                                piVar9->m_nRefCount = piVar9->m_nRefCount + 1;
                            }

                            CEngineLandscapePatch::UpdateForeground(
                                reinterpret_cast<CEngineLandscapePatch*>(
                                    reinterpret_cast<std::byte*>(self.m_pForegroundPatches) +
                                    (self.m_nPatchColumns * iRow + iCol) * 0x48),
                                uVar3,
                                reinterpret_cast<int*>(piVar9));

                            iCol = iCol + 1;
                        } while (iCol < self.m_nPatchColumns);
                    }

                    iRow = iRow + 1;
                } while (iRow < self.m_nPatchRows);

                self.m_bUpdateForegroundActive = false;
                return LS_LOADING_STATUS(1);
            }
        }
    }

LAB_00bdc96d:
    self.m_bUpdateForegroundActive = false;
    return LS_LOADING_STATUS(1);
}