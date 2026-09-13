"""Recover native dialogue operands with bounded CFG/register-constant analysis.

Calls are not executed. Cdecl/thiscall callee-saved registers are preserved;
volatile registers become unknown at calls. Unknown or ambiguous slices fail.
Only functions named by the reviewed entity inventories are traversed.
"""
import argparse
from collections import deque
import hashlib
import json
from pathlib import Path
import re
import struct
import capstone as c
from capstone import x86 as x

ROOT=Path(__file__).resolve().parents[2]
BASE=0x400000
REGS=('eax','ebx','ecx','edx','esi','edi','ebp','esp')
ALIASES={name:(full,shift,bits) for full,parts in {
 'eax':(('eax',0,32),('ax',0,16),('al',0,8),('ah',8,8)),
 'ebx':(('ebx',0,32),('bx',0,16),('bl',0,8),('bh',8,8)),
 'ecx':(('ecx',0,32),('cx',0,16),('cl',0,8),('ch',8,8)),
 'edx':(('edx',0,32),('dx',0,16),('dl',0,8),('dh',8,8)),
 'esi':(('esi',0,32),('si',0,16)), 'edi':(('edi',0,32),('di',0,16)),
 'ebp':(('ebp',0,32),('bp',0,16)), 'esp':(('esp',0,32),('sp',0,16))}.items()
 for name,shift,bits in parts}

def value(ins,operand,state):
 if operand.type==x.X86_OP_IMM:return operand.imm & 0xffffffff
 if operand.type==x.X86_OP_REG:
  name=ins.reg_name(operand.reg)
  if name not in ALIASES:return None
  full,shift,bits=ALIASES[name]
  if full in state:return (state[full]>>shift)&((1<<bits)-1)
 return None

def transfer(ins,state):
 out=dict(state);ops=ins.operands
 for reg in ins.regs_access()[1]:
  name=ins.reg_name(reg)
  if name in ALIASES:out.pop(ALIASES[name][0],None)
 if ins.mnemonic=='call':
  for reg in ('eax','ecx','edx'):out.pop(reg,None)
  return out
 if not ops or ops[0].type!=x.X86_OP_REG:return out
 dest=ins.reg_name(ops[0].reg)
 if dest not in ALIASES:return out
 full,shift,bits=ALIASES[dest];v=None
 if ins.mnemonic in ('mov','movzx') and len(ops)==2:v=value(ins,ops[1],state)
 elif ins.mnemonic=='xor' and len(ops)==2 and ops[1].type==x.X86_OP_REG and ops[0].reg==ops[1].reg:v=0
 elif ins.mnemonic=='lea' and ops[1].type==x.X86_OP_MEM:
  mem=ops[1].mem;base=state.get(ins.reg_name(mem.base),None) if mem.base else 0
  index=state.get(ins.reg_name(mem.index),None) if mem.index else 0
  if base is not None and index is not None:v=base+index*mem.scale+mem.disp
 elif ins.mnemonic in ('inc','dec','add','sub','and','or'):
  old=value(ins,ops[0],state);rhs=value(ins,ops[1],state) if len(ops)>1 else 1
  if old is not None and rhs is not None:
   v={'inc':lambda:old+1,'dec':lambda:old-1,'add':lambda:old+rhs,'sub':lambda:old-rhs,
      'and':lambda:old&rhs,'or':lambda:old|rhs}[ins.mnemonic]()
 if v is not None:
  mask=(1<<bits)-1
  if bits==32:out[full]=v&mask
  elif full in state:out[full]=(state[full]&~(mask<<shift))|((v&mask)<<shift)
 return out

def recover(pe,cs,start):
 pending=[start];instructions={};edges={};switches=[]
 while pending:
  a=pending.pop()
  if a in instructions:continue
  if not start<=a<start+0x10000:raise ValueError(f'Out-of-bound branch {a:x}')
  ins=next(cs.disasm(pe.get_data(a-BASE,15),a));instructions[a]=ins;targets=[]
  if ins.group(c.CS_GRP_JUMP):
   if ins.operands[0].type==x.X86_OP_IMM:targets.append(ins.operands[0].imm)
   elif a==0xDB5526:
    # No inferred table length: DB551D cmp eax,3 / unsigned ja default.
    assert pe.get_data(0xDB551D-BASE,9).hex()=='83f8030f87c8080000'
    assert ins.op_str=='dword ptr [eax*4 + 0xdb6b24]'
    targets=list(struct.unpack('<4I',pe.get_data(0xDB6B24-BASE,16)))
    switches.append({'address':hex(a),'table':hex(0xDB6B24),'targets':list(map(hex,targets))})
   else:raise ValueError(f'Unreviewed indirect branch {a:x}')
   if ins.mnemonic!='jmp':targets.append(a+ins.size)
  elif not ins.group(c.CS_GRP_RET):targets.append(a+ins.size)
  edges[a]=targets;pending.extend(targets)
 # Known constants must agree on every predecessor. Function entry is unknown.
 states={start:{}};queue=deque([start]);iterations=0
 while queue:
  a=queue.popleft();iterations+=1
  if iterations>1000000:raise ValueError('Constant analysis did not converge')
  out=transfer(instructions[a],states[a])
  for target in edges[a]:
   previous=states.get(target)
   joined=out if previous is None else {k:v for k,v in previous.items() if out.get(k)==v}
   if previous!=joined:states[target]=dict(joined);queue.append(target)
 return instructions,edges,states,switches

