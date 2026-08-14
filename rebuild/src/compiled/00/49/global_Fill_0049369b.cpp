#pragma optimize("s",on)
struct T { char raw[0x70]; };

extern "C" void* __cdecl op_new(unsigned int);

void __stdcall Fill(T** begin, T** end)
{
    for (T** p = begin; p < end; ++p)
    {
        *p = (T*)op_new(0x70);
    }
}