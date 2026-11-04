"""Recover a byte-proven nullable Thing-data snapshot followed by hero event dispatch."""
import re,struct
from capstone import Cs,CS_ARCH_X86,CS_MODE_32

PATTERN=re.compile(r'''if\s*\(\s*!__thing_valid\((?P<data>\w+)\)\s*\)\s*\{\s*
 (?P<extra>\w+)\s*=\s*\(void\s*\*\)\s*0x0\s*;\s*\}\s*else\s*\{\s*
 (?P=extra)\s*=\s*\(void\s*\*\)\s*CScriptThing::GetPThing\((?P=data)\)\s*;\s*\}\s*
 \w+\s*=\s*\*\*\(int\s*\*\*\)\(this\s*\+\s*0x40\)\s*;\s*
 (?P<receiver>\w+)\s*=\s*\(CScriptThing\s*\*\)\s*(?P=data)\s*;\s*
 (?P<sender>\w+)\s*=\s*GSI->GetHero\(\)\s*;\s*
 GSI->SendEntityEvent\((?P<event>0x[0-9a-f]+|\d+)\s*,\s*(?P=sender)\s*,\s*(?P=receiver)\s*,\s*(?P=extra)\)\s*;''',re.X)

def restore(text,fn,rdata):
 if not fn or rdata is None:return text
 def event_slot(value):
  try:return int(value,0)==0x3c4 if isinstance(value,str)else value==0x3c4
  except ValueError:return False
 sites=[c for c in fn.get('indirectCalls',[])if event_slot(c.get('slot'))]
 matches=list(PATTERN.finditer(text))
 # Bound to one witnessed event/source shape per function; an ambiguous second
 # call or an unpaired source occurrence must remain unresolved.
 if len(sites)!=1 or len(matches)!=1 or text.count('GSI->SendEntityEvent(')!=1:return text
 site=sites[0];address=int(site['site'],16);before=rdata.bytes_at(address-32,32)or b'';call=rdata.bytes_at(address,6)or b''
 if len(before)!=32 or call!=b'\xff\x97\xc4\x03\x00\x00':return text
 if before[:18]!=bytes.fromhex('8b01ff502c8b4e408bd18b3a8b11508d4424')or before[19:31]!=bytes.fromhex('50ff92180100008b4e40506a'):return text
 event=struct.unpack('b',before[31:])[0];m=matches[0]
 if int(m['event'],0)!=event:return text
 if len({m[n]for n in ('data','extra','receiver','sender')})!=4:return text
 # The coupled operation consumes these decompiler temporaries. A later read
 # would need separately proven materialization rather than removing it here.
 if any(re.search(r'\b'+re.escape(m[n])+r'\b',text[m.end():])for n in ('extra','receiver','sender')):return text
 depth=site.get('depth')
 if not isinstance(depth,int):return text
 initial_depth=depth-16;receiver_stack=before[18]-4-initial_depth
 suffix=re.search(r'(?:Stack_|local_)([0-9a-f]+)$',m['data'])
 if suffix is None or int(suffix[1],16)!=-receiver_stack:return text
 by_site={int(c['site'],16):c for c in fn.get('indirectCalls',[])}
 getter=by_site.get(address-30,{});hero=by_site.get(address-12,{})
 if getter.get('slot')!='0x2c'or getter.get('depth')!=initial_depth or getter.get('ecxValue')!=receiver_stack+4:return text
 if hero.get('slot')!='0x118'or hero.get('depth')!=initial_depth+8 or hero.get('pushedStack')!=[None,receiver_stack]:return text
 start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16);raw=rdata.bytes_at(start,end-start)
 if raw is None:return text
 instructions=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(raw,start));guards=[]
 for instruction in instructions:
  if instruction.mnemonic!='jne' or instruction.op_str!=hex(address-32):continue
  guard_start=instruction.address-6;guard=rdata.bytes_at(guard_start,12)or b''
  if len(guard)!=12 or guard[:3]!=b'\x8b\x4c\x24'or guard[4:7]!=b'\x85\xc9\x75'or guard[8:11]!=b'\x33\xc0\xeb':continue
  if guard_start+8+struct.unpack('b',guard[7:8])[0]!=address-32 or guard_start+12+struct.unpack('b',guard[11:12])[0]!=address-27:continue
  if guard[3]-initial_depth!=receiver_stack+4:continue
  guards.append(guard_start)
 if len(guards)!=1:return text
 replacement=f'ENGINE_SendEntityEventFromHeroWithThingData({event}, {m["data"]}, {m["data"]});'
 return text[:m.start()]+replacement+text[m.end():]
