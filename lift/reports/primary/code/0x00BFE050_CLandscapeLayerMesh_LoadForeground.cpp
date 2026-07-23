bool CLandscapeLayerMesh::LoadForeground(CDataInputStream& in, CEngineTexturePalette& texturePalette)
{
    struct CDataInputStreamOverlay
    {
        std::byte pad00[0x04];
        int m_nBytesRead;                    // 0x04
        std::byte pad08[0x04];
        uchar* m_pSrcChunkCursor;           // 0x0C
        std::byte pad10[0x04];
        int m_nSrcChunkBytesRemaining;      // 0x14
    };

    struct CEngineTexturePaletteOverlay
    {
        std::byte pad00[0x04];
        std::byte* m_pEntries;              // 0x04
        std::byte pad08[0x0C];
        std::uint8_t m_bDirectEntries;      // 0x14
    };

    struct CLandscapeLayerMeshOverlay
    {
        std::byte pad00[0x04];
        std::uint32_t m_TextureValue0;      // 0x04
        std::uint32_t m_TextureValue1;      // 0x08
        std::uint32_t m_TextureValue2;      // 0x0C
        std::uint32_t m_Value10;            // 0x10
        std::uint32_t m_ResolvedTexture0;   // 0x14
        std::uint32_t m_ResolvedTexture1;   // 0x18
        std::uint32_t m_ResolvedTexture2;   // 0x1C
        std::uint32_t m_Value20;            // 0x20
        std::uint32_t m_Value24;            // 0x24
        std::uint32_t m_Value28;            // 0x28
        std::byte pad2C[0x08];
        CIndexBuffer* m_pIndexBuffer;       // 0x34
        CVertexBufferWin32* m_pVertexBuffer;// 0x38
        std::byte pad3C[0x08];
        std::uint16_t m_nVertexCount;       // 0x44
        std::uint16_t m_nPrimitiveCount;    // 0x46
        bool m_bUseSharedIndexBuffer;       // 0x48
    };

    struct ForegroundVertex
    {
        std::uint16_t m_Value00;            // +0x00
        std::uint16_t m_Value02;            // +0x02
        std::uint32_t m_Value04;            // +0x04
        std::byte pad08[0x0C];              // +0x08..+0x13
        std::uint8_t m_Value14;             // +0x14, third trailing byte read
        std::uint8_t m_Value15;             // +0x15, second trailing byte read
        std::uint8_t m_Value16;             // +0x16, first trailing byte read
        std::byte pad17[0x01];              // +0x17
    };

    static_assert(offsetof(CLandscapeLayerMeshOverlay, m_TextureValue0) == 0x04);
    static_assert(offsetof(CLandscapeLayerMeshOverlay, m_TextureValue1) == 0x08);
    static_assert(offsetof(CLandscapeLayerMeshOverlay, m_TextureValue2) == 0x0C);
    static_assert(offsetof(CLandscapeLayerMeshOverlay, m_Value10) == 0x10);
    static_assert(offsetof(CLandscapeLayerMeshOverlay, m_ResolvedTexture0) == 0x14);
    static_assert(offsetof(CLandscapeLayerMeshOverlay, m_ResolvedTexture1) == 0x18);
    static_assert(offsetof(CLandscapeLayerMeshOverlay, m_ResolvedTexture2) == 0x1C);
    static_assert(offsetof(CLandscapeLayerMeshOverlay, m_Value20) == 0x20);
    static_assert(offsetof(CLandscapeLayerMeshOverlay, m_Value24) == 0x24);
    static_assert(offsetof(CLandscapeLayerMeshOverlay, m_Value28) == 0x28);
    static_assert(offsetof(CLandscapeLayerMeshOverlay, m_pIndexBuffer) == 0x34);
    static_assert(offsetof(CLandscapeLayerMeshOverlay, m_pVertexBuffer) == 0x38);
    static_assert(offsetof(CLandscapeLayerMeshOverlay, m_nVertexCount) == 0x44);
    static_assert(offsetof(CLandscapeLayerMeshOverlay, m_nPrimitiveCount) == 0x46);
    static_assert(offsetof(CLandscapeLayerMeshOverlay, m_bUseSharedIndexBuffer) == 0x48);
    static_assert(offsetof(ForegroundVertex, m_Value00) == 0x00);
    static_assert(offsetof(ForegroundVertex, m_Value02) == 0x02);
    static_assert(offsetof(ForegroundVertex, m_Value04) == 0x04);
    static_assert(offsetof(ForegroundVertex, m_Value14) == 0x14);
    static_assert(offsetof(ForegroundVertex, m_Value15) == 0x15);
    static_assert(offsetof(ForegroundVertex, m_Value16) == 0x16);
    static_assert(sizeof(ForegroundVertex) == 0x18);

    auto& stream = reinterpret_cast<CDataInputStreamOverlay&>(in);
    auto& palette = reinterpret_cast<CEngineTexturePaletteOverlay&>(texturePalette);
    auto& self = reinterpret_cast<CLandscapeLayerMeshOverlay&>(*this);

    auto ReadU8 = [&]() -> std::uint8_t
    {
        std::uint8_t value = 0;
        if (stream.m_nBytesRead + 1U < 0x80000000U)
        {
            if (stream.m_nSrcChunkBytesRemaining < 1)
            {
                CDataInputStream::ReadWithSrcChunkOverflow(&in, &value, 1);
            }
            else
            {
                value = *stream.m_pSrcChunkCursor;
                stream.m_pSrcChunkCursor = stream.m_pSrcChunkCursor + 1;
                stream.m_nSrcChunkBytesRemaining = stream.m_nSrcChunkBytesRemaining + -1;
                stream.m_nBytesRead = stream.m_nBytesRead + 1;
            }
        }
        return value;
    };

    auto ReadU16 = [&]() -> std::uint16_t
    {
        std::uint16_t value = 0;
        if (stream.m_nBytesRead + 2U < 0x80000000U)
        {
            if (stream.m_nSrcChunkBytesRemaining < 2)
            {
                CDataInputStream::ReadWithSrcChunkOverflow(&in, reinterpret_cast<uchar*>(&value), 2);
            }
            else
            {
                value = *reinterpret_cast<std::uint16_t*>(stream.m_pSrcChunkCursor);
                stream.m_pSrcChunkCursor = stream.m_pSrcChunkCursor + 2;
                stream.m_nSrcChunkBytesRemaining = stream.m_nSrcChunkBytesRemaining + -2;
                stream.m_nBytesRead = stream.m_nBytesRead + 2;
            }
        }
        return value;
    };

    auto ReadU32 = [&]() -> std::uint32_t
    {
        std::uint32_t value = 0;
        if (stream.m_nBytesRead + 4U < 0x80000000U)
        {
            if (stream.m_nSrcChunkBytesRemaining < 4)
            {
                CDataInputStream::ReadWithSrcChunkOverflow(&in, reinterpret_cast<uchar*>(&value), 4);
            }
            else
            {
                value = *reinterpret_cast<std::uint32_t*>(stream.m_pSrcChunkCursor);
                stream.m_pSrcChunkCursor = stream.m_pSrcChunkCursor + 4;
                stream.m_nSrcChunkBytesRemaining = stream.m_nSrcChunkBytesRemaining + -4;
                stream.m_nBytesRead = stream.m_nBytesRead + 4;
            }
        }
        return value;
    };

    auto ResolvePaletteValue = [&](std::uint32_t value) -> std::uint32_t
    {
        std::uint32_t resolved = value;
        if (palette.m_bDirectEntries == 0)
        {
            resolved = *reinterpret_cast<std::uint32_t*>(palette.m_pEntries + static_cast<int>(value) * 8);
        }
        return resolved;
    };

    self.m_nVertexCount = ReadU16();
    self.m_nPrimitiveCount = ReadU16();
    self.m_Value28 = ReadU8();

    self.m_TextureValue0 = ResolvePaletteValue(ReadU32());
    self.m_TextureValue1 = ResolvePaletteValue(ReadU32());
    self.m_TextureValue2 = ResolvePaletteValue(ReadU32());

    self.m_bUseSharedIndexBuffer = ReadU8() != 0;
    self.m_Value20 = ReadU32();
    self.m_Value24 = ReadU32();
    self.m_Value10 = ReadU32();

    CEngineLandscapeMemoryManager* const pLandscapeMemoryManager =
        *reinterpret_cast<CEngineLandscapeMemoryManager**>(DAT_01436ea8 + 0x6B0);
    const ulong usage = CEngineResourceManager::GetIndexBufferUsage(DAT_01436e98);

    self.m_pVertexBuffer = CEngineLandscapeMemoryManager::AllocateVertexBuffer(
        pLandscapeMemoryManager,
        reinterpret_cast<CMovableResource*>(this),
        0,
        static_cast<std::uint32_t>(self.m_nVertexCount),
        0x18,
        usage);
    if (self.m_pVertexBuffer == nullptr)
    {
        return false;
    }

    CThing* const pNullThing = nullptr;
    const std::uintptr_t vertexAcquireResult = reinterpret_cast<std::uintptr_t>(
        CThingFilter_IsValid::operator()(reinterpret_cast<CThingFilter_IsValid*>(self.m_pVertexBuffer), pNullThing));
    const bool bVar5 = static_cast<bool>(vertexAcquireResult & 0xFF);
    const std::uint32_t extraout_var = static_cast<std::uint32_t>(vertexAcquireResult >> 8);
    auto* puVar14 = reinterpret_cast<ulong*>(
        (static_cast<std::uintptr_t>(extraout_var) << 8) | static_cast<std::uint8_t>(bVar5));
    int iStack_10 = 0;

    if (self.m_nVertexCount != 0)
    {
        puVar14 = puVar14 + 1;
        do
        {
            reinterpret_cast<ForegroundVertex*>(puVar14 - 1)->m_Value00 = ReadU16();
            reinterpret_cast<ForegroundVertex*>(puVar14 - 1)->m_Value02 = ReadU16();

            const std::uint32_t value04 = ReadU32();
            *puVar14 = static_cast<ulong>(value04);

            const std::uint32_t targetVertexNormal = ReadU32();
            CLandscapeBackgroundPatch::SetTargetVertexNormal(
                reinterpret_cast<CTVertexLandscapeBackground*>(puVar14 - 1),
                static_cast<ulong>(targetVertexNormal));

            reinterpret_cast<ForegroundVertex*>(puVar14 - 1)->m_Value16 = ReadU8();
            reinterpret_cast<ForegroundVertex*>(puVar14 - 1)->m_Value15 = ReadU8();
            reinterpret_cast<ForegroundVertex*>(puVar14 - 1)->m_Value14 = ReadU8();

            iStack_10 = iStack_10 + 1;
            puVar14 = puVar14 + 6;
        } while (iStack_10 < static_cast<int>(self.m_nVertexCount));
    }

    CThingFilter_IsValid::operator()(
        *reinterpret_cast<CThingFilter_IsValid**>(reinterpret_cast<std::byte*>(this) + 0x38),
        pNullThing);

    {
        std::uint32_t uVar3 = self.m_TextureValue0;
        std::uint32_t uVar8;
        if (uVar3 == 0)
        {
            uVar8 = 0;
        }
        else
        {
            const int iVar1 = *reinterpret_cast<int*>(DAT_01436e98 + 0x954);
            const int iVar2 = *reinterpret_cast<int*>(iVar1 + 0x1E0);
            if ((static_cast<std::uint32_t>((*reinterpret_cast<int*>(iVar1 + 0x1E4) - iVar2) / 0x2C) <= uVar3) ||
                ((uVar3 = uVar3 * 0x2C + iVar2), uVar3 == 0))
            {
                uVar8 = 0;
            }
            else
            {
                uVar8 = *reinterpret_cast<std::uint32_t*>(
                    ((-(std::uint32_t)(self.m_TextureValue0 <
                    static_cast<std::uint32_t>((*reinterpret_cast<int*>(iVar1 + 0x1E4) - iVar2) / 0x2C)) & uVar3) + 0x28));
            }
        }
        self.m_ResolvedTexture0 = uVar8;
    }

    {
        std::uint32_t uVar3 = self.m_TextureValue1;
        std::uint32_t uVar8;
        if (uVar3 == 0)
        {
            uVar8 = 0;
        }
        else
        {
            const int iVar1 = *reinterpret_cast<int*>(DAT_01436e98 + 0x954);
            const int iVar2 = *reinterpret_cast<int*>(iVar1 + 0x1E0);
            if ((static_cast<std::uint32_t>((*reinterpret_cast<int*>(iVar1 + 0x1E4) - iVar2) / 0x2C) <= uVar3) ||
                ((uVar3 = uVar3 * 0x2C + iVar2), uVar3 == 0))
            {
                uVar8 = 0;
            }
            else
            {
                uVar8 = *reinterpret_cast<std::uint32_t*>(
                    ((-(std::uint32_t)(self.m_TextureValue1 <
                    static_cast<std::uint32_t>((*reinterpret_cast<int*>(iVar1 + 0x1E4) - iVar2) / 0x2C)) & uVar3) + 0x28));
            }
        }
        self.m_ResolvedTexture1 = uVar8;
    }

    {
        std::uint32_t uVar3 = self.m_TextureValue2;
        std::uint32_t uVar8;
        if (uVar3 != 0)
        {
            const int iVar1 = *reinterpret_cast<int*>(DAT_01436e98 + 0x954);
            const int iVar2 = *reinterpret_cast<int*>(iVar1 + 0x1E0);
            if ((uVar3 < static_cast<std::uint32_t>((*reinterpret_cast<int*>(iVar1 + 0x1E4) - iVar2) / 0x2C)) &&
                ((uVar3 = uVar3 * 0x2C + iVar2), uVar3 != 0))
            {
                uVar8 = *reinterpret_cast<std::uint32_t*>(
                    ((-(std::uint32_t)(self.m_TextureValue2 <
                    static_cast<std::uint32_t>((*reinterpret_cast<int*>(iVar1 + 0x1E4) - iVar2) / 0x2C)) & uVar3) + 0x28));
                goto LAB_00bfe808;
            }
        }
        uVar8 = 0;
LAB_00bfe808:
        self.m_ResolvedTexture2 = uVar8;
    }

    if (self.m_bUseSharedIndexBuffer)
    {
        self.m_pIndexBuffer = *reinterpret_cast<CIndexBuffer**>(DAT_01436ea8 + 0x494);
        return true;
    }

    const EPrimitiveType primitiveType = static_cast<EPrimitiveType>(5);
    const std::uint32_t indexCount = static_cast<std::uint16_t>(self.m_nPrimitiveCount) + 2;
    const EIndexBufferFormat indexFormat = static_cast<EIndexBufferFormat>(0x65);
    const std::uint32_t indexBytes = indexCount * 2;
    const ulong indexUsage = CEngineResourceManager::GetIndexBufferUsage(DAT_01436e98);

    self.m_pIndexBuffer = CEngineLandscapeMemoryManager::AllocateIndexBuffer(
        pLandscapeMemoryManager,
        reinterpret_cast<CMovableResource*>(this),
        indexBytes,
        indexUsage,
        indexFormat,
        primitiveType);
    if (self.m_pIndexBuffer == nullptr)
    {
        return false;
    }

    uchar* puVar10 = ::CIndexBuffer::Lock(self.m_pIndexBuffer, 0, 0, 0);
    if ((indexCount != 0) && (stream.m_nBytesRead + indexBytes < 0x80000000U))
    {
        if (static_cast<int>(indexBytes) <= stream.m_nSrcChunkBytesRemaining)
        {
            uchar* puVar13 = stream.m_pSrcChunkCursor;
            for (std::uint32_t uVar11 = indexCount >> 1; uVar11 != 0; uVar11 = uVar11 - 1)
            {
                *reinterpret_cast<std::uint32_t*>(puVar10) = *reinterpret_cast<std::uint32_t*>(puVar13);
                puVar13 = puVar13 + 4;
                puVar10 = puVar10 + 4;
            }
            for (std::uint32_t uVar11 = indexBytes & 3; uVar11 != 0; uVar11 = uVar11 - 1)
            {
                *puVar10 = *puVar13;
                puVar13 = puVar13 + 1;
                puVar10 = puVar10 + 1;
            }

            stream.m_pSrcChunkCursor = stream.m_pSrcChunkCursor + indexBytes;
            stream.m_nSrcChunkBytesRemaining = stream.m_nSrcChunkBytesRemaining + static_cast<int>(indexCount * -2);
            stream.m_nBytesRead = stream.m_nBytesRead + static_cast<int>(indexBytes);
            CVertexBufferWin32::DoUnlock(reinterpret_cast<CVertexBufferWin32*>(self.m_pIndexBuffer));
            return true;
        }

        CDataInputStream::ReadWithSrcChunkOverflow(&in, puVar10, indexBytes);
    }

    CVertexBufferWin32::DoUnlock(reinterpret_cast<CVertexBufferWin32*>(self.m_pIndexBuffer));
    return true;
}