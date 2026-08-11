#include <cstdio>

// Mirror the source's class shape so we can drive its logic in a standalone test.
struct CEngineSubPrimitiveGameMesh
{
    int scalarCalled;
    int deleteCalled;
    void ScalarDtor();
    void DeleteHelper();
    void* VectorDeletingDestructor(unsigned int flags);
};

void CEngineSubPrimitiveGameMesh::ScalarDtor()  { scalarCalled = 1; }
void CEngineSubPrimitiveGameMesh::DeleteHelper(){ deleteCalled = 1; }

void* CEngineSubPrimitiveGameMesh::VectorDeletingDestructor(unsigned int flags)
{
    this->ScalarDtor();
    if (flags & 1)
        this->DeleteHelper();
    return this;
}

int main()
{
    // flags bit0 clear: dtor runs, no delete
    CEngineSubPrimitiveGameMesh a;
    a.scalarCalled = 0; a.deleteCalled = 0;
    void* ra = a.VectorDeletingDestructor(0);
    bool ok = (ra == &a) && a.scalarCalled == 1 && a.deleteCalled == 0;

    // flags bit0 set: dtor runs, delete runs
    CEngineSubPrimitiveGameMesh b;
    b.scalarCalled = 0; b.deleteCalled = 0;
    void* rb = b.VectorDeletingDestructor(1);
    ok = ok && (rb == &b) && b.scalarCalled == 1 && b.deleteCalled == 1;

    // even flag value with bit0 clear -> no delete
    CEngineSubPrimitiveGameMesh c;
    c.scalarCalled = 0; c.deleteCalled = 0;
    c.VectorDeletingDestructor(2);
    ok = ok && c.scalarCalled == 1 && c.deleteCalled == 0;

    if (ok) { printf("VDD_OK\n"); return 0; }
    printf("VDD_FAIL\n"); return 1;
}