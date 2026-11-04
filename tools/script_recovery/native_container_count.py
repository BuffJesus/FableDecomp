"""Recover reviewed native CString-vector count-only uses, without table cardinality assumptions."""
import hashlib,re
METHOD_SHA256='e69bc4b68be94352292104859f2a20a715c7f6b13d692e8cc2cf053833a9b71b'
BINDING_SHA256='4fe32a501f58622e01fee30f20787bf2e5e7b473258e80a0cc9e2820161874b9'
PATTERN=re.compile(r'(?P<begin>\w+) = GSI->GetItemDefNamesFromContainer\((?P<target>[^;\n]+)\);\s*(?P<empty>\w+) = (?P=begin) == (?P<end>\w+);')
sha=lambda b:hashlib.sha256(b).hexdigest()
def restore(source,fn,rd,row=None):
 try:
  if not row or row.get('schema')!='reviewed-container-item-def-count/1' or not fn or not rd:return source
  start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16)
  if int(row['address'],16)!=start or row['sourceSha256']!=sha(fn['decompile'].encode()) or row['inputSha256']!=sha(source.encode()):return source
  if row['nativeSha256']!=sha(rd.bytes_at(start,end-start)):return source
  if row['sdkMethodSha256']!=METHOD_SHA256 or row['sdkBindingSha256']!=BINDING_SHA256:return source
  if row['operation']!='GetContainerItemDefCount':return source
  matches=list(PATTERN.finditer(source))
  if len(matches)!=1:return source
  m=matches[0]
  if [m['begin'],m['end'],m['target']]!=[row['begin'],row['end'],row['target']]:return source
  def stack_slot(name):
   match=re.fullmatch(r'[A-Za-z]\w*_(?:stk_)?([0-9a-f]+)',name)
   return -int(match[1],16)if match else None
  target_name=re.fullmatch(r'\(CScriptThing \*\)(\w+)',m['target'])
  if not target_name:return source
  if stack_slot(m['begin'])!=row['beginSlot'] or stack_slot(m['end'])!=row['beginSlot']+4 or stack_slot(target_name[1])!=row['targetSlot']:return source
  query=rd.bytes_at(int(row['callSite'],16)-27,33)
  if query is None or len(query)!=33:return source
  template=bytearray(query)
  for offset in [6,11,15,19,23]:template[offset]=0
  if bytes(template)!=bytes.fromhex('8b4e408d542400528d542400896c2400896c2400896c24008b0152ff9030090000'):return source
  frame=query[6]-row['beginSlot']
  if frame!=row['frameDepth'] or row['callDepth']!=frame+8 or query[11]-frame-4!=row['targetSlot'] or [query[k]for k in [15,19,23]]!=[query[6]+4,query[6]+8,query[6]+12]:return source
  call=next(c for c in fn['indirectCalls']if c['site']==row['callSite'])
  if call.get('slot')!='0x930' or call.get('depth')!=row['callDepth'] or call.get('pushedStack')!=[row['beginSlot'],row['targetSlot']] or call.get('edxStack')!=row['targetSlot']:return source
  dtors=[c for c in fn['calls']if c['site']in row['destructorSites']]
  if len(dtors)!=2 or any(int(c['target'],16)!=0x414ea0 or c.get('ecxStack')!=row['beginSlot'] or c.get('depth')!=row['frameDepth']for c in dtors):return source
  for address,anchor in row['anchors'].items():
   if rd.bytes_at(int(address,16),len(bytes.fromhex(anchor)))!=bytes.fromhex(anchor):return source
  if sha(rd.bytes_at(0x414ea0,49))!=row['destructorSha256']:return source
  destroy='CDefendingCombatantInfo::CCombatWheel::ResetRings((CCombatWheel *)&'+m['begin']+');'
  if source.count(destroy)!=2:return source
  # Declaration, fill, comparison and the two vector destructions only. No
  # element or borrowed string use may be silently discarded by a count bridge.
  if len(re.findall(r'\b'+re.escape(m['begin'])+r'\b',source))!=5 or len(re.findall(r'\b'+re.escape(m['end'])+r'\b',source))!=2:return source
  replacement=m['begin']+' = ENGINE_GetContainerItemDefCount('+m['target']+');\n          '+m['empty']+' = '+m['begin']+' == 0;'
  result=source[:m.start()]+replacement+source[m.end():]
  return result.replace(destroy,'')
 except (KeyError,ValueError,TypeError,StopIteration,AttributeError):return source
