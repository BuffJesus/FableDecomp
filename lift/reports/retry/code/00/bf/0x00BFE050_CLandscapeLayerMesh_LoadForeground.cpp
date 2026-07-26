bool CLandscapeLayerMesh::LoadForeground(CDataInputStream& stream, CEngineTexturePalette& texturePalette)
{
    struct CDataInputStreamOverlay
    {
        std::byte pad00[0x04];
        std::int32_t absoluteReadOffset; // +0x04
        std::byte pad08[0x04];
        std::uint8_t* cursor;            // +0x0C
        std::byte pad10[0x04];
        std::int32_t bytesAvailable;     // +0x14
    };
    static_assert(offsetof(CDataInputStreamOverlay, absoluteReadOffset) == 0x04);
    static_assert(offsetof(CDataInputStreamOverlay, cursor) == 0x0C);
    static_assert(offsetof(CDataInputStreamOverlay, bytesAvailable) == 0x14);

    struct CEngineTexturePaletteOverlay
    {
        std::byte pad00[0x04];
        std::uint32_t entries;           // +0x04
        std::byte pad08[0x0C];
        std::uint8_t directModeFlag;     // +0x14
    };
    static_assert(offsetof(CEngineTexturePaletteOverlay, entries) == 0x04);
    static_assert(offsetof(CEngineTexturePaletteOverlay, directModeFlag) == 0x14);

    struct ForegroundVertexOverlay
    {
        std::uint16_t value0;            // +0x00
        std::uint16_t value1;            // +0x02
        std::uint32_t normalBase;        // +0x04
        std::byte pad08[0x0C];
        std::uint8_t byte14;             // +0x14
        std::uint8_t byte15;             // +0x15
        std::uint8_t byte16;             // +0x16
        std::byte pad17[0x01];
    };
    static_assert(sizeof(ForegroundVertexOverlay) == 0x18);
    static_assert(offsetof(ForegroundVertexOverlay, value0) == 0x00);
    static_assert(offsetof(ForegroundVertexOverlay, value1) == 0x02);
    static_assert(offsetof(ForegroundVertexOverlay, normalBase) == 0x04);
    static_assert(offsetof(ForegroundVertexOverlay, byte14) == 0x14);
    static_assert(offsetof(ForegroundVertexOverlay, byte15) == 0x15);
    static_assert(offsetof(ForegroundVertexOverlay, byte16) == 0x16);

    struct CGraphicInfoFramePoolOverlay32
    {
        std::byte pad00[0x1E0];
        std::int32_t begin;              // +0x1E0
        std::int32_t end;                // +0x1E4
    };
    static_assert(offsetof(CGraphicInfoFramePoolOverlay32, begin) == 0x1E0);
    static_assert(offsetof(CGraphicInfoFramePoolOverlay32, end) == 0x1E4);

    static_assert(offsetof(CLandscapeLayerMesh, ForegroundTextureIndex) == 0x04);
    static_assert(offsetof(CLandscapeLayerMesh, BackgroundTextureIndex) == 0x08);
    static_assert(offsetof(CLandscapeLayerMesh, BumpMapTextureIndex) == 0x0C);
    static_assert(offsetof(CLandscapeLayerMesh, SelfIllumination) == 0x10);
    static_assert(offsetof(CLandscapeLayerMesh, ForegroundTexture) == 0x14);
    static_assert(offsetof(CLandscapeLayerMesh, BackgroundTexture) == 0x18);
    static_assert(offsetof(CLandscapeLayerMesh, BumpMap) == 0x1C);
    static_assert(offsetof(CLandscapeLayerMesh, MinTextureMipMap) == 0x20);
    static_assert(offsetof(CLandscapeLayerMesh, MinBumpMapMipMap) == 0x24);
    static_assert(offsetof(CLandscapeLayerMesh, MappingDirection) == 0x28);
    static_assert(offsetof(CLandscapeLayerMesh, IndexBuffer) == 0x34);
    static_assert(offsetof(CLandscapeLayerMesh, ForegroundVertexBuffer) == 0x38);
    static_assert(offsetof(CLandscapeLayerMesh, VertexCount) == 0x44);
    static_assert(offsetof(CLandscapeLayerMesh, PolyCountIncDegenerates) == 0x46);
    static_assert(offsetof(CLandscapeLayerMesh, UsesSharedIndexBuffer) == 0x48);
    static_assert(sizeof(CLandscapeLayerMesh) == 0x54);

    auto& streamOverlay = reinterpret_cast<CDataInputStreamOverlay&>(stream);
    auto& paletteOverlay = reinterpret_cast<CEngineTexturePaletteOverlay&>(texturePalette);

    std::uint32_t carriedValue = static_cast<std::uint32_t>(
        reinterpret_cast<std::uintptr_t>(&stream));

    if (static_cast<std::uint32_t>(streamOverlay.absoluteReadOffset) + 2U < 0x80000000U)
    {
        const std::int32_t bytesAvailable = streamOverlay.bytesAvailable;
        if (bytesAvailable < 2)
        {
            CDataInputStream::ReadWithSrcChunkOverflow(
                &stream,
                reinterpret_cast<std::uint8_t*>(&carriedValue),
                2);
        }
        else
        {
            const std::uint16_t value16 =
                *reinterpret_cast<std::uint16_t*>(streamOverlay.cursor);
            streamOverlay.cursor += 2;
            streamOverlay.bytesAvailable = bytesAvailable - 2;
            streamOverlay.absoluteReadOffset += 2;
            carriedValue = (carriedValue & 0xFFFF0000u) | value16;
        }
    }
    VertexCount = static_cast<std::uint16_t>(carriedValue);

    if (static_cast<std::uint32_t>(streamOverlay.absoluteReadOffset) + 2U < 0x80000000U)
    {
        const std::int32_t bytesAvailable = streamOverlay.bytesAvailable;
        if (bytesAvailable < 2)
        {
            CDataInputStream::ReadWithSrcChunkOverflow(
                &stream,
                reinterpret_cast<std::uint8_t*>(&carriedValue),
                2);
        }
        else
        {
            const std::uint16_t value16 =
                *reinterpret_cast<std::uint16_t*>(streamOverlay.cursor);
            streamOverlay.cursor += 2;
            streamOverlay.bytesAvailable = bytesAvailable - 2;
            streamOverlay.absoluteReadOffset += 2;
            carriedValue = (carriedValue & 0xFFFF0000u) | value16;
        }
    }
    PolyCountIncDegenerates = static_cast<std::uint16_t>(carriedValue);

    if (static_cast<std::uint32_t>(streamOverlay.absoluteReadOffset) + 1U < 0x80000000U)
    {
        const std::int32_t bytesAvailable = streamOverlay.bytesAvailable;
        if (bytesAvailable < 1)
        {
            CDataInputStream::ReadWithSrcChunkOverflow(
                &stream,
                reinterpret_cast<std::uint8_t*>(&carriedValue),
                1);
        }
        else
        {
            const std::uint8_t value8 = *streamOverlay.cursor;
            ++streamOverlay.cursor;
            streamOverlay.bytesAvailable = bytesAvailable - 1;
            ++streamOverlay.absoluteReadOffset;
            carriedValue = (carriedValue & 0xFFFFFF00u) | value8;
        }
    }
    MappingDirection = static_cast<LANDSCAPE_TEXTURE_MAPPING_DIRECTION>(carriedValue & 0xFFu);

    if (static_cast<std::uint32_t>(streamOverlay.absoluteReadOffset) + 4U < 0x80000000U)
    {
        const std::int32_t bytesAvailable = streamOverlay.bytesAvailable;
        if (bytesAvailable < 4)
        {
            CDataInputStream::ReadWithSrcChunkOverflow(
                &stream,
                reinterpret_cast<std::uint8_t*>(&carriedValue),
                4);
        }
        else
        {
            carriedValue = *reinterpret_cast<std::uint32_t*>(streamOverlay.cursor);
            streamOverlay.cursor += 4;
            streamOverlay.bytesAvailable = bytesAvailable - 4;
            streamOverlay.absoluteReadOffset += 4;
        }
    }
    if (paletteOverlay.directModeFlag == 0)
    {
        carriedValue = *reinterpret_cast<std::uint32_t*>(
            static_cast<std::uintptr_t>(paletteOverlay.entries) +
            static_cast<std::uint32_t>(static_cast<std::int32_t>(carriedValue)) * 8u);
    }
    ForegroundTextureIndex = static_cast<std::int32_t>(carriedValue);

    if (static_cast<std::uint32_t>(streamOverlay.absoluteReadOffset) + 4U < 0x80000000U)
    {
        const std::int32_t bytesAvailable = streamOverlay.bytesAvailable;
        if (bytesAvailable < 4)
        {
            CDataInputStream::ReadWithSrcChunkOverflow(
                &stream,
                reinterpret_cast<std::uint8_t*>(&carriedValue),
                4);
        }
        else
        {
            carriedValue = *reinterpret_cast<std::uint32_t*>(streamOverlay.cursor);
            streamOverlay.cursor += 4;
            streamOverlay.bytesAvailable = bytesAvailable - 4;
            streamOverlay.absoluteReadOffset += 4;
        }
    }
    if (paletteOverlay.directModeFlag == 0)
    {
        carriedValue = *reinterpret_cast<std::uint32_t*>(
            static_cast<std::uintptr_t>(paletteOverlay.entries) +
            static_cast<std::uint32_t>(static_cast<std::int32_t>(carriedValue)) * 8u);
    }
    BackgroundTextureIndex = static_cast<std::int32_t>(carriedValue);

    if (static_cast<std::uint32_t>(streamOverlay.absoluteReadOffset) + 4U < 0x80000000U)
    {
        const std::int32_t bytesAvailable = streamOverlay.bytesAvailable;
        if (bytesAvailable < 4)
        {
            CDataInputStream::ReadWithSrcChunkOverflow(
                &stream,
                reinterpret_cast<std::uint8_t*>(&carriedValue),
                4);
        }
        else
        {
            carriedValue = *reinterpret_cast<std::uint32_t*>(streamOverlay.cursor);
            streamOverlay.cursor += 4;
            streamOverlay.bytesAvailable = bytesAvailable - 4;
            streamOverlay.absoluteReadOffset += 4;
        }
    }
    if (paletteOverlay.directModeFlag == 0)
    {
        carriedValue = *reinterpret_cast<std::uint32_t*>(
            static_cast<std::uintptr_t>(paletteOverlay.entries) +
            static_cast<std::uint32_t>(static_cast<std::int32_t>(carriedValue)) * 8u);
    }
    BumpMapTextureIndex = static_cast<std::int32_t>(carriedValue);

    if (static_cast<std::uint32_t>(streamOverlay.absoluteReadOffset) + 1U < 0x80000000U)
    {
        const std::int32_t bytesAvailable = streamOverlay.bytesAvailable;
        if (bytesAvailable < 1)
        {
            CDataInputStream::ReadWithSrcChunkOverflow(
                &stream,
                reinterpret_cast<std::uint8_t*>(&carriedValue),
                1);
        }
        else
        {
            const std::uint8_t value8 = *streamOverlay.cursor;
            ++streamOverlay.cursor;
            streamOverlay.bytesAvailable = bytesAvailable - 1;
            ++streamOverlay.absoluteReadOffset;
            carriedValue = (carriedValue & 0xFFFFFF00u) | value8;
        }
    }
    UsesSharedIndexBuffer = (static_cast<std::uint8_t>(carriedValue) != 0);

    if (static_cast<std::uint32_t>(streamOverlay.absoluteReadOffset) + 4U < 0x80000000U)
    {
        const std::int32_t bytesAvailable = streamOverlay.bytesAvailable;
        if (bytesAvailable < 4)
        {
            CDataInputStream::ReadWithSrcChunkOverflow(
                &stream,
                reinterpret_cast<std::uint8_t*>(&carriedValue),
                4);
        }
        else
        {
            carriedValue = *reinterpret_cast<std::uint32_t*>(streamOverlay.cursor);
            streamOverlay.cursor += 4;
            streamOverlay.bytesAvailable = bytesAvailable - 4;
            streamOverlay.absoluteReadOffset += 4;
        }
    }
    MinTextureMipMap = static_cast<std::int32_t>(carriedValue);

    if (static_cast<std::uint32_t>(streamOverlay.absoluteReadOffset) + 4U < 0x80000000U)
    {
        const std::int32_t bytesAvailable = streamOverlay.bytesAvailable;
        if (bytesAvailable < 4)
        {
            CDataInputStream::ReadWithSrcChunkOverflow(
                &stream,
                reinterpret_cast<std::uint8_t*>(&carriedValue),
                4);
        }
        else
        {
            carriedValue = *reinterpret_cast<std::uint32_t*>(streamOverlay.cursor);
            streamOverlay.cursor += 4;
            streamOverlay.bytesAvailable = bytesAvailable - 4;
            streamOverlay.absoluteReadOffset += 4;
        }
    }
    MinBumpMapMipMap = static_cast<std::int32_t>(carriedValue);

    if (static_cast<std::uint32_t>(streamOverlay.absoluteReadOffset) + 4U < 0x80000000U)
    {
        const std::int32_t bytesAvailable = streamOverlay.bytesAvailable;
        if (bytesAvailable < 4)
        {
            CDataInputStream::ReadWithSrcChunkOverflow(
                &stream,
                reinterpret_cast<std::uint8_t*>(&carriedValue),
                4);
        }
        else
        {
            carriedValue = *reinterpret_cast<std::uint32_t*>(streamOverlay.cursor);
            streamOverlay.cursor += 4;
            streamOverlay.bytesAvailable = bytesAvailable - 4;
            streamOverlay.absoluteReadOffset += 4;
        }
    }
    SelfIllumination = std::bit_cast<float>(carriedValue);

    auto* const landscapeMemoryManager =
        *reinterpret_cast<CEngineLandscapeMemoryManager**>(
            static_cast<std::uintptr_t>(DAT_01436ea8) + 0x6B0u);
    std::uint32_t carriedLocal4 =
        static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(landscapeMemoryManager));
    std::uint8_t carriedRetaddr;
    std::uint32_t carriedEbpHigh;

    const auto vertexBufferUsage = CEngineResourceManager::GetIndexBufferUsage(DAT_01436e98);
    ForegroundVertexBuffer = CEngineLandscapeMemoryManager::AllocateVertexBuffer(
        landscapeMemoryManager,
        reinterpret_cast<CMovableResource*>(this),
        0,
        static_cast<std::uint32_t>(VertexCount),
        0x18,
        vertexBufferUsage);

    if (ForegroundVertexBuffer == nullptr)
    {
        return false;
    }

    CThing* const nullThing = nullptr;
    auto* currentVertex = reinterpret_cast<ForegroundVertexOverlay*>(
        CThingFilter_IsValid::operator()(
            reinterpret_cast<CThingFilter_IsValid*>(ForegroundVertexBuffer),
            nullThing));

    std::int32_t vertexIndex = 0;
    if (VertexCount != 0)
    {
        do
        {
            if (static_cast<std::uint32_t>(streamOverlay.absoluteReadOffset) + 2U < 0x80000000U)
            {
                const std::int32_t bytesAvailable = streamOverlay.bytesAvailable;
                if (bytesAvailable < 2)
                {
                    CDataInputStream::ReadWithSrcChunkOverflow(
                        &stream,
                        reinterpret_cast<std::uint8_t*>(&carriedValue),
                        2);
                }
                else
                {
                    const std::uint16_t value16 =
                        *reinterpret_cast<std::uint16_t*>(streamOverlay.cursor);
                    streamOverlay.cursor += 2;
                    streamOverlay.bytesAvailable = bytesAvailable - 2;
                    streamOverlay.absoluteReadOffset += 2;
                    carriedValue = (carriedValue & 0xFFFF0000u) | value16;
                }
            }
            currentVertex->value0 = static_cast<std::uint16_t>(carriedValue);

            if (static_cast<std::uint32_t>(streamOverlay.absoluteReadOffset) + 2U < 0x80000000U)
            {
                const std::int32_t bytesAvailable = streamOverlay.bytesAvailable;
                if (bytesAvailable < 2)
                {
                    CDataInputStream::ReadWithSrcChunkOverflow(
                        &stream,
                        reinterpret_cast<std::uint8_t*>(&carriedValue),
                        2);
                }
                else
                {
                    const std::uint16_t value16 =
                        *reinterpret_cast<std::uint16_t*>(streamOverlay.cursor);
                    streamOverlay.cursor += 2;
                    streamOverlay.bytesAvailable = bytesAvailable - 2;
                    streamOverlay.absoluteReadOffset += 2;
                    carriedValue = (carriedValue & 0xFFFF0000u) | value16;
                }
            }
            currentVertex->value1 = static_cast<std::uint16_t>(carriedValue);

            if (static_cast<std::uint32_t>(streamOverlay.absoluteReadOffset) + 4U < 0x80000000U)
            {
                const std::int32_t bytesAvailable = streamOverlay.bytesAvailable;
                if (bytesAvailable < 4)
                {
                    CDataInputStream::ReadWithSrcChunkOverflow(
                        &stream,
                        reinterpret_cast<std::uint8_t*>(&carriedValue),
                        4);
                }
                else
                {
                    carriedValue = *reinterpret_cast<std::uint32_t*>(streamOverlay.cursor);
                    streamOverlay.cursor += 4;
                    streamOverlay.bytesAvailable = bytesAvailable - 4;
                    streamOverlay.absoluteReadOffset += 4;
                }
            }
            currentVertex->normalBase = carriedValue;

            if (static_cast<std::uint32_t>(streamOverlay.absoluteReadOffset) + 4U < 0x80000000U)
            {
                const std::int32_t bytesAvailable = streamOverlay.bytesAvailable;
                if (bytesAvailable < 4)
                {
                    CDataInputStream::ReadWithSrcChunkOverflow(
                        &stream,
                        reinterpret_cast<std::uint8_t*>(&carriedValue),
                        4);
                }
                else
                {
                    carriedValue = *reinterpret_cast<std::uint32_t*>(streamOverlay.cursor);
                    streamOverlay.cursor += 4;
                    streamOverlay.bytesAvailable = bytesAvailable - 4;
                    streamOverlay.absoluteReadOffset += 4;
                }
            }
            CLandscapeBackgroundPatch::SetTargetVertexNormal(
                currentVertex,
                static_cast<std::uint32_t>(carriedValue));

            if (static_cast<std::uint32_t>(streamOverlay.absoluteReadOffset) + 1U < 0x80000000U)
            {
                const std::int32_t bytesAvailable = streamOverlay.bytesAvailable;
                if (bytesAvailable < 1)
                {
                    CDataInputStream::ReadWithSrcChunkOverflow(
                        &stream,
                        reinterpret_cast<std::uint8_t*>(&carriedLocal4),
                        1);
                }
                else
                {
                    const std::uint8_t value8 = *streamOverlay.cursor;
                    ++streamOverlay.cursor;
                    streamOverlay.bytesAvailable = bytesAvailable - 1;
                    ++streamOverlay.absoluteReadOffset;
                    carriedLocal4 = (carriedLocal4 & 0xFFFFFF00u) | value8;
                }
            }
            currentVertex->byte16 = static_cast<std::uint8_t>(carriedLocal4);

            if (static_cast<std::uint32_t>(streamOverlay.absoluteReadOffset) + 1U < 0x80000000U)
            {
                const std::int32_t bytesAvailable = streamOverlay.bytesAvailable;
                if (bytesAvailable < 1)
                {
                    CDataInputStream::ReadWithSrcChunkOverflow(&stream, &carriedRetaddr, 1);
                }
                else
                {
                    carriedRetaddr = *streamOverlay.cursor;
                    ++streamOverlay.cursor;
                    streamOverlay.bytesAvailable = bytesAvailable - 1;
                    ++streamOverlay.absoluteReadOffset;
                }
            }
            currentVertex->byte15 = carriedRetaddr;

            if (static_cast<std::uint32_t>(streamOverlay.absoluteReadOffset) + 1U < 0x80000000U)
            {
                const std::int32_t bytesAvailable = streamOverlay.bytesAvailable;
                if (bytesAvailable < 1)
                {
                    CDataInputStream::ReadWithSrcChunkOverflow(
                        &stream,
                        reinterpret_cast<std::uint8_t*>(&carriedEbpHigh) + 3,
                        1);
                }
                else
                {
                    carriedEbpHigh =
                        static_cast<std::uint32_t>(*streamOverlay.cursor) << 24;
                    ++streamOverlay.cursor;
                    streamOverlay.bytesAvailable = bytesAvailable - 1;
                    ++streamOverlay.absoluteReadOffset;
                }
            }
            currentVertex->byte14 = static_cast<std::uint8_t>(carriedEbpHigh >> 24);

            ++vertexIndex;
            currentVertex = reinterpret_cast<ForegroundVertexOverlay*>(
                reinterpret_cast<std::uint8_t*>(currentVertex) + 0x18);
        } while (vertexIndex < static_cast<std::int32_t>(VertexCount));
    }

    CThingFilter_IsValid::operator()(
        reinterpret_cast<CThingFilter_IsValid*>(ForegroundVertexBuffer),
        nullThing);

    {
        std::uint32_t uVar3 = static_cast<std::uint32_t>(ForegroundTextureIndex);
        std::uint32_t uVar8;
        if (uVar3 == 0)
        {
            uVar8 = 0;
        }
        else
        {
            const auto manager954 = *reinterpret_cast<std::int32_t*>(
                static_cast<std::uintptr_t>(DAT_01436e98) + 0x954u);
            const auto& pool = *reinterpret_cast<const CGraphicInfoFramePoolOverlay32*>(
                static_cast<std::uintptr_t>(manager954));
            const std::int32_t iVar2 = pool.begin;
            std::uint32_t uVar9;
            if ((static_cast<std::uint32_t>((pool.end - iVar2) / 0x2C) <= uVar3) ||
                ((uVar9 = uVar3 * 0x2Cu + static_cast<std::uint32_t>(iVar2)) == 0))
            {
                uVar8 = 0;
            }
            else
            {
                uVar8 = *reinterpret_cast<std::uint32_t*>(
                    static_cast<std::uintptr_t>(
                        (-(static_cast<std::uint32_t>(
                            uVar3 < static_cast<std::uint32_t>((pool.end - iVar2) / 0x2C))) &
                         uVar9) +
                        0x28u));
            }
        }
        ForegroundTexture = reinterpret_cast<CGraphicInfoFrame*>(
            static_cast<std::uintptr_t>(uVar8));
    }

    {
        std::uint32_t uVar3 = static_cast<std::uint32_t>(BackgroundTextureIndex);
        std::uint32_t uVar8;
        if (uVar3 == 0)
        {
            uVar8 = 0;
        }
        else
        {
            const auto manager954 = *reinterpret_cast<std::int32_t*>(
                static_cast<std::uintptr_t>(DAT_01436e98) + 0x954u);
            const auto& pool = *reinterpret_cast<const CGraphicInfoFramePoolOverlay32*>(
                static_cast<std::uintptr_t>(manager954));
            const std::int32_t iVar2 = pool.begin;
            std::uint32_t uVar9;
            if ((static_cast<std::uint32_t>((pool.end - iVar2) / 0x2C) <= uVar3) ||
                ((uVar9 = uVar3 * 0x2Cu + static_cast<std::uint32_t>(iVar2)) == 0))
            {
                uVar8 = 0;
            }
            else
            {
                uVar8 = *reinterpret_cast<std::uint32_t*>(
                    static_cast<std::uintptr_t>(
                        (-(static_cast<std::uint32_t>(
                            uVar3 < static_cast<std::uint32_t>((pool.end - iVar2) / 0x2C))) &
                         uVar9) +
                        0x28u));
            }
        }
        BackgroundTexture = reinterpret_cast<CGraphicInfoFrame*>(
            static_cast<std::uintptr_t>(uVar8));
    }

    {
        std::uint32_t uVar3 = static_cast<std::uint32_t>(BumpMapTextureIndex);
        std::uint32_t uVar8 = 0;
        if (uVar3 != 0)
        {
            const auto manager954 = *reinterpret_cast<std::int32_t*>(
                static_cast<std::uintptr_t>(DAT_01436e98) + 0x954u);
            const auto& pool = *reinterpret_cast<const CGraphicInfoFramePoolOverlay32*>(
                static_cast<std::uintptr_t>(manager954));
            const std::int32_t iVar2 = pool.begin;
            if (uVar3 < static_cast<std::uint32_t>((pool.end - iVar2) / 0x2C))
            {
                const std::uint32_t uVar9 =
                    uVar3 * 0x2Cu + static_cast<std::uint32_t>(iVar2);
                if (uVar9 != 0)
                {
                    uVar8 = *reinterpret_cast<std::uint32_t*>(
                        static_cast<std::uintptr_t>(
                            (-(static_cast<std::uint32_t>(
                                uVar3 < static_cast<std::uint32_t>((pool.end - iVar2) / 0x2C))) &
                             uVar9) +
                            0x28u));
                }
            }
        }
        BumpMap = reinterpret_cast<CGraphicInfoFrame*>(
            static_cast<std::uintptr_t>(uVar8));
    }

    if (UsesSharedIndexBuffer)
    {
        IndexBuffer = *reinterpret_cast<CIndexBuffer**>(
            static_cast<std::uintptr_t>(DAT_01436ea8) + 0x494u);
        return true;
    }

    const EPrimitiveType primitiveType = static_cast<EPrimitiveType>(5);
    const std::uint32_t indexCount =
        static_cast<std::uint16_t>(PolyCountIncDegenerates) + 2u;
    const EIndexBufferFormat indexFormat = static_cast<EIndexBufferFormat>(0x65);
    const std::uint32_t indexBufferSize = indexCount * 2u;
    const auto indexBufferUsage = CEngineResourceManager::GetIndexBufferUsage(DAT_01436e98);

    IndexBuffer = CEngineLandscapeMemoryManager::AllocateIndexBuffer(
        landscapeMemoryManager,
        reinterpret_cast<CMovableResource*>(this),
        indexBufferSize,
        indexBufferUsage,
        indexFormat,
        primitiveType);

    if (IndexBuffer == nullptr)
    {
        return false;
    }

    auto* lockedIndices = CIndexBuffer::Lock(IndexBuffer, 0, 0, 0);
    if ((indexCount != 0) &&
        (static_cast<std::uint32_t>(streamOverlay.absoluteReadOffset) + indexBufferSize <
         0x80000000U))
    {
        if (static_cast<std::int32_t>(indexBufferSize) <= streamOverlay.bytesAvailable)
        {
            auto* src = streamOverlay.cursor;
            auto* dst = lockedIndices;

            for (std::uint32_t copyDwords = indexCount >> 1; copyDwords != 0; --copyDwords)
            {
                *reinterpret_cast<std::uint32_t*>(dst) =
                    *reinterpret_cast<const std::uint32_t*>(src);
                src += 4;
                dst += 4;
            }
            for (std::uint32_t copyBytes = indexBufferSize & 3u; copyBytes != 0; --copyBytes)
            {
                *dst = *src;
                ++src;
                ++dst;
            }

            streamOverlay.cursor += indexBufferSize;
            streamOverlay.bytesAvailable += static_cast<std::int32_t>(indexCount) * -2;
            streamOverlay.absoluteReadOffset += static_cast<std::int32_t>(indexBufferSize);

            CVertexBufferWin32::DoUnlock(reinterpret_cast<CVertexBufferWin32*>(IndexBuffer));
            return true;
        }

        CDataInputStream::ReadWithSrcChunkOverflow(&stream, lockedIndices, indexBufferSize);
    }

    CVertexBufferWin32::DoUnlock(reinterpret_cast<CVertexBufferWin32*>(IndexBuffer));
    return true;
}