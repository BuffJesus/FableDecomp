void operator delete(void* p);
struct Ctl {
    long rc;
    void (__fastcall *fn)(void* self);
    void* ctx;
};
struct CCPtr {
    void* info;
    Ctl*  p;
    void ShareData(void* newInfo, Ctl* np);
};
void CCPtr::ShareData(void* newInfo, Ctl* np) {
    if (this->p != np) {
        if (this->p) {
            if (--this->p->rc == 0) {
                this->p->fn(this->p->ctx);
                ::operator delete(this->p);
            }
        }
        this->info = newInfo;
        this->p = np;
        if (np) ++np->rc;
    }
}