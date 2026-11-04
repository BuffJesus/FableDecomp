// Behaviour fixture for C3DMeshPartitionElement BuildSubElement / ReadFromFile / WriteToFile / Sizeof and the partition
// chunk's WriteToFile / ReadFromFile (provider C3DMeshPartitionElement_Methods_00ac2160.cpp). The output writers are
// supplied here and serialise into a byte buffer (float / long 4 bytes, bool 1 byte); the input stream is raw storage
// whose inline-read window (StreamPos +4, StreamSize +8, CurrentSourcePtr +0xC, SourceChunkBytesRemaining +0x14) covers
// that buffer. Global new / delete count live nodes. Checked: BuildSubElement replaces (and frees) an existing child
// with a fresh zeroed node; Sizeof is 4 per child slot summed over the tree; WriteToFile writes the box, the sphere,
// the triangle range, then a split flag and the two children depth-first; ReadFromFile frees the old children and
// rebuilds an identical tree from that stream; the chunk forwards WriteToFile to its tree and ReadFromFile replaces
// the tree (the old one is deleted with all its nodes).
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_mesh_partition.h"
#include "fable_chunked_file.h"
#include "fable_data_input_stream.h"
#include "fable_data_output_stream.h"

static int g_fail, g_live, g_news, g_overflows;
static unsigned char g_buf[4096]; static long g_len;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

void* operator new(size_t n) { ++g_live; ++g_news; return calloc(1, n + 16); }
void operator delete(void* p) { if (p) { --g_live; free(p); } }

static void Put(const void* p, long n) { memcpy(g_buf + g_len, p, n); g_len += n; }
void CDataOutputStream::WriteFloat(float v) { Put(&v, 4); }
void CDataOutputStream::WriteSLONG(long v) { Put(&v, 4); }
void CDataOutputStream::WriteEBOOL(bool v) { unsigned char b = v ? 1 : 0; Put(&b, 1); }
extern "C" void __fastcall FixOverflow(void*, int, void*, long) { ++g_overflows; }
#pragma comment(linker, "/alternatename:?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z=@FixOverflow@16")

static unsigned long Word(const void* p, unsigned o) { unsigned long v; memcpy(&v, (const unsigned char*)p + o, 4); return v; }
static void SetU(void* p, unsigned o, unsigned long v) { memcpy((unsigned char*)p + o, &v, 4); }

static void Fill(C3DMeshPartitionElement* e, float base, long start, long count)
{
    e->BoundingBox.MinCoord = C3DVector(base, base + 1, base + 2);
    e->BoundingBox.MaxCoord = C3DVector(base + 3, base + 4, base + 5);
    e->BoundingSphere.Center = C3DVector(base + 6, base + 7, base + 8);
    e->BoundingSphere.Radius = base + 9;
    e->StartTriangle = start; e->TriangleCount = count;
}
static bool Same(const C3DMeshPartitionElement* a, const C3DMeshPartitionElement* b)
{
    if (!a || !b) return a == b;
    return !memcmp(&a->BoundingBox, &b->BoundingBox, 24) && !memcmp(&a->BoundingSphere, &b->BoundingSphere, 16) &&
           a->StartTriangle == b->StartTriangle && a->TriangleCount == b->TriangleCount &&
           Same(a->SubElements[0], b->SubElements[0]) && Same(a->SubElements[1], b->SubElements[1]);
}
static void Window(unsigned char* in)
{
    memset(in, 0, 0x40); SetU(in, 4, 0); SetU(in, 8, g_len); SetU(in, 0xC, (unsigned long)g_buf); SetU(in, 0x14, g_len);
}

