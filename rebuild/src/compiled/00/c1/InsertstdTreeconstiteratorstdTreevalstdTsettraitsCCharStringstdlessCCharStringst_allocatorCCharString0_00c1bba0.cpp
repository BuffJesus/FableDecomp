struct Node {
    char pad[0x12];
    unsigned short w;
    unsigned short Helper_c1ba20(void* p);
    void TreeInsert_B(void* p);
};

void Node::TreeInsert_B(void* p)
{
    this->w = this->Helper_c1ba20(p);
}