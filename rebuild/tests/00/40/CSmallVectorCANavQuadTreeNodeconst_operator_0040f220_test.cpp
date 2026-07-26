#include <cstdio>
struct CANavQuadTreeNode;
struct CSmallVector_Nav {
    CANavQuadTreeNode** m_data;
};
CANavQuadTreeNode** __fastcall CSmallVector_operator_index(CSmallVector_Nav* self, void* edx, int index) {
    (void)edx;
    return self->m_data + index;
}
int main() {
    CANavQuadTreeNode* buf[8];
    CSmallVector_Nav v;
    v.m_data = buf;
    if (CSmallVector_operator_index(&v, 0, 0) != &buf[0]) { std::printf("FAIL idx0\n"); return 1; }
    if (CSmallVector_operator_index(&v, 0, 3) != &buf[3]) { std::printf("FAIL idx3\n"); return 1; }
    if (CSmallVector_operator_index(&v, 0, 7) != &buf[7]) { std::printf("FAIL idx7\n"); return 1; }
    std::printf("CSmallVectorNav_0040f220_TEST PASS\n");
    return 0;
}