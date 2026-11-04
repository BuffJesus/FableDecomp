"""Retarget adjacent live generated label aliases;keep every definition here."""
from collections import Counter
import hashlib,re
from tools.script_recovery import readable_guard_region as G
from tools.script_recovery.readable_scope_exit import _roles

def fold(source):
 parsed=G._lines(source)
 if parsed is None:return source,None
 lines,by_line=parsed
 if _roles(by_line) is None:return source,None
 code=[t for row in by_line for t in row if t.lastgroup not in {'comment','longcomment'}]
 definitions=Counter();jumps=[]
 for n,t in enumerate(code):
  if t[0]=='::' and n+2<len(code) and code[n+1].lastgroup=='identifier' and code[n+2][0]=='::':definitions[code[n+1][0]]+=1
  if t.lastgroup=='identifier' and t[0]=='goto' and n+1<len(code) and code[n+1].lastgroup=='identifier':jumps.append((t,code[n+1]))
 uses=Counter(target[0] for _,target in jumps);groups=[];cluster=[]
 def finish():
  if len(cluster)>1 and all(definitions[name]==1 and uses[name]>0 for _,name in cluster):
   groups.append(dict(canonical=cluster[0][1],labels=[dict(line=line+1,label=name,references=uses[name]) for line,name in cluster]))
  cluster.clear()
 for line,row in enumerate(by_line):
  if not row:continue
  if len(row)==3 and row[0][0]==row[2][0]=='::' and row[1].lastgroup=='identifier' and re.fullmatch(r'(?:LAB_|FLOW_)\w+',row[1][0]):cluster.append((line,row[1][0]))
  else:finish()
 finish()
 aliases={entry['label']:group['canonical'] for group in groups for entry in group['labels'][1:]};edits=[]
 for jump,target in jumps:
  if target[0] in aliases:edits.append(dict(start=target.start(),end=target.end(),line=source[:jump.start()].count('\n')+1,targetLine=source[:target.start()].count('\n')+1,old=target[0],new=aliases[target[0]]))
 if not edits:return source,None
 after=source
 for row in sorted(edits,key=lambda r:r['start'],reverse=True):after=after[:row['start']]+row['new']+after[row['end']:]
 assert len(after.splitlines())==len(source.splitlines())
 return after,dict(kind='adjacent_live_generated_label_aliases',groups=groups,aliases=aliases,retargetedGotos=len(edits),edits=edits,allLabelDefinitionsRetained=True,allOtherBytesExact=True,onlyWhitespaceBetweenAliases=True,uniqueDefinitionsInFunction=True,noHostTypePremise=True,beforeSha256=hashlib.sha256(source.encode()).hexdigest(),afterSha256=hashlib.sha256(after.encode()).hexdigest())
