from pathlib import Path
from datetime import datetime,timezone
import sys,json,re,hashlib,subprocess,struct
import pefile
import argparse
parser=argparse.ArgumentParser();parser.add_argument('--staging',type=Path);parser.add_argument('--output',type=Path);args=parser.parse_args()
R=Path(__file__).resolve().parents[2];w=args.staging.resolve() if args.staging else None
b=args.output.resolve() if args.output else R/'rebuild/build/native-vertex-buffer-dispatch'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True)
sys.path.insert(0,str(R/'tools/decomp_pipeline'));import check_one as V
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
helpers=['00/99/CBaseClass_CBaseClass_0099a2d0.cpp','00/9e/CVertexBufferWin32_GCVertexBufferWin32_009e2890.cpp','00/a6/CVertexBufferWin32_Initialise_00a63150.cpp','00/a6/CVertexBufferWin32_Uninitialise_00a63200.cpp','00/a0/CRenderManagerCore_WaitWhileResourceIsBusy_00a0aa70.cpp'];sources=[w/'Candidate.cpp' if w else R/'rebuild/src/compiled/00/a6/CVertexBuffer_Dispatch_00a63240.cpp']+[R/'rebuild/src/compiled'/p for p in helpers]+[w/'Fixture.cpp' if w else R/'rebuild/tests/integration/NativeVertexBufferDispatch_test.cpp'];overlay=w/'include' if w else R/'rebuild/include';inc=b/'include';inc.mkdir();inputs={EXE,Path(__file__)};seen=set()
def closure(p):
 if p in seen:return
 seen.add(p);inputs.add(p)
 for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(),re.M):closure(next(q for q in (p.parent/name,overlay/name,R/'rebuild/include'/name) if q.is_file()))
for p in sources:closure(p)
for p in seen:
 if p.is_relative_to(overlay):q=inc/p.relative_to(overlay)
 elif p.is_relative_to(R/'rebuild/include'):q=inc/p.relative_to(R/'rebuild/include')
 else:continue
 q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
inputs.update(inc.rglob('*.h'))
inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file())
inputs.update(p for d in (V.VC/'bin',V.VC/'lib') for p in d.iterdir() if p.is_file() and p.suffix.lower() in ('.exe','.dll','.lib'))
for module in tuple(sys.modules.values()):
 name=getattr(module,'__file__',None)
 if name:
  p=Path(name).resolve()
  if p.suffix=='.py' and p.is_relative_to(R/'tools/decomp_pipeline'):inputs.add(p)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();pins={str(p):sha(p) for p in inputs};(b/'inputs.json').write_text(json.dumps(pins,indent=2));env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(inc);objects=[]
for i,p in enumerate(sources):
 snapshot=b/('unit'+str(i)+'.cpp');snapshot.write_bytes(p.read_bytes());obj=snapshot.with_suffix('.obj');x=V.cl(['/nologo','/c','/O2','/MT','/Gy','/GR','/I'+str(inc),'/Fo'+str(obj),str(snapshot)],env,b);(b/('unit'+str(i)+'.compile.json')).write_text(json.dumps(dict(code=x.returncode,stdout=x.stdout,stderr=x.stderr)));assert not x.returncode,x.stdout+x.stderr;objects.append(obj)
exe=b/'probe.exe';x=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/map:'+str(b/'probe.map'),'/out:'+str(exe),*map(str,objects)],env=env,cwd=b,capture_output=True,text=True);(b/'link.json').write_text(json.dumps(dict(code=x.returncode,stdout=x.stdout,stderr=x.stderr)));assert not x.returncode,x.stdout+x.stderr
mapped={}
for line in (b/'probe.map').read_text().splitlines():
 m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
 if m:mapped[m[1]]=int(m[2],16)
