struct CNavigationLayer_t {
    void* a;
    void* b;
    void* c;
    char pad;
    char flag;
};

CNavigationLayer_t* __fastcall CNavigationLayer(CNavigationLayer_t* self) {
    self->a = 0;
    self->b = 0;
    self->c = 0;
    self->flag = 0;
    return self;
}