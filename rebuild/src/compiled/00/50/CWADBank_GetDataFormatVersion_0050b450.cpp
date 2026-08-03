// CWADBank::GetDataFormatVersion @ 0x0050b450
// Retail bytes: b8 02 00 00 00 c3  =>  mov eax, 2 ; ret
// __fastcall: this is passed in ecx but unused; the method just returns
// the constant data-format version 2. A plain int-returning function
// lowers to exactly `mov eax,2 ; ret`.
int GetDataFormatVersion()
{
    return 2;
}