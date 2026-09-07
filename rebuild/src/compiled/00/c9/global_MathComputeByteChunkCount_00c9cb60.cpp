int __fastcall Math_ComputeByteChunkCount(int value)
{
    return value / 255 + 1 + value;
}