struct CPersistContext;
struct CStringParser;

// helper at 0x9e2590: __fastcall(ecx=context, edx=parser, stack: out-byte-ptr)
extern unsigned char __fastcall PersistByteHelper(CPersistContext* context,
                                                  CStringParser* parser,
                                                  unsigned char* out);

struct CPersistTraits_uchar
{
    void TransferIn(CPersistContext* context, CStringParser* parser, unsigned char* out);
};

void CPersistTraits_uchar::TransferIn(CPersistContext* context,
                                      CStringParser* parser,
                                      unsigned char* out)
{
    PersistByteHelper(context, parser, (unsigned char*)&parser);
    *out = *(unsigned char*)&parser;
}