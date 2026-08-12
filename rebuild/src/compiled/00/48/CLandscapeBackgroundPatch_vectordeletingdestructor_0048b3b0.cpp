// CLandscapeBackgroundPatch::`vector deleting destructor'
// Retail 0x0048b3b0 (VC7.1 /O2 /Oy)
//
//   push esi
//   mov  esi, ecx
//   call scalar_dtor            ; this (ecx) still live
//   test byte ptr [esp+8], 1
//   je   skip
//   push esi
//   call operator_delete
//   add  esp, 4
//   skip:
//   mov  eax, esi
//   pop  esi
//   ret  4

// operator delete (cdecl, one ptr arg) -> matches call;add esp,4
void operator_delete_impl(void* p);

struct CLandscapeBackgroundPatch
{
	// scalar destructor: this in ecx, no args. Out-of-line -> a plain call.
	void ScalarDtor();

	// vector deleting destructor: __fastcall, one stack arg (flags), returns this.
	void* vector_deleting_destructor(unsigned int flags);
};

// ScalarDtor is left as an external declaration (no body in this TU) so the
// compiler must emit a real 'call' relocation instead of inlining an empty body.

void* CLandscapeBackgroundPatch::vector_deleting_destructor(unsigned int flags)
{
	this->ScalarDtor();
	if (flags & 1)
		operator_delete_impl(this);
	return this;
}