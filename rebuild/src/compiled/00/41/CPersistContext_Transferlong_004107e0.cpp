// CPersistContext::Transfer<long> @ 004107e0
// __fastcall void Transfer(CPersistContext* self, char* name, long* value)
// Forwards to Transfer<signed_char> @ 0x410620 with a zero-inited stack temp.

struct CPersistContext {
    // Callee: __fastcall Transfer<signed_char>(char*, char*, char*) @ 0x410620
    void Transfer_schar_(char* name, char* value, char* extra);
    // Target forwarder
    void Transfer_long_(char* name, long* value);
};

void CPersistContext::Transfer_long_(char* name, long* value)
{
    long local = 0;
    this->Transfer_schar_(name, (char*)value, (char*)&local);
}