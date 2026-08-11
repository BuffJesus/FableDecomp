// CPersistTraits<CReactionFrequencyTraitsArray>::~CPersistTraits
// retail 0x00cb1900:  mov eax,[ecx]; push eax; call 0xcb13f0; ret
//
// The destructor loads the object's first member (a pointer, field0),
// passes it to a __stdcall helper (callee-cleans, so no add esp), and returns.

struct CReactionFrequencyTraitsArray;

// External helper at 0xcb13f0: one pointer arg, callee-cleans the stack.
void __stdcall PersistTraitsHelper(void* p);

struct CPersistTraits_CReactionFrequencyTraitsArray_
{
    void* field0;
};

void __fastcall dtor_CPersistTraits_CReactionFrequencyTraitsArray_(
        CPersistTraits_CReactionFrequencyTraitsArray_* self)
{
    PersistTraitsHelper(self->field0);
}