"""Classify dead Lua staging backed by an already reviewed resource dispatch."""
import hashlib
import re
from capstone import Cs, CS_ARCH_X86, CS_MODE_32, x86
from tools.script_recovery.reviewed_resource_operands import reviewed_resource_operands
from tools.script_recovery.readable_lua import tokens


def classify(source, todo, fn, row, parent, spec, rdata, resource_evidence, liveness):
    if row is None:
        return source, list(todo), []
    def require(ok, message):
        if not ok:
            raise ValueError('resource dispatch staging '+fn['address']+': '+message)
    digest=lambda data:hashlib.sha256(data).hexdigest()
    start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16)
    raw=rdata.bytes_at(start,end-start)
    require(raw is not None and digest(raw)==row['nativeExtentSha256'],'native extent changed')
    require(digest(source.encode())==row['luaPhaseSha256'],'lifted phase changed')
    inferred=resource_evidence(rdata,fn,spec,include_pause=True,include_resource_table_loads=True)
    require(parent is not None,'missing parent operand evidence')
    accepted=reviewed_resource_operands(fn,parent,rdata,inferred)
    decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
    instructions=list(decoder.disasm(raw,start));by={i.address:i for i in instructions}
    edits=[];reviews=[];remaining=list(todo)
    reads=row['reads'];require(bool(reads),'empty staging evidence')
    require(len({read['nativeRead'] for read in reads})==len(reads),'duplicate native read')
    for read in reads:
        site=int(read['nativeRead'],16);dispatch=int(read['dispatch'],16)
        calls=[call for call in parent['calls'] if int(call['dispatch']['site'],16)==dispatch]
        require(len(calls)==1 and dispatch in accepted,'missing unique reviewed dispatch')
        call=calls[0];fact=inferred.get(site)
        if read.get('reviewedMemberLifetime', False):
            require(read['reviewedMemberLifetime'] is True, 'invalid reviewed lifetime opt-in')
            require(call['family']=='self' and call['offset']>0, 'unsupported reviewed member')
            lifetime=call.get('receiverLifetime')
            require(lifetime is not None and int(lifetime['tableLoad']['site'],16)==site,
                    'reviewed lifetime load mismatch')
            from tools.script_recovery.native_register_snapshot_provenance import analyze
            states=analyze(raw,start,member_offsets=(call['offset'],))['states']
            load=by.get(site)
            require(load is not None and len(load.operands)==2 and load.operands[1].type==x86.X86_OP_MEM,
                    'invalid reviewed member load')
            receiver=load.reg_name(load.operands[1].mem.base)
            require(states.get(site,{}).get(receiver)==('member_address',call['offset']),
                    'unproved reviewed member origin')
            fact=dict(family=call['family'],offset=call['offset'],kind='resource_vtable_load',
                      receiverRegister=receiver,tableRegister=load.reg_name(load.operands[0].reg),
                      provenance='reviewed receiver lifetime and independent entry-this member origin')
        require(fact is not None and fact.get('kind')=='resource_vtable_load','unproved table load')
        require(fact['family']==call['family'] and fact['offset']==call['offset'],'resource mismatch')
        load=by.get(site);spill=by.get(int(call['vtableStack'][0]['site'],16))
        require(load is not None and spill is not None,'missing load/spill instruction')
        require(load.mnemonic=='mov' and load.operands[0].type==x86.X86_OP_REG and load.operands[0].size==4 and load.operands[1].type==x86.X86_OP_MEM and load.operands[1].size==4,'invalid load')
        memory=load.operands[1].mem
        require(memory.disp==0 and not memory.index and not memory.segment,'not direct resource table')
        register=load.operands[0].reg
        require(spill.operands[1].type==x86.X86_OP_REG and spill.operands[1].reg==register,'wrong saved table register')
        middle=[i for i in instructions if site<i.address<spill.address]
        sequence=[load]+middle+[spill]
        require(site<spill.address<dispatch and all(a.address+a.size==b.address for a,b in zip(sequence,sequence[1:])),'noncontiguous table lifetime')
        # The linear load/spill interval must have only its normal entry.
        # A branch from elsewhere can bypass the load even when no intervening
        # instruction writes its register. Reject such entries conservatively.
        for instruction in instructions:
            if instruction.group(x86.X86_GRP_JUMP):
                require(instruction.operands[0].type == x86.X86_OP_IMM,
                        'unknown entry into table lifetime')
                require(not site < instruction.operands[0].imm <= spill.address,
                        'branch entry into table lifetime')
        families={'eax':{'eax','ax','ah','al'},'ebx':{'ebx','bx','bh','bl'},'ecx':{'ecx','cx','ch','cl'},'edx':{'edx','dx','dh','dl'},'esi':{'esi','si'},'edi':{'edi','di'},'ebp':{'ebp','bp'}}
        names=families.get(load.reg_name(register));require(names is not None,'unsupported table register')
        for instruction in middle:
            require(not instruction.group(x86.X86_GRP_JUMP) and not instruction.group(x86.X86_GRP_CALL) and not instruction.group(x86.X86_GRP_RET) and not instruction.group(x86.X86_GRP_INT),'control transfer in table lifetime')
            require(not {instruction.reg_name(r) for r in instruction.regs_access()[1]}.intersection(names),'table register overwritten')
        fragment=read['nativeSourceFragment']
        require(bool(fragment) and fn['decompile'].count(fragment)==1,'ambiguous exported fragment')
        diagnostic=read['diagnostic'];alias=read['alias']
        require(re.fullmatch(r'[A-Za-z_]\w*',alias) is not None and diagnostic.startswith('unlifted: '+alias+' = '),'invalid diagnostic alias')
        require(remaining.count(diagnostic)==1,'diagnostic multiplicity changed')
        comment_only=read.get('commentOnly',False)
        require(type(comment_only) is bool,'invalid comment-only opt-in')
        if comment_only:
            pattern=re.compile(r'(?m)^(?P<indent>[ \t]*)-- TODO\(native\): '+re.escape(diagnostic[10:])+r'\n')
            matches=list(pattern.finditer(source));require(len(matches)==1,'ambiguous Lua staging')
            match=matches[0];marker=match['indent']+alias+' = nil\n'
            analysis_source=source[:match.end()]+marker+source[match.end():]
            proof=liveness(analysis_source,match.end()+len(match['indent']),alias)
            proof=dict(proof,analysisOnlyAssignment=True)
            replacement=match[0].replace('-- TODO(native): ','-- reviewed native dispatch staging: ',1)
        else:
            pattern=re.compile(r'(?m)^[ \t]*-- TODO\(native\): '+re.escape(diagnostic[10:])+r'\n[ \t]*(?P<assignment>'+re.escape(alias)+r' = nil) --\[\[unresolved native value\]\]\n')
            matches=list(pattern.finditer(source));require(len(matches)==1,'ambiguous Lua staging')
            match=matches[0];proof=liveness(source,match.start('assignment'),alias)
            segment=match[0];replacement=segment[segment.index('\n')+1:].replace('--[[unresolved native value]]','--[[reviewed native dispatch staging]]')
        edits.append((match.start(),match.end(),replacement));remaining.remove(diagnostic)
        reviews.append(dict(nativeRead=read['nativeRead'],dispatch=read['dispatch'],resource=fact,liveness=proof,executableLuaUnchanged=True))
    require(all(a[1]<=b[0] for a,b in zip(sorted(edits),sorted(edits)[1:])),'overlapping edits')
    out=source
    for begin,end,replacement in sorted(edits,reverse=True):out=out[:begin]+replacement+out[end:]
    significant=lambda text:[(t.lastgroup,t[0]) for t in tokens(text) if t.lastgroup not in ('space','comment','longcomment')]
    require(significant(source)==significant(out),'executable Lua changed')
    return out,remaining,reviews
