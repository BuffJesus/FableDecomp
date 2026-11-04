"""Shared fresh-compile parity check for canonical roots that are exact after relocation.

Each root is compiled from rebuild/src with canonical rebuild/include only. Every
relocation must name a symbol in the caller's independently pinned ADDRESS map, the
retail displacement/absolute at that site must resolve to the same address, and the
patched root must equal retail byte for byte. Input hashes must not drift.
"""
import hashlib,json,re,struct,sys,tempfile
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
FLAGS=['/nologo','/c','/O2','/MT','/W3']
def import_names(raw):
 """IAT slot VA -> imported name, from the retail PE import directory."""
 img=0x400000;e=struct.unpack_from('<I',raw,0x3c)[0]
 def d(va):return struct.unpack('<I',read_virtual_data(raw,va,4))[0]
 out={};a=img+struct.unpack_from('<I',raw,e+24+104)[0]
 while d(a+12):
  oft,ft=d(a),d(a+16);i=0
  while d(img+(oft or ft)+4*i):
   t=d(img+(oft or ft)+4*i)
   if not t&0x80000000:out[img+ft+4*i]=read_virtual_data(raw,img+t+2,64).split(b'\0')[0].decode()
   i+=1
  a+=20
 return out
def run(gate_file,roots,address,out_name,behavior_evidence,boundary):
 inputs={Path(gate_file),Path(__file__)}
 for name in ('check_one.py','gate_game_component_abi.py','pe_oracle.py'):inputs.add(R/'tools/decomp_pipeline'/name)
 def closure(p):
  if p in inputs:return
  inputs.add(p)
  for name in re.findall(r'^\s*#include\s*"([^"]+)"',p.read_text(encoding='utf-8',errors='replace'),re.M):
   q=next((q for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file()),None);assert q,name
   closure(q)
 for root in roots:closure(root['source'])
 def hashes():return {str(p.relative_to(R)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sorted(inputs)}
 before=hashes();env=V.env();build=Path(tempfile.mkdtemp(prefix='native-exact-roots-'))
 raw=EXE.read_bytes();md=Cs(CS_ARCH_X86,CS_MODE_32);results=[];imports=None
 for root in roots:
  obj=build/(root['name']+'.obj')
  # Optional per-root flags (e.g. /GS for retail stack-cookie TUs); default FLAGS.
  r=V.cl(root.get('flags',FLAGS)+['/I'+str(R/'rebuild/include'),'/Fo'+str(obj),str(root['source'])],env,build)
  assert r.returncode==0,r.stdout+r.stderr
  c=Coff(obj)
  # Anonymous-namespace roots carry a per-unit hash (?A0x...@); a root may name its symbol by full-match pattern.
  if root.get('symbol_pattern'):
   found=[s for s in c.symbols if re.fullmatch(root['symbol_pattern'],s)];assert len(found)==1,(root['name'],found);root['symbol']=found[0]
  sec,off=c.symbols[root['symbol']];assert off==0
  built,rels=c.sections[sec];assert len(built)==root['size'] and len(rels)==root['relocations'],(root['name'],len(built),len(rels))
  original=read_virtual_data(raw,root['address'],root['size'])
  patched=bytearray(built);audit=[]
  for site,(name,kind) in sorted(rels.items()):
   assert kind in (6,20),(site,name)
   # The object stores the addend in place (a field of a global: ErrorArray+4).
   addend=struct.unpack_from('<i',built,site)[0]
   # Switch jump tables point at compiler-numbered local labels ($Lnnnn) in the
   # root's own section; the numbering moves whenever a header changes, so
   # resolve them by their offset in the section instead of by name.
   if name.startswith('$L') and name in c.symbols and c.symbols[name][0]==sec:
    target=root['address']+c.symbols[name][1]
   else:
    target=address[name]
   if kind==20:
    assert (root['address']+site+4+struct.unpack_from('<i',original,site)[0])&0xffffffff==(target+addend)&0xffffffff,(site,name)
    struct.pack_into('<I',patched,site,(target+addend-(root['address']+site+4))&0xffffffff)
   else:
    assert struct.unpack_from('<I',original,site)[0]==(target+addend)&0xffffffff,(site,name)
    struct.pack_into('<I',patched,site,(target+addend)&0xffffffff)
   audit.append(dict(site=site,symbol=name,kind=kind,target=hex(target),**({'addend':addend} if addend else {})))
  assert bytes(patched)==original,root['name']
  # Import slots are pinned by address only above; require the IAT entry's name too.
  for name in sorted(set(n for n,k in rels.values())):
   if name.startswith('__imp__'):
    if imports is None:imports=import_names(raw)
    assert imports.get(address[name])==name[7:].split('@')[0],('import',name,imports.get(address[name]))
   elif name not in address:
    # Only the root's own local labels ($Lnnnn, checked by offset above) lack a mapping.
    assert name.startswith('$L'),name
   else:
    # A direct call into a retail import thunk (jmp [IAT]) must name that import:
    # malloc/free and operator new[]/delete[] (??_U/??_V) are separate thunks, so
    # a wrong CRT allocator would otherwise pass by address alone.
    thunk=read_virtual_data(raw,address[name],6)
    if thunk[:2]==b'\xff\x25':
     if imports is None:imports=import_names(raw)
     imported=imports.get(struct.unpack_from('<I',thunk,2)[0])
     expected=name if name.startswith('?') else name[1:].split('@')[0]
     assert imported==expected,('thunk',name,imported)
  # String literals are pinned by address only above; also require their bytes to match
  # (a mis-escaped literal keeps its relocation but changes its content).
  for name in sorted(set(n for n,k in rels.values())):
   if name.startswith('??_C@') and name in c.symbols:
    lsec,loff=c.symbols[name];ldata=c.sections[lsec][0][loff:]
    lend=(next(i for i in range(0,len(ldata),2) if ldata[i:i+2]==b'\0\0')+2) if name.startswith('??_C@_1') else ldata.index(b'\0')+1
    assert read_virtual_data(raw,address[name],lend)==ldata[:lend],('literal',name)
  # An embedded jump table (switch) follows the code; disassemble only the code.
  code_size=root.get('code_size',root['size'])
  ins=list(md.disasm(original[:code_size],root['address']));assert len(ins)==root['instructions'] and sum(i.size for i in ins)==code_size
  # Root section only; the whole-object hash carries a timestamp.
  results.append(dict(flags=root.get('flags',FLAGS),name=root['name'],symbol=root['symbol'],address=hex(root['address']),grade='MATCH',root_bytes=root['size'],root_instructions=root['instructions'],relocations=audit,root_section_sha256=hashlib.sha256(bytes(built)).hexdigest()))
 after=hashes();assert before==after
 report=dict(status='PASS',roots=results,compiler_flags=FLAGS,input_sha256=after,original_exe_sha256=hashlib.sha256(raw).hexdigest(),behavior_evidence=behavior_evidence,boundary=boundary)
 (R/'rebuild/compile-gate'/out_name).write_text(json.dumps(report,indent=2)+'\n')
 return results
