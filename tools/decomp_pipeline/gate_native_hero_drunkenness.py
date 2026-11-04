"""Native hero render dispatch through real player/pointer/component providers."""
import hashlib,json,struct,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
w=Path(tempfile.mkdtemp(prefix='native-hero-drunkenness-'));env=V.env()
root=ROOT/'rebuild/src/compiled/00/43/CDisplayEngine_RenderHeroDrunkenness_00435070.cpp'
if not root.exists():root=ROOT/'work/parity_addtext/display_hero_drunkenness_20261009.cpp'
specs=[(root,'?RenderHeroDrunkenness@CDisplayEngine@@IAEXABVCInterpolationInfo@@@Z',0x435070,94,5),
(ROOT/'rebuild/src/compiled/00/44/CPlayerManager_GetMainPlayer_00449970.cpp','?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ',0x449970,10,1),
(ROOT/'rebuild/src/compiled/00/44/CPlayerManager_GetPlayer_004498c0.cpp','?GetPlayer@CPlayerManager@@QAEAAVCPlayer@@J@Z',0x4498c0,73,0),
(ROOT/'rebuild/src/compiled/00/48/CPlayer_GetPControlledCreature_00487dc0.cpp','?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ',0x487dc0,8,1),
(ROOT/'rebuild/src/compiled/00/a0/CBaseIntelligentPointer_Operations_00a01b20.cpp','?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ',0xa01b50,54,1),
(ROOT/'rebuild/src/compiled/00/40/CVectorMapThingComponents_LowerBound_0040f020.cpp','?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@_STL@@ABW4ETCInterfaceType@@@Z',0x40f020,60,0)]
targets={s[1]:s[2] for s in specs};targets.update({'?PMainGameComponent@CMainGameComponent@@1PAV1@A':0x13b86a0,'?RenderHeroDrunkenness@CTCHeroStats@@QAEXPAVCIEngine@@@Z':0x57b43f,'??3@YAXPAX@Z':0xbfe9bc})
objects=[];proofs=[];fixups=[];body=bytearray(6*256);raw=EXE.read_bytes();offsets={s[2]:i*256 for i,s in enumerate(specs)};offsets.update({0x13b86a0:0xffffffff,0x57b43f:0xfffffffe,0xbfe9bc:0xfffffffd})
for i,(source,symbol,address,length,relcount) in enumerate(specs):
 obj=w/f'{i}.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj);proofs.append(check(obj,symbol,address,length,relcount,targets))
 c=Coff(obj);b,rels=c.sections[c.symbols[symbol][0]];body[i*256:i*256+length]=read_virtual_data(raw,address,length)
 for site,(name,kind) in rels.items():
  value=struct.unpack_from('<I',body,i*256+site)[0];actual=value if kind==6 else (address+site+4+value)&0xffffffff;assert actual==targets[name],(name,actual)
  fixups.append((i*256+site,offsets[actual],kind))
(w/'hero_drunkenness_retail.h').write_text('struct Fixup {unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\n')
(w/'hero_drunkenness_aliases.h').write_text('extern "C" void __fastcall DrawBoundary(void*,void*);\nextern "C" void DeleteBoundary(void*);\n'+ '\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in [('@HeroDraw@12',specs[0][1]),( '?PMainGameComponent@CMainGameComponent@@1PAV1@A','_MainSlot'),('@DrawBoundary@8','?RenderHeroDrunkenness@CTCHeroStats@@QAEXPAVCIEngine@@@Z'),('_DeleteBoundary','??3@YAXPAX@Z')])+'\n')
fixture=ROOT/'rebuild/tests/integration/NativeHeroDrunkenness_test.cpp';obj=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(6):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);assert 'cases=672 snapshots=1344' in r.stdout
report=dict(cases=672,snapshots=1344,providers=proofs,omission_checks=6,source_sha256={str(s[0].relative_to(ROOT)):hashlib.sha256(s[0].read_bytes()).hexdigest() for s in specs},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual native hero render, main/neutral/first-match player lookup, live/null/expired intelligent pointer cleanup and component lower bound. Draw and delete callbacks only; full16KiB independent-model memory/global snapshots; component flag/key/pointer variants, two locations/fills and draw mutation.')
(ROOT/'rebuild/compile-gate/native-hero-drunkenness.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_HERO_DRUNKENNESS_GATE_PASS')
