"""Whole GF shutdown order through actual IME getter/shutdown/defaults/storage."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-gfuninitialise-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/40/Global_GFUninitialise_00401b80.cpp','00/40/CIME_Get_00404a80.cpp','00/40/CIME_Shutdown_004051c0.cpp','00/40/CIME_ResetToDefaultSettings_00404a90.cpp','01/3b/CIME_SettingsData_013b7cd8.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeGFUninitialise_test.cpp'
gf='?GFUninitialise@@YAXXZ';get='?Get@CIME@NUISystem@@SIAAV12@XZ';shutdown='?Shutdown@CIME@NUISystem@@QAEXXZ';defaults='?ResetToDefaultSettings@CIME@NUISystem@@SIXXZ';ime='?g_FableIME_013B7CD8@@3VCIME@NUISystem@@A'
calls=[('?Uninitialise@CGame@@QAEXXZ',0x412f60,'@GameClose@8'),('?FableFileInstallerSingletonRelease@@YIXXZ',0x406df0,'@FileRelease@0'),('?Close@CBankFileManager@@QAEXXZ',0x9a7990,'@BankClose@8'),('?GFGetSystemManager@@YIPAVCSystemManager@@XZ',0x9a4ec0,'@Root@0'),('?Uninitialise@CSystemManager@@QAEXXZ',0x9a60d0,'@SystemClose@8'),('?Uninitialise@CThreadedFileManager@@QAEXXZ',0x98e0d0,'@ThreadClose@8'),('?UninitializeStreamingFontTextureCache@@YIXXZ',0x9a1660,'@FontClose@0'),('?ImeUi_FinalizeString@@YIX_N@Z',0xcb2c60,'@Finalize@4'),('?ImeUi_EnableIme@@YIX_N@Z',0xcb5130,'@Enable@4'),('?ImeUi_Initialize@@YI_NPAUHWND__@@_N@Z',0xcb5180,'@Initialize@8')]
globals=[('?g_Game_013B83D0@@3VCGame@@A',0x13b83d0,'_Game'),('?g_FableBankFileManager_013CA79C@@3VCBankFileManager@@A',0x13ca79c,'_Bank'),('?g_FableThreadedFileManager_013BC870@@3VCThreadedFileManager@@A',0x13bc870,'_Thread')]
targets={s:a for s,a,_ in calls+globals};targets.update({get:0x404a80,shutdown:0x4051c0,defaults:0x404a90,ime:0x13b7cd8,'??3@YAXPAX@Z':0xbfe9bc})
aliases={s:alias for s,a,alias in calls+globals};aliases.update({'_RunGF':gf,'_IME':ime})
(w/'gfuninitialise_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=bytearray(323);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
specs=[(gf,0x401b80,64,12,0),(get,0x404a80,6,1,64),(shutdown,0x4051c0,90,5,80),(defaults,0x404a90,147,19,176)]
for sym,address,length,count,offset in specs:
 code=read_virtual_data(raw,address,length);body[offset:offset+length]=code
 for i in md.disasm(code,address):
  if i.mnemonic in ('call','jmp') and i.operands[0].type==2 and not address<=i.operands[0].imm<address+length:assert i.operands[0].imm in targets.values();fixups.append((offset+i.address-address+i.imm_offset,i.operands[0].imm,1))
  else:
   for o in i.operands:
    value=o.imm if o.type==2 else o.mem.disp if o.type==3 else 0
    if value in (0x13b83d0,0x13ca79c,0x13bc870) or 0x13b7cd8<=value<0x13b7d2c:fixups.append((offset+i.address-address+(i.imm_offset if o.type==2 else i.disp_offset),value,0))
assert len(fixups)==37
(w/'gfuninitialise_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[n],sym,address,length,count,targets) for n,(sym,address,length,count,offset) in enumerate(specs)]
assert coff_data(Coff(objects[4]),ime,84)==read_virtual_data(raw,0x13b7cd8,84)==bytes(84)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(5):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0
with (w/'input.bin').open('wb') as f:
 for initialized,mode,font,fill in itertools.product((0,1,255),range(8),range(2),range(8)):
  f.write(struct.pack('<4I',initialized,mode,font,fill));cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
assert (w/'retail.bin').read_bytes()==(w/'compiled.bin').read_bytes(),w
size=(w/'compiled.bin').stat().st_size;assert size%1156==0;snapshots=size//1156
report=dict(cases=cases,snapshots=snapshots,omission_checks=5,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual GF shutdown64B,IMEgetter6B,Shutdown90B,defaults147B,andzero84Bsingleton. Full1KiB+84BIME+three16Bfixtureglobals independent expected state. Enforces game,fileinstaller,bank,systemroot/system,thread,font,IME order. IMEinitialized0/1/255,null/non-nullfont,eightcallbackmodes/eightfills;IMEAPIs alter window,font,flags/editor/cancel state;actualshutdown rereads mutateddata,ignores Initialize result,clearsownedstate,restores appearance defaults whilepreserving unrelatedpadding/cancel byte. Other subsystem lifetimes/IMEAPIs/operator delete andthreeglobalstorages remainexplicit boundaries.')
(ROOT/'rebuild/compile-gate/native-gfuninitialise.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_GFUNINITIALISE_GATE_PASS',snapshots,'snapshots')
