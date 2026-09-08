#include <stdio.h>
struct C3DMesh2 {
    char m_leading[0xE5];
    bool m_bRelocationBlocked;
    bool CanRelocateData(long) const;
};
bool C3DMesh2::CanRelocateData(long) const { return !m_bRelocationBlocked; }
int main() {
    C3DMesh2 mesh;
    mesh.m_bRelocationBlocked = false;
    if (!mesh.CanRelocateData(123)) return 1;
    mesh.m_bRelocationBlocked = true;
    if (mesh.CanRelocateData(123)) return 2;
    printf("CAN_RELOCATE_DATA PASS\n");
    return 0;
}