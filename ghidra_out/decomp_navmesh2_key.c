/* Key navmesh functions extracted from decomp_navmesh.c / decomp_navmesh2.c
 * for the per-node byte-layout RE. Addresses are FableWin base 0x400000.
 * This file is a curated index; full decompiles live in decomp_navmesh.c
 * (SaveToFile/LoadFromFile) and decomp_navmesh2.c (node writers/ctors).
 *
 * ===================================================================
 * GROUND TRUTH READER: CNavQuadTree::LoadFromFile @ 0x0328a760
 *   (decomp_navmesh.c line ~1986)
 * ===================================================================
 * All reads are CCachedFile::Read(ptr, nbytes, 0) little-endian.
 * this = param_1.  worldTL (C2DVector at this+7) added to every stored point.
 *
 * HEADER:
 *   ReadVar<long>(reservedSize)             // 4 bytes, block byte-length (back-patched on write)
 *   Read(&version, 4)          local_6c     // must be 8 (assert version==QUAD_TREE_VERSION_NUMBER); <5 => "saved again"
 *   Read(this+0x10, 4)         MapWidth      // float; assert == map_area.GetWidth()
 *   Read(this+0x14, 4)         MapHeight     // float; assert == map_area.GetHeight()
 *   Read(&numRegions, 4)       local_8c      // uint; regions rebuilt from index only (NO per-region bytes)
 *   Read(&numPositions, 4)     local_20      // uint
 *   numPositions x { Read(pos,0xc) }         // 12 bytes each: float x, float y, long data (+worldTL on x,y)
 *   Read(&numLayers, 4)        local_1c      // uint
 *   Read(&totalNodeCount, 4)   local_68      // long
 *
 * NODE LOOP (local_a8 = 0 .. totalNodeCount-1):
 *   byte b2 (blockedFlag):  if (version>5) Read(&b2,1);  else b2=0    // local_b2
 *   byte a9 (isRealNode):   Read(&a9,1)                               // local_a9
 *   byte b1 (payload/kind): Read(&b1,1)                               // local_b1
 *   if (b2==0 && b1!=0):                                              // BLOCKED SINGLETON
 *       (no further bytes) -> node = shared CNavBlockedQuadTreeNode
 *   else:                                                             // REAL NODE
 *       byte cd (leafKind): Read(&cd,1)     // local_cd: 0 => internal CNavQuadTreeNode; !=0 => leaf
 *       byte c2 (navLayer): Read(&c2,1)     // local_c2 -> ctor param2 -> node+0xc (QuadTreeLevel/detail)
 *       byte c1 (pref):     Read(&c1,1)     // local_c1 -> ctor param3 -> node+0xd (preferability)
 *       C2DVector pos:      Read(pos,8)     // local_c0: float x, float y (+worldTL)
 *       long selfIndex:     Read(&b8,4)     // local_b8: node's own file index (consumed)
 *       DISPATCH:
 *         cd==0        -> CNavQuadTreeNode(pos,c2,c1,file)             // INTERNAL, tail = 4 child longs (16 bytes)
 *         else b2==0   -> CNavNavigableLeafNode(pos,c2,c1,file,regions, version>6)  // NAV LEAF
 *         else         -> CNavSwitchableLeafNode(pos,c2,c1,file,regions, version>6) // SWITCHABLE LEAF
 *   only nodes with a9(isReal)!=0 are placed into the layer grid.
 *
 * END:
 *   Read(&endSym, 4)   // must == 0xACBDEF12 (signed -0x534210ee): "test_end_symbol==END_QUAD_TREE_SYMBOL"
 *
 * ===================================================================
 * NODE TAILS (from ctors in decomp_navmesh2.c, cross-checked vs writers):
 * ===================================================================
 * CNavQuadTreeNode::ctor(...,CAFile&) @ 0x032834d0  [INTERNAL]:
 *   Read(child0,4) Read(child1,4) Read(child2,4) Read(child3,4)   // 16 bytes, node+0x10..+0x1c
 *   Writer CNavQuadTreeNode::SaveToFile @0x032841e0 writes exactly these 4 longs then recurses.
 *
 * CNavNavigableLeafNode::ctor(...,CAFile&,regions,readPref) @ 0x0327fed0  [NAV LEAF]:
 *   Read(&regionIndex,4)                          // 4 bytes; long region internal index (0 => none)
 *   if (readPref) Read(node+0x1c, 1) else node+0x1c=0x40   // 1 byte preferability, ONLY if version>6
 *   Read(&neighbourCount,4)                       // 4 bytes
 *   neighbourCount x Read(&nb,4)                   // 4 bytes each: neighbour node indices
 *   Writer CNavNavigableLeafNode::SaveToFile @0x03280b60: WriteVar<long>(region),
 *     WriteVar<E>(node+0x1c pref), WriteVar<long>(count), count x WriteVar<long>(nbIndex).
 *   NOTE: writer ALWAYS writes the pref byte; reader only consumes it when version>6.
 *         For version 8 (shipped) version>6 is TRUE, so pref byte present. Consistent.
 *
 * CNavSwitchableLeafNode::ctor(...,CAFile&,...) @ 0x032823a0  [SWITCHABLE LEAF]:
 *   (first runs the NAV-LEAF read above: region long + pref byte + neighbour long-list)
 *   Read(&switchStateCount,4)                      // uint (writer: WriteVar<I>)
 *   switchStateCount x Read(&key,8)                // unsigned __int64 each (writer WriteVar<_K>, pair key)
 *   Writer CNavSwitchableLeafNode::SaveToFile @0x03283270 calls the nav-leaf writer first,
 *     then WriteVar<uint>(count) + count x WriteVar<_K>(8-byte key).
 *
 * CNavBlockedQuadTreeNode::SaveToFile @0x032889a0 [BLOCKED]:
 *   if (param3) WriteVar<bool>(vtbl+0x24()) WriteVar<bool>(param3) WriteVar<bool>(node+0xf)
 *   -> exactly the 3-byte prologue, no pos/index.  Matches reader blocked branch.
 *
 * ===================================================================
 * BASE PROLOGUE WRITER: CANavQuadTreeNode::SaveToFile @ 0x0327f5c0
 *   WriteVar<bool>(vtbl+0x24())   // byte1 -> reader b2 (blockedFlag)
 *   WriteVar<bool>(blockedArg)    // byte2 -> reader a9 (isRealNode)
 *   WriteVar<bool>(node+0xf)      // byte3 -> reader b1
 *   WriteVar<bool>(node+0xe)      // byte4 -> reader cd (leafKind: node+0xe; internal ctor sets 0, base sets 1)
 *   WriteVar<E>(node+0xc)         // byte5 -> reader c2 (navLayer/level)
 *   WriteVar<E>(node+0xd)         // byte6 -> reader c1 (preferability)
 *   WriteVar<C2DVector>(pos)      // 8 bytes
 *   WriteVar<long>(selfIndex)     // 4 bytes (from index map)
 *   (Reader consumes these as b2,a9,b1 then cd,c2,c1,pos(8),selfIdx(4).)
 *
 * BASE CTOR fields: CANavQuadTreeNode::ctor(pos,E level,E pref) @ 0x0327f1f0
 *   +0x00 vftable; +0x04 posX(float); +0x08 posY(float);
 *   +0x0c u8 QuadTreeLevel (<=7, MAX_DETAIL_LEVEL); +0x0d u8 preferability;
 *   +0x0e u8 =1 (leaf flag; internal CNavQuadTreeNode ctor sets 0); +0x0f u8 =0 (blocked/switch flag)
 *   GetSize() = DAT_044f344c[level]*... (per-level cell size table).
 *
 * CNavQuadTree::SaveToFile @ 0x03289b50 (decomp_navmesh.c ~1492): header write order =
 *   WriteVar<long>(reservedSize=0 placeholder, back-patched via file->Tell vtbl+0x1c)
 *   WriteVar<long>(version=8)  WriteVar<float>(this+0x10 MapW)  WriteVar<float>(this+0x14 MapH)
 *   WriteVar<uint>(regions.size())  WriteVar<uint>(positions.size())
 *   for_each position: CSaveNavigationPositionToFile ctor  (writes 12 bytes: x,y,data)
 *   WriteVar<uint>(layers.size())
 *   insert (blockedNode -> index 0) into map; counter=1
 *   pass1: for each layer/node node.vtbl[+0x50](file,map,counter)   // assign index (+ payload? see note)
 *   WriteVar<long>(counter-1)  // totalNodeCount
 *   pass2: for each layer/node node.vtbl[+0x54](file,map,0,worldTL) // node payloads
 *   WriteVar<long>(0xACBDEF12)  // END_QUAD_TREE_SYMBOL
 *   Then back-patch reservedSize = current file offset (Tell) written at the reservedSize slot.
 */
