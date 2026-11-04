"""Bounded actual memory-stream lifetime/overflow; LZO remains external."""
import ast,hashlib,itertools,json,re,struct,subprocess,sys,tempfile
from pathlib import Path
from unicorn import Uc,UcError,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from variable_allocator_parity import check
B=Path(tempfile.mkdtemp(prefix='loader-memory-composition-'))
A=0x2000000;SIZE=16384;S=0x3000000;STOP=0x4000000;ERROR=0x5000000;ERRORVAR=0x13bc9fc;LZO=0xc069d0
CTOR=0x994930;DTOR=0x993b10;ENTRY=0x995b10;VT=0x123675c
specs=[
 ('CMemoryDataInputStream_ReadCompressed_00995b10.cpp','?ReadCompressed@CMemoryDataInputStream@@UAEXPAXJ@Z',ENTRY,540,6),
 ('DataInputStream_ReadWithSrcChunkOverflow_00993ca0.cpp','?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z',0x993ca0,351,1),
 ('CDataInputStream_CDataInputStream_00993b20.cpp','??0CMemoryDataInputStream@@QAE@PBXK@Z',CTOR,48,2),
 ('CMemoryDataInputStream_DestructorTyped_00993b10.cpp','??1CMemoryDataInputStream@@UAE@XZ',DTOR,11,2),
 ('CDataInputStream_InternalReadNextSourceChunk_00993b90.cpp','?InternalReadNextSourceChunk@CDataInputStream@@EAEXAAPBEAAJ@Z',0x993b90,3,0),
 ('CDataInputStream_NeedsBufferedRead_00993ba0.cpp','?NeedsBufferedRead@CDataInputStream@@EAE_NJ@Z',0x993ba0,5,0),
 ('CDataInputStream_InternalReadUnbuffered_00993bb0.cpp','?InternalReadUnbuffered@CDataInputStream@@EAEXPAEJ@Z',0x993bb0,3,0),
 ('CBaseClass_CBaseClass_0099a2d0.cpp','??0CBaseClassNonCopyable@@QAE@XZ',0x99a2f0,9,1),
 ('CBaseClass_CBaseClass_0099a2d0.cpp','??1CBaseClassNonCopyable@@UAE@XZ',0x99a300,7,1),
 ('CDataInputStream_CDataInputStream_00993b20.cpp','?GetPosition@CDataInputStream@@UBEKXZ',0x405fa0,4,0)]
mapping={symbol:address for path,symbol,address,size,n in specs}
mapping.update({'_lzo1x_decompress_asm_fast':LZO,'?m_errorCallback@CAFile@@2P6IXXZA':ERRORVAR,'??_7CMemoryDataInputStream@@6B@':VT,'??_7CDataStream@@6B@':0x129a69c,'??_7CBaseClassNonCopyable@@6B@':0x129a7c4})
mapping['??_7CBaseClass@@6B@']=0x1231710
objects={};proofs=[];bodies={'retail':{},'compiled':{}}
for path,symbol,address,size,nrels in specs:
 if path not in objects:
  obj=B/(path+'.obj');result=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(R/'rebuild/src/compiled/00/99'/path)],V.env(),B);assert not result.returncode,result.stdout+result.stderr;objects[path]=(obj,Coff(obj))
 obj,co=objects[path];section,offset=co.symbols[symbol];body,rels=co.sections[section];assert offset==0 and len(body)==size and len(rels)==nrels
 patched=bytearray(body)
 for site,(name,kind) in rels.items():
  assert name in mapping and kind in (6,20) and body[site:site+4]==b'\0'*4,(site,name,kind)
  target=mapping[name];struct.pack_into('<I',patched,site,(target-address-site-4 if kind==20 else target)&0xffffffff)
 native=read_virtual_data(EXE.read_bytes(),address,size);assert native==bytes(patched),(path,symbol)
 p=check(obj,symbol,address,size,nrels,mapping);assert p['grade']=='MATCH' and p['complete_decode'];proofs.append(p)
 bodies['retail'][address]=native;bodies['compiled'][address]=bytes(patched)
