import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-frame-data-is-loaded-'));env=V.env()
specs=[('00/9f/CGraphicInfoFrame_CFrameData_IsResourceLoaded_009fd7f0.cpp','?IsResourceLoaded@CFrameData@CGraphicInfoFrame@@UBE_NXZ',0x9fd7f0,20,0,1),('00/9d/CBankFileAsyncData_Accessors_009d54e0.cpp','?IsFinished@CBankFileAsyncData@@QBE_NXZ',0x9d54e0,65,64,3),('00/a6/CCriticalSectionLock_Constructor_00a62920.cpp','??0CCriticalSectionLock@@QAE@AAU_RTL_CRITICAL_SECTION@@_N@Z',0xa62920,54,160,2),('00/a6/CCriticalSectionLock_Destructor_00a629a0.cpp','??1CCriticalSectionLock@@QAE@XZ',0xa629a0,25,224,1)]
targets={symbol:address for rel,symbol,address,size,offset,count in specs};targets.update({'__imp__EnterCriticalSection@4':0x143fe0c,'__imp__TryEnterCriticalSection@4':0x143fd30,'__imp__LeaveCriticalSection@4':0x143fe10});offsets={address:offset for rel,symbol,address,size,offset,count in specs};offsets.update({0x143fe0c:0x80000000,0x143fd30:0x80000001,0x143fe10:0x80000002})
objects=[];sources=[];proofs=[];body=bytearray(256);raw=EXE.read_bytes();cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True;fixups=[]
rename={'__imp__EnterCriticalSection@4':'__imp__FrameEnter@4','__imp__TryEnterCriticalSection@4':'__imp__FrameTry@4','__imp__LeaveCriticalSection@4':'__imp__FrameLeave@4'}
for rel,symbol,address,size,offset,count in specs:
 source=ROOT/'rebuild/src/compiled'/rel
 if not source.exists():source=ROOT/'work/parity_addtext/frame_data_is_loaded_candidate_20261009.cpp'
 obj=w/f'{address:x}.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
 sources.append(source);proofs.append(check(obj,symbol,address,size,count,targets));b=read_virtual_data(raw,address,size);body[offset:offset+size]=b
 for ins in cs.disasm(b,address):
  if ins.mnemonic in ('call','jmp') and ins.bytes[0] in (0xe8,0xe9):
   target=int(ins.op_str,16)
   if not address<=target<address+size:assert target in offsets;fixups.append((offset+ins.address-address+1,offsets[target],20))
  if ins.disp_size==4 and ins.disp in offsets:fixups.append((offset+ins.address-address+ins.disp_offset,offsets[ins.disp],6))
 if address in (0xa62920,0xa629a0):
  replaced=w/f'{address:x}-imports.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(replaced),'/DEnterCriticalSection=FrameEnter','/DTryEnterCriticalSection=FrameTry','/DLeaveCriticalSection=FrameLeave',str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
  original,renamed=Coff(obj),Coff(replaced);a,off=original.symbols[symbol];b,other=renamed.symbols[symbol];left,lr=original.sections[a];right,rr=renamed.sections[b];assert off==other and left==right and {p:(rename.get(t,t),k) for p,(t,k) in lr.items()}==rr;obj=replaced
 objects.append(obj)
assert len(fixups)==7
(w/'frame_data_is_loaded_retail.h').write_text('struct Fixup{unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\n')
(w/'frame_data_is_loaded_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in {'@IsLoaded@8':specs[0][1],'__imp__FrameEnter@4':'_EnterIAT','__imp__FrameTry@4':'_TryIAT','__imp__FrameLeave@4':'_LeaveIAT'}.items())+'\n')
fixture=ROOT/'rebuild/tests/integration/NativeFrameDataIsLoaded_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)]
for omit in range(4):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=1200 snapshots=4800' in r.stdout
headers=[ROOT/'rebuild/include'/p for p in ('fable_graphic_info.h','fable_graphic_info_frame.h','fable_game_text_bank.h','fable_critical_section_lock.h','fable_counted_pointer.h')]
report=dict(cases=1200,snapshots=int(r.stdout.split('snapshots=')[1]),providers=proofs,omission_checks=4,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Complete20B nested frame-data IsResourceLoaded plusactual65B async IsFinished,54B lock ctor and25B lock dtor. Win32 critical-section callbacks only; unused TryEnter aborts. Import-only renames preserve complete code/relocation maps. Independent/native/compiled full16KiB callback/final snapshots;null/live async read,raw Finished/AllowRestart bytes, callback swaps frame data/bank pointer and finished/restart changes,old captured section and cached async self,returnbool,two placements/fills. Four source omissions fail. Stack lock address/padding not output.')
(ROOT/'rebuild/compile-gate/native-frame-data-is-loaded.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_FRAME_DATA_IS_LOADED_GATE_PASS')
