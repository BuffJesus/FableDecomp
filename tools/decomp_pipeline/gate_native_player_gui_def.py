import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-player-gui-def-'));env=V.env()
specs=[('00/43/CPlayerGui_PeekGuiDef_00438570.cpp','player_gui_peek_def_candidate_20261009.cpp','?PeekGuiDef@CPlayerGui@@SIABVCPlayerGuiDef@@XZ',0x438570,63,8),('00/43/CDefinitionManager_GetDefPlayerGuiName_0043ff30.cpp','getdef_gui_candidate_20261009.cpp','??$GetDef@VCPlayerGuiDef@@@CDefinitionManager@@QBE_NABVCCharString@@PAV?$CDefPointer@$$CBVCPlayerGuiDef@@@@@Z',0x43ff30,104,1),('00/99/CCharString_Constructor_0099ebf0.cpp',None,'??0CCharString@@QAE@PBDJ@Z',0x99ebf0,61,3),('00/99/CCharString_Destructor_0099eae0.cpp',None,'??1CCharString@@QAE@XZ',0x99eae0,12,2),('00/44/CGameDefinitionManager_Get_0044c6b0.cpp',None,'?Get@CGameDefinitionManager@@SIAAV1@XZ',0x44c6b0,6,1)]
boundaries={'?PGuiDef@CPlayerGui@@0V?$CDefPointer@$$CBVCPlayerGuiDef@@@@A':(0x13b878c,0xffffffff),'?PClass@CGameDefinitionManager@@1V?$CCountedPointer@VCGameDefinitionManager@@@@A':(0x13b879c,0xfffffffe),'?g_CCharStringInstanceCount_013BD800@@3HA':(0x13bd800,0xfffffffd),'?AllocStringData@CCharString@@AAEPAVCCharStringData@@PBDJ@Z':(0x99ea60,0xfffffffc),'?UnassignString@CCharString@@AAEXXZ':(0x99e9b0,0xfffffffb),'?GetPDefFromInstantiationName@CDefinitionManager@@QBE?AV?$CDefPointer@$$CBVCDefClassBase@@@@ABVCCharString@@@Z':(0x9ada40,0xfffffffa),'??_C@_0O@ICPGPMLM@PLAYER_GUI_PC?$AA@':(0x123173c,1280)}
targets={s[2]:s[3] for s in specs};targets.update({k:v[0] for k,v in boundaries.items()});offsets={s[3]:i*256 for i,s in enumerate(specs)};offsets.update({v[0]:v[1] for v in boundaries.values()})
body=bytearray(1294);body[1280:]=b'PLAYER_GUI_PC\0';raw=EXE.read_bytes();fixups=[];objects=[];sources=[];proofs=[];cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True
for i,(rel,scratch,symbol,address,length,n) in enumerate(specs):
 source=ROOT/'rebuild/src/compiled'/rel
 if scratch and ('--stage' in sys.argv or not source.exists()):source=ROOT/'work/parity_addtext'/scratch
 sources.append(source);obj=w/f'{i}.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj);proofs.append(check(obj,symbol,address,length,n,targets));b=read_virtual_data(raw,address,length);body[i*256:i*256+length]=b
 for ins in cs.disasm(b,address):
  if ins.mnemonic in ('call','jmp') and ins.bytes[0] in (0xe8,0xe9):
   target=int(ins.op_str,16);assert target in offsets;fixups.append((i*256+ins.address-address+ins.size-4,offsets[target],20))
  if ins.disp_size==4 and ins.disp in offsets:fixups.append((i*256+ins.address-address+ins.disp_offset,offsets[ins.disp],6))
  if ins.imm_size==4 and ins.mnemonic not in ('call','jmp'):
   for op in ins.operands:
    if op.type==2 and op.imm in offsets:fixups.append((i*256+ins.address-address+ins.imm_offset,offsets[op.imm],6))
(w/'player_gui_def_retail.h').write_text('struct Fixup{unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\n')
aliases={'@GuiPeek@0':specs[0][2],list(boundaries)[1]:'_ManagerSlot','_AllocBoundary':list(boundaries)[3],'_UnassignBoundary':list(boundaries)[4],'_LookupBoundary':list(boundaries)[5]}
(w/'player_gui_def_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items()))
cache=ROOT/'rebuild/src/compiled/01/3b/CPlayerGui_PGuiDef_013b878c.cpp';sources.append(cache);obj=w/'cache.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(cache)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
fixture=ROOT/'rebuild/tests/integration/NativePlayerGuiDef_test.cpp';obj=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(6):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=1728 snapshots=7168' in r.stdout
report=dict(cases=1728,snapshots=7168,providers=proofs,omission_checks=6,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual GUI loader,GetDef specialization,narrow string ctor/dtor,manager getter and real cache/lifecycle storage. Lookup,string allocation/release and pointee virtual deletion are callback boundaries. Singleton fixture storage does not prove singleton global lifetime. Independent full64KiB and globals snapshots at callbacks and return. Cache initialization/shutdown also has a separate native lifecycle gate.')
(ROOT/'rebuild/compile-gate/native-player-gui-def.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_PLAYER_GUI_DEF_GATE_PASS')
