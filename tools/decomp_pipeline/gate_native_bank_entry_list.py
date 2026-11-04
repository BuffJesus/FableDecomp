"""Native ReadEntryList: rebuilt and transplanted retail root against semantic cases.

No installed executable is modified. All native executables are build-owned.
"""
from pathlib import Path
from datetime import datetime, timezone
import sys, json, subprocess, re, struct, hashlib
R=Path(__file__).resolve().parents[2]
sys.path[:0]=[str(R/'tools/decomp_pipeline'),str(R/'rebuild/tests/integration')]
import check_one as V
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from pe_oracle import EXE,read_virtual_data
import NativeBankEntryList_model as M
import pefile,capstone,csv

b=R/'rebuild/build/native-bank-entry-list/00/9c/009cfbc0'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True)
print(b,flush=True)
SOURCES=['00/99/CCharString_AllocStringData_0099ea60.cpp', '00/99/CCharString_CCharString_0099ec30.cpp', '00/99/CCharString_Constructor_0099ebf0.cpp', '00/99/CCharString_ConversionOperator_0099e4c0.cpp', '00/99/CCharString_CopyAssignment_0099efb0.cpp', '00/99/CCharString_DefaultConstructor_0099e4b0.cpp', '00/99/CCharString_Destructor_0099eae0.cpp', '00/99/CCharString_UnassignString_0099e9b0.cpp', '00/99/CDataInputStream_ReadPresizedString_00996390.cpp', '00/9a/CCharString_InitFromCStr_009a0590.cpp', '00/9a/global_CBasicStringCharAssignFromData_009a0300.cpp', '00/9b/CArrayVector_SaveData_DeallocateMemory_009b4ae0.cpp', '00/9b/CVectorMap_CompressedOffset_DeallocateUnusedMemory_009b7b10.cpp', '00/9b/STL_AdjustHeap_CompressedOffset_009b3060.cpp', '00/9b/STL_InsertionSort_CompressedOffset_009b56f0.cpp', '00/9b/STL_Introsort_CompressedOffset_009b7fa0.cpp', '00/9b/STL_LinearInsert_CompressedOffset_009b46b0.cpp', '00/9b/STL_MakeHeap_CompressedOffset_009b3cf0.cpp', '00/9b/STL_PartialSort_CompressedOffset_009b6be0.cpp', '00/9b/STL_Partition_CompressedOffset_009b3900.cpp', '00/9b/STL_PushHeap_CompressedOffset_009b1d80.cpp', '00/9b/STL_SortHeap_CompressedOffset_009b5c10.cpp', '00/9b/STL_Sort_CompressedOffset_009b85a0.cpp', '00/9b/STL_UnguardedInsertionSort_CompressedOffset_009b2db0.cpp', '00/9c/CBankFile_ReadEntryList_009cfbc0.cpp', '00/9d/CSmallVector_CCharString_AllocateCopy_009d2160.cpp', '00/9d/CSmallVector_CCharString_Clear_009d2110.cpp', '01/3b/CharStringInstanceCount_013bd800.cpp']
sources=[R/'rebuild/src/compiled'/s for s in SOURCES]
fixture=R/'rebuild/tests/integration/NativeBankEntryList_test.cpp'
inputs={Path(__file__),Path(M.__file__),EXE,R/'rebuild/build/symbol_index.tsv'};visited=set()
def closure(p):
 if p in visited:return
 visited.add(p);inputs.add(p)
 for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(),re.M):
  closure(next(q for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file()))
for p in sources+[fixture]:closure(p)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file())
inputs.update(p for d in (V.VC/'bin',V.VC/'lib') for p in d.iterdir() if p.is_file() and p.suffix.lower() in ('.exe','.dll','.lib'))
for module in tuple(sys.modules.values()):
 name=getattr(module,'__file__',None)
 if name:
  path=Path(name).resolve()
  if path.suffix=='.py' and path.is_relative_to(R/'tools/decomp_pipeline'):inputs.add(path)
pins={str(p):sha(p) for p in inputs}
(b/'inputs.json').write_text(json.dumps(pins,indent=2))
inc=b/'include';inc.mkdir()
for p in visited:
 q=b/('root%d.cpp'%sources.index(p) if p in sources else 'Fixture.cpp') if p in sources+[fixture] else inc/p.relative_to(R/'rebuild/include')
 q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(inc)
objects=[];coffs=[]
for i,p in enumerate(sources+[fixture]):
 name='root%d'%i if i<len(sources) else 'Fixture';obj=b/(name+'.obj')
 result=V.cl(['/nologo','/c','/O2','/MT','/Gy','/I'+str(inc),'/Fo'+str(obj),str(b/(name+'.cpp'))],env,b)
 (b/(name+'.compile.json')).write_text(json.dumps(dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr)))
 assert not result.returncode,result.stdout+result.stderr
 objects.append(obj);coffs.append(Coff(obj))
