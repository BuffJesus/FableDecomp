"""Fresh VC71 parity gate: CTCContainerRewardHero::AdjustTrajectoryVector (0x0074E080; inline C3DVector::Normalise / GFFastOneOverSqrt), exact after relocation; needs wf-dd's fable_2d_vector.h"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?g_FableInvSqrtTable_013CE558@@3PAKA':0x13ce558,
 '__real@3e4ccccd':0x12305a0,
 '__real@3e99999a':0x1238170,
 '__real@3f000000':0x122f59c,
 '__real@3f800000':0x122ded8,
 '__real@40400000':0x122ded4,
}
ROOTS=[
 dict(name='reward_hero_adjust_trajectory',source=R/'rebuild/src/compiled/00/74/CTCContainerRewardHero_AdjustTrajectoryVector_0074e080.cpp',symbol='?AdjustTrajectoryVector@CTCContainerRewardHero@@IBEXJAAVC3DVector@@@Z',address=0x74e080,size=183,instructions=57,relocations=8),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-reward-hero-adjust-trajectory.json',[],
  'CTCContainerRewardHero::AdjustTrajectoryVector (0x0074E080; inline C3DVector::Normalise / GFFastOneOverSqrt), exact afte')
 print('NATIVE_REWARD_HERO_ADJUST_TRAJECTORY_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
