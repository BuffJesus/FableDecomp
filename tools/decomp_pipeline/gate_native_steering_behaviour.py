"""Fresh VC71 parity gate: steering behaviours and navigator route lists in 0xA20000..0xA30000 - CSBGoal /
CSBCAgents / CSBIAgents / CSBCCorridor GetType, CSBIAgents::Display, STLport list<CWall> clear / ~list (CSBCCorridor::Walls)
and list<CNavWayPoint> ~list / pop_front (CNavigatorAStar::Route). The list roots carry the project single-class list
names; their genuine owners are the _List_base members (spec true_symbol). Headers from workflow wf-a2 steer batch
(work/native-recreation/wf-a2/hdr/steer, reviewed); replaces the CGuiControlTreePane / list<long> stand-ins."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??3@YAXPAX@Z':0xbfe9bc,
 '??_7CSB@@6B@':0x129cc1c,
 '_free':0xbfea14,
}
ROOTS=[
 dict(name='csbgoal_gettype',source=R/'rebuild/src/compiled/00/a2/CSBGoal_GetType_00a20860.cpp',symbol='?GetType@CSBGoal@@UAE?AW4ESB_TYPE@CSB@@XZ',address=0xa20860,size=6,instructions=2,relocations=0),
 dict(name='csbcagents_gettype',source=R/'rebuild/src/compiled/00/a2/CSBCAgents_GetType_00a20910.cpp',symbol='?GetType@CSBCAgents@@UAE?AW4ESB_TYPE@CSB@@XZ',address=0xa20910,size=6,instructions=2,relocations=0),
 dict(name='csbiagents_display',source=R/'rebuild/src/compiled/00/a2/CSBIAgents_Display_00a211c0.cpp',symbol='?Display@CSBIAgents@@UAEXPBVCSteeringAgent@@@Z',address=0xa211c0,size=3,instructions=1,relocations=0),
 dict(name='csbiagents_gettype',source=R/'rebuild/src/compiled/00/a2/CSBIAgents_GetType_00a211d0.cpp',symbol='?GetType@CSBIAgents@@UAE?AW4ESB_TYPE@CSB@@XZ',address=0xa211d0,size=6,instructions=2,relocations=0),
 dict(name='csbccorridor_gettype',source=R/'rebuild/src/compiled/00/a2/CSBCCorridor_GetType_00a21f90.cpp',symbol='?GetType@CSBCCorridor@@UAE?AW4ESB_TYPE@CSB@@XZ',address=0xa21f90,size=6,instructions=2,relocations=0),
 dict(name='cwall_list_clear',source=R/'rebuild/src/compiled/00/a2/CWallList_clear_00a20e10.cpp',symbol='?clear@?$list@VCWall@@V?$allocator@VCWall@@@_STL@@@_STL@@QAEXXZ',address=0xa20e10,size=47,instructions=23,relocations=1),
 dict(name='cwall_list_dtor',source=R/'rebuild/src/compiled/00/a2/CWallList_clear_00a20e10.cpp',symbol='??1?$list@VCWall@@V?$allocator@VCWall@@@_STL@@@_STL@@QAE@XZ',address=0xa21250,size=62,instructions=29,relocations=2),
 dict(name='cnavwaypoint_list_dtor',source=R/'rebuild/src/compiled/00/a2/CNavWayPointList_Destructor_00a214c0.cpp',symbol='??1?$list@VCNavWayPoint@@V?$allocator@VCNavWayPoint@@@_STL@@@_STL@@QAE@XZ',address=0xa214c0,size=62,instructions=29,relocations=2),
 dict(name='cnavwaypoint_list_pop_front',source=R/'rebuild/src/compiled/00/a2/CNavWayPointList_pop_front_00a219e0.cpp',symbol='?pop_front@?$list@VCNavWayPoint@@V?$allocator@VCNavWayPoint@@@_STL@@@_STL@@QAEXXZ',address=0xa219e0,size=22,instructions=10,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-steering-behaviour.json',[],
  'steering behaviour GetType/Display and navigator route list instantiations (0xA20000..0xA30000)')
 print('NATIVE_STEERING_BEHAVIOUR_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