targets={0x9a4ec0:'?GFGetSystemManager@@YIPAVCSystemManager@@XZ',0xa0aa70:'?WaitWhileResourceIsBusy@CRenderManagerCore@@QAEXPAUIDirect3DResource9@@M@Z',0xa63200:'?Uninitialise@CVertexBufferWin32@@QAEXXZ',0x99a300:'??1CBaseClassNonCopyable@@UAE@XZ',0xbfe9bc:'??3@YAXPAX@Z',0x129ba20:'??_7CVertexBufferWin32@@6B@'}
spans=[(0xac20a0,5,0),(0xac20b0,5,16),(0xac20c0,5,32),(0xac20d0,5,48),(0xa63240,82,64),(0xa63120,10,160),(0xa63130,3,176),(0xa63140,7,192),(0x9e2890,43,208)];body=bytearray(276);patches=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
for address,size,offset in spans:
 original=read_virtual_data(EXE.read_bytes(),address,size);body[offset:offset+size]=original
 for i in md.disasm(original,address):
  if i.mnemonic=='call' and i.operands[0].type==2:patches.append((offset+i.address-address+i.imm_offset,mapped[targets[i.operands[0].imm]],1))
  elif struct.pack('<I',0x129ba20) in i.bytes:patches.append((offset+i.address-address+bytes(i.bytes).index(struct.pack('<I',0x129ba20)),mapped[targets[0x129ba20]],0))
body[256:276]=read_virtual_data(EXE.read_bytes(),0x129ba20,20);assert struct.unpack_from('<5I',body,256)==(0x9e2890,0xa63240,0xa63120,0xa63130,0xa63140)
(b/'original.bin').write_bytes(body);(b/'patches.bin').write_bytes(struct.pack('<I',len(patches))+b''.join(struct.pack('<III',*p) for p in patches))
runs=[]
for argv in ([],[b/'original.bin',b/'patches.bin']):
 x=subprocess.run([str(exe),*map(str,argv)],capture_output=True,text=True,timeout=60);record=dict(code=x.returncode,stdout=x.stdout,stderr=x.stderr);runs.append(record);(b/'runs.json').write_text(json.dumps(runs,indent=2));print(record,flush=True);assert not x.returncode,record
assert all(sha(Path(p))==h for p,h in pins.items()),'input drift'
r=dict(objects={str(p):sha(p) for p in objects},object_sources={str(o):str(p) for o,p in zip(objects,sources)},input_sha256=pins)
specs = [
 (0, '?Lock@CVertexBufferBase@@QAEPAEKKK@Z', 0xac20a0, 5),
 (0, '?Unlock@CVertexBufferBase@@QAEXXZ', 0xac20b0, 5),
 (0, '?IsBusy@CVertexBufferBase@@QBE_NXZ', 0xac20c0, 5),
 (0, '?Sizeof@CVertexBufferBase@@QBEKXZ', 0xac20d0, 5),
 (0, '?DoLock@CVertexBufferWin32@@EAEPAEKKK@Z', 0xa63240, 82),
 (0, '?DoUnlock@CVertexBufferWin32@@EAEXXZ', 0xa63120, 10),
 (0, '?DoIsBusy@CVertexBufferWin32@@EBE_NXZ', 0xa63130, 3),
 (0, '?DoSizeof@CVertexBufferWin32@@EBEKXZ', 0xa63140, 7),
 (2, '??_GCVertexBufferWin32@@UAEPAXI@Z', 0x9e2890, 43),
 (2, '??_7CVertexBufferWin32@@6B@', 0x129ba20, 20),
 (3, '?Initialise@CVertexBufferWin32@@QAE_NW4EVertexType@@JJKW4EVertexBufferPool@@@Z', 0xa63150, 165),
 (4, '?Uninitialise@CVertexBufferWin32@@QAEXXZ', 0xa63200, 53),
 (5, '?WaitWhileResourceIsBusy@CRenderManagerCore@@QAEXPAUIDirect3DResource9@@M@Z', 0xa0aa70, 3),
]
addresses = {symbol: address for _, symbol, address, _ in specs}
addresses.update({'?GFGetSystemManager@@YIPAVCSystemManager@@XZ': 0x9a4ec0,
 '??1CBaseClassNonCopyable@@UAE@XZ': 0x99a300, '??3@YAXPAX@Z': 0xbfe9bc,
 '??_ECVertexBufferWin32@@UAEPAXI@Z': 0x9e2890})