command=[str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/out:'+str(b/'probe.exe'),'/map:'+str(b/'probe.map'),*map(str,objects)]
result=subprocess.run(command,cwd=b,env=env,capture_output=True,text=True)
(b/'link.json').write_text(json.dumps(dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr)))
assert not result.returncode,result.stdout+result.stderr
mapping={}
for line in (b/'probe.map').read_text().splitlines():
 m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
 if m:mapping[m[1]]=int(m[2],16)
symbol='?ReadEntryList@CBankFile@@AAE_NAAVCFileDataInputStream@@KK@Z'
c=coffs[sources.index(R/'rebuild/src/compiled/00/9c/CBankFile_ReadEntryList_009cfbc0.cpp')]
sec,off=c.symbols[symbol];body,rels=c.sections[sec];assert off==0
image=EXE.read_bytes();original=read_virtual_data(image,0x9cfbc0,1969)
score=compare(body,original,tuple(rels),retail_relocations('009cfbc0',1969)).to_dict();assert score['near_match'],score
raw=(b/'probe.exe').read_bytes();pe=pefile.PE(data=raw);at=mapping[symbol]
# Resolve /alternatename observers from the fixture's own explicit declarations.
for original_name,observer in re.findall(r'/alternatename:([^=\"]+)=([^\"]+)',fixture.read_text()):
 if observer in mapping:mapping[original_name]=mapping[observer]
fixed=bytearray(body)
for site,(name,kind) in rels.items():
 assert kind in (6,20)
 struct.pack_into('<I',fixed,site,(mapping[name]+struct.unpack_from('<i',body,site)[0]-(at+site+4 if kind==20 else 0))&0xffffffff)
assert bytes(fixed)==pe.get_data(at-pe.OPTIONAL_HEADER.ImageBase,len(body))
stats=M.write_cases(b/'cases.bin')
case_count=len(M.cases());expected_summary='cases=%d entries=%d updates=%d'%(case_count,stats['entries'],stats['updates'])
result=subprocess.run([str(b/'probe.exe'),str(b/'cases.bin')],cwd=b,capture_output=True,text=True,timeout=30)
(b/'run.json').write_text(json.dumps(dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr)))
assert result.returncode==0 and expected_summary in result.stdout,result.stdout+result.stderr
decoder=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32)
calls=[dict(address=hex(i.address),instruction=i.mnemonic+' '+i.op_str) for i in decoder.disasm(original,0x9cfbc0) if i.mnemonic=='call']
(b/'original-calls.json').write_text(json.dumps(calls,indent=2))
(b/'root-relocations.json').write_text(json.dumps({hex(k):v for k,v in rels.items()},indent=2))
# Transplant only the original root into a new executable code section. Every
# direct call is resolved against the independent symbol index; cleanup calls
# cannot be paired by instruction order. CRT boundaries have explicit addresses.
index={}
with (R/'rebuild/build/symbol_index.tsv').open() as f:
 for row in csv.DictReader(f,delimiter='\t'):index.setdefault(row['symbol'],set()).add(int(row['address'],16))
external={'_malloc':0xbfea0e,'_free':0xbfea14,'??2@YAPAXI@Z':0xbfea1a,'??_V@YAXPAX@Z':0xbfeb1c,'??_U@YAPAXI@Z':0xbfeb22,
 '?AllocateBuffers@CBankFile@@AAEXK@Z':0x9ceae0,'?InitEntry@CBankFile@@AAEXKABVCCharString@@0@Z':0x9ce050,'?PackRuntimeData@CBankFile@@AAEXXZ':0x9cd740}
# This vector constructor is emitted at several retail sites; the generic index
# records its other homes. ReadPresizedString calls the 00411910 copy.
vector_char='??0?$vector@DV?$allocator@D@_STL@@@_STL@@QAE@I@Z'
index.setdefault(vector_char,set()).add(0x411910)
helper_symbol='?ReadPresizedString@CDataInputStream@@QAE?AVCCharString@@XZ'
helper_coff=coffs[8];helper_section,helper_offset=helper_coff.symbols[helper_symbol]
helper_body,helper_rels=helper_coff.sections[helper_section]
assert helper_offset==0 and len(helper_body)==334 and len(helper_rels)==9
helper_original=read_virtual_data(image,0x996390,334);helper_fixed=bytearray(helper_body);helper_proof=[]
for site,(name,kind) in helper_rels.items():
 target=(struct.unpack_from('<I',helper_original,site)[0]+(0x996390+site+4 if kind==20 else 0))&0xffffffff
 if kind==6:assert name=='??_C@_00CNPNBAHC@?$AA@' and target==0x122d70e and read_virtual_data(image,target,1)==b'\0'
 else:assert target in index.get(name,set()) or external.get(name)==target,(name,hex(target))
 helper_fixed[site:site+4]=helper_original[site:site+4];helper_proof.append(dict(site=site,symbol=name,target=hex(target),kind=kind))
