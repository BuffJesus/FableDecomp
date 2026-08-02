struct CSmallVector_NavNode {
    void** data;

    void** index_b(unsigned long i);
};

void** CSmallVector_NavNode::index_b(unsigned long i) {
    return (void**)data[i];
}