#pragma optimize("s",on)
// CPersistTraits<EInputKey>::TransferIn @ 0x005d8167
typedef int EInputKey;

struct CPersistContext;
struct CStringParser;
struct CPersistTraits;

// Callee at 0x9e2590: takes ecx, edx, one stack pointer arg.
extern void __fastcall Persist_ReadValue(CPersistContext* ecx, CStringParser* edx, void* slot);

// __fastcall member modeled as __fastcall free fn:
//   self -> ecx (unused this), edx_dummy -> edx, then 3 stack args.
void __fastcall TransferIn(CPersistTraits* self, int edx_dummy,
                           CPersistContext* ctx, CStringParser* parser, EInputKey* out)
{
    (void)self; (void)edx_dummy;
    Persist_ReadValue(ctx, parser, &parser);
    *out = (EInputKey)(long)parser;
}