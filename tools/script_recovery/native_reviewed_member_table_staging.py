"""Classify alias-free table comments backed by a replayed member speech call."""
import hashlib,json,re
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,x86 as x
from tools.script_recovery.native_reviewed_member_speech import restore
from tools.script_recovery.native_member_resource_calls import resource_call_evidence
from tools.script_recovery.readable_lua import tokens

def significant(text):return [(t.lastgroup,t[0]) for t in tokens(text) if t.lastgroup not in ('space','comment','longcomment')]
def classify(source,todo,fn,row,speech_input,parent,spec,image,unit,owner):
    if row is None:return source,list(todo),[]
    def require(ok,why):
        if not ok:raise ValueError('reviewed member table staging '+fn['address']+': '+why)
    digest=lambda data:hashlib.sha256(data).hexdigest()
    require(owner==row['owner'] and owner in unit.get('entities',{}),'owner changed')
    entity=unit['entities'][owner]
    require(entity['vtable'].lower()==row['entityVtable'].lower() and entity['functions']['Main']['address'].lower()==fn['address'].lower(),'owner/function changed')
    start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16);raw=image.bytes_at(start,end-start)
    require(raw is not None and digest(raw)==row['nativeExtentSha256'],'native extent changed')
    require(digest(fn['decompile'].encode())==row['exportedSourceSha256'],'exported source changed')
    require(digest(source.encode())==row['luaPhaseSha256'],'Lua phase changed')
    require(digest(speech_input.encode())==row['speechInputSha256'],'normalized speech input changed')
    require(parent is not None and digest(json.dumps(parent,sort_keys=True,separators=(',',':')).encode())==row['parentReviewSha256'],'parent review changed')
    # Re-run the existing independent operand/receiver/stack replay, including
    # original borrowed-string helper and wrapper ABI checks. Do not treat a
    # prior report or a matching text fragment as native call evidence.
    restored=restore(speech_input,fn,parent,image,spec)
    require(restored!=speech_input,'parent restoration did not occur')
    md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;instructions=list(md.disasm(raw,start));by={i.address:i for i in instructions}
    require(instructions and sum(i.size for i in instructions)==len(raw),'incomplete native extent')
    facts=resource_call_evidence(image,fn,spec,include_pause=True,include_resource_table_loads=True)
    reviews=[];edits=[];remaining=list(todo);reads=row['reads']
    require(reads and len({r['nativeRead'] for r in reads})==len(reads),'empty/duplicate native reads')
    for read in reads:
        site,dispatch=int(read['nativeRead'],16),int(read['dispatch'],16)
        matches=[call for call in parent['calls'] if int(call['site'],16)==dispatch]
        require(len(matches)==1,'missing unique reviewed speech');call=matches[0]
        require(int(call['start'],16)<=site<dispatch,'read outside reviewed speech window')
        fact=facts.get(site)
        require(fact is not None and fact.get('kind')=='resource_vtable_load' and fact['family']=='parent' and fact['offset']==call['offset'],'native resource table role changed')
        require(spec.parent_resources.get(call['offset'])==call['resource'],'resource classification changed')
        load=by.get(site);require(load is not None and load.mnemonic=='mov' and len(load.operands)==2 and load.operands[0].type==x.X86_OP_REG and load.operands[1].type==x.X86_OP_MEM and load.operands[1].size==load.operands[0].size==4,'not a dword table load')
        require(load.operands[1].mem.disp==call['offset'] and not load.operands[1].mem.index and not load.operands[1].mem.segment,'table address changed')
        for i in instructions:
            if i.group(x.X86_GRP_JUMP):
                require(i.operands[0].type==x.X86_OP_IMM,'unknown normal entry')
                require(not site<i.operands[0].imm<=dispatch,'branch enters speech setup')
        alias=read['alias'];require(re.fullmatch(r'[A-Za-z_]\w*',alias) is not None,'invalid Lua alias')
        require(not [t for t in tokens(source) if t.lastgroup=='identifier' and t[0]==alias],'Lua alias has executable presence')
        require(read['nativeSourceFragment'] and fn['decompile'].count(read['nativeSourceFragment'])==1,'exported assignment changed')
        assignment=read['normalizedAssignment'];require(re.fullmatch(re.escape(alias)+r' = .+;',assignment) is not None and speech_input.count(assignment)==restored.count(assignment)==1,'normalized assignment changed')
        def checked_assignment(fragment,name):
            pattern=(re.escape(name)+r' = \*\((?:int|uint|undefined4|CCharString_bv) \*\)'
                +r'\(\*\(int \*\)\(\(int\)this \+ (0x[0-9a-fA-F]+|[0-9]+)\) \+ (0x[0-9a-fA-F]+|[0-9]+)\);')
            match=re.fullmatch(pattern,fragment)
            return match is not None and int(match[1],0)==spec.parent_off and int(match[2],0)==call['offset']
        require(checked_assignment(assignment,alias),'normalized table path changed')
        require(re.fullmatch(r'[A-Za-z_]\w*',read['exportedAlias']) is not None and checked_assignment(read['nativeSourceFragment'],read['exportedAlias']),'exported table path changed')
        call_pattern=r'^\(\*\*\(code \*\*\)\(\(int\)'+re.escape(alias)+r' \+ 0x34\)\)\('
        require(call['old'].count(alias)==1 and re.match(call_pattern,call['old']) and call['old'] not in restored,'reviewed source use changed')
        diagnostic=read['diagnostic']
        expected=('unlifted: '+alias+' = *(int *)(*(int *)(this + '+hex(spec.parent_off)+') + '+hex(call['offset'])+');')
        require(diagnostic==expected and remaining.count(diagnostic)==1,'diagnostic changed')
        pattern=re.compile(r'(?m)^[ \t]*-- TODO\(native\): '+re.escape(diagnostic[10:])+r'\n')
        found=list(pattern.finditer(source));require(len(found)==1,'ambiguous comment')
        match=found[0];edits.append((match.start(),match.end(),match[0].replace('-- TODO(native): ','-- reviewed native member speech table: ',1)))
        remaining.remove(diagnostic)
        reviews.append(dict(nativeRead=read['nativeRead'],dispatch=read['dispatch'],resource=fact,
            reviewedSourceCall=call['old'],alias=alias,executableAliasTokens=0,commentOnly=True,
            proof='Existing member-speech operand/receiver replay independently repeated; inferred parent resource table load and unique exported/normalized assignments checked. No Lua alias identifier exists; no assignment emitted.',
            limits='Normal native child ABI and valid object/string storage contracts. No active heap/resource lifetime, exceptions/reentry/yield/task polling or gameplay proof.'))
    require(all(a[1]<=b[0] for a,b in zip(sorted(edits),sorted(edits)[1:])),'overlapping comments')
    out=source
    for a,b,replacement in sorted(edits,reverse=True):out=out[:a]+replacement+out[b:]
    require(significant(out)==significant(source),'executable Lua changed')
    return out,remaining,reviews