mapped = {}
for line in (b / 'probe.map').read_text().splitlines():
 m = re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b', line)
 if m: mapped[m[1]] = int(m[2], 16)
g = '??_GCVertexBufferWin32@@UAEPAXI@Z'
e = '??_ECVertexBufferWin32@@UAEPAXI@Z'
assert mapped[e] == mapped[g], 'scalar weak alias changed'
pe = pefile.PE(str(b / 'probe.exe'))
image = pe.OPTIONAL_HEADER.ImageBase
proof = []
for index, symbol, address, size in specs:
 c = Coff(objects[index]); section, offset = c.symbols[symbol]
 raw, rels = c.sections[section]
 if symbol == '??_7CVertexBufferWin32@@6B@':
  # /GR adds the complete-object-locator pointer immediately before slot zero.
  assert offset == 4 and len(raw) == size + 4
  raw, rels = raw[offset:], {site-offset: value for site, value in rels.items() if site >= offset}
 else:
  assert offset == 0 and len(raw) == size, (symbol, len(raw), size)
 fixed = bytearray(raw); linked = bytearray(raw)
 for site, (target, kind) in rels.items():
  assert kind in (6, 20)
  addend = struct.unpack_from('<i', raw, site)[0]
  struct.pack_into('<I', fixed, site, (addresses[target] + addend - (address + site + 4 if kind == 20 else 0)) & 0xffffffff)
  struct.pack_into('<I', linked, site, (mapped[target] + addend - (mapped[symbol] + site + 4 if kind == 20 else 0)) & 0xffffffff)
 assert bytes(fixed) == read_virtual_data(EXE.read_bytes(), address, size), symbol
 assert bytes(linked) == pe.get_data(mapped[symbol] - image, size), symbol
 proof.append(dict(symbol=symbol, address=hex(address), bytes=size, object_exact=True, final_image_exact=True))

def hierarchy(read, vtable):
 u32 = lambda address: struct.unpack('<I', read(address, 4))[0]
 words = lambda address, n: struct.unpack('<' + 'I'*n, read(address, 4*n))
 name = lambda descriptor: read(descriptor + 8, 128).split(b'\0', 1)[0].decode('ascii')
 locator = u32(vtable-4)
 signature, offset, cd_offset, descriptor, chd = words(locator, 5)
 assert (signature, offset, cd_offset) == (0, 0, 0)
 chd_signature, attributes, count, base_array = words(chd, 4)
 assert chd_signature == 0 and attributes == 0 and count == 4
 bases = []
 for base in words(base_array, count):
  typ, contained, member, vbtable, vdisp, attrs = words(base, 6)
  bases.append(dict(type=name(typ), contained=contained, pmd=[member, vbtable, vdisp], attributes=attrs))
 return dict(type=name(descriptor), bases=bases)

retail_bytes = EXE.read_bytes()
retail = hierarchy(lambda address, size: read_virtual_data(retail_bytes, address, size), 0x129ba20)
built = hierarchy(lambda address, size: pe.get_data(address-image, size), mapped['??_7CVertexBufferWin32@@6B@'])
assert retail == built, (retail, built)
assert [row['type'] for row in built['bases']] == ['.?AVCVertexBufferWin32@@', '.?AVCVertexBufferBase@@', '.?AVCBaseClassNonCopyable@@', '.?AVCBaseClass@@']
assert all(sha(Path(p)) == h for p, h in r['input_sha256'].items())
qualification=dict(verified_bytes=sum(row['bytes'] for row in proof),proof=proof,rtti=built)
out=b/'controls';out.mkdir();records=[]
def index(name):return 0 if name=='Candidate.cpp' else next(i for i,p in enumerate(objects) if Path(r['object_sources'][str(p)]).name==name)
def reject(label,items):
 exe=out/('fault'+str(len(records))+'.exe');x=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/out:'+str(exe),*map(str,items)],env=env,capture_output=True,text=True);assert not x.returncode,x.stdout+x.stderr;x=subprocess.run([str(exe)],capture_output=True,text=True,timeout=60);record=dict(name=label,code=x.returncode,stdout=x.stdout,stderr=x.stderr);assert x.returncode==80 and 'VERTEX_BUFFER_FAILURE' in x.stderr,record;records.append(record);(out/'progress.json').write_text(json.dumps(records,indent=2));print(label,'rejected',flush=True)
