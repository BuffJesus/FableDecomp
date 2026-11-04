import hashlib,json,subprocess,tempfile,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
import check_one as V
from variable_allocator_parity import check
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-scene-stats-reset-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled/00/b9'/p for p in ('CEngineScene_ResetStats_00b93df0.cpp','SceneStatsTree_Erase_00b95ee0.cpp')]
fixture=ROOT/'rebuild/tests/integration/NativeSceneStatsReset_test.cpp'
reset='?ResetStats@CEngineScene@@QAEXXZ'
from gate_game_component_abi import Coff
trial=w/'identity.obj';r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(trial),str(sources[1])],env,w);assert not r.returncode,r.stdout+r.stderr
erase=next(s for s in Coff(trial).symbols if s.startswith('?_M_erase'))
targets={erase:0xb95ee0,'_free':0xbfea14}
aliases={'@ResetStats@8':reset}
(w/'scene_stats_reset_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=bytearray(131);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
specs=[(reset,0xb93df0,73,1,0),(erase,0xb95ee0,51,2,80)]
for sym,address,length,count,offset in specs:
 code=read_virtual_data(raw,address,length);body[offset:offset+length]=code
 for i in md.disasm(code,address):
  if i.mnemonic in ('call','jmp') and i.operands[0].type==2 and not address<=i.operands[0].imm<address+length:
   assert i.operands[0].imm in targets.values();fixups.append((offset+i.address-address+i.imm_offset,i.operands[0].imm,1))
  else:
   for o in i.operands:
    if o.type==3 and o.mem.disp==0x1436e14:fixups.append((offset+i.address-address+i.disp_offset,o.mem.disp,0))
assert len(fixups)==2
(w/'scene_stats_reset_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[n],sym,a,l,c,targets) for n,(sym,a,l,c,off) in enumerate(specs)]
replay=w/'erase-replay.obj';r=V.cl(['/nologo','/c','/O2','/MT','/Dfree=SceneStatsFree','/Fo'+str(replay),str(sources[1])],env,w);assert not r.returncode,r.stdout+r.stderr
original,renamed=Coff(objects[1]),Coff(replay);sec,off=original.symbols[erase];other,otheroff=renamed.symbols[erase];body1,rel1=original.sections[sec];body2,rel2=renamed.sections[other];assert body1==body2 and off==otheroff;assert {o:('_SceneStatsFree' if t=='_free' else t,k) for o,(t,k) in rel1.items()}==rel2;objects[1]=replay
link=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(2):
 r=subprocess.run(link+[str(o) for n,o in enumerate(objects) if n!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip())
assert 'cases=768 snapshots=13056' in r.stdout
report=dict(cases=768,snapshots=13056,providers=proofs,omission_checks=2,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),result=r.stdout.strip(),boundary='Actual scene ResetStats and stats-tree recursive destruction. Independent full4KiB model covers0..15nodes,balanced/left/right topology, freed-node scribbling, header pointer replacement and counter mutations duringfree. Only free boundary, renamed body/relocations audited. No drawing.')
(ROOT/'rebuild/compile-gate/native-scene-stats-reset.json').write_text(json.dumps(report,indent=2)+'\n')

print('NATIVE_SCENE_STATS_RESET_GATE_PASS')
