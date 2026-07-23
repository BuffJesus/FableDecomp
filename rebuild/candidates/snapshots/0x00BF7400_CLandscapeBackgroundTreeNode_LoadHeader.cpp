void CLandscapeBackgroundTreeNode::LoadHeader(CDataInputStream& in)
{
    struct LayerEntry
    {
        std::uint32_t x;        // 0x00
        std::uint32_t y;        // 0x04
        std::uint32_t z;        // 0x08
        std::uint8_t selector;  // 0x0C
        std::uint8_t pad[3];    // 0x0D
    };

    struct HeaderOverlay
    {
        std::byte pad00[0x0C];
        LayerEntry layerEntries[8];        // 0x0C
        std::uint32_t headerVec0X;         // 0x8C
        std::uint32_t headerVec0Y;         // 0x90
        std::uint32_t headerVec0Z;         // 0x94
        std::byte pad98[0x10];
        std::uint32_t headerVec1X;         // 0xA8
        std::uint32_t headerVec1Y;         // 0xAC
        std::uint32_t headerVec1Z;         // 0xB0
        std::uint32_t headerVec2X;         // 0xB4
        std::uint32_t headerVec2Y;         // 0xB8
        std::uint32_t headerVec2Z;         // 0xBC
        std::byte padC0[0x18];
        std::uint16_t headerWord0;         // 0xD8
        std::uint16_t headerWord1;         // 0xDA
        std::uint16_t headerWord2;         // 0xDC
        std::uint16_t headerWord3;         // 0xDE
        std::uint8_t unknownByte0;         // 0xE0
        std::uint8_t layerStartIndex;      // 0xE1
        std::uint8_t layerEndIndex;        // 0xE2
        std::byte padE3[3];
        std::uint8_t headerLoaded;         // 0xE6
    };

    static_assert(offsetof(HeaderOverlay, layerEntries) == 0x0C);
    static_assert(offsetof(HeaderOverlay, headerVec0X) == 0x8C);
    static_assert(offsetof(HeaderOverlay, headerVec1X) == 0xA8);
    static_assert(offsetof(HeaderOverlay, headerWord0) == 0xD8);
    static_assert(offsetof(HeaderOverlay, headerLoaded) == 0xE6);

    auto& self = *reinterpret_cast<HeaderOverlay*>(this);
    auto* const streamBase = reinterpret_cast<std::uint8_t*>(&in);

    auto& readPosition = *reinterpret_cast<std::int32_t*>(streamBase + 0x04);
    auto& srcPtr16 = *reinterpret_cast<std::uint16_t**>(streamBase + 0x0C);
    auto& srcPtr8 = *reinterpret_cast<std::uint8_t**>(streamBase + 0x0C);
    auto& srcPtr32 = *reinterpret_cast<std::uint32_t**>(streamBase + 0x0C);
    auto& available = *reinterpret_cast<std::int32_t*>(streamBase + 0x14);

    std::uint32_t scratch = 0;
    std::uint32_t local_28 = 0;
    std::uint32_t local_24 = 0;
    std::uint32_t local_20 = 0;
    std::uint32_t local_30 = 0;
    std::uint32_t local_2c = 0;

    if (readPosition + 2U < 0x80000000U) {
        if (available < 2) {
            CDataInputStream::ReadWithSrcChunkOverflow(&in, reinterpret_cast<uchar*>(&scratch), 2);
        } else {
            scratch = (scratch & 0xFFFF0000U) | *srcPtr16;
            ++srcPtr16;
            available = available - 2;
            readPosition = static_cast<std::int32_t>(readPosition + 2U);
        }
    }
    self.headerWord0 = static_cast<std::uint16_t>(scratch);

    if (readPosition + 2U < 0x80000000U) {
        if (available < 2) {
            CDataInputStream::ReadWithSrcChunkOverflow(&in, reinterpret_cast<uchar*>(&scratch), 2);
        } else {
            scratch = (scratch & 0xFFFF0000U) | *srcPtr16;
            ++srcPtr16;
            available = available - 2;
            readPosition = static_cast<std::int32_t>(readPosition + 2U);
        }
    }
    self.headerWord1 = static_cast<std::uint16_t>(scratch);

    if (readPosition + 2U < 0x80000000U) {
        if (available < 2) {
            CDataInputStream::ReadWithSrcChunkOverflow(&in, reinterpret_cast<uchar*>(&scratch), 2);
        } else {
            scratch = (scratch & 0xFFFF0000U) | *srcPtr16;
            ++srcPtr16;
            available = available - 2;
            readPosition = static_cast<std::int32_t>(readPosition + 2U);
        }
    }
    self.headerWord2 = static_cast<std::uint16_t>(scratch);

    if (readPosition + 2U < 0x80000000U) {
        if (available < 2) {
            CDataInputStream::ReadWithSrcChunkOverflow(&in, reinterpret_cast<uchar*>(&scratch), 2);
        } else {
            scratch = (scratch & 0xFFFF0000U) | *srcPtr16;
            ++srcPtr16;
            available = available - 2;
            readPosition = static_cast<std::int32_t>(readPosition + 2U);
        }
    }
    self.headerWord3 = static_cast<std::uint16_t>(scratch);

    if (readPosition + 1U < 0x80000000U) {
        if (available < 1) {
            CDataInputStream::ReadWithSrcChunkOverflow(&in, reinterpret_cast<uchar*>(&scratch), 1);
        } else {
            scratch = (scratch & 0xFFFFFF00U) | *srcPtr8;
            ++srcPtr8;
            available = available - 1;
            readPosition = static_cast<std::int32_t>(readPosition + 1U);
        }
    }
    self.unknownByte0 = static_cast<std::uint8_t>(scratch);

    if (readPosition + 1U < 0x80000000U) {
        if (available < 1) {
            CDataInputStream::ReadWithSrcChunkOverflow(&in, reinterpret_cast<uchar*>(&scratch), 1);
        } else {
            scratch = (scratch & 0xFFFFFF00U) | *srcPtr8;
            ++srcPtr8;
            available = available - 1;
            readPosition = static_cast<std::int32_t>(readPosition + 1U);
        }
    }
    self.layerStartIndex = static_cast<std::uint8_t>(scratch);

    if (readPosition + 1U < 0x80000000U) {
        if (available < 1) {
            CDataInputStream::ReadWithSrcChunkOverflow(&in, reinterpret_cast<uchar*>(&scratch), 1);
        } else {
            scratch = (scratch & 0xFFFFFF00U) | *srcPtr8;
            ++srcPtr8;
            available = available - 1;
            readPosition = static_cast<std::int32_t>(readPosition + 1U);
        }
    }
    self.layerEndIndex = static_cast<std::uint8_t>(scratch);

    if (readPosition + 4U < 0x80000000U) {
        if (available < 4) {
            CDataInputStream::ReadWithSrcChunkOverflow(&in, reinterpret_cast<uchar*>(&scratch), 4);
        } else {
            scratch = *srcPtr32;
            ++srcPtr32;
            available = available - 4;
            readPosition = static_cast<std::int32_t>(readPosition + 4U);
        }
    }
    self.headerVec0X = scratch;

    if (readPosition + 4U < 0x80000000U) {
        if (available < 4) {
            CDataInputStream::ReadWithSrcChunkOverflow(&in, reinterpret_cast<uchar*>(&scratch), 4);
        } else {
            scratch = *srcPtr32;
            ++srcPtr32;
            available = available - 4;
            readPosition = static_cast<std::int32_t>(readPosition + 4U);
        }
    }
    self.headerVec0Y = scratch;

    if (readPosition + 4U < 0x80000000U) {
        if (available < 4) {
            CDataInputStream::ReadWithSrcChunkOverflow(&in, reinterpret_cast<uchar*>(&scratch), 4);
        } else {
            scratch = *srcPtr32;
            ++srcPtr32;
            available = available - 4;
            readPosition = static_cast<std::int32_t>(readPosition + 4U);
        }
    }
    self.headerVec0Z = scratch;

    if (readPosition + 4U < 0x80000000U) {
        if (available < 4) {
            CDataInputStream::ReadWithSrcChunkOverflow(&in, reinterpret_cast<uchar*>(&local_28), 4);
        } else {
            local_28 = *srcPtr32;
            ++srcPtr32;
            available = available - 4;
            readPosition = static_cast<std::int32_t>(readPosition + 4U);
        }
    }

    if (readPosition + 4U < 0x80000000U) {
        if (available < 4) {
            CDataInputStream::ReadWithSrcChunkOverflow(&in, reinterpret_cast<uchar*>(&local_24), 4);
        } else {
            local_24 = *srcPtr32;
            ++srcPtr32;
            available = available - 4;
            readPosition = static_cast<std::int32_t>(readPosition + 4U);
        }
    }

    if (readPosition + 4U < 0x80000000U) {
        if (available < 4) {
            CDataInputStream::ReadWithSrcChunkOverflow(&in, reinterpret_cast<uchar*>(&local_20), 4);
        } else {
            local_20 = *srcPtr32;
            ++srcPtr32;
            available = available - 4;
            readPosition = static_cast<std::int32_t>(readPosition + 4U);
        }
    }

    if (readPosition + 4U < 0x80000000U) {
        if (available < 4) {
            CDataInputStream::ReadWithSrcChunkOverflow(&in, reinterpret_cast<uchar*>(&scratch), 4);
        } else {
            scratch = *srcPtr32;
            ++srcPtr32;
            available = available - 4;
            readPosition = static_cast<std::int32_t>(readPosition + 4U);
        }
    }

    if (readPosition + 4U < 0x80000000U) {
        if (available < 4) {
            CDataInputStream::ReadWithSrcChunkOverflow(&in, reinterpret_cast<uchar*>(&local_30), 4);
        } else {
            local_30 = *srcPtr32;
            ++srcPtr32;
            available = available - 4;
            readPosition = static_cast<std::int32_t>(readPosition + 4U);
        }
    }

    if (readPosition + 4U < 0x80000000U) {
        if (available < 4) {
            CDataInputStream::ReadWithSrcChunkOverflow(&in, reinterpret_cast<uchar*>(&local_2c), 4);
        } else {
            local_2c = *srcPtr32;
            ++srcPtr32;
            available = available - 4;
            readPosition = static_cast<std::int32_t>(readPosition + 4U);
        }
    }

    self.headerVec1X = local_28;
    self.headerVec1Y = local_24;
    self.headerVec1Z = local_20;
    self.headerVec2X = scratch;
    self.headerVec2Y = local_30;
    self.headerVec2Z = local_2c;

    if (self.layerStartIndex > 7) {
        self.headerLoaded = 1;
        return;
    }

    std::uint32_t layerIndex = static_cast<std::uint32_t>(self.layerStartIndex);
    if (self.layerEndIndex < layerIndex) {
        self.headerLoaded = 1;
        return;
    }

    LayerEntry* entry = reinterpret_cast<LayerEntry*>(reinterpret_cast<std::uint8_t*>(this) + 0x0C + layerIndex * 0x10);

    do {
        if (readPosition + 1U < 0x80000000U) {
            if (available < 1) {
                CDataInputStream::ReadWithSrcChunkOverflow(&in, reinterpret_cast<uchar*>(&scratch), 1);
            } else {
                scratch = (scratch & 0xFFFFFF00U) | *srcPtr8;
                ++srcPtr8;
                available = available - 1;
                readPosition = static_cast<std::int32_t>(readPosition + 1U);
            }
        }
        entry->selector = static_cast<std::uint8_t>(scratch);

        if (readPosition + 4U < 0x80000000U) {
            if (available < 4) {
                CDataInputStream::ReadWithSrcChunkOverflow(&in, reinterpret_cast<uchar*>(&local_20), 4);
            } else {
                local_20 = *srcPtr32;
                ++srcPtr32;
                available = available - 4;
                readPosition = static_cast<std::int32_t>(readPosition + 4U);
            }
        }
        entry->x = local_20;

        if (readPosition + 4U < 0x80000000U) {
            if (available < 4) {
                CDataInputStream::ReadWithSrcChunkOverflow(&in, reinterpret_cast<uchar*>(&local_24), 4);
            } else {
                local_24 = *srcPtr32;
                ++srcPtr32;
                available = available - 4;
                readPosition = static_cast<std::int32_t>(readPosition + 4U);
            }
        }
        entry->y = local_24;

        if (readPosition + 4U < 0x80000000U) {
            if (available < 4) {
                CDataInputStream::ReadWithSrcChunkOverflow(&in, reinterpret_cast<uchar*>(&local_28), 4);
            } else {
                local_28 = *srcPtr32;
                ++srcPtr32;
                available = available - 4;
                readPosition = static_cast<std::int32_t>(readPosition + 4U);
            }
        }
        entry->z = local_28;

        layerIndex = layerIndex + 1;
        entry = reinterpret_cast<LayerEntry*>(reinterpret_cast<std::uint8_t*>(entry) + 0x10);
    } while (static_cast<std::int32_t>(layerIndex) <= static_cast<std::int32_t>(static_cast<std::uint32_t>(self.layerEndIndex)));

    self.headerLoaded = 1;
}