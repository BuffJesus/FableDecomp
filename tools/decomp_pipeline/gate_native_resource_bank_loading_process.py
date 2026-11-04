import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
w=Path(tempfile.mkdtemp(prefix='native-resource-bank-loading-process-'));env=V.env();source=ROOT/'rebuild/src/compiled/00/9f/CResourceList_RemoveResource_009fc020.cpp'
bankSource=ROOT/'rebuild/src/compiled/00/9f/CResourceBank_ProcessLoadingQueue_009fc410.cpp'
if not bankSource.exists():bankSource=ROOT/'work/parity_addtext/resource_bank_loading_process_candidate_20261009.cpp'
symbol='?ProcessLoadingQueue@CResourceList@@QAEX_NN@Z';obj=w/'clear.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr;proof=check(obj,symbol,0x9fc250,260,2,{'?GFGetTime@@YINXZ':0x9e1bc0,'?ResourceAbortingEnabled@CResourceBank@@0_NA':0x1396c5c})
bankObj=w/'bank.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(bankObj),str(bankSource)],env,w);assert not r.returncode,r.stdout+r.stderr
bankSymbol='?ProcessLoadingQueue@CResourceBank@@QAEX_NN@Z';bankProof=check(bankObj,bankSymbol,0x9fc410,26,1,{'?ProcessLoadingQueue@CResourceList@@QAEX_NN@Z':0x9fc250})
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
body=read_virtual_data(EXE.read_bytes(),0x9fc250,260);cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True;fixups=[]
for ins in cs.disasm(body,0x9fc250):
 if ins.mnemonic=='call' and ins.bytes[0]==0xe8:assert int(ins.op_str,16)==0x9e1bc0;fixups.append((ins.address-0x9fc250+1,20))
 if ins.disp_size==4 and ins.disp==0x1396c5c:fixups.append((ins.address-0x9fc250+ins.disp_offset,6))
assert len(fixups)==2
bankBody=read_virtual_data(EXE.read_bytes(),0x9fc410,26);assert bankBody[18]==0xe8 and 0x9fc410+23+int.from_bytes(bankBody[19:23],'little',signed=True)==0x9fc250;fixups.append((384+19,20));body=body+b'\x00'*(384-len(body))+bankBody
(w/'resource_bank_loading_process_retail.h').write_text('struct Fixup{unsigned site,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u}'%f for f in fixups)+'};\n');(w/'resource_bank_loading_process_aliases.h').write_text('#pragma comment(linker,"/alternatename:@ProcessQueue@20='+bankSymbol+'")\n')
fixture=ROOT/'rebuild/tests/integration/NativeResourceBankLoadingProcess_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
dataSource=ROOT/'rebuild/src/compiled/01/39/CResourceBank_ResourceAbortingEnabled_01396c5c.cpp';dataObj=w/'abort-flag.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(dataObj),str(dataSource)],env,w);assert not r.returncode,r.stdout+r.stderr
import struct
from gate_game_component_abi import Coff
coff=Coff(dataObj);sec,off=coff.symbols['?ResourceAbortingEnabled@CResourceBank@@0_NA'];raw=dataObj.read_bytes();sectionHeader=20+struct.unpack_from('<H',raw,16)[0]+40*(sec-1);length,rawOffset=struct.unpack_from('<II',raw,sectionHeader+16);characteristics=struct.unpack_from('<I',raw,sectionHeader+36)[0];assert length==1 and off==0
assert rawOffset and raw[rawOffset:rawOffset+1]==b'\1'
assert read_virtual_data(EXE.read_bytes(),0x1396c5c,1)==b'\1'
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)]
for omitted in (obj,bankObj,dataObj):
 r=subprocess.run(cmd+[str(o) for o in (obj,bankObj,dataObj) if o!=omitted],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
r=subprocess.run(cmd+[str(obj),str(bankObj),str(dataObj)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=55296 snapshots=398496' in r.stdout
header=ROOT/'rebuild/include/fable_game_text_bank.h'
report=dict(cases=55296,snapshots=int(r.stdout.split('snapshots=')[1]),providers=[bankProof,proof],data_proof=dict(address="01396c5c",symbol="?ResourceAbortingEnabled@CResourceBank@@0_NA",bytes=1,initial=1,grade="DATA_MATCH"),omission_checks=3,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in (source,bankSource,dataSource,header,ROOT/'rebuild/include/fable_resource_list.h')},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Complete26B bank wrapper plus260B actual loading-queue process with originalinline removal. GFGetTime and virtual CanAbort/IsLoaded/OnLoaded/OnAborted boundaries;original private abort global is actual one-byte provider; original PE and COFF initial enabled byte verified. Independent/native/compiled full16KiB/global callback/final snapshots. Empty/three-node lists,abort/load statuses,time finite/NaN/infinities,callback frame/global/size/link mutations,restart after notifications,count/accountingwrap,two placements/fills;three mandatory source omissions.')
(ROOT/'rebuild/compile-gate/native-resource-bank-loading-process.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_RESOURCE_BANK_LOADING_PROCESS_GATE_PASS')
