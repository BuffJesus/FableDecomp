"""Bounded native AddLine scalar/argument-slice inventory; no engine execution."""
import argparse
import hashlib
import json
from pathlib import Path
import sys
import capstone as c
from capstone import x86 as x
import pefile

ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT))
from tools.script_recovery.audit_new_oakvale_speech_operands import recover,value,BASE

def audit(exe):
 digest=hashlib.sha256(exe.read_bytes()).hexdigest()
 if digest!='41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10':raise ValueError('Unexpected executable')
 pe=pefile.PE(str(exe));cs=c.Cs(c.CS_ARCH_X86,c.CS_MODE_32);cs.detail=True
 inventory=json.loads((ROOT/'refs/script_recovery/new_oakvale_intro/runtime_evidence/speech-callsite-inventory-20260912.json').read_text())
 records=[]
 for site in inventory['routes']['Speak']:
  name=Path(site['file'].replace('\\','/')).stem
  meta=json.loads((ROOT/f'refs/script_recovery/new_oakvale_intro/entities/{name}.json').read_text())
  for fn,details in meta['functions'].items():
   if fn=='QuestInit(vectors)':continue
   ins,edges,states,_=recover(pe,cs,int(details['address'],16));seq=sorted(ins.values(),key=lambda i:i.address)
   for n,call in enumerate(seq):
    if call.mnemonic!='call' or call.operands[0].type!=x.X86_OP_MEM or call.operands[0].mem.disp!=0x5b8:continue
    pushes=[];discard=0
    for i in reversed(seq[max(0,n-55):n]):
     if i.mnemonic=='call':
      if i.op_str=='0x99ebf0':discard+=2
      elif i.op_str=='0x99f570':discard+=1
      else:raise ValueError(f'Unreviewed nested call {i.address:x}')
     elif i.mnemonic=='push':
      if discard:discard-=1
      else:pushes.append(i)
      if len(pushes)==5:break
     elif i.mnemonic in ('pop','pushal','popal') or (i.mnemonic in ('add','sub') and i.op_str.startswith('esp,')):
      raise ValueError('Unreviewed stack adjustment')
    if len(pushes)!=5 or discard:raise ValueError('Incomplete argument slice')
    pushes.reverse();begin=pushes[0].address
    fragment=[i for i in seq if begin<=i.address<call.address]
    for i in fragment:
     if edges[i.address]!=[i.address+i.size]:raise ValueError('Branch in argument slice')
    alternate=[(a,t) for a,ts in edges.items() if not begin<=a<call.address for t in ts if begin<t<=call.address]
    prefixes=[]
    if alternate:
     # Affair Man swaps the two participants and text after the first line.
     # Both prefixes push two participant arguments before the common scalar/key/id.
     if call.address!=0xDB19D8 or alternate!=[(0xDB199D,0xDB19CF)]:
      raise ValueError(f'Unreviewed alternate argument entry {name}/{call.address:x}')
     assert [i.address for i in pushes[2:]]==[0xDB19D4,0xDB19D6,0xDB19D7]
     for lo,hi in [(0xDB1993,0xDB199F),(0xDB19C5,0xDB19CF)]:
      body=[i for i in seq if lo<=i.address<hi]
      assert sum(i.mnemonic=='push' for i in body)==2
      prefixes.append({'entry':hex(lo),'bytes':pe.get_data(lo-BASE,hi-lo).hex(),
                       'join':'0xdb19cf','pushes':[i.op_str for i in body if i.mnemonic=='push']})
    flag=value(pushes[2],pushes[2].operands[0],states[pushes[2].address])
    if flag!=0:raise ValueError(f'Unexpected/unknown line boolean {call.address:x}: {flag}')
    records.append({'entity':name,'function':fn,'call':hex(call.address),'line_boolean':flag,
      'push_order':['listener','speaker','line_boolean','text','conversation_id'],
      'pushes':[{'address':hex(i.address),'operand':i.op_str} for i in pushes],
      'slice_start':hex(begin),'bytes':pe.get_data(begin-BASE,call.address+call.size-begin).hex(),'alternate_prefixes':prefixes,
      'context':[f'{i.address:08X} {i.mnemonic} {i.op_str}' for i in seq[max(0,n-38):n+1]]})
 return {'scope':__doc__,'exe_sha256':digest,'calls':records,'count':len(records),
         'limits':'Scalar false is proven; participant roles require reviewed bound/lookup aliases, not text-prefix inference.'}

if __name__=='__main__':
 parser=argparse.ArgumentParser(description=__doc__)
 parser.add_argument('--exe',type=Path,default=Path('C:/Programs/Steam/steamapps/common/Fable The Lost Chapters/Fable.exe'))
 parser.add_argument('--output',type=Path,required=True)
 args=parser.parse_args()
 if args.output.exists():raise SystemExit('Output exists')
 report=audit(args.exe);args.output.write_text(json.dumps(report,indent=2)+'\n')
 print(f"{report['count']} native conversation-line calls have false scalar operand")
