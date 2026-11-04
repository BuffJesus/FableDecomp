"""Finite numeric-local facts over the restricted generated Lua normal CFG."""
import ast,itertools,re
from collections import deque
from tools.script_recovery.lua_staging_liveness import build_cfg

def prove(source,aliases,regions):
    def require(ok,why):
        if not ok:raise ValueError('Lua numeric alias invariants: '+why)
    require(aliases and len(set(aliases))==len(aliases),'empty/duplicate aliases')
    cfg=build_cfg(source);nodes=cfg['nodes'];branches=cfg['branches'];entry=cfg['entry'];require(all(re.fullmatch(r'[A-Za-z_]\w*',alias) and alias in cfg['locals'] for alias in aliases),'nonlocal/invalid alias')
    alternatives='|'.join(re.escape(a) for a in aliases);assignment_pattern=re.compile(r'('+alternatives+r')\s*=\s*(.+)')
    for alias in aliases:
        parsed=sum(bool(re.fullmatch(re.escape(alias)+r'\s*=\s*.+',line)) for line,_ in nodes.values())
        from tools.script_recovery.lua_staging_liveness import _masked
        require(parsed==len(re.findall(r'\b'+re.escape(alias)+r'\s*=(?!=)',_masked(source))),'unparsed or parallel alias write')
    def value(expr,state):
        def visit(node):
            if isinstance(node,ast.Constant):require(type(node.value) is int,'noninteger literal');return {node.value}
            if isinstance(node,ast.Name):require(node.id in aliases,'untracked value read');return state[node.id]
            require(isinstance(node,ast.BinOp) and isinstance(node.op,(ast.BitAnd,ast.BitOr)),'unsupported expression')
            a,b=visit(node.left),visit(node.right);require(None not in a|b,'uninitialized alias used');return {x&y if isinstance(node.op,ast.BitAnd) else x|y for x,y in itertools.product(a,b)}
        try:return visit(ast.parse(expr,mode='eval').body)
        except SyntaxError as exc:raise ValueError('Lua numeric alias invariants: invalid expression') from exc
    incoming={entry:{alias:{None} for alias in aliases}};pending=deque([entry]);visits=0;writes=set()
    while pending:
        k=pending.popleft();visits+=1;require(visits<50000,'unbounded flow');line,edges=nodes[k];state={a:set(v) for a,v in incoming[k].items()};assignment=assignment_pattern.fullmatch(line)
        if assignment:
            state[assignment[1]]=value(assignment[2],state);require(len(state[assignment[1]])<=64,'constant set widened');writes.add(k)
        compact=re.sub(r'[\s()]','',line);test=re.fullmatch(r'if('+alternatives+r')&(0x[0-9a-fA-F]+|\d+)~=0then',compact)
        for edge in edges:
            if edge is None:continue
            out={a:set(v) for a,v in state.items()}
            if test:
                require(k in branches,'missing conditional edge');yes,no=branches[k];require(yes!=no,'ambiguous branch');require(None not in out[test[1]],'uninitialized mask test');out[test[1]]={v for v in out[test[1]] if bool(v&int(test[2],0))==(edge==yes)}
                if not out[test[1]]:continue
            if edge not in incoming:incoming[edge]=out;pending.append(edge)
            else:
                merged={a:incoming[edge][a]|out[a] for a in aliases}
                if merged!=incoming[edge]:incoming[edge]=merged;pending.append(edge)
    facts=[]
    for region in regions:
        prefix=region['prefix'];require(prefix and source.count(prefix)==1,'ambiguous region');at=source.index(prefix);end=at+len(prefix);a=source[:at].count('\n');b=source[:end].count('\n');require(a in incoming and b in incoming,'unreachable/non-node region boundary')
        for boundary,expected in [('entry',region['entry']),('exit',region['exit'])]:
            for alias,constant in expected.items():require(alias in aliases and type(constant) is int and incoming[a if boundary=='entry' else b][alias]=={constant},'unproved '+boundary+' constant '+alias)
        facts.append(dict(entryLine=a+1,exitLine=b+1,entry={k:sorted(v,key=lambda x:-1 if x is None else x) for k,v in incoming[a].items()},exit={k:sorted(v,key=lambda x:-1 if x is None else x) for k,v in incoming[b].items()}))
    return dict(facts=facts,reachableNodes=len(incoming),worklistVisits=visits,assignmentLines=sorted(k+1 for k in writes),scope='Restricted generated Lua normal CFG;finite numeric locals and bit-test edges only,other predicates retain both alternatives.')
