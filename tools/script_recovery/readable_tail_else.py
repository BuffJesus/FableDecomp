"""Turn an existing then-tail jump over an outer-declaration-free false region into else.

The original condition and then branch keep their scopes. Target labels stay;
labels in either body,outer else,closures,opaque text and unknown boundaries refuse.
"""
import hashlib
from tools.script_recovery import readable_guard_region as G

def _then_end(by_line,start):
 stack=['if'];last=None;locals_count=0
 for j in range(start,len(by_line)):
  raw=by_line[j];ts=[t for t in raw if t.lastgroup not in {'comment','longcomment'}]
  if not ts:continue
  if any(t[0]=='::' for t in ts):return None
  depth_before=len(stack)
  for t in ts:
   if t.lastgroup!='identifier':continue
   word=t[0]
   if word=='local':locals_count+=1
   elif word=='return' and len(stack)==1:return None
   elif word=='function':return None
   elif word in {'if','repeat'}:stack.append(word)
   elif word in {'while','for'}:stack.append(word+'_await_do')
   elif word=='do':
    if stack and stack[-1].endswith('_await_do'):stack[-1]=stack[-1].removesuffix('_await_do')
    else:stack.append('do')
   elif word in {'else','elseif'}:
    if not stack or stack[-1]!='if' or len(stack)==1:return None
   elif word=='until':
    if not stack or stack[-1]!='repeat':return None
    stack.pop()
   elif word=='end':
    if not stack or stack[-1]=='repeat' or stack[-1].endswith('_await_do'):return None
    stack.pop()
    if not stack:
     if [x[0] for x in ts]!=['end'] or last is None or any(x.lastgroup in {'comment','longcomment'} for x in raw):return None
     g,tail,before,after=last
     if before!=1 or after!=1 or len(tail)!=2 or tail[0][0]!='goto' or tail[1].lastgroup!='identifier':return None
     if any(x.lastgroup in {'comment','longcomment'} for x in by_line[g]):return None
     return j,g,tail[1][0],locals_count
  last=(j,ts,depth_before,len(stack))
 return None

def fold_once(source):
 parsed=G._lines(source)
 if parsed is None:return source,None
 lines,by_line=parsed
 for i,ts in enumerate(by_line):
  if len(ts)<3 or ts[0][0]!='if' or ts[-1][0]!='then' or any(t.lastgroup in {'comment','longcomment'} for t in ts):continue
  condition=ts[1:-1]
  if not condition or any(t.lastgroup=='identifier' and t[0] in G.CONTROL for t in condition):continue
  found=_then_end(by_line,i+1)
  if found is None:continue
  end,g,label,then_locals=found;target=G._target(by_line,end+1,label)
  if target is None:continue
  j,false_locals=target;indent=lines[i][:len(lines[i])-len(lines[i].lstrip())]
  if not indent:continue
  newline='\r\n' if lines[i].endswith('\r\n') else '\n'
  then_body=lines[i+1:g]+lines[g+1:end]
  false_body=['    '+line if line.strip() else line for line in lines[end+1:j]]
  replacement=[lines[i]]+then_body+[indent+'else'+newline]+false_body+[indent+'end'+newline]
  after=''.join(lines[:i]+replacement+lines[j:])
  proof=dict(kind='then_tail_jump_to_else',line=i+1,tailGotoLine=g+1,oldEndLine=end+1,targetLine=j+1,label=label,condition=source[condition[0].start():condition[-1].end()],thenDeclarationsRetained=then_locals,falseNestedDeclarationsRetained=false_locals,conditionHeaderExact=True,thenBranchScopeRetained=True,noFalseOuterDeclarationsMoved=True,noBodyLabels=True,targetLabelRetained=True,allLiveCallsRetained=True,beforeSha256=hashlib.sha256(source.encode()).hexdigest(),afterSha256=hashlib.sha256(after.encode()).hexdigest())
  return after,proof
 return source,None

def fold(source):
 text=source;proofs=[]
 for _ in range(128):
  after,proof=fold_once(text)
  if proof is None:break
  proofs.append(proof);text=after
 if not proofs:return source,None
 return text,dict(kind='then_tail_else_regions',regions=proofs,removedGotos=len(proofs),allDeclarationsRetained=True,labelsRetained=True,nearestThenScopesRetained=True,noHostReturnTypePremise=True)
