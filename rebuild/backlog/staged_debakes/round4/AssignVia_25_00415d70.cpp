// Hand (dst, this->data) to a fixed global helper object (0x013CA828) and return dst.
// __fastcall this=ecx, dst=stack (ret 4).
struct Pool { void Assign(void* dst, void* src); };
extern Pool g_pool;
struct CCharString {
    void* data;
    void* AssignTo(void* dst);
};
void* CCharString::AssignTo(void* dst) {
    g_pool.Assign(dst, this->data);
    return dst;
}
