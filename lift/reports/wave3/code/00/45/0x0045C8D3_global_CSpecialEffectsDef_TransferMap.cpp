struct CBinarySearchTreeWriteRangeContext;

void __fastcall BinarySearchTree_WriteRangeToStream(
    CBinarySearchTreeWriteRangeContext* context,
    std::map<long, long, std::less<long>>* value);

struct CPersistContext_TransferMapOverlay {
    std::byte pad00[0x18];
    int mode;
    std::byte pad1C[0x08];
    CMemoryDataInputStream* inputStream;
    CBinarySearchTreeWriteRangeContext* writeRangeContext;
};

static_assert(offsetof(CPersistContext_TransferMapOverlay, mode) == 0x18);
static_assert(offsetof(CPersistContext_TransferMapOverlay, inputStream) == 0x24);
static_assert(offsetof(CPersistContext_TransferMapOverlay, writeRangeContext) == 0x28);

void __thiscall CSpecialEffectsDef_TransferMap(
    CPersistContext* const context,
    std::map<long, long, std::less<long>>* const value)
{
    alignas(void*) std::byte local10[12];
    auto* const contextView =
        reinterpret_cast<CPersistContext_TransferMapOverlay*>(context);

    CPersistContext::CheckCRC(context, "");
    CDisplayManager::CopyBackBufferToTexture(
        reinterpret_cast<CDisplayManager*>(local10));

    if (contextView->mode == 2) {
        GFSerialiseMapBinaryIn<long, long, std::less<long>>(
            contextView->inputStream,
            value);
    } else if (contextView->mode == 3) {
        BinarySearchTree_WriteRangeToStream(
            contextView->writeRangeContext,
            value);
    }

    CFileInstaller::CActiveFile::OnReadFinished(
        reinterpret_cast<CFileInstaller::CActiveFile*>(local10));
}