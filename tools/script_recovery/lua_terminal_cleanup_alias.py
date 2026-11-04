"""Check a zero-argument cleanup closure writes an alias only on terminal paths."""
import re
from tools.script_recovery.readable_lua import tokens

def prove_terminal_cleanup_alias(source,alias,cleanup):
    def require(ok,why):
        if not ok:raise ValueError('terminal cleanup alias: '+why)
    require(all(re.fullmatch(r'[A-Za-z_]\w*',name) for name in [alias,cleanup]),'invalid identifiers')
    clean=''.join(re.sub(r'[^\r\n]',' ',t[0]) if t.lastgroup in ('comment','longcomment','string','longstring') else t[0] for t in tokens(source))
    definition=re.compile(r'(?m)^    local function (?P<name>[A-Za-z_]\w*)\(\)\n(?P<body>(?:        [^\n]*\n)+)    end\n')
    closures=list(definition.finditer(clean));target=[m for m in closures if m['name']==cleanup]
    require(len(target)==1,'missing unique straight-line zero-argument closure')
    body=target[0]['body'];writes=0
    require(not re.search(r'\b'+re.escape(cleanup)+r'\b',body),'recursive or escaping cleanup')
    for line in body.splitlines():
        require(not re.match(r'\s*(?:local|function|if|while|for|repeat|until|return|goto|do|end)\b',line),'unsupported closure control')
        if re.search(r'\b'+re.escape(alias)+r'\b',line):
            require(re.fullmatch(r'\s*'+re.escape(alias)+r' = nil\s*',line) is not None,'observable closure alias use')
            writes+=1
    require(writes==1,'missing unique nil staging write')
    for m in closures:
        if m['name']!=cleanup:
            require(not re.search(r'\b'+re.escape(alias)+r'\b',m['body']),'other closure captures alias')
            require(not re.search(r'\b'+re.escape(cleanup)+r'\b',m['body']),'other closure captures cleanup')
    outer=definition.sub('',clean)
    require(not re.search(r'\bfunction\b','\n'.join(outer.splitlines()[1:])),'unparsed nested closure')
    lines=outer.splitlines();calls=[]
    for n,line in enumerate(lines):
        if not re.search(r'\b'+re.escape(cleanup)+r'\b',line):continue
        inline=re.fullmatch(r'\s*if (?:not )?[A-Za-z_]\w* then '+re.escape(cleanup)+r'\(\); return end\s*',line)
        require(len(re.findall(r'\b'+re.escape(cleanup)+r'\b',line))==1,'multiple closure references')
        direct=re.fullmatch(r'\s*'+re.escape(cleanup)+r'\(\)\s*',line)
        if direct:
            following=next((s.strip() for s in lines[n+1:] if s.strip()),None)
            require(following=='return','nonterminal direct call')
        require(inline or direct,'closure escapes, rebinds or has unsupported invocation')
        calls.append(n+1)
    require(calls,'no cleanup invocations')
    return dict(alias=alias,cleanup=cleanup,nilWritesInClosure=writes,terminalCallLines=calls,
                otherAliasCaptures=0,scope='normal zero-argument closure return paths only; exceptions/reentry unverified')


def prove_staging_before_terminal_cleanup(source,assignment_offset,alias,cleanup):
    """Model terminal calls to straight-line closures that cannot read the alias.

    This checks normal Lua value use only. External callbacks,exceptions and
    engine reentry remain unverified;no executable source is rewritten.
    """
    from tools.script_recovery.lua_staging_liveness import _masked,prove_unused_staging
    import hashlib
    def require(ok,why):
        if not ok:raise ValueError('terminal staging cleanup: '+why)
    require(all(re.fullmatch(r'[A-Za-z_]\w*',name) for name in [alias,cleanup]),'invalid identifiers')
    require(type(assignment_offset)is int and 0<=assignment_offset<len(source),'invalid assignment offset')
    clean=_masked(source)
    definition=re.compile(r'(?m)^    local function (?P<name>[A-Za-z_]\w*)\(\)\n(?P<body>(?:        [^\n]*\n)+)    end\n')
    closures=list(definition.finditer(clean));names=[m['name'] for m in closures]
    require(len(names)==len(set(names)) and names.count(cleanup)==1,'missing or duplicate cleanup definition')
    alias_word=r'\b'+re.escape(alias)+r'\b'
    for m in closures:
        body=m['body'];require(not re.search(alias_word,body),'closure captures staging alias')
        for line in body.splitlines():
            require(not re.match(r'\s*(?:local|function|if|while|for|repeat|until|return|goto|do|end)\b',line),'unsupported closure control')
        require(not any(re.search(r'\b'+re.escape(name)+r'\b',body) for name in names),'closure refers to another cleanup')
    blank=lambda text:re.sub(r'[^\r\n]',' ',text)
    outer=definition.sub(lambda m:blank(m[0]),clean)
    require(not re.search(r'\bfunction\b','\n'.join(outer.splitlines()[1:])),'unparsed nested closure')
    require(not any(m.start()<=assignment_offset<m.end() for m in closures),'staging assignment inside closure')
    lines=outer.splitlines(True);at=source[:assignment_offset].count('\n')
    require(re.fullmatch(r'\s*'+re.escape(alias)+r' = nil\s*',lines[at]) is not None,'expected staging assignment')
    following=[(n,l.strip()) for n,l in enumerate(lines[at+1:],at+1) if l.strip()]
    require(following and (following[0][1]==cleanup+'(); return' or following[0][1]==cleanup+'()' and len(following)>1 and following[1][1]=='return'),'staging does not immediately enter terminal cleanup')
    call_count=0
    for n,line in enumerate(lines):
        mentioned=[name for name in names if re.search(r'\b'+re.escape(name)+r'\b',line)]
        if not mentioned:continue
        require(len(mentioned)==1,'multiple cleanup references');name=mentioned[0]
        require(len(re.findall(r'\b'+re.escape(name)+r'\b',line))==1,'duplicate cleanup reference')
        plain=line.rstrip('\r\n');indent=plain[:len(plain)-len(plain.lstrip())]
        inline=re.fullmatch(r'\s*if (?P<condition>(?:not )?[A-Za-z_]\w*) then '+re.escape(name)+r'\(\); return end\s*',plain)
        if inline:replacement=indent+'if '+inline['condition']+' then return end'
        elif plain.strip()==name+'(); return':replacement=indent+'return'
        elif plain.strip()==name+'()':
            next_line=next((l.strip() for l in lines[n+1:] if l.strip()),None)
            require(next_line=='return','nonterminal cleanup call');replacement=indent+'return'
        else:require(False,'cleanup escapes or rebinds')
        require(len(replacement)<=len(plain),'projection length')
        lines[n]=replacement+' '*(len(plain)-len(replacement))+line[len(plain):];call_count+=1
    projected=''.join(lines);require(len(projected)==len(source),'projection coordinates changed')
    proof=prove_unused_staging(projected,assignment_offset,alias)
    return dict(alias=alias,cleanup=cleanup,straightLineClosures=len(closures),terminalCalls=call_count,closureAliasCaptures=0,liveness=proof,
        projectionSha256=hashlib.sha256(projected.encode()).hexdigest(),scope='Normal terminal closure calls only; external callbacks,exceptions and engine reentry unverified. Source unchanged.')