specs=[('no-lock-dispatch','Candidate.cpp','return DoLock(flags,offset,bytes);','return 0;'),('no-unlock-dispatch','Candidate.cpp','void CVertexBufferBase::Unlock() {DoUnlock();}','void CVertexBufferBase::Unlock() {}'),('busy-dispatch','Candidate.cpp','return DoIsBusy();','return true;'),('size-dispatch','Candidate.cpp','return DoSizeof();','return DoSizeof()+1;'),('always-full-length','Candidate.cpp','if(bytes==0)bytes=ByteSize;','bytes=ByteSize;'),('zero-length','Candidate.cpp','if(bytes==0)bytes=ByteSize;',';'),('late-full-length','Candidate.cpp','if(bytes==0)bytes=ByteSize;','bool allBytes=bytes==0;'),('cached-buffer','Candidate.cpp','if(bytes==0)bytes=ByteSize;','IDirect3DVertexBuffer9* saved=Buffer; if(bytes==0)bytes=ByteSize;'),('swapped-lock-range','Candidate.cpp','Buffer->Lock(offset,bytes,','Buffer->Lock(bytes,offset,'),('lock-flags','Candidate.cpp','reinterpret_cast<void**>(&data),flags','reinterpret_cast<void**>(&data),0'),('no-output','Candidate.cpp','return data;','return 0;'),('no-device-unlock','Candidate.cpp','void CVertexBufferWin32::DoUnlock(){Buffer->Unlock();}','void CVertexBufferWin32::DoUnlock(){}'),('busy-policy','Candidate.cpp','DoIsBusy() const{return false;}','DoIsBusy() const{return true;}'),('size-overhead','Candidate.cpp','ByteSize)+56','ByteSize)+52'),('initial-byte-length','CVertexBufferWin32_Initialise_00a63150.cpp','long bytes = count * stride;','long bytes = count * stride + 1;'),('initial-type','CVertexBufferWin32_Initialise_00a63150.cpp','VertexType = type;','VertexType = 0;'),('failed-create-shortcut','CVertexBufferWin32_Initialise_00a63150.cpp','    ByteSize = bytes;','    if(result<0)return false;\n    ByteSize = bytes;'),('release-missing','CVertexBufferWin32_Uninitialise_00a63200.cpp','reinterpret_cast<IUnknown*>(Buffer)->Release();',';'),('buffer-clear-missing','CVertexBufferWin32_Uninitialise_00a63200.cpp','Buffer = 0;',';')]
for label,name,old,new in specs:
 i=index(name);text=Path(r['object_sources'][str(objects[i])]).read_text();assert text.count(old)==1,(label,text.count(old));body=text.replace(old,new)
 if label=='late-full-length':body=body.replace('    unsigned char* data=0;','    if(allBytes)bytes=ByteSize;\n    unsigned char* data=0;')
 if label=='cached-buffer':body=body.replace('Buffer->Lock(offset,bytes,','saved->Lock(offset,bytes,')
 path=out/(label+'.cpp');path.write_text(body);obj=path.with_suffix('.obj');x=V.cl(['/nologo','/c','/O2','/MT','/Gy','/GR','/I'+str(inc),'/Fo'+str(obj),str(path)],env,out);assert not x.returncode,x.stdout+x.stderr;items=list(objects);items[i]=obj;reject(label,items)
