"""Replace an inline conditional jump over a declaration-free outer region.

The target label stays. Nested body scopes stay intact; outer declarations,
labels,closures,opaque text and scope boundaries make the candidate refuse.
"""
import bisect,hashlib
from tools.script_recovery.readable_lua import tokens
HAZARDS={'_ENV','debug','getfenv','setfenv','load','loadstring','getlocal','setlocal','getupvalue','setupvalue','upvaluejoin'}
CONTROL={'function','if','then','else','elseif','end','do','for','while','repeat','until','goto','break','return','local','in'}

def _lines(source):
 stream=tokens(source);ids=[t[0] for t in stream if t.lastgroup=='identifier']
 if ids.count('function')!=1 or set(ids)&HAZARDS:return None
 if any(t.lastgroup!='space' and any(c in t[0] for c in '\r\n') for t in stream):return None
 lines=source.splitlines(True);starts=[];position=0
 for line in lines:starts.append(position);position+=len(line)
 by_line=[[] for _ in lines]
 for t in stream:
  if t.lastgroup!='space':by_line[bisect.bisect_right(starts,t.start())-1].append(t)
 return lines,by_line

def _target(by_line,start,label):
 stack=[];executable=False;nested_locals=0
 for j in range(start,len(by_line)):
  ts=[t for t in by_line[j] if t.lastgroup not in {'comment','longcomment'}]
  if not ts:continue
  vals=[t[0] for t in ts]
  if '::' in vals:
   return (j,nested_locals) if not stack and vals==['::',label,'::'] and executable else None
  executable=True
  for t in ts:
   if t.lastgroup!='identifier':continue
   word=t[0]
   if word=='local':
    if not stack:return None
    nested_locals+=1
   elif word=='return' and not stack:return None
   elif word=='function':return None
   elif word in {'if','repeat'}:stack.append(word)
   elif word in {'while','for'}:stack.append(word+'_await_do')
   elif word=='do':
    if stack and stack[-1].endswith('_await_do'):stack[-1]=stack[-1].removesuffix('_await_do')
    else:stack.append('do')
   elif word in {'else','elseif'}:
    if not stack or stack[-1]!='if':return None
   elif word=='end':
    if not stack or stack[-1]=='repeat' or stack[-1].endswith('_await_do'):return None
    stack.pop()
   elif word=='until':
    if not stack or stack[-1]!='repeat':return None
    stack.pop()
 return None

def fold_once(source):
 parsed=_lines(source)
 if parsed is None:return source,None
 lines,by_line=parsed
 for i,ts in enumerate(by_line):
  if any(t.lastgroup in {'comment','longcomment'} for t in ts):continue
  if len(ts)<6 or ts[0][0]!='if' or [t[0] for t in ts[-4:-2]]!=['then','goto'] or ts[-1][0]!='end' or ts[-2].lastgroup!='identifier':continue
  condition_tokens=ts[1:-4]
  if not condition_tokens or any(t.lastgroup=='identifier' and t[0] in CONTROL for t in condition_tokens):continue
  label=ts[-2][0];target=_target(by_line,i+1,label)
  if target is None:continue
  j,nested_locals=target
  condition=source[condition_tokens[0].start():condition_tokens[-1].end()]
  indent=lines[i][:len(lines[i])-len(lines[i].lstrip())]
  if not indent:continue
  newline='\r\n' if lines[i].endswith('\r\n') else '\n'
  body=lines[i+1:j]
  shifted=['    '+line if line.strip() else line for line in body]
  replacement=[indent+'if not ('+condition+') then'+newline]+shifted+[indent+'end'+newline]
  after=''.join(lines[:i]+replacement+lines[j:])
  proof=dict(kind='conditional_skip_declaration_free_region',line=i+1,targetLine=j+1,label=label,condition=condition,nestedDeclarationsRetained=nested_locals,noOuterDeclarationsMoved=True,noBodyLabels=True,targetLabelRetained=True,conditionEvaluatedOnce=True,noLookupOrCleanupReordered=True,beforeSha256=hashlib.sha256(source.encode()).hexdigest(),afterSha256=hashlib.sha256(after.encode()).hexdigest())
  return after,proof
 return source,None

def fold(source):
 text=source;proofs=[]
 for _ in range(128):
  after,proof=fold_once(text)
  if proof is None:break
  proofs.append(proof);text=after
 if not proofs:return source,None
 return text,dict(kind='conditional_skip_regions',regions=proofs,removedGotos=len(proofs),allDeclarationsRetained=True,labelsRetained=True,nestedScopesRetained=True,noHostReturnTypePremise=True)
