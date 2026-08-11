#pragma optimize("s",on)
// std::_Fill_n<unsigned long*, unsigned int, unsigned long>
// void __fastcall(unsigned long* dest /*ecx*/, unsigned int count /*edx*/,
//                 const unsigned long& value /*[esp+4]*/)
//
// Retail body:
//   test edx,edx; mov eax,ecx; jbe end     ; if count==0 return, iterate in eax
//   loop:
//     test eax,eax; je skip                ; skip store when element ptr is null
//       mov ecx,[esp+4]; mov ecx,[ecx]; mov [eax],ecx   ; reload &value + value each iter
//     skip: add eax,4; dec edx; jne loop
//   end: ret 4
//
// The per-element null test on the (incremented) destination pointer and the
// per-iteration reload of the value reference reproduce exactly with an
// index-based loop that guards on (_First + _I) and keeps the induction pointer
// in eax while ecx is the value scratch.

void __fastcall _Fill_n(unsigned long *_First, unsigned int _Count, const unsigned long &_Val)
{
    for (unsigned int _I = 0; _I < _Count; ++_I)
        if (_First + _I != 0)
            _First[_I] = _Val;
}