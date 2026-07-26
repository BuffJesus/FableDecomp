struct CActionInputControl { int m[7]; };

CActionInputControl* __fastcall Copy_impl(CActionInputControl* _First, CActionInputControl* _Last, CActionInputControl* _Dest, int _pad)
{
    for (; _First != _Last; ++_First, ++_Dest)
        if (_Dest)
            *_Dest = *_First;
    return _Dest;
}