# Actual emitted table and original retail table, including genuine GetPosition COMDAT.
co=objects[specs[2][0]][1];section,offset=co.symbols['??_7CMemoryDataInputStream@@6B@'];table,rels=co.sections[section]
native_table=read_virtual_data(EXE.read_bytes(),VT,44);assert offset==0 and len(table)==44 and len(rels)==11
slots=struct.unpack('<11I',native_table);assert slots==(0x485420,0x993bc0,0x405fa0,0x993c80,ENTRY,0x996230,0x995d30,0x994960,0x993b90,0x993ba0,0x993bb0)
slot_symbols=('??_ECMemoryDataInputStream@@UAEPAXI@Z','?SetPosition@CDataInputStream@@UAEXK@Z','?GetPosition@CDataInputStream@@UBEKXZ','?Close@CDataInputStream@@UAEXXZ','?ReadCompressed@CMemoryDataInputStream@@UAEXPAXJ@Z','?ReadZlibCompressed@CDataInputStream@@UAEXPAXJ@Z','?ReadNullTerminatedString@CMemoryDataInputStream@@UAE?AVCCharString@@XZ','?ReadNullTerminatedWString@CMemoryDataInputStream@@UAE?AVCWideString@@XZ','?InternalReadNextSourceChunk@CDataInputStream@@EAEXAAPBEAAJ@Z','?NeedsBufferedRead@CDataInputStream@@EAE_NJ@Z','?InternalReadUnbuffered@CDataInputStream@@EAEXPAEJ@Z')
assert set(rels)==set(range(0,44,4))
patched_table=bytearray(table)
for site,(symbol,kind) in rels.items():
 assert kind==6 and table[site:site+4]==b'\0'*4
 assert symbol==slot_symbols[site//4],(site,symbol,slot_symbols[site//4])
 if symbol in mapping:assert mapping[symbol]==slots[site//4]
 struct.pack_into('<I',patched_table,site,slots[site//4])
assert bytes(patched_table)==native_table
omissions=[]
for index,(path,symbol,address,size,nrels) in enumerate(specs):
 required=Path(__file__).with_name('NativeMemoryStreamComposition_required%d.cpp'%index);obj,co=objects[path];ro=B/('required%d.obj'%index)
 result=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(ro),str(required)],V.env(),B);assert not result.returncode,result.stdout+result.stderr
 command=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(B/('required%d.exe'%index)),'kernel32.lib',str(ro)]
 missing=subprocess.run(command,env=V.env(),capture_output=True,text=True);diagnostic=missing.stdout+missing.stderr
 assert missing.returncode and any(('LNK2001' in line or 'LNK2019' in line) and symbol in line for line in diagnostic.splitlines()),diagnostic
 linked=subprocess.run(command+[str(obj)],env=V.env(),capture_output=True,text=True);assert not linked.returncode,linked.stdout+linked.stderr
 omissions.append(dict(symbol=symbol,source=path,missing_failed=True,provider_link_passed=True))
machines={};active=[None];registers=[UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI]
hits={kind:{symbol:0 for path,symbol,address,size,n in specs} for kind in bodies};address_symbols={address:symbol for path,symbol,address,size,n in specs}
def dispatch(u,pc,size,user):active[0](u,pc)
for kind in bodies:
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 pages={address&~4095 for address in bodies[kind]}|{VT&~4095,ERRORVAR&~4095,LZO&~4095,ERROR&~4095}
 for page in pages:u.mem_map(page,4096)
 u.mem_map(A,SIZE);u.mem_map(S,65536);u.mem_map(STOP,4096)
 for address,body in bodies[kind].items():u.mem_write(address,body)
 u.mem_write(VT,native_table if kind=='retail' else bytes(patched_table));u.hook_add(UC_HOOK_CODE,dispatch);machines[kind]=u
def word(v):return struct.pack('<I',v&0xffffffff)
def signed(v):return v if v<0x80000000 else v-0x100000000
class Memory:
 def __init__(self,buf=None,u=None):self.buf=buf;self.u=u
 def read(self,p,n):return bytes(self.buf[p-A:p-A+n]) if self.buf is not None else bytes(self.u.mem_read(p,n))
 def write(self,p,b):
  if self.buf is not None:self.buf[p-A:p-A+len(b)]=b
  else:self.u.mem_write(p,b)
 def w(self,p):return struct.unpack('<I',self.read(p,4))[0]
 def put(self,p,v):self.write(p,word(v))
 def snap(self):return self.read(A,SIZE)
def lzo_boundary(m,ctx,args,events,snaps):
 src,count,dest,length,work=args;assert work==0
 events.append(('lzo',src,count,dest,length,work));snaps.append(m.snap())
 m.write(dest,b'\xa7'*length)
 result=(length-3)&0xffffffff if ctx['mode']==0 else length
 stream=ctx['stream']
 if ctx['mode']==2:m.put(stream+20,0);m.put(stream+12,0)
 if ctx['mode']==3:m.put(stream+4,0x7fffffff)
 snaps.append(m.snap());return result
def model(initial,ctx):
 m=Memory(bytearray(initial));p=ctx['stream'];source=ctx['source'];dest=ctx['dest'];events=[];snaps=[]
 m.put(p,VT);m.put(p+4,0);m.put(p+8,ctx['source_size']);m.put(p+12,source);m.put(p+16,0);m.put(p+20,ctx['source_size']);m.put(p+24,source);snaps.append(m.snap())
 def read(target,n):
  if n<=0:return
  if ((m.w(p+4)+n)&0xffffffff)>0x7fffffff:return
  remaining=signed(m.w(p+20))
  if n<=remaining:
   m.write(target,m.read(m.w(p+12),n));m.put(p+12,m.w(p+12)+n);m.put(p+20,remaining-n);m.put(p+4,m.w(p+4)+n)
  else:
   if remaining>0:
    read(target,remaining);target+=remaining;n-=remaining
   # Actual memory subtype NeedsBufferedRead false; Unbuffered hook is no-op.
   m.put(p+4,m.w(p+4)+n);m.put(p+20,0);m.put(p+12,0);m.put(p+16,0)
 n=ctx['size']
 if n>0:
  prefix=struct.unpack('<h',m.read(m.w(p+12),2))[0];scratch=A+0x3f00
  read(scratch,2)
  # Scratch above is a model temporary, restore arena bytes rather than claiming a stack write.
  m.write(scratch,initial[0x3f00:0x3f02])
  if prefix==-1:
   prefix=signed(m.w(m.w(p+12)));saved=m.read(scratch,4);read(scratch,4);m.write(scratch,saved)
  if prefix>0:
   position=m.w(p+4);nextpos=(position+prefix)&0xffffffff;compressed=0
   if nextpos<=0x7fffffff:
    delta=(nextpos-m.w(p+4))&0xffffffff;m.put(p+12,m.w(p+12)+delta);m.put(p+20,m.w(p+20)-delta);m.put(p+4,nextpos);compressed=(m.w(p+24)+position)&0xffffffff
   result=lzo_boundary(m,ctx,(compressed,prefix,dest,n,0),events,snaps)
   if result!=((n-3)&0xffffffff) and ctx['error']:
    events.append(('error',));snaps.append(m.snap())
   read(dest+n-3,3)
  else:read(dest,n)
 snaps.append(m.snap());m.put(p,0x1231710);snaps.append(m.snap());return events,snaps
def run(kind,initial,ctx):
 u=machines[kind];u.mem_write(A,initial);u.mem_write(ERRORVAR,word(ERROR if ctx['error'] else 0));m=Memory(u=u);events=[];snaps=[]
 def hook(uc,pc):
  if pc in address_symbols:hits[kind][address_symbols[pc]]+=1
  if pc==STOP:uc.emu_stop();return
  if pc==0x993b90:raise AssertionError('memory subtype must not refill')
  if pc not in (LZO,ERROR):return
  stack=uc.reg_read(UC_X86_REG_ESP)
  if pc==LZO:
   src,count,dest,lengthptr,work=struct.unpack('<5I',uc.mem_read(stack+4,20));length=struct.unpack('<I',uc.mem_read(lengthptr,4))[0]
   result=lzo_boundary(m,ctx,(src,count,dest,length,work),events,snaps);uc.mem_write(lengthptr,word(result))
  else:events.append(('error',));snaps.append(m.snap())
  ret=struct.unpack('<I',uc.mem_read(stack,4))[0];uc.reg_write(UC_X86_REG_EAX,0xffffffff);uc.reg_write(UC_X86_REG_ECX,0xabcdef01);uc.reg_write(UC_X86_REG_EDX,0x98765432);uc.reg_write(UC_X86_REG_ESP,stack+4);uc.reg_write(UC_X86_REG_EIP,ret)
 active[0]=hook
 for address,args in [(CTOR,(ctx['source'],ctx['source_size'])),(ENTRY,(ctx['dest'],ctx['size'])),(DTOR,())]:
  sp=S+0x8000;u.mem_write(S,bytes(65536));u.mem_write(sp,word(STOP)+b''.join(word(a) for a in args));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,ctx['stream']);u.reg_write(UC_X86_REG_EFLAGS,2)
  for i,r in enumerate(registers):u.reg_write(r,0x11223300+i)
  u.emu_start(address,STOP,count=3000)
  assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4+4*len(args)
  assert [u.reg_read(r) for r in registers]==[0x11223300+i for i in range(4)]
  if address==CTOR:assert u.reg_read(UC_X86_REG_EAX)==ctx['stream']
  snaps.append(m.snap())
 return events,snaps
