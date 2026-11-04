"""Recover witnessed quest entry condition registration, never Lua polling."""
import hashlib,re,struct
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
PATTERN=re.compile(r'(?P<actor>\w+)\s*=\s*GSI->GetHero\(\);\s*CScriptThing::CScriptThing\((?P<temporary>\w+),\s*(?P=actor),\s*\w+\);\s*std::_Cons_val[^;]+__atcb7920\(this\);(?=\s*GSI->NewScriptFrame\(\);)')
ALIAS_PATTERN=re.compile(r'(?P<actor>\w+)\s*=\s*GSI->GetHero\(\);\s*(?P<temporary>\w+)\s*=\s*&PTR_LAB_012c2fe8;\s*CScriptThing::CScriptThing\((?P=temporary),\s*(?P=actor),\s*\w+\);\s*std::_Cons_val[^;]+__atcb7920\(this\);\s*(?P=temporary)\s*=\s*&PTR_LAB_012c2fe8;(?=\s*GSI->NewScriptFrame\(\);)')
CAST_PATTERN=re.compile(r'(?P<actor>\w+)\s*=\s*GSI->GetHero\(\);\s*CScriptThing::CScriptThing\(\(\s*CScriptThing\s*\*\s*\)\s*&(?P<temporary>\w+),\s*(?P=actor),\s*\w+\);\s*std::_Cons_val[^;]+__atcb7920\(this\);(?=\s*GSI->NewScriptFrame\(\);)')
REGIONS=[
 (0x4abe90,33,'3c497d08782ebf50f8d8813805f1e16fc8c68869028ff7033e4ecbfed3ef1da9'),
 (0xcb7920,8,'2409002f93a4e3c24776b4a26a0a3d138233a019e0ec1439db4ff8a14968bf84'),
 (0xf35a10,25,'767d42ff00975f8a6acc948de55d85a5bc952f39b6fbbded79211aaff51ec8df'),
 (0xcddab0,44,'249a1e38f22e5e6764bafe4a9021b000c67b2b83bc13c59a2c615716d96f04f3'),
 (0xcddae0,48,'ab72c702b630e7f28d0cbdd10ef51c5b1a7d0da2d90104516a88861777a6c3b2'),
 (0x4aa840,66,'5aa58f8e6b624fb6abbc2496c0f63791f6a7ae480b3e86d9e93f148d23fe4afa'),
 (0xcddb10,45,'af7362b6e2e39f34ab902be295e3a10d554fad73ec912c2d280406647a09f850')]
