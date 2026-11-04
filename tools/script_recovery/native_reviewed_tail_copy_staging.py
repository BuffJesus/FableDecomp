"""Review a dead table alias copied by the actual native sibling-tail pass."""
import hashlib,re
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,x86

def review_copies(source,result,fn,row,image,copy_trace,liveness):
    def require(ok,why):
        if not ok:raise ValueError('interface table copy '+fn['address']+': '+why)
    copies=row['copies'];alias,receiver=row['alias'],row['receiverAlias']
    require(len(re.findall(r'\b'+re.escape(alias)+r'\s*=\s*[^;\r\n]*\b'+re.escape(receiver)+r'\b[^;\r\n]*;',fn['decompile']))==1,'source read origin is not unique')
    require(isinstance(copies,list) and 0<len(copies)<=8,'invalid copy inventory')
    require(isinstance(copy_trace,list) and copy_trace,'missing actual lifter copy provenance')
    require(len({c['luaRegion'] for c in copies})==len(copies) and len({c['nativeBranch'] for c in copies})==len(copies),'duplicate copy region/branch')
    decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
    start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16)
    ins=list(decoder.disasm(image.bytes_at(start,end-start),start));by={i.address:n for n,i in enumerate(ins)}
    read=int(row['nativeRead'],16);label='LAB_'+format(read,'08x');reviews=[]
    for copy in copies:
        require(re.fullmatch('[0-9a-f]{64}',copy['acceptedEvidenceSha256']) is not None,'missing copy evidence seal')
        require(all(word in copy['scopeLimit'] for word in ['seeded','unverified']),'missing copy limits')
        condition,movie=copy['conditionAlias'],copy['movieAlias']
        require(all(re.fullmatch(r'[A-Za-z_]\w*',name) for name in [condition,movie]),'invalid copy aliases')
        event=copy['copyTrace'];require(sum(actual==event for actual in copy_trace)==1,'copy trace missing or ambiguous')
        require(event['target']==label and event['terminated'] is True and event['syntheticExit'] is None,'not a terminal copy of native read')
        require(event['originalJump'].strip()=='if ('+condition+') goto '+label+';','copied condition/jump changed')
        expected=['if ('+condition+') {',alias+' = *(int *)'+receiver+';',
                  'GSI->'+row['method']+'(0);','RESOURCE_DestroyMovie('+movie+');','return;','}']
        require([line.strip() for line in event['copiedStatements']]==expected,'copied operations/order changed')
        require(event['outputStatementCount']==len(expected) and event['outputStartIndex']>=0,'invalid copy coordinates')
        require(all(re.fullmatch('[0-9a-f]{64}',event[key]) for key in ['inputSha256','preparedInputSha256','outputSha256']),'missing C phase identities')
        branch=int(copy['nativeBranch'],16);require(branch in by and by[branch]>=3,'missing original predicate branch')
        caller=ins[by[branch]-3:by[branch]+1];predicate=copy['predicate']
        wrapper_start=int(predicate['wrapper']['address'],16);accessor_start=int(predicate['accessor']['address'],16);tail_start=int(predicate['tail']['address'],16)
        expected_caller=[('mov','ecx, '+row['ownerRegister']),('call',hex(wrapper_start)),('test','al, al'),('jne',hex(read))]
        require([(i.mnemonic,i.op_str) for i in caller]==expected_caller and all(a.address+a.size==b.address for a,b in zip(caller,caller[1:])),'native predicate/branch changed')
        caller_witness=copy['nativeCaller'];caller_bytes=bytes.fromhex(caller_witness['bytes'])
        require(int(caller_witness['address'],16)==caller[0].address and caller_bytes==b''.join(bytes(i.bytes) for i in caller),'caller evidence window changed')
        decoded={}
        for key in ['wrapper','accessor','tail']:
            witness=predicate[key];address=int(witness['address'],16);raw=bytes.fromhex(witness['bytes'])
            require(0<len(raw)<=256 and image.bytes_at(address,len(raw))==raw,'predicate '+key+' bytes changed')
            body=list(decoder.disasm(raw,address));require(sum(i.size for i in body)==len(raw),'incomplete predicate '+key)
            decoded[key]=body
        require([(i.mnemonic,i.op_str) for i in decoded['wrapper']]==[
            ('mov','eax, dword ptr [ecx]'),('call','dword ptr [eax + 0xc]'),('mov','ecx, eax'),('jmp',hex(tail_start))],'predicate wrapper shape changed')
        require([(i.mnemonic,i.op_str) for i in decoded['accessor']]==[
            ('mov','eax, dword ptr [ecx + '+hex(predicate['accessorOffset'])+']'),('ret','')],'predicate accessor shape changed')
        require(image.bytes_at(int(predicate['vtable'],16)+0xc,4)==accessor_start.to_bytes(4,'little'),'predicate accessor vtable identity changed')
        require([(i.mnemonic,i.op_str) for i in decoded['tail']]==[
            ('mov','eax, dword ptr [ecx + '+hex(predicate['stateOffset'])+']'),('test','eax, eax'),('jne',hex(tail_start+10)),
            ('xor','al, al'),('ret',''),('mov','al, byte ptr [eax + '+str(predicate['flagOffset'])+']'),('ret','')],'predicate flag/null shape changed')
        region=copy['luaRegion'];require(source.count(region)==result.count(region)==1,'copy Lua region missing or ambiguous')
        pattern=re.compile(r'(?m)^(?P<indent>[ \t]*)if '+re.escape(condition)+r' then\n(?P=indent)    -- TODO\(native\): '+re.escape(alias+' = *'+receiver)+r'\n(?P=indent)    (?P<assignment>'+re.escape(alias)+r' = nil) --\[\[unresolved native value\]\]\n(?P=indent)    quest:'+re.escape(row['method'])+r'\(false\)\n(?P=indent)    resources:DestroyMovie\('+re.escape(movie)+r'\)\n(?P=indent)    return\n(?P=indent)end\n')
        match=pattern.fullmatch(region);require(match is not None,'copy Lua guard/nil/API/return changed')
        proof=liveness(source,source.index(region)+match.start('assignment'),alias)
        replacement=region.replace('-- TODO(native): ','-- reviewed native interface-table staging: ',1).replace('--[[unresolved native value]]','--[[reviewed native interface-table staging]]',1)
        result=result.replace(region,replacement,1)
        reviews.append(dict(nativeBranch=copy['nativeBranch'],nativeRead=row['nativeRead'],copyTarget=event['target'],
                            inputSha256=event['inputSha256'],outputSha256=event['outputSha256'],liveness=proof,
                            originalGuardAndOperationsRetained=True,limits=copy['scopeLimit']))
    return result,reviews
