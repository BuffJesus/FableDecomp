import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
w=Path(tempfile.mkdtemp(prefix='native-tc-manager-accessors-'));env=V.env()
specs=[('00686a70','GetWorld','?GetWorld@CTCBase@@IBEAAVCWorld@@XZ'),('00686a80','PeekWorld','?PeekWorld@CTCBase@@IBEABVCWorld@@XZ'),('00686d40','GetPlayerManager','?GetPlayerManager@CTCBase@@IAEAAVCPlayerManager@@XZ'),('00686d50','PeekPlayerManager','?PeekPlayerManager@CTCBase@@IBEABVCPlayerManager@@XZ'),('00686d60','PeekComponent','?PeekComponent@CTCBase@@IBEABVCMainGameComponent@@XZ'),('006869d0','GetDefinitionManager','?GetDefinitionManager@CTCBase@@IBEABVCGameDefinitionManager@@XZ'),('00686db0','GetPLocalSeed','?GetPLocalSeed@CTCBase@@IBEP AKXZ'.replace(' ',''))]
slot='?PCurrentThingManager@CThing@@0PAVCThingManager@@A';sources=[];objects=[];proofs=[];body=bytearray(144);raw=EXE.read_bytes()
for i,(address,name,symbol) in enumerate(specs):
 source=ROOT/'rebuild/src/compiled/00/68'/f'CTCBase_{name}_{address}.cpp'
 if not source.exists() or ('--seed-stage' in sys.argv and name=='GetPLocalSeed'):source=ROOT/'work/parity_addtext/ctc_manager_accessors_20261009'/source.name
 sources.append(source);obj=w/f'{i}.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj);proofs.append(check(obj,symbol,int(address,16),8 if address=='006869d0' else 14 if address=='00686db0' else 9,1,{slot:0x13b8a1c,'?PeekDefinitionManager@CThing@@QBEABVCGameDefinitionManager@@XZ':0x4c7990}));length=8 if address=='006869d0' else 14 if address=='00686db0' else 9;body[i*16:i*16+length]=read_virtual_data(raw,int(address,16),length)
body[128:137]=read_virtual_data(raw,0x4c7990,9)
(w/'tc_manager_accessors_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\n')
(w/'tc_manager_accessors_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in list(zip(('@TCGetWorld@8','@TCPeekWorld@8','@TCGetPlayers@8','@TCPeekPlayers@8','@TCPeekComponent@8','@TCGetDefinitions@8','@TCGetSeed@8'),[s[2] for s in specs]))+[('_CurrentThingSlot',slot)])+'\n')
provider=ROOT/'rebuild/src/compiled/00/4c/CThing_SetCurrentThingManager_004c74f0.cpp';fixture=ROOT/'rebuild/tests/integration/NativeTCManagerAccessors_test.cpp';sources.append(provider)
for i,path in enumerate([provider,ROOT/'rebuild/src/compiled/00/4c/CThing_PeekDefinitionManager_004c7990.cpp',fixture]):
 obj=w/f'extra{i}.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proofs.append(check(objects[7],'?SetCurrentThingManager@CThing@@SIXAAVCThingManager@@@Z',0x4c74f0,7,1,{slot:0x13b8a1c}))
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
proofs.append(check(objects[8],'?PeekDefinitionManager@CThing@@QBEABVCGameDefinitionManager@@XZ',0x4c7990,9,1,{slot:0x13b8a1c}))
sources.append(ROOT/'rebuild/src/compiled/00/4c/CThing_PeekDefinitionManager_004c7990.cpp')
for omit in range(9):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);assert 'cases=14336 snapshots=28672' in r.stdout
report=dict(cases=14336,snapshots=28672,providers=proofs,omission_checks=9,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},header_sha256={n:hashlib.sha256((ROOT/'rebuild/include'/n).read_bytes()).hexdigest() for n in ['fable_world.h','fable_thing_components.h','fable_thing_manager.h']},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Seven actual CTCBase world/player/component/definition/local-seed accessors plus real currentmanager setter/storage;no callbacks.256rawpointer patterns,manager switch between two locations,zero/live incomingthis ignored by six singleton accessors;definition wrapper reads ParentThing then actual thing getter ignores that pointer,two fills.Full16KiB and currentmanager nonmutation snapshots;independent selectedoffset return values.')
(ROOT/'rebuild/compile-gate/native-tc-manager-accessors.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_TC_MANAGER_ACCESSORS_GATE_PASS')
