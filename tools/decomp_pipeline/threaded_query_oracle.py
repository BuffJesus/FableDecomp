"""Const UID lookup and query snapshots across lock-entry/exit mutations."""
import hashlib,itertools,re,struct,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from capstone.x86_const import X86_OP_IMM,X86_OP_MEM
from unicorn import Uc,UcError,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_INVALID
from unicorn.x86_const import *
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from movable_pool_construct_oracle import compile_codes
from native_exact_roots import import_names
from near_match import compare
R=Path(__file__).resolve().parents[2]
SOURCE=R/'rebuild/src/compiled/00/98/CThreadedFileManager_Queries_0098e590.cpp'
FIND_SOURCE=R/'rebuild/src/compiled/00/99/STLThreadedUIDTree_ConstFind_00990320.cpp'
TREE='?$_Rb_tree@KU?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@U?$_Select1st@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@2@@_STL@@'
FIND='?find@'+TREE+'QBE?AU?$_Rb_tree_const_iterator@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@2@ABK@Z'
ROOTS=[dict(name=name,source=source,symbol=symbol,address=address,original_size=size,grade=grade) for name,source,symbol,address,size,grade in (
 ('const_find',FIND_SOURCE,FIND,0x990320,71,'MATCH'),
 ('manager_finished',SOURCE,'?IsObjectFinished@CThreadedFileManager@@QBE_NK@Z',0x98e590,95,'MATCH'),
 ('manager_exists',SOURCE,'?ObjectExists@CThreadedFileManager@@QBE_NK@Z',0x98e5f0,61,'MATCH'),
 ('file_finished',SOURCE,'?IsObjectFinished@CThreadedFile@@QBE_NK@Z',0x98e630,10,'MATCH'),
 ('file_exists',SOURCE,'?ObjectExists@CThreadedFile@@QBE_NK@Z',0x98e640,70,'MATCH'))]
RUN_ADDRESS={r['address']:0x6000000+index*0x100 for index,r in enumerate(ROOTS)}
GLOBAL=0x13bc870
ADDRESS={FIND:0x990320,'?IsObjectFinished@CThreadedFileManager@@QBE_NK@Z':0x98e590,
 '?g_FableThreadedFileManager_013BC870@@3VCThreadedFileManager@@A':GLOBAL,
 '__imp__EnterCriticalSection@4':0x143fe0c,'__imp__LeaveCriticalSection@4':0x143fe10}
ARENA,LENGTH,LOCAL,HEADER,EMPTY,NODES,OBJECTS,OUT,KEY=0x2000000,0x10000,0x2000100,0x2001000,0x2001100,0x2002000,0x2004000,0x2009000,0x2009020
STACK,STOP,API=0x3000000,0x4000000,0x5000000
INITIAL={s:bytes((i*37+s*19)&255 for i in range(LENGTH)) for s in (0,7)}
GINITIAL={s:bytes((i*17+s*23)&255 for i in range(0x1000)) for s in (0,7)}

