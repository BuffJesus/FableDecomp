#pragma optimize("s",on)
// std::_Fill_n<unsigned long*, unsigned int, unsigned long>
// __fastcall(dest in ecx, count in edx, const val& on stack); ret 4.
// Writes _Val into _Count consecutive slots starting at _First, skipping a
// store when the slot address is null. VC7.1 /O2 with size-favoring reaches
// byte parity (index form pins dest into eax + per-iteration value reload).
void __fastcall Fill_n_unsigned_long(unsigned long *_First, unsigned int _Count, unsigned long &_Val)
{
    unsigned int i;
    for (i = 0; i < _Count; ++i)
        if (_First + i != 0)
            _First[i] = _Val;
}