"""Remove a jump over only transparent if/do exits to its unique target label."""
from collections import Counter
import hashlib
from tools.script_recovery import readable_guard_region as G

def _roles(by_line):
 stack=[];roles={}
 for i,raw in enumerate(by_line):
  ts=[t for t in raw if t.lastgroup not in {'comment','longcomment'}]
  for t in ts:
   if t.lastgroup!='identifier':continue
   word=t[0]
   if word in {'function','if','repeat'}:stack.append(word)
   elif word in {'while','for'}:stack.append(word+'_await_do')
   elif word=='do':
    if stack and stack[-1].endswith('_await_do'):stack[-1]=stack[-1].removesuffix('_await_do')
    else:stack.append('do')
   elif word in {'else','elseif'}:
    if not stack or stack[-1]!='if':return None
   elif word=='until':
    if not stack or stack[-1]!='repeat':return None
    stack.pop()
   elif word=='end':
    if not stack or stack[-1]=='repeat' or stack[-1].endswith('_await_do'):return None
    role=stack.pop()
    if [x[0] for x in ts]==['end']:roles[i]=role
 return roles if not stack else None

def fold_once(source, *, allow_intervening_labels=False):
 parsed=G._lines(source)
 if parsed is None:return source,None
 lines,by_line=parsed;roles=_roles(by_line)
 if roles is None:return source,None
 labels=Counter()
 for raw in by_line:
  ts=[t for t in raw if t.lastgroup not in {'comment','longcomment'}]
  for k,t in enumerate(ts[:-2]):
   if t[0]=='::' and ts[k+1].lastgroup=='identifier' and ts[k+2][0]=='::':labels[ts[k+1][0]]+=1
 for i,ts in enumerate(by_line):
  if len(ts)!=2 or ts[0][0]!='goto' or ts[1].lastgroup!='identifier':continue
  label=ts[1][0]
  if labels[label]!=1:continue
  exits=[];skipped_labels=[]
  for j in range(i+1,len(by_line)):
   code=[t[0] for t in by_line[j] if t.lastgroup not in {'comment','longcomment'}]
   if not code:continue
   if code==['end'] and roles.get(j) in {'if','do'}:
    exits.append(dict(line=j+1,kind=roles[j]));continue
   if code==['::',label,'::']:
    after=''.join(lines[:i]+lines[i+1:])
    proof=dict(kind='transparent_scope_exit_jump',line=i+1,targetLine=j+1,label=label,exits=exits,uniqueLabelInFunction=True,allScopeClosersRetained=True,allDeclarationsAndLabelsRetained=True,noLoopOrFunctionExitSkipped=True,noLiveCallsRemoved=True,beforeSha256=hashlib.sha256(source.encode()).hexdigest(),afterSha256=hashlib.sha256(after.encode()).hexdigest())
    if skipped_labels:proof['interveningLabelsRetained']=skipped_labels
    return after,proof
   if allow_intervening_labels and len(code)==3 and code[0]==code[2]=='::':
    skipped_labels.append(dict(line=j+1,label=code[1]));continue
   break
 return source,None

def fold(source, *, allow_intervening_labels=False):
 text=source;proofs=[]
 for _ in range(128):
  after,proof=fold_once(text,allow_intervening_labels=allow_intervening_labels)
  if proof is None:break
  proofs.append(proof);text=after
 if not proofs:return source,None
 return text,dict(kind='transparent_scope_exit_jumps',regions=proofs,removedGotos=len(proofs),allScopeClosersDeclarationsAndLabelsRetained=True,noHostTypePremise=True)
