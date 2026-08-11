struct CResource {
    char pad[0xc];
    CResource* a;   // +0xc
    CResource* b;   // +0x10
    void ListInsert(CResource* p1, CResource* p2);
};

void CResource::ListInsert(CResource* p1, CResource* p2)
{
    this->a = p1;
    this->b = p2;
    p1->b = this;
    this->b->a = this;
}