def keys_for(count):
 return [0x80000000] if count==1 else [i*0xffffffff//(count-1) for i in range(count)] if count else []

def audit_bodies(build):
 addresses=dict(ADDRESS);addresses.update({r['symbol']:r['address'] for r in ROOTS})
 codes=compile_codes(ROOTS,addresses,build);raw=EXE.read_bytes();imports=import_names(raw);reports=[]
 md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
 for root in ROOTS:
  body=codes[root['address']];original=read_virtual_data(raw,root['address'],root['original_size'])
  grade=compare(body,original).to_dict();assert grade['near_match'],(root['name'],grade)
  if root['grade']=='MATCH':assert body==original
  # Independently decoded external call/IAT identity, excluding intra-function jumps.
  def calls(code):
   result=set()
   for i in md.disasm(code,root['address']):
    if i.mnemonic not in ('call','jmp'):continue
    op=i.operands[0]
    if op.type==X86_OP_IMM and not root['address']<=op.imm<root['address']+len(code):result.add(('direct',op.imm))
    if op.type==X86_OP_MEM and not op.mem.base and not op.mem.index:
     result.add(('import',op.mem.disp));assert imports[op.mem.disp] in ('EnterCriticalSection','LeaveCriticalSection')
   return result
  assert calls(body)==calls(original),(root['name'],calls(body),calls(original))
  reports.append(dict(name=root['name'],symbol=root['symbol'],address=hex(root['address']),bytes=len(body),original_bytes=len(original),acceptance_grade=root['grade'],grade=grade))
 runtime=build/'runtime';runtime.mkdir()
 runtime_roots=[dict(root,address=RUN_ADDRESS[root['address']]) for root in ROOTS]
 runtime_targets=dict(ADDRESS);runtime_targets.update({r['symbol']:RUN_ADDRESS[r['address']] for r in ROOTS})
 return compile_codes(runtime_roots,runtime_targets,runtime),reports

def execute(codes,root,args):
 count,shape,key,status,enter,leave,depth0,null_value,seed=args
 receiver=GLOBAL if root['name'].startswith('file_') else LOCAL
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in ((0x6000000,0x1000),(0x98e000,0x1000),(0x990000,0x1000),(0x13bc000,0x1000),(0x143f000,0x1000),(ARENA,LENGTH),(STACK,0x10000),(STOP,0x1000),(API,0x1000)):
  u.mem_map(a,n);u.mem_write(a,b'\xcc'*n)
 for a,b in codes.items():u.mem_write(a,b)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 u.mem_write(ARENA,INITIAL[seed]);u.mem_write(0x13bc000,GINITIAL[seed])
 keys=keys_for(count);objects={};nodes={}
 for i,k in enumerate(keys):
  n=NODES+i*24;o=OBJECTS+i*64;nodes[k]=n;objects[k]=o
  put(n,1);put(n+16,k);put(n+20,0 if null_value else o)
  put(o+28,i%6 if status=='mixed' else status)
 def build(indices,parent):
  if not indices:return 0
  mid=0 if shape=='right' else len(indices)-1 if shape=='left' else len(indices)//2
  index=indices[mid];n=NODES+index*24;put(n+4,parent)
  put(n+8,build(indices[:mid],n));put(n+12,build(indices[mid+1:],n));return n
 put(HEADER,0);put(HEADER+4,build(list(range(count)),HEADER));put(HEADER+8,NODES if count else HEADER);put(HEADER+12,NODES+(count-1)*24 if count else HEADER)
 put(EMPTY,0);put(EMPTY+4,0);put(EMPTY+8,EMPTY);put(EMPTY+12,EMPTY)
 for obj in (LOCAL,GLOBAL):put(obj+0x13c,HEADER);put(obj+0x140,count)
 put(KEY,key);u.mem_write(OUT,b'\xcd'*16)
 before=bytes(u.mem_read(ARENA,LENGTH));gbefore=bytes(u.mem_read(0x13bc000,0x1000));expected=bytearray(before);gexpected=bytearray(gbefore)
 def expect(a,b):
  if ARENA<=a<ARENA+LENGTH:expected[a-ARENA:a-ARENA+len(b)]=b
  else:gexpected[a-0x13bc000:a-0x13bc000+len(b)]=b
 def ep(a,v):expect(a,struct.pack('<I',v&0xffffffff))
 is_find=root['name']=='const_find'
 effective_hit=key in nodes and (is_find or not enter)
 expected_finished=not effective_hit or (not null_value and (keys.index(key)%6 if status=='mixed' else status)==4)
 expected_bool=expected_finished if root['name'].endswith('finished') else effective_hit
 expected_fault=bool(not is_find and root['name'].endswith('finished') and effective_hit and null_value)
 trace=[];depth=depth0;seen=set()
 for slot,api in ((0x143fe0c,API),(0x143fe10,API+0x20)):put(slot,api);u.mem_write(api,b'\xc3')
 def hook(uc,a,size,_):
  nonlocal depth
  seen.add(a)
  if a not in (API,API+0x20):return
  sp=uc.reg_read(UC_X86_REG_ESP);assert get(sp+4)==receiver+0x14c
  if a==API:
   assert depth==depth0;depth+=1;trace.append(('enter',depth))
   if enter:
    put(receiver+0x13c,EMPTY);put(receiver+0x140,0);ep(receiver+0x13c,EMPTY);ep(receiver+0x140,0)
  else:
   assert depth==depth0+1;depth-=1;trace.append(('leave',depth))
   if leave==1 and effective_hit and not null_value:
    o=objects[key];value=0 if get(o+28)==4 else 4;put(o+28,value);ep(o+28,value)
   elif leave==2:
    put(receiver+0x13c,EMPTY);put(receiver+0x140,0);ep(receiver+0x13c,EMPTY);ep(receiver+0x140,0)
   elif leave==3 and effective_hit and not null_value:
    o=objects[key];u.mem_write(o,b'\xdd'*64);expect(o,b'\xdd'*64)
  uc.reg_write(UC_X86_REG_EAX,0xdeadbeef);uc.reg_write(UC_X86_REG_ECX,0x87654321);uc.reg_write(UC_X86_REG_EDX,0xaabbccdd)
  uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+8)
 u.hook_add(UC_HOOK_CODE,hook)
 sp=STACK+0x8000;put(sp,STOP)
 if is_find:put(sp+4,OUT);put(sp+8,KEY);this=LOCAL+0x13c
 else:put(sp+4,key);this=0xdeadbeef if root['name'].startswith('file_') else receiver
 saved=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
 for i,reg in enumerate(saved):u.reg_write(reg,0x55660000+seed+i)
 u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,this)
 fault=False;invalid=[]
 def bad_memory(uc,access,address,size,value,_):invalid.append((address,size));return False
 u.hook_add(UC_HOOK_MEM_INVALID,bad_memory)
 entry=RUN_ADDRESS[root['address']] if any(a>=0x6000000 for a in codes) else root['address']
 try:u.emu_start(entry,STOP,count=4096)
 except UcError:
  assert expected_fault,(root['name'],args);fault=True
 assert fault==expected_fault
 if fault:
  assert depth==depth0+1 and trace==[('enter',depth0+1)] and invalid==[(0x1c,4)],invalid
 else:
  assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+(12 if is_find else 8)
  assert all(u.reg_read(reg)==0x55660000+seed+i for i,reg in enumerate(saved))
  assert depth==depth0 and trace==([] if is_find else [('enter',depth0+1),('leave',depth0)])
  if is_find:
   found=nodes.get(key,HEADER);assert get(OUT)==found and u.reg_read(UC_X86_REG_EAX)==OUT
   ep(OUT,found)
  else:assert (u.reg_read(UC_X86_REG_EAX)&255)==int(expected_bool),(root['name'],args)
 state=bytes(u.mem_read(ARENA,LENGTH));gstate=bytes(u.mem_read(0x13bc000,0x1000))
 assert state==bytes(expected) and gstate==bytes(gexpected)
 return (state,gstate,trace,fault),seen