cases=snapshots=0
for fill,place,size,prefix,source_size,mode,error in itertools.product((0,0x95),range(2),(-1,0,1,3,4,7,16),((0,0),(-2,0),(4,0),(-1,-2),(-1,0),(-1,4)),(6,7,8,16,32,64),range(4),range(2)):
 initial=bytearray([fill]*SIZE);base=A+place*0x1800;ctx=dict(stream=base+0x100,source=base+0x400,dest=base+0x800,source_size=source_size,size=size,mode=mode,error=error)
 offset=ctx['source']-A;initial[offset:offset+64]=bytes(range(64));struct.pack_into('<hi',initial,offset,*prefix)
 expected=model(initial,ctx)
 for kind in ('retail','compiled'):assert run(kind,bytes(initial),ctx)==expected,(fill,place,size,prefix,source_size,mode,error,kind)
 cases+=1;snapshots+=len(expected[1])
inputs=set()
def collect(path):
 path=path.resolve()
 if path in inputs:return
 inputs.add(path)
 for include in re.findall(r'^\s*#\s*include\s*"([^"]+)"',path.read_text(errors='replace'),re.M):
  target=next((p for p in (path.parent/include,R/'rebuild/include'/include) if p.exists()),None);assert target is not None,(path,include);collect(target)