assert bytes(helper_fixed)==helper_original
(b/'read-presized-string-retail-proof.json').write_text(json.dumps(dict(grade='MATCH',entry='00996390',bytes=334,fixups=helper_proof),indent=2))
direct=[i for i in decoder.disasm(original,0x9cfbc0) if i.mnemonic=='call' and i.bytes[0]==0xe8]
relative=[(site,name) for site,(name,kind) in sorted(rels.items()) if kind==20]
assert len(direct)==len(relative)
align=lambda n,a:(n+a-1)//a*a
last=pe.sections[-1];rva=align(last.VirtualAddress+max(last.Misc_VirtualSize,last.SizeOfRawData),pe.OPTIONAL_HEADER.SectionAlignment)
base=pe.OPTIONAL_HEADER.ImageBase+rva;transplant=bytearray(original);callproof=[]
for ins in direct:
 target=int(ins.op_str,16)
 names={name for site,name in relative if target in index.get(name,set()) or external.get(name)==target}
 assert len(names)==1,(hex(target),names)
 name=names.pop()
 pos=ins.address-0x9cfbc0+1;struct.pack_into('<I',transplant,pos,(mapping[name]-(base+pos+4))&0xffffffff)
 callproof.append(dict(retail_call=hex(ins.address),target=hex(target),symbol=name,linked_target=hex(mapping[name])))
absolute=[(site,name) for site,(name,kind) in rels.items() if kind==6]
decoder.detail=True
original_absolute=[]
for ins in decoder.disasm(original,0x9cfbc0):
 if ins.mnemonic=='push' and len(ins.bytes)==5 and ins.bytes[0]==0x68:
  value=struct.unpack_from('<I',ins.bytes,1)[0]
  if 0x400000<=value<0x1600000:original_absolute.append(ins.address-0x9cfbc0+1)
assert len(absolute)==len(original_absolute)==1
for pos,(site,name) in zip(original_absolute,absolute):
 old=struct.unpack_from('<I',original,pos)[0];assert read_virtual_data(image,old,1)==b'\0' and name.startswith('??_C@_00')
 struct.pack_into('<I',transplant,pos,mapping[name])
header=last.get_file_offset()+40
assert header+40<=pe.OPTIONAL_HEADER.SizeOfHeaders and not any(raw[header:header+40])
start=align(len(raw),pe.OPTIONAL_HEADER.FileAlignment);size=align(len(transplant),pe.OPTIONAL_HEADER.FileAlignment)
oracle=bytearray(raw);oracle.extend(bytes(start+size-len(oracle)));oracle[start:start+len(transplant)]=transplant
struct.pack_into('<8sIIIIIIHHI',oracle,header,b'.oracle\0',len(transplant),rva,size,start,0,0,0,0,0x60000020)
struct.pack_into('<H',oracle,pe.DOS_HEADER.e_lfanew+6,pe.FILE_HEADER.NumberOfSections+1)
struct.pack_into('<I',oracle,pe.OPTIONAL_HEADER.get_field_absolute_offset('SizeOfImage'),align(rva+len(transplant),pe.OPTIONAL_HEADER.SectionAlignment))
root_offset=pe.get_offset_from_rva(at-pe.OPTIONAL_HEADER.ImageBase);oracle[root_offset:root_offset+5]=b'\xe9'+struct.pack('<I',(base-at-5)&0xffffffff)
(b/'original-root.exe').write_bytes(oracle)
result=subprocess.run([str(b/'original-root.exe'),str(b/'cases.bin')],cwd=b,capture_output=True,text=True,timeout=30)
(b/'original-root.run.json').write_text(json.dumps(dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr)))
assert result.returncode==0 and expected_summary in result.stdout,result.stdout+result.stderr
(b/'original-root-relocation-proof.json').write_text(json.dumps(callproof,indent=2))
faults=[('runtime-valid',bytes.fromhex('c6400901'),3,0),('symbol-flag',bytes.fromhex('f6467808'),3,0),
 ('timestamp-flag',bytes.fromhex('f6467810'),3,0),('update-flag',bytes.fromhex('f6467820'),3,0),
 ('read-only-field',bytes.fromhex('8a878d000000'),2,0x8c),('file-valid',bytes.fromhex('c6879400000001'),6,0)]