def restore(text,fn,rdata,row=None):
 if not fn or rdata is None or not row:return text
 if row.get('schema')!='reviewed-quest-conscious-condition/1' or row.get('address')!=str(fn['address']).lower():return text
 if hashlib.sha256(fn['decompile'].encode()).hexdigest()!=row.get('sourceSha256'):return text
 start=int(fn['address'],16);end=int(fn['bodyEndExclusive'],16);body=rdata.bytes_at(start,end-start)
 if body is None or hashlib.sha256(body).hexdigest()!=row.get('nativeSha256'):return text
 matches=list(PATTERN.finditer(text));aliased=False;casted=False
 if not matches:
  matches=list(ALIAS_PATTERN.finditer(text));aliased=bool(matches)
 if not matches:
  matches=list(CAST_PATTERN.finditer(text));casted=bool(matches)
 if len(matches)!=1:return text
 m=matches[0]
 if aliased:
  # The export coalesces the condition header/Thing and later movie under
  # one local name. The original local destructor runs before the first
  # frame. Accept reuse only with a reviewed first redefinition and native
  # movie output at the same16-byte header slot; never carry that alias out.
  if not re.search(r'(?:Stack_|local_)10$',m['temporary']):return text
  reuse=row.get('temporaryReuse',{})
  if reuse.get('inputSha256')!=hashlib.sha256(text.encode()).hexdigest():return text
  assignment=f'{m["temporary"]} = RESOURCE_StartMovie("");'
  if reuse.get('sourceAssignment')!=assignment:return text
  tail=text[m.end():];occurrence=re.search(r'\b'+re.escape(m['temporary'])+r'\b',tail)
  if occurrence is None or not tail[occurrence.start():].startswith(assignment)or tail.count(assignment)!=1:return text
 else:
  if not re.search(r'(?:Stack_|local_)'+('2c'if casted else'c')+r'$',m['temporary']):return text
  if re.search(r'\b'+re.escape(m['temporary'])+r'\b',text[m.end():]):return text
 for address,size,expected in REGIONS:
  raw=rdata.bytes_at(address,size)
  if raw is None or hashlib.sha256(raw).hexdigest()!=expected:return text
 if rdata.bytes_at(0x12c2fe8,12)!=struct.pack('<III',0xcddab0,0xcddae0,0xcddb10):return text
 start=int(fn['address'],16)
 # Witnessed compiler entry layouts retain their saved-register count and
 # condition-local stack positions; a construction flag stays outside the lift.
 profiles=[
  ('83ec4c55568bf133ed896c24108b4e408b0157ff9018010000bfe82f2c01508d4c2450897c244ce8000000008d4c2448518bcee8000000008d4c244c897c2448e8000000008b4e408b11ff521c',
   ((34,4),(38,0),(47,-4),(59,0),(63,-4)),(39,51,64),19,74,12,-12,-16),
  ('83ec3453568bf18b4e408b0157ff9018010000bfe82f2c01508d4c2438897c2434e8000000008d4c2430518bcee8000000008d4c2434897c2430e8000000008b4e408b11ff521c',
   ((28,4),(32,0),(41,-4),(53,0),(57,-4)),(33,45,58),13,68,12,-12,-16),
  ('83ec3c53568bf18b4e408b0133db57895c2410ff9018010000bfe82f2c01508d4c2440897c243ce8000000008d4c2438518bcee8000000008d4c243c897c2438e8000000008b4e408b11ff521c',
   ((34,4),(38,0),(47,-4),(59,0),(63,-4)),(39,51,64),19,74,12,-12,-16),
  ('83ec34568bf18b4e408b0157ff9018010000bfe82f2c01508d4c2434897c2430e8000000008d4c242c518bcee8000000008d4c2430897c242ce8000000008b4e408b11ff521c',
   ((27,0),(31,-4),(40,-8),(52,-4),(56,-8)),(32,44,57),12,67,8,-12,-16),
  ('83ec7c5355568bf18b4e408b0157ff9018010000bfe82f2c01508d4c2464897c2460e8000000008d4c245c518bcee8000000008d4c2460897c245ce8000000008b4e408b11ff521c',
   ((29,-24),(33,-28),(42,-32),(54,-28),(58,-32)),(34,46,59),14,69,16,-44,-48)]
 profile=None
 for template,stack_fields,call_offsets,hero_offset,frame_offset,saved_bytes,thing_slot,condition_slot in profiles:
  expected=bytearray.fromhex(template);raw=rdata.bytes_at(start,len(expected))or b''
  if len(raw)!=len(expected):continue
  frame=raw[2]
  if frame<4 or frame>127 or any(not 0<=frame+delta<=127 for _,delta in stack_fields):continue
  expected[2]=frame
  for offset,delta in stack_fields:expected[offset]=frame+delta
  for offset,target in zip(call_offsets,(0x4abe90,0xcb7920,0x4aa840)):
   expected[offset+1:offset+5]=struct.pack('<i',target-(start+offset+5))
  if raw==bytes(expected):
   profile=(frame,call_offsets,hero_offset,frame_offset,len(expected),saved_bytes,thing_slot,condition_slot);break
 if profile is None:return text
 frame,call_offsets,hero_offset,frame_offset,prefix_size,saved_bytes,thing_slot,condition_slot=profile
 if casted!=(thing_slot==-44):return text
 end=int(fn['bodyEndExclusive'],16);body=rdata.bytes_at(start,end-start)
 if body is None:return text
 # The source correspondence covers a single entry sequence. A later native
 # edge back into it would need separate evidence for repeated registration.
 for instruction in Cs(CS_ARCH_X86,CS_MODE_32).disasm(body,start):
  if instruction.mnemonic.startswith('j'):
   try:target=int(instruction.op_str,16)
   except ValueError:return text
   if start<=target<start+prefix_size:return text
 calls={int(c['site'],16):c for c in fn.get('calls',[])};indirect={int(c['site'],16):c for c in fn.get('indirectCalls',[])}
 depth=frame+saved_bytes
 copy=calls.get(start+call_offsets[0],{});register=calls.get(start+call_offsets[1],{});destroy=calls.get(start+call_offsets[2],{})
 if copy.get('depth')!=depth+4 or copy.get('ecxStack')!=thing_slot or copy.get('pushedStack')!=[None]:return text
 if register.get('depth')!=depth+4 or register.get('pushedStack')!=[condition_slot]:return text
 if destroy.get('depth')!=depth or destroy.get('ecxStack')!=thing_slot:return text
 if indirect.get(start+hero_offset,{}).get('slot')!='0x118' or indirect.get(start+hero_offset,{}).get('depth')!=depth:return text
 if indirect.get(start+frame_offset,{}).get('slot')!='0x1c' or indirect.get(start+frame_offset,{}).get('depth')!=depth:return text
 if aliased:
  movies=[c for c in fn.get('indirectCalls',[])if c.get('slot')=='0x5c8']
  if len(movies)!=1:return text
  movie=movies[0];reuse=row['temporaryReuse']
  if movie.get('site')!=reuse.get('nativeSite')or movie.get('pushedStack')!=reuse.get('pushedStack'):return text
  if len(movie.get('pushedStack',[]))!=2 or movie['pushedStack'][0]!=-16:return text
  if int(movie['site'],16)<start+prefix_size:return text
 replacement=f'{m["actor"]} = GSI->GetHero();\nENGINE_RegisterQuestConsciousCondition({m["actor"]});'
 return text[:m.start()]+replacement+text[m.end():]