def audit(exe):
 import pefile
 digest=hashlib.sha256(exe.read_bytes()).hexdigest()
 if digest!='41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10':raise ValueError('Unexpected retail executable')
 pe=pefile.PE(str(exe));cs=c.Cs(c.CS_ARCH_X86,c.CS_MODE_32);cs.detail=True
 inventory=json.loads((ROOT/'refs/script_recovery/new_oakvale_intro/runtime_evidence/speech-callsite-inventory-20260912.json').read_text())
 records=[];functions=[]
 for site in inventory['routes']['Speak']:
  name=Path(site['file'].replace('\\','/')).stem
  metadata=json.loads((ROOT/f'refs/script_recovery/new_oakvale_intro/entities/{name}.json').read_text())
  for fn,meta in metadata['functions'].items():
   if fn=='QuestInit(vectors)':continue
   start=int(meta['address'],16);instructions,edges,states,switches=recover(pe,cs,start)
   functions.append({'entity':name,'name':fn,'entry':hex(start),'instruction_count':len(instructions),'switches':switches})
   sequence=sorted(instructions.values(),key=lambda i:i.address)
   for k,ins in enumerate(sequence):
    route='Speak' if ins.mnemonic=='call' and ins.op_str=='0x7e7390' else (
      'AddNewConversation' if ins.mnemonic=='call' and ins.operands[0].type==x.X86_OP_MEM and ins.operands[0].mem.disp==0x5b0 else None)
    if route is None:continue
    argc=6 if route=='Speak' else 3
    pushes=[i for i in sequence[max(0,k-30):k] if i.mnemonic=='push'][-argc:]
    if len(pushes)!=argc:raise ValueError(f'Incomplete args {ins.address:x}')
    begin=pushes[0].address;slice_ins=[i for i in sequence if begin<=i.address<ins.address]
    for i in slice_ins:
     if edges[i.address]!=[i.address+i.size]:raise ValueError(f'Branch in argument slice {ins.address:x}')
     if i.mnemonic=='call' and not (route=='Speak' and i.operands[0].type==x.X86_OP_MEM and i.operands[0].mem.disp==0x118):
      raise ValueError(f'Unreviewed nested argument call {i.address:x}')
    # Exclude alternate entry points into the middle of an argument slice.
    if any(begin<t<=ins.address for a,ts in edges.items() if not begin<=a<ins.address for t in ts):
     raise ValueError(f'Alternate entry into argument slice {ins.address:x}')
    values=[value(i,i.operands[0],states[i.address]) for i in pushes]
    operand_count=4 if route=='Speak' else 2
    if any(v is None for v in values[:operand_count]):raise ValueError(f'Unknown scalar operand {name}/{ins.address:x}: {values}')
    record={'entity':name,'function':fn,'route':route,'call':hex(ins.address),'slice_start':hex(begin),
            'bytes':pe.get_data(begin-BASE,ins.address+ins.size-begin).hex(),
            'pushes':[{'address':hex(i.address),'operand':i.op_str,'constant':v} for i,v in zip(pushes,values)]}
    if route=='Speak':
     nested=[i for i in slice_ins if i.mnemonic=='call']
     if len(nested)!=1 or pushes[-1].op_str!='eax' or nested[0].address+nested[0].size!=pushes[-1].address:
      raise ValueError(f'Unreviewed Hero argument forwarding {ins.address:x}')
     pointer=values[4]
     if pointer is None:raise ValueError('Nonconstant text key')
     record.update(text=pe.get_data(pointer-BASE,180).split(b'\0')[0].decode('ascii'),
                   method=values[3],listen=values[2],sound2d=values[1],over_fade=values[0])
     if values[:3]!=[0,1,0]:raise ValueError(f'Unexpected native speech flags {record}')
    else:
     record['booleans']=list(reversed(values[:2]))
     if record['booleans']!=[0,0]:raise ValueError(f'Unexpected conversation flags {record}')
    records.append(record)
 return {'scope':__doc__,'exe_sha256':digest,
         'speak_thunk':{'address':'0x7e7390','bytes':pe.get_data(0x7E7390-BASE,16).hex(),
                        'contract':'six stack arguments (ret 0x18), forwards to resource vtable +0x34'},
         'functions':functions,'calls':records,
         'counts':{route:sum(r['route']==route for r in records) for route in ('Speak','AddNewConversation')}}

if __name__=='__main__':
 parser=argparse.ArgumentParser(description=__doc__)
 parser.add_argument('--exe',type=Path,default=Path('C:/Programs/Steam/steamapps/common/Fable The Lost Chapters/Fable.exe'))
 parser.add_argument('--output',type=Path,required=True)
 args=parser.parse_args()
 if args.output.exists():raise SystemExit('Output exists')
 report=audit(args.exe);args.output.write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps(report['counts']))
