struct CPersistContext_TransferCSimVoiceOverlay
{
    std::byte pad00[0x18];
    int mode; // 0x18
    std::byte pad1C[0x08];
    CDataInputStream* inputStream;   // 0x24
    CDataOutputStream* outputStream; // 0x28
};

static_assert(offsetof(CPersistContext_TransferCSimVoiceOverlay, mode) == 0x18);
static_assert(offsetof(CPersistContext_TransferCSimVoiceOverlay, inputStream) == 0x24);
static_assert(offsetof(CPersistContext_TransferCSimVoiceOverlay, outputStream) == 0x28);

struct TransferCSimVoice_Temp
{
    std::byte storage[0x18];
};

static_assert(sizeof(TransferCSimVoice_Temp) == 0x18);

void __thiscall TransferCSimVoice(
    CPersistContext* const context,
    CSimVoice* const voice)
{
    auto* const ctx =
        reinterpret_cast<CPersistContext_TransferCSimVoiceOverlay*>(context);
    alignas(void*) TransferCSimVoice_Temp temp{};

    CPersistContext::CheckCRC(context, "");

    CSimVoice::CSimVoice(reinterpret_cast<CSimVoice*>(&temp));

    if (ctx->mode == 2) {
        // The call site pushes only one stack argument and sets ECX to `voice`;
        // Ghidra's extra carried values are decompiler artifacts, not real parameters.
        C3DMeshFileXTriangleStripChunk::ReadFromFile(
            reinterpret_cast<C3DMeshFileXTriangleStripChunk*>(voice),
            ctx->inputStream);
    } else if (ctx->mode == 3) {
        CSimVoice::TransferBinaryOut(voice, ctx->outputStream);
    }

    CTCInventoryItem::OnDie(reinterpret_cast<CTCInventoryItem*>(&temp));
}