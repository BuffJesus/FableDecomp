#pragma optimize("s",on)
// operator_new @ 004c73b0
// push ecx ; call helper_bfeb1c ; pop ecx ; ret
// __fastcall(size in ecx) -> restack as cdecl stack arg, cdecl cleanup via pop ecx
typedef unsigned int size_type;

extern "C" void * __cdecl helper_bfeb1c(size_type n);

void * __fastcall operator_new(size_type n)
{
    return helper_bfeb1c(n);
}