collect(Path(__file__))
for path in objects:collect(R/'rebuild/src/compiled/00/99'/path)
for index in range(10):collect(Path(__file__).with_name('NativeMemoryStreamComposition_required%d.cpp'%index))
for name in ('check_one.py','gate_game_component_abi.py','pe_oracle.py','variable_allocator_parity.py','near_match.py','verify_and_land.py','crawl/rowtrim.py','crawl/purity.py'):collect(R/'tools/decomp_pipeline'/name)
collect(R/'tools/artifact_layout.py')
collect(R/'tools/decomp_pipeline/gate_native_memory_stream_composition.py')
assert hits['retail']==hits['compiled'] and all(count>0 for symbol,count in hits['retail'].items() if symbol!=specs[4][1]) and hits['retail'][specs[4][1]]==0,hits
report=dict(cases=cases,snapshots=snapshots,providers=proofs,installed_providers=10,executed_providers=9,provider_hits=hits,omission_checks=omissions,unique_provider_sources=len(objects),input_sha256={str(p.relative_to(R)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sorted(inputs)},original_exe_sha256=hashlib.sha256(EXE.read_bytes()).hexdigest(),memory_vtable_sha256=hashlib.sha256(native_table).hexdigest(),boundary='Actual memory ctor/read/dtor, genuine overflow, GetPosition COMDAT and inherited false/no-op hooks. Source windows >=6 permit complete short/extended prefixes; uninitialized-local reads excluded. NextChunk provider audited but trap-unexecuted. Real emitted 11-slot table audited against PE; unused slots never execute. Full16KiB arena snapshots at lifetime/LZO/error boundaries, external stack excluded, original RET8 and callee-register preservation. LZO callback synthetic output/mutations, not decompression. Error callback/global pointer externally configured, no initializer claim. No buffered-subtype/source-complete/rendering/new-recovery claim.')
out=R/'rebuild/compile-gate/native-memory-stream-composition.json';out.write_text(json.dumps(report,indent=2)+'\n')
print('MEMORY_STREAM_COMPOSITION_PASS',cases,snapshots,flush=True)