int main()
{
    // BuildSubElement: a fresh zeroed node; an existing child (with its own child) is freed first.
    C3DMeshPartitionElement* root = new C3DMeshPartitionElement;
    Fill(root, 10, 0, 40);
    C3DMeshPartitionElement* a = root->BuildSubElement(0);
    CHECK(a == root->SubElements[0] && a->StartTriangle == 0 && a->TriangleCount == 0 && !a->SubElements[0] && !a->SubElements[1]);
    a->BuildSubElement(1);
    int live = g_live;
    a = root->BuildSubElement(0);
    CHECK(g_live == live - 1 && a == root->SubElements[0] && !a->SubElements[1]);
    Fill(a, 20, 0, 16);
    C3DMeshPartitionElement* b = root->BuildSubElement(1); Fill(b, 30, 16, 24);
    C3DMeshPartitionElement* c = b->BuildSubElement(0); Fill(c, 40, 16, 10);
    C3DMeshPartitionElement* d = b->BuildSubElement(1); Fill(d, 50, 26, 14);

    // Sizeof: 8 per leaf, children + 4 each otherwise.
    CHECK(c->Sizeof() == 8 && b->Sizeof() == 24 && root->Sizeof() == (8 + 4) + (24 + 4));

    // WriteToFile: node record = 10 floats, 2 longs, flag byte; children depth-first.
    g_len = 0;
    root->WriteToFile(*reinterpret_cast<CDataOutputStream*>(g_buf + 3000));
    CHECK(g_len == 5 * 49);
    float f; long l;
    memcpy(&f, g_buf + 0, 4); CHECK(f == 10);
    memcpy(&f, g_buf + 36, 4); CHECK(f == 19);
    memcpy(&l, g_buf + 40, 4); CHECK(l == 0);
    memcpy(&l, g_buf + 44, 4); CHECK(l == 40);
    CHECK(g_buf[48] == 1);
    memcpy(&f, g_buf + 49, 4); CHECK(f == 20);
    CHECK(g_buf[49 + 48] == 0);
    memcpy(&f, g_buf + 98, 4); CHECK(f == 30);
    CHECK(g_buf[98 + 48] == 1);
    memcpy(&f, g_buf + 147, 4); CHECK(f == 40);
    memcpy(&f, g_buf + 196, 4); CHECK(f == 50);
    CHECK(g_buf[196 + 48] == 0);

    // ReadFromFile into a node that already has children: they are freed, the tree is rebuilt identically.
    static unsigned char in[0x40];
    Window(in);
    C3DMeshPartitionElement* copy = new C3DMeshPartitionElement;
    copy->BuildSubElement(0)->BuildSubElement(1); copy->BuildSubElement(1);
    live = g_live;
    copy->ReadFromFile(*reinterpret_cast<CDataInputStream*>(in));
    CHECK(Same(copy, root) && g_live == live - 3 + 4 && g_overflows == 0);
    CHECK(Word(in, 4) == (unsigned long)g_len && Word(in, 0x14) == 0);

    // The chunk: WriteToFile forwards to the tree; ReadFromFile replaces the tree.
    static unsigned char chunk[0x18];
    memset(chunk, 0, sizeof(chunk));
    C3DMeshFilePartitionChunk* ch = reinterpret_cast<C3DMeshFilePartitionChunk*>(chunk);
    g_len = 0;
    ch->PartitionTree.Data = root;
    ch->C3DMeshFilePartitionChunk::WriteToFile(*reinterpret_cast<CDataOutputStream*>(g_buf + 3000),
                                               *reinterpret_cast<CChunkedFileSaver*>(chunk));
    CHECK(g_len == 5 * 49);
    ch->PartitionTree.Data = 0;
    Window(in);
    live = g_live;
    ch->C3DMeshFilePartitionChunk::ReadFromFile(*reinterpret_cast<CDataInputStream*>(in), g_len,
                                                *reinterpret_cast<CChunkedFileLoader*>(chunk));
    C3DMeshPartitionElement* t1 = ch->PartitionTree.Data;
    CHECK(t1 && Same(t1, root) && Word(chunk, 0x14) != 0 && g_live == live + 5 + 1);
    Window(in);
    live = g_live;
    ch->C3DMeshFilePartitionChunk::ReadFromFile(*reinterpret_cast<CDataInputStream*>(in), g_len,
                                                *reinterpret_cast<CChunkedFileLoader*>(chunk));
    CHECK(ch->PartitionTree.Data != 0 && Same(ch->PartitionTree.Data, root) && g_live == live);

    if (g_fail) { printf("MESH_PARTITION_FAIL %d\n", g_fail); return 1; }
    printf("MESH_PARTITION_OK\n");
    return 0;
}