controls=[]
for label,pattern,delta,value in faults:
 for variant,code,executable,position in [('candidate',body,raw,root_offset),('original',original,oracle,start)]:
  matches=[i for i in range(len(code)) if code.startswith(pattern,i)]
  assert len(matches)==1,(label,variant,matches)
  altered=bytearray(executable);altered[position+matches[0]+delta]=value
  path=b/('control-%02d.exe'%len(controls));path.write_bytes(altered)
  result=subprocess.run([str(path),str(b/'cases.bin')],cwd=b,capture_output=True,text=True,timeout=30)
  observation=dict(name=label,variant=variant,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr)
  (b/('control-'+label+'-'+variant+'.json')).write_text(json.dumps(observation))
  assert result.returncode==80 and 'BANK_ENTRY_LIST_FAILURE' in result.stderr,observation
  controls.append(observation)
omissions=[]
for n in range(len(sources)):
 cmd=['/out:'+str(b/('missing-%d.exe'%n)) if a.startswith('/out:') else a for a in command if a!=str(objects[n]) and not a.startswith('/map:')]
 result=subprocess.run(cmd,cwd=b,env=env,capture_output=True,text=True)
 row=dict(source=str(sources[n]),returncode=result.returncode,stdout=result.stdout,stderr=result.stderr)
 (b/('missing-%d.link.json'%n)).write_text(json.dumps(row))
 if result.returncode:assert 'LNK2001' in result.stdout or 'LNK2019' in result.stdout,row
 else:
  run=subprocess.run([str(b/('missing-%d.exe'%n)),str(b/'cases.bin')],cwd=b,capture_output=True,text=True,timeout=30)
  row['runtime_returncode']=run.returncode;row['runtime_stdout']=run.stdout;row['runtime_stderr']=run.stderr
 omissions.append(row)
(b/'omissions.json').write_text(json.dumps(omissions,indent=2))
assert all(row['returncode']!=0 for row in omissions),omissions
linked=[]
for line in (b/'probe.map').read_text().splitlines():
 m=re.match(r'^\s+0001:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\s+f\s+(?:i\s+)?root(\d+)\.obj\s*$',line)
 if not m:continue
 name,location,n=m[1],int(m[2],16),int(m[3]);coff=coffs[n]
 if name not in coff.symbols:
  assert name=='@ReadList@20' and location==mapping[symbol],name
  continue
 section,offset=coff.symbols[name];payload,relocations=coff.sections[section]
 assert offset==0,(name,offset)
 patched=bytearray(payload)
 for site,(dependency,kind) in relocations.items():
  assert kind in (6,20),(name,dependency,kind)
  struct.pack_into('<I',patched,site,(mapping[dependency]+struct.unpack_from('<i',payload,site)[0]-(location+site+4 if kind==20 else 0))&0xffffffff)
 assert bytes(patched)==pe.get_data(location-pe.OPTIONAL_HEADER.ImageBase,len(payload)),name
 linked.append(dict(symbol=name,source=str(sources[n]),bytes=len(payload),fixups=len(relocations)))
assert any(x['symbol']==symbol for x in linked)
(b/'linked-bodies.json').write_text(json.dumps(linked,indent=2))
unique_linked_bodies=len({mapping[x['symbol']] for x in linked})
for p,h in pins.items():assert sha(Path(p))==h,('input drift',p)
for p in inputs:
 if p.suffix=='.py' and p.is_relative_to(R):
  q=b/'python-provenance'/p.relative_to(R);q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
report=dict(status='PASS',grade='near_match',input_sha256=pins,build=str(b),host=stats,host_cases=case_count,original_root_host_cases=case_count,similarity=score,linked_root_verified=True,linked_symbols=len(linked),unique_linked_bodies=unique_linked_bodies,read_presized_string_grade='MATCH',read_presized_string_bytes=334,mutation_controls=len(controls),required_provider_omissions=sum(x['returncode']!=0 for x in omissions),redundant_providers=[x['source'] for x in omissions if x['returncode']==0],object_sha256={str(p):sha(p) for p in objects},executable_sha256=sha(b/'probe.exe'),original_executable_sha256=sha(b/'original-root.exe'),boundary='Fresh native host and transplanted original root both pass independent semantic expectations across all eight indexing-flag combinations. Helpers remain rebuilt and AllocateBuffers/InitEntry/PackRuntimeData/chunk overflow/virtual notifications are observed boundaries. ReadPresizedString334B matches retail after nine verified relocations. Twelve semantic mutations detected. All28 provider omissions rejected. Original and rebuilt roots satisfy the same independent semantic assertions; no full-state/allocation-order equality, malformed-input safety, allocation-failure or unwind claim. This gate alone does not promote metadata.')
(b/'receipt.json').write_text(json.dumps(report,indent=2)+'\n')
(R/'rebuild/compile-gate/native-bank-entry-list.json').write_text(json.dumps(report,indent=2)+'\n')
print('NATIVE_BANK_ENTRY_LIST_PASS host=%d original_root=%d entries=%d controls=%d omissions=%d linked_symbols=%d'%(case_count,case_count,stats['entries'],len(controls),sum(x['returncode']!=0 for x in omissions),len(linked)))
