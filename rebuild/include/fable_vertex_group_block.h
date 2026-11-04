#pragma once
// Ego_r CVertexGroupBlock (lib_vertex_group_block.cpp, retail 0x00AD8400-
// 0x00ADA4xx): a set of vertex-group indices kept in insertion order, with a
// map from group to its position for the membership tests. 0x18 with
// STLport's 12-byte vector and map (Ego_r 0x20).
#include "fable_carray.h"
#include "fable_stlport_tree.h"

class CVertexGroupBlock
{
public:
    CVertexGroupBlock();                          // 0x00AD9030
    explicit CVertexGroupBlock(unsigned long capacity);  // 0x00AD9070
    ~CVertexGroupBlock();                         // retail COMDAT 0x00A96DA0 (out of line)

    bool HasGroup(unsigned long group) const;
    void AddGroup(unsigned long group);
    void AddGroupsFrom(const CVertexGroupBlock& other);
    void RemoveGroup(unsigned long group);
    void RemoveLast(unsigned long count);
    void Clear();
    bool SharesGroupWith(const CVertexGroupBlock& other) const;
    bool IsSubsetOf(const CVertexGroupBlock& other) const;
    bool IsProperSubsetOf(const CVertexGroupBlock& other) const;

    unsigned long GetGroupCount() const { return Groups.size(); }
    unsigned long GetGroup(unsigned long index) const { return Groups[index]; }

    CArray<unsigned long> Groups;                 // +0x00
    _STL::map<unsigned long, long> GroupToIndexMap;  // +0x0C

private:
    // Retail copies blocks (SetGroupBlock, the optimiser's vectors), but our STLport map
    // model has no deep copy yet; an implicit copy would share the tree nodes.
    CVertexGroupBlock(const CVertexGroupBlock&);
    CVertexGroupBlock& operator=(const CVertexGroupBlock&);
};
FABLE_STATIC_ASSERT(sizeof(CVertexGroupBlock) == 0x18);
