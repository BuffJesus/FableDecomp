// Behaviour fixture for the face partition tree's small members (provider CPhysicsFacesSegment_Members_00a85fb0.cpp;
// it defines none of them). Branches and leaves are raw storage at the retail offsets the header pins, driven by
// qualified calls; children are counting test segments.
#include "fable_physics_faces_segment.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int g_fail, g_destroyed;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

struct CTestSegment : CIPhysicsFacesSegmentBase
{
    long Faces, Size;
    CTestSegment(long faces, long size) : Faces(faces), Size(size) {}
    virtual bool HasChildren() const { return false; }
    virtual long Sizeof() const { return Size; }
    virtual long GetNoFaces() const { return Faces; }
    virtual ~CTestSegment() { ++g_destroyed; }
protected:
    virtual void GetPossibleCollisionSet(float, const C3DVector&, const C3DVector&, _STL::vector<long>&) const {}
};

// The collision-set queries (vtable slot 4 of both nodes) are not reached here.
extern "C" void NeverCalledCollisionSet() { printf("FAIL unreached slot\n"); abort(); }
#pragma comment(linker, "/alternatename:?GetPossibleCollisionSet@CPhysicsFacesSegmentBranch@@MBEXMABVC3DVector@@0AAV?$vector@JV?$allocator@J@_STL@@@_STL@@@Z=_NeverCalledCollisionSet")
#pragma comment(linker, "/alternatename:?GetPossibleCollisionSet@CPhysicsFacesSegmentLeaf@@MBEXMABVC3DVector@@0AAV?$vector@JV?$allocator@J@_STL@@@_STL@@@Z=_NeverCalledCollisionSet")

struct CBranchTest : CPhysicsFacesSegmentBranch
{
    bool Has() const { return CPhysicsFacesSegmentBranch::HasChildren(); }
    long Size() const { return CPhysicsFacesSegmentBranch::Sizeof(); }
    long Faces() const { return CPhysicsFacesSegmentBranch::GetNoFaces(); }
    void Destroy() { CPhysicsFacesSegmentBranch::~CPhysicsFacesSegmentBranch(); }
};
struct CLeafTest : CPhysicsFacesSegmentLeaf
{
    bool Has() const { return CPhysicsFacesSegmentLeaf::HasChildren(); }
    long Size() const { return CPhysicsFacesSegmentLeaf::Sizeof(); }
    long Faces() const { return CPhysicsFacesSegmentLeaf::GetNoFaces(); }
    void Destroy() { CPhysicsFacesSegmentLeaf::~CPhysicsFacesSegmentLeaf(); }
};

int main()
{
    // Branch: +0x14 positive child, +0x18 negative child.
    static unsigned char branch[0x1C];
    memset(branch, 0, sizeof(branch));
    CTestSegment* positive = new CTestSegment(7, 0x40);
    CTestSegment* negative = new CTestSegment(5, 0x24);
    memcpy(branch + 0x14, &positive, 4); memcpy(branch + 0x18, &negative, 4);
    CBranchTest* b = reinterpret_cast<CBranchTest*>(branch);
    CHECK(b->Has());
    CHECK(b->Faces() == 12);
    CHECK(b->Size() == 0x40 + 0x24 + 0x1C);
    b->Destroy();
    CHECK(g_destroyed == 2);
    CHECK(*reinterpret_cast<void**>(branch + 0x14) == 0 && *reinterpret_cast<void**>(branch + 0x18) == 0);
    // A branch without children destroys nothing.
    b->Destroy();
    CHECK(g_destroyed == 2);

    // Leaf: TriangleListIndex at +0x04 (start, finish, end of storage) over unsigned short indices.
    static unsigned char leaf[0x10];
    unsigned short* indices = static_cast<unsigned short*>(malloc(10 * sizeof(unsigned short)));
    unsigned short* parts[3] = { indices, indices + 3, indices + 10 };
    memcpy(leaf + 4, parts, sizeof(parts));
    CLeafTest* l = reinterpret_cast<CLeafTest*>(leaf);
    CHECK(!l->Has());
    CHECK(l->Faces() == 3);
    CHECK(l->Size() == 0xC + 10 * 2);        // the array object plus its whole allocation
    l->Destroy();                            // frees the index storage

    printf(g_fail ? "FAIL\n" : "PHYSICS_FACES_SEGMENT_OK\n");
    return g_fail != 0;
}