for name in ('Candidate.cpp','CVertexBufferWin32_GCVertexBufferWin32_009e2890.cpp','CVertexBufferWin32_Initialise_00a63150.cpp','CVertexBufferWin32_Uninitialise_00a63200.cpp','CRenderManagerCore_WaitWhileResourceIsBusy_00a0aa70.cpp'):
 i=index(name);x=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/out:'+str(out/('missing-'+name+'.exe')),*map(str,[p for n,p in enumerate(objects) if n!=i])],env=env,capture_output=True,text=True);assert x.returncode;records.append(dict(name='missing-'+name,link_rejected=True,stdout=x.stdout,stderr=x.stderr))
body=bytearray((b/'original.bin').read_bytes());assert body[208+25]==1;body[208+25]=0;(out/'no-original-free.bin').write_bytes(body);x=subprocess.run([str(b/'probe.exe'),str(out/'no-original-free.bin'),str(b/'patches.bin')],capture_output=True,text=True,timeout=60);assert x.returncode==80 and 'VERTEX_BUFFER_FAILURE' in x.stderr;records.append(dict(name='original-scalar-no-free',code=x.returncode,stderr=x.stderr))
# A user-provided (inline, empty) abstract-base destructor adds a transient base vptr store to the original scalar body;
# retail's CVertexBufferBase destructor is implicit (??_GCVertexBufferBase 0x009E2B20 calls ~CBaseClassNonCopyable).
fault_inc=out/'transient-base-vptr-include'
for original_header in inc.rglob('*.h'):
 dest=fault_inc/original_header.relative_to(inc);dest.parent.mkdir(parents=True,exist_ok=True);dest.write_bytes(original_header.read_bytes())
header=fault_inc/'fable_font_vertex_buffer.h';body=header.read_text();base_open='class CVertexBufferBase : public CBaseClassNonCopyable {\npublic:\n';assert body.count(base_open)==1;header.write_text(body.replace(base_open,base_open+'    virtual ~CVertexBufferBase() {}\n'))
fault_obj=out/'transient-base-vptr.obj';x=V.cl(['/nologo','/c','/O2','/MT','/Gy','/GR','/I'+str(fault_inc),'/Fo'+str(fault_obj),str(b/'unit2.cpp')],env,out);assert not x.returncode,x.stdout+x.stderr
fault_coff=Coff(fault_obj);section,start=fault_coff.symbols['??_GCVertexBufferWin32@@UAEPAXI@Z'];fault_raw,_=fault_coff.sections[section];assert start==0 and len(fault_raw)==49
records.append(dict(name='transient-base-vptr-store',structural_rejected=True,original_scalar_bytes=43,fault_scalar_bytes=len(fault_raw)))
again=subprocess.run([str(exe),str(b/'original.bin'),str(b/'patches.bin')],capture_output=True,text=True,timeout=60)
assert not again.returncode and again.stdout==runs[1]['stdout'],again
assert all(sha(Path(p))==h for p,h in pins.items()),'input drift'
report=dict(status='PASS',build=str(b),cases=23328,original_variants=4,original_paths=93312,repeat=True,qualification=qualification,controls=records,runs=runs,**r,boundary='Actual base dispatch, Win32 methods, Initialise/Uninitialise, scalar destructor and PC wait(no-op). Four original/candidate dispatch combinations. Borrowed system/display/core; COM CreateVertexBuffer/Lock/Unlock/Release adapters. Raw metadata and invalid real-buffer ranges are pass-through tests; no GPU or gameplay certification.')
(b/'receipt.json').write_text(json.dumps(report,indent=2))
if args.output is None and w is None:(R/'rebuild/compile-gate/native-vertex-buffer-dispatch.json').write_text(json.dumps(report,indent=2))
print('NATIVE_VERTEX_BUFFER_DISPATCH_PASS cases=23328 original_paths=93312 controls='+str(len(records)),b,flush=True)
