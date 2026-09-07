struct CPersistContext_Overlay
{
    std::byte pad00[0x18];
    int mode; // 0x18
    std::byte pad1C[0x08];
    CMemoryDataInputStream* inputStream; // 0x24
    CDataOutputStream* outputStream;     // 0x28
};

static_assert(offsetof(CPersistContext_Overlay, mode) == 0x18);
static_assert(offsetof(CPersistContext_Overlay, inputStream) == 0x24);
static_assert(offsetof(CPersistContext_Overlay, outputStream) == 0x28);

void __thiscall CPhysicalPrimitiveInit_Transfer(
    CPersistContext* const self,
    CPhysicalPrimitiveInit* const value)
{
    auto* const ctx = reinterpret_cast<CPersistContext_Overlay*>(self);

    // Register-carried side outputs materialized by the decompiler as extraout_EDX*.
    CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>* extraout_EDX;
    CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>* extraout_EDX_00;
    CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>* extraout_EDX_01;
    CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>* countedPtr;

    alignas(void*) std::byte transferTemp[sizeof(void*) * 2];

    CPersistContext::CheckCRC(self, "");

    auto* const tempSlots = reinterpret_cast<void**>(transferTemp);
    tempSlots[0] = nullptr;
    tempSlots[1] = nullptr;

    if (ctx->mode == 2) {
        CPersistTraits<CPhysicalPrimitiveInit>::TransferBinaryIn(
            reinterpret_cast<CPersistTraits<CPhysicalPrimitiveInit>*>(transferTemp),
            ctx->inputStream,
            value);
        countedPtr = extraout_EDX_01;
    } else {
        countedPtr = extraout_EDX;
        if (ctx->mode == 3) {
            CPersistTraits<CPhysicalPrimitiveInit>::TransferBinaryOut(
                reinterpret_cast<CPersistTraits<CPhysicalPrimitiveInit>*>(transferTemp),
                ctx->outputStream,
                value);
            countedPtr = extraout_EDX_00;
        }
    }

    std::_Dest_val<
        std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>>,
        CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>>(
            reinterpret_cast<std::allocator<
                CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>>*>(transferTemp),
            countedPtr);
}