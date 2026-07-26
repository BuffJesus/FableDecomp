struct CANavQuadTreeNode;
struct CSmallVector_Nav {
    CANavQuadTreeNode** m_data;
};
CANavQuadTreeNode** __fastcall CSmallVector_operator_index(CSmallVector_Nav* self, void* edx, int index) {
    (void)edx;
    return self->m_data + index;
}