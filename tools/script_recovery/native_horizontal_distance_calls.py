"""Recover byte-proven fastcall horizontal predicates, preserving native bool return."""
import hashlib,re,struct
TARGET=0xCBE371
SIZE=121
DIGEST='3fad395778ce5428e3abfb96830332781756a9168ffdced10cc81250c8d31b31'
def restore(text,function,rdata):
 if not function or rdata is None:return text
 calls=[c for c in function.get('calls',[])if int(c.get('target','0'),16)==TARGET]
 if not calls:return text
 if hashlib.sha256(rdata.bytes_at(TARGET,SIZE)or b'').hexdigest()!=DIGEST:return text
 from tools.script_recovery.native_string_returns import _target_sites
 from tools.script_recovery.native_evidence_lowering import _strip_addr
 entries=_target_sites(text,function,calls)
 if len(entries)!=len(calls):return text
 edits=[]
 for a,e,args,site,label,_ in entries:
  address=int(site['site'],16);before=rdata.bytes_at(address-17,17)or b'';after=rdata.bytes_at(address,7)or b''
  if len(args)!=3 or len(before)!=17 or len(after)!=7:continue
  # push IEEE32 range; GSI GetHero; lea EDX,[ESP+disp8]; mov ECX,EAX;
  # direct predicate call; test AL,AL. This is deliberately bounded to the
  # proven hero/stack-Thing call shape, leaving other operand shapes unresolved.
  if before[:1]!=b'\x68'or before[5:11]!=b'\xff\x92\x18\x01\x00\x00'or before[11:14]!=b'\x8d\x54\x24'or before[15:]!=b'\x8b\xc8':continue
  if after[0]!=0xe8 or address+5+struct.unpack('<i',after[1:5])[0]!=TARGET or after[5:]!=b'\x84\xc0':continue
  depth=site.get('depth');stack=site.get('edxStack')
  if not isinstance(depth,int)or stack!=before[14]-depth:continue
  second=_strip_addr(args[1]);suffix=re.search(r'(?:Stack_|local_)([0-9a-f]+)$',second)
  first=_strip_addr(args[0])
  if suffix is None or int(suffix[1],16)!=-stack or not first.isidentifier():continue
  head=re.search(re.escape(label).replace('::',r'\s*::\s*')+r'\s*\($',text[:a])
  if head is None:continue
  prior=text[:head.start()];lhs=re.search(r'\b\w+\s*=\s*$',prior)
  if lhs is None:continue
  prefix=prior[:lhs.start()]
  hero_spellings=(f'{first} = GSI->GetHero();',f'{first} = (**(code **)(**(int **)(this + 0x40) + 0x118))(*(int **)(this + 0x40));')
  compact=re.sub(r'\s+','',prefix)
  if not any(compact.endswith(re.sub(r'\s+','',s))for s in hero_spellings):continue
  radius=struct.unpack('<f',before[1:5])[0]
  if radius!=radius or abs(radius)==float('inf'):continue # Lua literals cannot represent these raw immediates safely here.
  edits.append((head.start(),e+1,f'ENGINE_IsHorizontalDistanceBetweenThingsUnder({first}, {second}, {radius!r})'))
 for a,e,replacement in sorted(edits,reverse=True):text=text[:a]+replacement+text[e:]
 return text
