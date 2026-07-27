int __fastcall Math_ComputeByteChunkCount(int value)
{
    return value / 0xFF + 1 + value;
}