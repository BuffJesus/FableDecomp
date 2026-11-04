import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-frame-data-stop-finished-'));env=V.env()
specs=[('00/9f/CGraphicInfoFrame_CFrameData_StopLoading_009fdc20.cpp','?StopLoading@CFrameData@CGraphicInfoFrame@@QAE_NXZ',0x9fdc20,136,0,6),('00/9f/CResource_SetResourceSize_009fbf20.cpp','?SetResourceSize@CResource@@QAEXK@Z',0x9fbf20,46,256,0),('00/9d/CBankFileAsyncData_Accessors_009d54e0.cpp','?IsFinished@CBankFileAsyncData@@QBE_NXZ',0x9d54e0,65,320,3),('00/a6/CCriticalSectionLock_Constructor_00a62920.cpp','??0CCriticalSectionLock@@QAE@AAU_RTL_CRITICAL_SECTION@@_N@Z',0xa62920,54,448,2),('00/a6/CCriticalSectionLock_Destructor_00a629a0.cpp','??1CCriticalSectionLock@@QAE@XZ',0xa629a0,25,512,1)]
targets={symbol:address for rel,symbol,address,size,offset,count in specs};targets.update({'__imp__EnterCriticalSection@4':0x143fe0c,'__imp__TryEnterCriticalSection@4':0x143fd30,'__imp__LeaveCriticalSection@4':0x143fe10,'?IsFinished@CBankFileAsyncData@@QBE_NXZ':0x9d54e0,'?Release@CManagedTexture@@QAE_NXZ':0xa69ad0,'?Sizeof@CGraphicFrameData@@QBEKXZ':0x9fcbf0,'?Sizeof@CManagedTexture@@QBEKXZ':0xa69e60,'??3@YAXPAX@Z':0xbfe9bc});offsets={address:offset for rel,symbol,address,size,offset,count in specs};offsets.update({0x9d54e0:320,0xa62920:448,0xa629a0:512,0x143fe0c:0x80000005,0x143fd30:0x80000006,0x143fe10:0x80000007,0xa69ad0:0x80000001,0x9fcbf0:0x80000002,0xa69e60:0x80000003,0xbfe9bc:0x80000004})

objects=[];sources=[];proofs=[];coffs=[];body=bytearray(576);raw=EXE.read_bytes();cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True;fixups=[];initFixups=[]
for rel,symbol,address,size,offset,count in specs:
 source=ROOT/'rebuild/src/compiled'/rel
 if not source.exists():source=ROOT/'work/parity_addtext/frame_data_stop_loading_candidate_20261009.cpp'
 obj=w/f'{address:x}.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
 sources.append(source);coffs.append(Coff(obj));proofs.append(check(obj,symbol,address,size,count,targets));b=read_virtual_data(raw,address,size);body[offset:offset+size]=b
 for ins in cs.disasm(b,address):
  if ins.mnemonic in ('call','jmp') and ins.bytes[0] in (0xe8,0xe9):
   target=int(ins.op_str,16)
   if not address<=target<address+size:assert target in offsets;fixups.append((offset+ins.address-address+1,offsets[target],20))
  if ins.disp_size==4 and ins.disp in offsets:fixups.append((offset+ins.address-address+ins.disp_offset,offsets[ins.disp],6))
  if ins.mnemonic not in ('call','jmp') and ins.imm_size==4:
   for operand in ins.operands:
    if operand.type==2 and operand.imm in offsets:fixups.append((offset+ins.address-address+ins.imm_offset,offsets[operand.imm],6))
 if address in (0xa62920,0xa629a0):
  renamedObj=w/f'{address:x}-imports.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(renamedObj),'/DEnterCriticalSection=StopEnter','/DTryEnterCriticalSection=StopTry','/DLeaveCriticalSection=StopLeave',str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
  rename={'__imp__EnterCriticalSection@4':'__imp__StopEnter@4','__imp__TryEnterCriticalSection@4':'__imp__StopTry@4','__imp__LeaveCriticalSection@4':'__imp__StopLeave@4'};original,replaced=Coff(obj),Coff(renamedObj);a,off=original.symbols[symbol];b,other=replaced.symbols[symbol];left,lr=original.sections[a];right,rr=replaced.sections[b];assert off==other and left==right and {p:(rename.get(t,t),k) for p,(t,k) in lr.items()}==rr;obj=renamedObj
 objects.append(obj)
assert len(fixups)==12
(w/'frame_data_stop_finished_retail.h').write_text('struct Fixup{unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\n')
aliases=['#pragma comment(linker,"/alternatename:@StopLoading@8='+specs[0][1]+'")']+['#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in {'__imp__StopEnter@4':'_EnterIAT','__imp__StopTry@4':'_TryIAT','__imp__StopLeave@4':'_LeaveIAT'}.items()]
(w/'frame_data_stop_finished_aliases.h').write_text('\n'.join(aliases)+'\n')
fixture=ROOT/'rebuild/tests/integration/NativeFrameDataStopFinished_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)]
for omit in range(5):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=55296 snapshots=339552' in r.stdout,(w,r.stdout)
headers=[ROOT/'rebuild/include'/p for p in ('fable_graphic_info.h','fable_graphic_info_frame.h','engine/CGraphicDataBank.h','engine/CManagedTextureOwner.h','fable_managed_texture.h','fable_managed_texture_data.h','engine/CGraphicFrame.h','fable_graphic_frame.h','fable_graphic_frame_data.h','fable_game_text_bank.h','fable_counted_pointer.h','fable_resource_list.h','fable_iv_counted_pointer.h','fable_base_class.h')]
report=dict(cases=55296,snapshots=int(r.stdout.split('snapshots=')[1]),providers=proofs,omission_checks=5,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual136B StopLoading and46B sizeaccounting with actual65B Async IsFinished and54B/25B lock ctor/dtor;five mandatory sources. Only Win32section,LoadingManagedRelease,deferred Sizeof andcounted-pointee/heap callbacks remain. Import-only renames preserve complete code/relocations. Independent/native/compiled full16KiB callback/final snapshots;rawcounts/sizes,reloadedInfoincludingnullheap,callback swapsData/Info/frame/managed/resource pointers,actualFinished clearingAllowRestart/cachedresultbeforeLeave,frame/sizecounteralias,earlyfailure,two placements/fills. Unused TryEnter aborts;stacklockpadding notoutput;sizequeries remaindeferred.')
(ROOT/'rebuild/compile-gate/native-frame-data-stop-finished.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_FRAME_DATA_STOP_FINISHED_GATE_PASS')
