"""Normalize an empty then/nonempty else without moving branch-local lifetimes."""
import hashlib
from tools.script_recovery import readable_guard_region as G
from tools.script_recovery.readable_unreachable_goto_tail import _call,RESERVED

def _full_operand(ts):
 if not ts:return False
 if ts[0][0]=='not':return _full_operand(ts[1:])
 if len(ts)==1 and (ts[0].lastgroup in {'string','longstring'} or ts[0].lastgroup=='identifier' and (ts[0][0] not in RESERVED or ts[0][0] in {'nil','false','true'})):return True
 if _call(ts):return True
 if ts[0][0]=='(':
  depth=0
  for i,t in enumerate(ts):
   if t.lastgroup in {'string','longstring'}:continue
   if t[0]=='(':depth+=1
   elif t[0]==')':
    depth-=1
    if depth==0:return i==len(ts)-1
  return False
 # A dotted primary has no binary operator outside the leading unary not.
 if len(ts)%2==1 and all(t.lastgroup=='identifier' and t[0] not in RESERVED if i%2==0 else t[0]=='.' for i,t in enumerate(ts)):return True
 return False

def _end(by_line,start):
 stack=['if']
 for j in range(start,len(by_line)):
  ts=[t for t in by_line[j] if t.lastgroup not in {'comment','longcomment'}]
  for t in ts:
   if t.lastgroup!='identifier':continue
   word=t[0]
   if word in {'if','repeat'}:stack.append(word)
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
    if not stack:return j if [x[0] for x in ts]==['end'] else None
 return None

def fold_once(source):
 parsed=G._lines(source)
 if parsed is None:return source,None
 lines,by_line=parsed
 for i,ts in enumerate(by_line):
  if len(ts)<3 or ts[0][0]!='if' or ts[-1][0]!='then' or any(t.lastgroup in {'comment','longcomment'} for t in ts):continue
  condition=ts[1:-1]
  if not condition or any(t.lastgroup=='identifier' and t[0] in G.CONTROL for t in condition):continue
  e=next((j for j in range(i+1,len(by_line)) if by_line[j]),None)
  if e is None or [t[0] for t in by_line[e]]!=['else']:continue
  end=_end(by_line,e+1)
  if end is None or not any(t.lastgroup not in {'comment','longcomment'} for row in by_line[e+1:end] for t in row):continue
  raw=source[condition[0].start():condition[-1].end()]
  cancelled=condition[0][0]=='not' and _full_operand(condition[1:])
  replacement=source[condition[1].start():condition[-1].end()] if cancelled else 'not ('+raw+')'
  indent=lines[i][:len(lines[i])-len(lines[i].lstrip())]
  if not indent:continue
  newline='\r\n' if lines[i].endswith('\r\n') else '\n'
  after=''.join(lines[:i]+[indent+'if '+replacement+' then'+newline]+lines[i+1:e]+lines[e+1:])
  proof=dict(kind='empty_then_else_to_guard',line=i+1,elseLine=e+1,endLine=end+1,condition=raw,newCondition=replacement,cancelledWholeUnaryNot=cancelled,branchBodyExact=True,branchDeclarationsAndCloseBoundaryRetained=True,labelsAndAnnotationsRetained=True,conditionEvaluatedOnce=True,noHostTypePremise=True,beforeSha256=hashlib.sha256(source.encode()).hexdigest(),afterSha256=hashlib.sha256(after.encode()).hexdigest())
  return after,proof
 return source,None

def fold(source):
 text=source;proofs=[]
 for _ in range(128):
  after,proof=fold_once(text)
  if proof is None:break
  proofs.append(proof);text=after
 if not proofs:return source,None
 return text,dict(kind='empty_then_guards',regions=proofs,normalizedGuards=len(proofs),allBranchBodiesExact=True,allDeclarationsAndLabelsRetained=True,noHostTypePremise=True)