def compare_queries():
 inputs={Path(__file__),EXE,SOURCE,FIND_SOURCE}
 for name in ('check_one.py','gate_game_component_abi.py','pe_oracle.py','movable_pool_construct_oracle.py','native_exact_roots.py','near_match.py'):
  inputs.add(R/'tools/decomp_pipeline'/name)
 def closure(p):
  for name in re.findall(r'^\s*#include\s*"([^"]+)"',p.read_text(encoding='utf-8'),re.M):
   q=R/'rebuild/include'/name;assert q.is_file(),name
   if q not in inputs:inputs.add(q);closure(q)
 closure(SOURCE);closure(FIND_SOURCE);pins={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
 with tempfile.TemporaryDirectory(prefix='threaded-queries-') as tmp:codes,grades=audit_bodies(Path(tmp))
 raw=EXE.read_bytes();native={r['address']:read_virtual_data(raw,r['address'],r['original_size']) for r in ROOTS}
 cases=faults=0;witnesses={};partitions={}
 for root in ROOTS:
  matrix=[]
  for count,shape,status,seed in itertools.product((0,1,3,7,15),('balanced','left','right'),(0,1,2,3,4,0xffffffff,'mixed'),(0,7)):
   keys=keys_for(count);probes=sorted(set([0,1,0x80000000,0xffffffff]+[keys[i] for i in set((0,len(keys)//2,len(keys)-1)) if 0<=i<len(keys)]))
   for key in probes:matrix.append((count,shape,key,status,0,0,0,False,seed))
  if root['name']!='const_find':
   for count,hit,status,enter,leave,depth0,null in itertools.product((3,7),(False,True),(0,4,'mixed'),range(2),range(4),range(2),(False,True)):
    key=keys_for(count)[count//2] if hit else 17
    matrix.append((count,'balanced',key,status,enter,leave,depth0,null,7))
  for args in matrix:
   try:a,seen=execute(codes,root,args);b,_=execute(native,root,args);assert a==b
   except Exception as e:raise AssertionError((root['name'],args)) from e
   cases+=1;faults+=a[-1]
   for provider in ROOTS:
    if RUN_ADDRESS[provider['address']] in seen:witnesses.setdefault(provider['address'],(root,args))
  partitions[root['name']]=len(matrix);print('queries',root['name'],cases,flush=True)
 for provider in ROOTS:
  caller,args=witnesses[provider['address']];detected=False
  try:execute({a:b for a,b in codes.items() if a!=RUN_ADDRESS[provider['address']]},caller,args)
  except Exception:detected=True
  assert detected,provider['name']
 assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==h for p,h in pins.items())
 return dict(status='PASS',cases=cases,intentional_null_value_faults=faults,partitions=partitions,providers=len(ROOTS),missing_provider_checks=len(ROOTS),roots=grades,input_sha256=pins,execution_addresses={hex(k):hex(v) for k,v in RUN_ADDRESS.items()},
  boundaries=['Actual const lookup, manager queries and global file-wrapper paths; Win32 lock effects controlled, no concurrency here.',
   'Missing UID is finished; mapped null still exists and finished query intentionally faults. Unsigned boundary keys and three ordered tree shapes.',
   'Const lookup changes only its four-byte hidden result, no tree/request writes. Entire 64KiB arena/global page, guard bytes, ABI and callee-saved registers checked.',
   'Entry can swap the tree; exit can alter status, replace header or poison request. Boolean/iterator snapshots and recursive-lock balance checked. File receiver intentionally invalid to prove ignored.',
   'Rebuilt functions execute at separate relocated slots: larger near bodies may overlap a neighbor at retail positions. Original bodies stay at original addresses; data/IAT identities unchanged.'])
