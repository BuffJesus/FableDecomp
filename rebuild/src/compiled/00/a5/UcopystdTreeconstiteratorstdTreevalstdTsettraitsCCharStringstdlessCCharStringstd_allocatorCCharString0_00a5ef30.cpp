// Faithful reconstruction of retail 0x00a5ef30
// _Ucopy<std::_Tree_const_iterator<...>> forwarder (__fastcall, struct-ptr return)
//
// Retail body:
//   mov eax,[esp+0xc] ; arg3 (dest)
//   mov edx,[esp+4]   ; arg1 (first iter)
//   mov ecx,[ecx+0x188]; inner container
//   push eax          ; arg3
//   mov eax,[esp+0xc] ; arg2 (last iter)
//   push edx          ; arg1
//   push eax          ; arg2
//   call inner->_Ucopy
//   ret 0xc
//
// i.e. return this->m188->_Ucopy(arg2, arg1, arg3);

struct CCharString;

struct Inner;

// inner thiscall member: _Ucopy(last, first, dest) -> dest ptr
struct Inner {
	CCharString* Ucopy(void* last, void* first, CCharString* dest);
};

struct Outer {
	char pad[0x188];
	Inner* m188;
	// method models retail forwarder: params first(arg1), last(arg2), dest(arg3)
	CCharString* Ucopy_forward(void* first, void* last, CCharString* dest);
};

CCharString* Outer::Ucopy_forward(void* first, void* last, CCharString* dest)
{
	return this->m188->Ucopy(last, first, dest);
}