class CDataOutputStream {
public:
    void Write4Bytes(unsigned long value);
};

struct CFloatVector {
    float* First;
    float* Last;
    float* EndOfStorage;
};

void __fastcall GFSerialiseVectorBinaryOut_float(CDataOutputStream* output, CFloatVector* values)
{
    const long count = values->Last - values->First;
    output->Write4Bytes(count);
    for (long index = 0; index < count; ++index)
        output->Write4Bytes(*(unsigned long*)&values->First[index]);
}