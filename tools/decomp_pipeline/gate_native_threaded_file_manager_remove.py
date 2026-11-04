"""Fresh VC71 parity gate for CThreadedFileManager::RemoveObject 0098EB20, the uid map find 009902D0 and the
pending-queue iterator operator++ 0098F9D0, the pending sort-key less<> 0098F570, pending _M_find 0098FE70 and the
pending key-erase closure (00990CD0, 009905D0, 0098FCA0, 009901D0, 0098FF50, 0098FD40) and the uid
key-erase closure (00990D60, 009907D0, 0098FFA0, 009906E0, 0098FD80), plus OnFinishObject 0098EBC0
(native-first 15,360 cases in work/native-recreation/threaded/0098ebc0).

Both roots compile from canonical rebuild/include (retail PC manager layout in
fable_gfuninitialise.h, CFileObject in fable_threaded_file_object.h) to retail bytes
once every relocation is patched with an independently pinned symbol address.
Behavior was exercised in the work lane (retail bytes vs candidate with genuine find,
erase(iterator) and rebalance, real critical section, 75,264 cases); this gate
re-proves byte identity, so no model is re-run here.
"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
TREE='?$_Rb_tree@KU?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@U?$_Select1st@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@2@@_STL@@'
FIND='?find@'+TREE+'QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@2@ABK@Z'
ERASE='?erase@'+TREE+'QAEXU?$_Rb_tree_iterator@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@2@@Z'
# Symbol identities pinned by other accepted gates/imports, not derived from these roots.
ADDRESS={
 '__imp__EnterCriticalSection@4':0x143fe0c,
 '__imp__LeaveCriticalSection@4':0x143fe10,
 '__imp__GlobalFree@4':0x143fdf8,
 '??3@YAXPAX@Z':0xbfe9bc,
 FIND:0x9902d0,
 ERASE:0x990430,  # landed 00/99/STLThreadedUIDTree_IteratorErase_00990430.cpp
 '?erase@?$_Rb_tree@UCPendingObjectSortKey@CThreadedFileManager@@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@U?$_Select1st@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@4@U?$less@UCPendingObjectSortKey@CThreadedFileManager@@@4@V?$allocator@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@4@@_STL@@QAEIABUCPendingObjectSortKey@CThreadedFileManager@@@Z':0x990cd0,
 '?erase@?$_Rb_tree@UCPendingObjectSortKey@CThreadedFileManager@@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@U?$_Select1st@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@4@U?$less@UCPendingObjectSortKey@CThreadedFileManager@@@4@V?$allocator@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@4@@_STL@@QAEXU?$_Rb_tree_iterator@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@2@0@Z':0x9905d0,
 '?_M_upper_bound@?$_Rb_tree@UCPendingObjectSortKey@CThreadedFileManager@@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@U?$_Select1st@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@4@U?$less@UCPendingObjectSortKey@CThreadedFileManager@@@4@V?$allocator@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@4@@_STL@@QBEPAU?$_Rb_tree_node@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@2@ABUCPendingObjectSortKey@CThreadedFileManager@@@Z':0x98fca0,
 '?_M_lower_bound@?$_Rb_tree@UCPendingObjectSortKey@CThreadedFileManager@@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@U?$_Select1st@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@4@U?$less@UCPendingObjectSortKey@CThreadedFileManager@@@4@V?$allocator@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@4@@_STL@@QBEPAU?$_Rb_tree_node@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@2@ABUCPendingObjectSortKey@CThreadedFileManager@@@Z':0x9901d0,
 '??$__distance@U?$_Rb_tree_iterator@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@_STL@@@_STL@@YAHABU?$_Rb_tree_iterator@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@0@0ABUinput_iterator_tag@0@@Z':0x98ff50,
 '?_M_erase@?$_Rb_tree@UCPendingObjectSortKey@CThreadedFileManager@@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@U?$_Select1st@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@4@U?$less@UCPendingObjectSortKey@CThreadedFileManager@@@4@V?$allocator@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@4@@_STL@@QAEXPAU?$_Rb_tree_node@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@2@@Z':0x98fd40,
 '?_Rebalance_for_erase@?$_Rb_global@_N@_STL@@SAPAU_Rb_tree_node_base@2@PAU32@AAPAU32@11@Z':0x42a34e,
 '_free':0xbfea14,
 '?erase@?$_Rb_tree@KU?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@U?$_Select1st@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@2@@_STL@@QAEIABK@Z':0x990d60,
 '?equal_range@?$_Rb_tree@KU?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@U?$_Select1st@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@2@@_STL@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@_STL@@U12@@2@ABK@Z':0x9907d0,
 '??$__distance@U?$_Rb_tree_iterator@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@_STL@@@_STL@@YAHABU?$_Rb_tree_iterator@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@0@0ABUinput_iterator_tag@0@@Z':0x98ffa0,
 '?erase@?$_Rb_tree@KU?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@U?$_Select1st@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@2@@_STL@@QAEXU?$_Rb_tree_iterator@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@2@0@Z':0x9906e0,
 '?_M_erase@?$_Rb_tree@KU?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@U?$_Select1st@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@2@@_STL@@QAEXPAU?$_Rb_tree_node@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@2@@Z':0x98fd80,
 # Unlanded manager helper; identity from the retail TU (steps CurrentPosition, erases the pending key).
 '?RemoveObjectFromPendingMap@CThreadedFileManager@@AAEXPAVCFileObject@1@@Z':0x98e770,
}
ROOTS=[
 dict(name='remove_object',source=R/'rebuild/src/compiled/00/98/CThreadedFileManager_RemoveObject_0098eb20.cpp',symbol='?RemoveObject@CThreadedFileManager@@QAE_NK@Z',address=0x98eb20,size=156,instructions=63,relocations=8),
 dict(name='uid_find',source=R/'rebuild/src/compiled/00/99/STLThreadedUIDTree_find_009902d0.cpp',symbol=FIND,address=0x9902d0,size=71,instructions=32,relocations=0),
 dict(name='pending_less',source=R/'rebuild/src/compiled/00/98/STLPendingObjectTree_Find_0098fe70.cpp',symbol='??R?$less@UCPendingObjectSortKey@CThreadedFileManager@@@_STL@@QBE_NABUCPendingObjectSortKey@CThreadedFileManager@@0@Z',address=0x98f570,size=64,instructions=27,relocations=0),
 dict(name='pending_find',source=R/'rebuild/src/compiled/00/98/STLPendingObjectTree_Find_0098fe70.cpp',symbol='?_M_find@?$_Rb_tree@UCPendingObjectSortKey@CThreadedFileManager@@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@U?$_Select1st@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@4@U?$less@UCPendingObjectSortKey@CThreadedFileManager@@@4@V?$allocator@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@4@@_STL@@QAEPAU_Rb_tree_node_base@2@ABUCPendingObjectSortKey@CThreadedFileManager@@@Z',address=0x98fe70,size=150,instructions=65,relocations=0),
 # Pending key erase closure (00/99 TU); 0x98FCA0 is _M_upper_bound, called first because pair(lower,upper) args evaluate right to left.
 dict(name='pending_key_erase',source=R/'rebuild/src/compiled/00/99/STLPendingObjectTree_KeyErase_00990cd0.cpp',symbol='?erase@?$_Rb_tree@UCPendingObjectSortKey@CThreadedFileManager@@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@U?$_Select1st@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@4@U?$less@UCPendingObjectSortKey@CThreadedFileManager@@@4@V?$allocator@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@4@@_STL@@QAEIABUCPendingObjectSortKey@CThreadedFileManager@@@Z',address=0x990cd0,size=92,instructions=39,relocations=4),
 dict(name='pending_range_erase',source=R/'rebuild/src/compiled/00/99/STLPendingObjectTree_KeyErase_00990cd0.cpp',symbol='?erase@?$_Rb_tree@UCPendingObjectSortKey@CThreadedFileManager@@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@U?$_Select1st@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@4@U?$less@UCPendingObjectSortKey@CThreadedFileManager@@@4@V?$allocator@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@4@@_STL@@QAEXU?$_Rb_tree_iterator@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@2@0@Z',address=0x9905d0,size=189,instructions=78,relocations=3),
 dict(name='pending_upper_bound',source=R/'rebuild/src/compiled/00/99/STLPendingObjectTree_KeyErase_00990cd0.cpp',symbol='?_M_upper_bound@?$_Rb_tree@UCPendingObjectSortKey@CThreadedFileManager@@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@U?$_Select1st@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@4@U?$less@UCPendingObjectSortKey@CThreadedFileManager@@@4@V?$allocator@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@4@@_STL@@QBEPAU?$_Rb_tree_node@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@2@ABUCPendingObjectSortKey@CThreadedFileManager@@@Z',address=0x98fca0,size=89,instructions=39,relocations=0),
 dict(name='pending_lower_bound',source=R/'rebuild/src/compiled/00/99/STLPendingObjectTree_KeyErase_00990cd0.cpp',symbol='?_M_lower_bound@?$_Rb_tree@UCPendingObjectSortKey@CThreadedFileManager@@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@U?$_Select1st@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@4@U?$less@UCPendingObjectSortKey@CThreadedFileManager@@@4@V?$allocator@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@4@@_STL@@QBEPAU?$_Rb_tree_node@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@2@ABUCPendingObjectSortKey@CThreadedFileManager@@@Z',address=0x9901d0,size=89,instructions=39,relocations=0),
 dict(name='pending_distance',source=R/'rebuild/src/compiled/00/99/STLPendingObjectTree_KeyErase_00990cd0.cpp',symbol='??$__distance@U?$_Rb_tree_iterator@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@_STL@@@_STL@@YAHABU?$_Rb_tree_iterator@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@0@0ABUinput_iterator_tag@0@@Z',address=0x98ff50,size=78,instructions=35,relocations=0),
 dict(name='pending_node_erase',source=R/'rebuild/src/compiled/00/99/STLPendingObjectTree_KeyErase_00990cd0.cpp',symbol='?_M_erase@?$_Rb_tree@UCPendingObjectSortKey@CThreadedFileManager@@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@U?$_Select1st@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@4@U?$less@UCPendingObjectSortKey@CThreadedFileManager@@@4@V?$allocator@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@4@@_STL@@QAEXPAU?$_Rb_tree_node@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@2@@Z',address=0x98fd40,size=51,instructions=23,relocations=2),
 # Uid map key erase closure (00/99 TU); equal_range stays out of line here, unlike the pending map.
 dict(name='uid_key_erase',source=R/'rebuild/src/compiled/00/99/STLThreadedUIDTree_KeyErase_00990d60.cpp',symbol='?erase@?$_Rb_tree@KU?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@U?$_Select1st@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@2@@_STL@@QAEIABK@Z',address=0x990d60,size=83,instructions=33,relocations=3),
 dict(name='uid_equal_range',source=R/'rebuild/src/compiled/00/99/STLThreadedUIDTree_KeyErase_00990d60.cpp',symbol='?equal_range@?$_Rb_tree@KU?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@U?$_Select1st@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@2@@_STL@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@_STL@@U12@@2@ABK@Z',address=0x9907d0,size=83,instructions=38,relocations=0),
 dict(name='uid_distance',source=R/'rebuild/src/compiled/00/99/STLThreadedUIDTree_KeyErase_00990d60.cpp',symbol='??$__distance@U?$_Rb_tree_iterator@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@_STL@@@_STL@@YAHABU?$_Rb_tree_iterator@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@0@0ABUinput_iterator_tag@0@@Z',address=0x98ffa0,size=78,instructions=35,relocations=0),
 dict(name='uid_range_erase',source=R/'rebuild/src/compiled/00/99/STLThreadedUIDTree_KeyErase_00990d60.cpp',symbol='?erase@?$_Rb_tree@KU?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@U?$_Select1st@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@2@@_STL@@QAEXU?$_Rb_tree_iterator@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@2@0@Z',address=0x9906e0,size=189,instructions=78,relocations=3),
 dict(name='uid_node_erase',source=R/'rebuild/src/compiled/00/99/STLThreadedUIDTree_KeyErase_00990d60.cpp',symbol='?_M_erase@?$_Rb_tree@KU?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@U?$_Select1st@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@2@@_STL@@QAEXPAU?$_Rb_tree_node@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@2@@Z',address=0x98fd80,size=51,instructions=23,relocations=2),
 dict(name='on_finish_object',source=R/'rebuild/src/compiled/00/98/CThreadedFileManager_OnFinishObject_0098ebc0.cpp',symbol='?OnFinishObject@CThreadedFileManager@@AAEXPAVCFileObject@1@@Z',address=0x98ebc0,size=121,instructions=39,relocations=5),
 # Same bytes as CBankFileManager::CBankInfoMap::iterator::operator++ 0x9A9E20 (its own accepted gate/tests).
 dict(name='pending_increment',source=R/'rebuild/src/compiled/00/98/STLPendingObjectTree_IteratorIncrement_0098f9d0.cpp',symbol='??E?$_Rb_tree_iterator@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@_STL@@QAEAAU01@XZ',address=0x98f9d0,size=62,instructions=30,relocations=0),
]
# NEAR_MATCH roots: same size and instruction count, similarity floor, and the set of call/IAT
# targets of the built relocations equals retail's decoded call/IAT targets. Behaviour evidence:
# work/native-recreation/threaded/0098eff0/20261015/native-first (241,920 cases).
NEAR_ROOTS=[
 dict(name='set_object_priority',source=R/'rebuild/src/compiled/00/98/CThreadedFileManager_SetObjectPriority_0098eff0.cpp',symbol='?SetObjectPriority@CThreadedFileManager@@QAEXKW4EThreadedFilePriority@@@Z',address=0x98eff0,size=151,instructions=59,floor=0.98),
]
NEAR_ADDRESS={
 '?RemoveObjectFromPendingMap@CThreadedFileManager@@AAEXPAVCFileObject@1@@Z':0x98e770,
 '??A?$map@UCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@U?$less@UCPendingObjectSortKey@CThreadedFileManager@@@_STL@@V?$allocator@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@5@@_STL@@QAEAAPAVCFileObject@CThreadedFileManager@@ABUCPendingObjectSortKey@3@@Z':0x991350,
}
def near_roots():
 import tempfile
 import check_one as V
 from gate_game_component_abi import Coff
 from pe_oracle import EXE,read_virtual_data
 from near_match import compare
 from capstone import Cs,CS_ARCH_X86,CS_MODE_32
 addr=dict(ADDRESS);addr.update(NEAR_ADDRESS);raw=EXE.read_bytes();md=Cs(CS_ARCH_X86,CS_MODE_32);out=[];md.detail=True
 for root in NEAR_ROOTS:
  build=Path(tempfile.mkdtemp(prefix='near-root-'));obj=build/(root['name']+'.obj')
  r=V.cl(['/nologo','/c','/O2','/MT','/W3','/I'+str(R/'rebuild/include'),'/Fo'+str(obj),str(root['source'])],V.env(),build);assert r.returncode==0,r.stdout+r.stderr
  c=Coff(obj);sec,off=c.symbols[root['symbol']];assert off==0;built,rels=c.sections[sec]
  original=read_virtual_data(raw,root['address'],root['size']);assert len(built)==root['size'],(root['name'],len(built))
  score=compare(bytes(built),original,list(rels),()).to_dict()
  assert score['complete_decode'] and score['near_match'] and score['opcode_similarity']>=root['floor'],score
  ins=list(md.disasm(original,root['address']));assert len(ins)==root['instructions']==score['built_instructions']
  retail_targets=set()
  for i in ins:
   if i.mnemonic=='call':
    op=i.operands[0]
    retail_targets.add(op.imm if op.type==2 else op.mem.disp&0xffffffff)
  built_targets={addr[name] for name,kind in rels.values() if name.startswith('__imp__') or kind==20}
  assert built_targets==retail_targets,(sorted(map(hex,built_targets)),sorted(map(hex,retail_targets)))
  out.append(dict(name=root['name'],grade='NEAR_MATCH',similarity=score['opcode_similarity'],targets=sorted(map(hex,built_targets))))
 return out
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-threaded-file-manager-remove.json',
  ['work/native-recreation/threaded/0098eb20/20261015/native-first (75,264 cases / 230,400 snapshots / 225,792 RB tree checks)',
   'work/native-recreation/threaded/0098ebc0/20261015/native-first (OnFinishObject + uid erase closure, 15,360 cases / 86,016 snapshots)',
   'work/native-recreation/threaded/pending-map/20261015/native-first native-snapshots (retail vs compiled pending closure, 4,096 cases / 172,032 calls)'],
  'RemoveObject and the out-of-line uid map find (shares bytes with CCodeSectionTree::find 0x9F0D90) are byte-identical to retail after audited relocation patching. Manager layout recovered from the retail TU (vtable 0x129A164, ctor 0x98F3A0, sizeof 0x178). Pending-map removal, GlobalFree and operator delete remain separate providers; no threading or I/O claim beyond byte identity.')
 results+=near_roots()
 print('NATIVE_THREADED_FILE_MANAGER_REMOVE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
