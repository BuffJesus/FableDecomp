"""Reviewed native key count/CString output mapped to the public bridge table."""
import hashlib,re
sha=lambda b:hashlib.sha256(b).hexdigest()
METHOD_SHA256='29fde4a5d7748c146d100ac1f99731d48d5c76a0c7f0b0200f81bb1f0bf9b183'
BINDING_SHA256='4f04be5a9d0bdbfd64eac77f60dc983b812bd95431f5a554a9dec7e145bf42b6'
def restore(source,fn,rd,row=None):
 try:
  if not row or row['schema']!='reviewed-chest-key-result/1':return source
  start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16)
  if row['address']!=fn['address'].lower()or row['sourceSha256']!=sha(fn['decompile'].encode())or row['inputSha256']!=sha(source.encode())or row['nativeSha256']!=sha(rd.bytes_at(start,end-start)):return source
  if row['sdkMethodSha256']!=METHOD_SHA256 or row['sdkBindingSha256']!=BINDING_SHA256:return source
  for a,raw in row['anchors'].items():
   if rd.bytes_at(int(a,16),len(bytes.fromhex(raw)))!=bytes.fromhex(raw):return source
  calls={c['site']:c for c in fn['indirectCalls']}
  q=calls[row['callSite']];inventory=calls[row['inventorySite']]
  if q['slot']!='0x6d4'or inventory['slot']!='0x2dc':return source
  if q['pushedStack']!=[row['keySlot'],row['targetSlot']]or q['depth']!=row['frameDepth']+8:return source
  if inventory['pushedStack']!=[row['keySlot']]or inventory['depth']!=row['frameDepth']+4:return source
  raw=rd.bytes_at(int(row['callSite'],16)-15,21);inv=rd.bytes_at(int(row['inventorySite'],16)-10,16)
  if raw is None or inv is None:return source
  template=bytearray(raw);template[8]=template[13]=0
  inv_template=bytearray(inv);inv_template[8]=0
  if bytes(template)!=bytes.fromhex('8b4e408b118d442400508d44240050ff92d4060000')or bytes(inv_template)!=bytes.fromhex('8b4e408b118d44240050ff92dc020000'):return source
  if raw[8]-row['frameDepth']!=row['keySlot']or raw[13]-row['frameDepth']-4!=row['targetSlot']or inv[8]!=raw[8]:return source
  count,target,key,result=[row[k]for k in ['count','target','key','result']]
  if any(not re.fullmatch(r'[A-Za-z_]\w*',name)for name in [count,key,result]):return source
  if not re.fullmatch(r'native_[A-Za-z]\w*',result):return source
  if not re.fullmatch(r'\(CScriptThing \*\)[A-Za-z_]\w*',target):return source
  if len(re.findall(r'\b'+re.escape(key)+r'\b',source))!=6:return source
  if re.search(r'\b'+re.escape(result)+r'\b',source):return source
  query=f'{count} = GSI->GetNumberOfKeysNeededToUnlockChest({target},&{key});'
  ctor=f'CCharString::CCharString(&{key});'
  dtor=f'std::_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>__at99eae0(&{key});'
  if source.count(query)!=1 or source.count(ctor)!=1 or source.count(dtor)!=2 or source.count(f'GSI->GetNumberOfItemsOfTypeInInventory(&{key})')!=1:return source
  replacement=f'{result} = ENGINE_ChestKeyResult({target});\n    {count} = ENGINE_ChestKeyCount({result});\n    {key} = ENGINE_ChestKeyName({result});'
  return source.replace(query,replacement).replace(ctor,'').replace(dtor,'').replace(f'GSI->GetNumberOfItemsOfTypeInInventory(&{key})',f'ENGINE_InventoryCountWithKeyName({key})')
 except(KeyError,ValueError,TypeError,AttributeError):return source
