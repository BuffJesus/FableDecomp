// Faithful reconstruction of the GFPredicateAnd<...>::CPredicate_And<...>::Build
// forwarder at retail 0x00b32ff0.
//
// Retail body (__fastcall, struct return):
//   mov eax,[esp+8]          ; arg2 = thing
//   mov ecx,[ecx+0x298]      ; ecx = this->m298  (pointer member)
//   push esi
//   mov esi,[esp+8]          ; esi = arg1 = hidden return buffer
//   push eax                 ; thing
//   push esi                 ; retbuf
//   call 0xbaba70            ; this->m298->Build(retbuf, thing)  (thiscall, struct ret)
//   mov eax,esi
//   pop esi
//   ret 8
//
// Model: an outer predicate object holding a pointer to a nested predicate at
// +0x298; the outer Build forwards to the nested predicate's struct-returning
// Build, returning the result object by value.  NRVO forwards the caller's
// return buffer straight through to the nested call.

struct CIsThingAlive;

// The returned-by-value result object (large enough that MSVC returns it via a
// hidden buffer pointer -> ret 8 with one real param).
struct CResult
{
	int a;
	int b;
	int c;
};

// Nested predicate whose Build the outer forwards to.  Build is a struct-return
// thiscall method taking the CIsThingAlive* argument.
struct CNested
{
	CResult Build(CIsThingAlive* thing);
};

// Outer predicate: pointer to the nested object lives at offset 0x298.
struct COuter
{
	char pad[0x298];
	CNested* m298;

	CResult CPredicate_And_Build(CIsThingAlive* thing);
};

// Out-of-line definition so the function is actually emitted.
CResult COuter::CPredicate_And_Build(CIsThingAlive* thing)
{
	return m298->Build(thing);
}