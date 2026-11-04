"""Review dead saved-table annotations under sealed native/ordinary-ABI evidence."""
import hashlib,re
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,x86 as x
from tools.script_recovery.annotate_interface_slots import VTABLE_BASE,load_slots
from tools.script_recovery.native_cache_epoch_proof import prove
from tools.script_recovery.native_cache_call_contracts import resolve
from tools.script_recovery.native_register_snapshot_provenance import analyze
from tools.script_recovery.readable_lua import tokens

def classify(source,todo,fn,row,spec,image,copy_trace,liveness,hoister):
    if row is None:return source,list(todo),[]
    def require(ok,why):
        if not ok:raise ValueError('saved table staging '+fn['address']+': '+why)
    sha=lambda data:hashlib.sha256(data).hexdigest()
    require(getattr(image,'ok',False) and sha(image.data)==row['imageSha256'],'retail image changed')
    require(sha(fn['decompile'].encode())==row['sourceSha256'],'exported source changed')
    require(sha(source.encode())==row['luaPhaseSha256'],'Lua phase changed')
    require(all(word in row['scopeLimit'] for word in ['ABI','in-bounds','unverified']),'missing evidence limits')
    require(all(re.fullmatch('[0-9a-f]{64}',row[k]) for k in ['cacheEvidenceSha256','prefixEvidenceSha256']),'missing evidence seals')
    require(spec.entity and row['cacheProof']['interfaceOffset']==4,'unsupported owner interface')
    start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16);raw=image.bytes_at(start,end-start)
    require(raw is not None and sha(raw)==row['cacheProof']['nativeSha256'],'native body changed')
    from tools.script_recovery.convert_quest_unit import callee_stack_words
    contracts,call_reviews=resolve(image,raw,start,row,callee_stack_words)
    native=prove(raw,start,row['cacheProof'],contracts,analyze,image=image) if row['cacheProof'].get('branchTables') else prove(raw,start,row['cacheProof'],contracts,analyze)
    md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;ins=list(md.disasm(raw,start));by={i.address:n for n,i in enumerate(ins)}
    pointer_read=int(row['cacheProof']['pointerRead'],16);table_read=int(row['cacheProof']['tableRead'],16)
    tail,dispatch=int(row['sharedTail'],16),int(row['dispatch'],16);table=row['cacheProof']['tableRegister']
    label='LAB_'+format(pointer_read,'08x');delegate='LAB_'+format(tail,'08x')
    require(pointer_read in by and table_read in by and tail in by and dispatch in by,'missing cleanup endpoint')
    shape=row.get('readShape','labeled_jump')
    require(shape in ('labeled_jump','guarded_fallthrough'),'unsupported read shape')
    guarded=shape=='guarded_fallthrough'
    branch=ins[by[pointer_read]:by[pointer_read]+(2 if guarded else 3)]
    require(len(branch)==(2 if guarded else 3) and branch[1].address==table_read and sum(i.size for i in branch)==row['readBranchBytes']
        and (branch[-1].address+branch[-1].size==tail if guarded else branch[-1].mnemonic=='jmp' and branch[-1].op_str==hex(tail)),
        'cached table branch changed')
    api=row['api'];slot=int(api['slot'],16);target=int(api['target'],16)
    require(api['method']=='PauseAllNonScriptedEntities' and slot==0x5ec and load_slots().get(slot)==api['method'],'cleanup API changed')
    require(image.bytes_at(VTABLE_BASE+slot,4)==target.to_bytes(4,'little') and callee_stack_words(target)==1,'cleanup target/ABI changed')
    prefix=bytes.fromhex(api['prefix']);require(len(prefix)>=16 and image.bytes_at(target,len(prefix))==prefix,'cleanup callee changed')
    require([(i.mnemonic,i.op_str) for i in ins[by[tail]:by[dispatch]+1]]==[('push','0'),('call','dword ptr ['+table+' + '+hex(slot)+']')],'false cleanup dispatch changed')
    table_targets={}
    if row['cacheProof'].get('branchTables'):
        from tools.script_recovery.native_verified_branch_tables import verify
        table_targets=verify(raw,start,image,row['cacheProof']['branchTables'])['targets']
    for i in ins:
        if i.group(x.X86_GRP_JUMP) or i.mnemonic=='call' and i.operands[0].type==x.X86_OP_IMM:
            if i.operands[0].type==x.X86_OP_IMM:destinations=(i.operands[0].imm,)
            else:
                require(i.address in table_targets,'unverified indirect branch')
                destinations=table_targets[i.address]
            for dest in destinations:
                require(not pointer_read<dest<pointer_read+row['readBranchBytes'] and not tail<dest<=dispatch,'branch bypasses reload/table/argument')
    predicate=row['predicate'];decoded={}
    for key in ['wrapper','accessor','tail']:
        witness=predicate[key];a=int(witness['address'],16);data=bytes.fromhex(witness['bytes'])
        require(0<len(data)<=256 and image.bytes_at(a,len(data))==data,'predicate '+key+' changed')
        decoded[key]=list(md.disasm(data,a));require(sum(i.size for i in decoded[key])==len(data),'incomplete predicate '+key)
    wrapper,accessor,pred_tail=(int(predicate[k]['address'],16) for k in ['wrapper','accessor','tail'])
    require([(i.mnemonic,i.op_str) for i in decoded['wrapper']]==[('mov','eax, dword ptr [ecx]'),('call','dword ptr [eax + 0xc]'),('mov','ecx, eax'),('jmp',hex(pred_tail))],'predicate wrapper shape')
    require([(i.mnemonic,i.op_str) for i in decoded['accessor']]==[('mov','eax, dword ptr [ecx + '+hex(predicate['accessorOffset'])+']'),('ret','')],'predicate parent shape')
    require(image.bytes_at(int(predicate['vtable'],16)+0xc,4)==accessor.to_bytes(4,'little'),'predicate parent slot')
    require([(i.mnemonic,i.op_str) for i in decoded['tail']]==[
        ('mov','eax, dword ptr [ecx + '+hex(predicate['stateOffset'])+']'),('test','eax, eax'),('jne',hex(pred_tail+10)),
        ('xor','al, al'),('ret',''),('mov','al, byte ptr [eax + '+str(predicate['flagOffset'])+']'),('ret','')],'predicate flag shape')
    states=analyze(raw,start,branch_tables=row['cacheProof']['branchTables'],image=image)['states'] if row['cacheProof'].get('branchTables') else analyze(raw,start)['states'];incoming=[]
    for n,i in enumerate(ins):
        if i.mnemonic=='jne' and i.op_str==hex(pointer_read):
            require(n>=2,'missing predicate call');call,test=ins[n-2:n]
            require((call.mnemonic,call.op_str)==('call',hex(wrapper)) and (test.mnemonic,test.op_str)==('test','al, al')
                and states[call.address].get('ecx')==('entry_this',),'predicate caller/receiver changed')
            incoming.append(dict(call=hex(call.address),branch=hex(i.address),bytes=b''.join(bytes(j.bytes) for j in [call,test,i]).hex()))
    expected=[{k:r[k] for k in ['call','branch','bytes']} for r in row['nativePredicateBranches']]
    require(incoming==expected and len(incoming)==1+len(row['copies']),'native incoming branch inventory changed')
    alias,cache,source_cache=row['alias'],row['cacheAlias'],row['sourceCacheAlias']
    require(all(re.fullmatch(r'[A-Za-z_]\w*',a) for a in [alias,cache,source_cache]) and alias!=cache,'invalid aliases')
    fragment=row['nativeSourceFragment']
    if guarded:
        guard,receiver=row['sourceConditionAlias'],row['sourceReceiverAlias']
        require(all(re.fullmatch(r'[A-Za-z_]\w*',a) for a in [guard,receiver]) and receiver not in [alias,source_cache],'invalid guarded source aliases')
        source_pattern=(re.escape(guard)+r' = '+re.escape(row['sourcePredicate'])+r'\(this\);\s*if \('+re.escape(guard)+r'\) \{\s*'
            +re.escape(alias)+r' = \*'+re.escape(source_cache)+r';\s*'+re.escape(receiver)+r' = '+re.escape(source_cache)+r';\s*goto '+re.escape(delegate)+r';\s*\}')
    else:source_pattern=re.escape(label)+r':\s*'+re.escape(alias)+r' = \*\(int \*\)'+re.escape(source_cache)+r';\s*goto '+re.escape(delegate)+r';'
    require(fn['decompile'].count(fragment)==1 and re.fullmatch(source_pattern,fragment),'unique exported read/delegate changed')
    diagnostic='unlifted: '+alias+' = *'+cache
    require(todo.count(diagnostic)==row['diagnosticMultiplicity']==1+len(row['copies']),'diagnostic multiplicity changed')
    require(isinstance(row['copies'],list) and (len(row['copies'])==0 if guarded else 0<len(row['copies'])<=8),'invalid copy inventory')
    require(isinstance(copy_trace,list) and (not copy_trace if guarded else bool(copy_trace)),'missing actual copy trace')
    result=source;reviews=[];regions=[row['originalLuaRegion']]+[c['luaRegion'] for c in row['copies']]
    require(len(set(regions))==len(regions),'duplicate Lua regions')
    original_pattern=re.compile(r'(?m)^(?P<indent>[ \t]*)-- '+label+r': \(native jump target\)\n(?P=indent)-- TODO\(native\): '+re.escape(diagnostic[10:])+r'\n(?P=indent)(?P<assignment>'+re.escape(alias)+r' = nil) --\[\[unresolved native value\]\]\n(?P=indent)-- TODO\(native\): goto '+delegate+r'\n')
    if guarded:
        condition,alive=row['luaConditionAlias'],row['luaAliveAlias']
        require(all(re.fullmatch(r'[A-Za-z_]\w*',a) for a in [condition,alive]) and len({condition,alive,alias,cache})==4,'invalid guarded Lua aliases')
        original_pattern=re.compile(r'(?m)^(?P<indent>[ \t]*)'+re.escape(alive)+r' = not quest:IsActiveThreadTerminating\(\)\n'
            +r'(?P=indent)'+re.escape(condition)+r' = not '+re.escape(alive)+r'\n'
            +r'(?P=indent)if '+re.escape(condition)+r' then\n'
            +r'(?P=indent)    -- TODO\(native\): '+re.escape(diagnostic[10:])+r'\n'
            +r'(?P=indent)    (?P<assignment>'+re.escape(alias)+r' = nil) --\[\[unresolved native value\]\]\n'
            +r'(?P=indent)    -- TODO\(native\): goto '+re.escape(delegate)+r'\n(?P=indent)end\n')
    for n,region in enumerate(regions):
        require(source.count(region)==1,'ambiguous Lua region')
        if n==0:match=original_pattern.fullmatch(region)
        else:
            copy=row['copies'][n-1];event=copy['copyTrace'];condition,movie=copy['conditionAlias'],copy['movieAlias']
            require(sum(actual==event for actual in copy_trace)==1 and event['target']==label and event['terminated'] is True and event['syntheticExit'] is None,'actual copy identity changed')
            require(event['originalJump'].strip()=='if ('+condition+') goto '+label+';','copied jump guard changed')
            require(all(re.fullmatch('[0-9a-f]{64}',event[k]) for k in ['inputSha256','preparedInputSha256','outputSha256'])
                and event['outputStartIndex']>=0,'invalid copy coordinates/phase seals')
            expected=['if ('+condition+') {',alias+' = *(int *)'+cache+';','GSI->'+api['method']+'(0);','RESOURCE_DestroyMovie('+movie+');','return;','}']
            require([s.strip() for s in event['copiedStatements']]==expected and event['outputStatementCount']==len(expected),'copied operation order changed')
            pattern=re.compile(r'(?m)^(?P<indent>[ \t]*)if '+re.escape(condition)+r' then\n(?P=indent)    -- TODO\(native\): '+re.escape(diagnostic[10:])+r'\n(?P=indent)    (?P<assignment>'+re.escape(alias)+r' = nil) --\[\[unresolved native value\]\]\n(?P=indent)    quest:'+api['method']+r'\(false\)\n(?P=indent)    resources:DestroyMovie\('+re.escape(movie)+r'\)\n(?P=indent)    return\n(?P=indent)end\n')
            match=pattern.fullmatch(region)
        require(match is not None,'Lua read/guard/cleanup shape changed')
        proof=liveness(source,source.index(region)+match.start('assignment'),alias)
        replacement=region.replace('-- TODO(native): '+diagnostic[10:],'-- reviewed native saved-table staging: '+diagnostic[10:],1).replace('--[[unresolved native value]]','--[[reviewed native interface-table staging]]',1)
        result=result.replace(region,replacement,1);reviews.append(dict(kind='original' if n==0 else 'actual_tail_copy',liveness=proof))
    significant=lambda text:[(t.lastgroup,t[0]) for t in tokens(text) if t.lastgroup not in ('space','comment','longcomment')]
    require(significant(source)==significant(result),'executable phase changed')
    before,old_regions=hoister(source);after,new_regions=hoister(result)
    require(significant(before)==significant(after),'executable post-hoist output changed')
    normalize=lambda value:__import__('json').dumps(value,sort_keys=True).replace('reviewed native interface-table staging','unresolved native value')
    require(normalize(old_regions)==normalize(new_regions),'cleanup region operations changed')
    if guarded:
        require(after.count(alias+' = nil --[[reviewed native interface-table staging]]')==1,'post-hoist staging identity changed')
        from tools.script_recovery.lua_terminal_cleanup_alias import prove_staging_before_terminal_cleanup
        post_liveness=prove_staging_before_terminal_cleanup(after,after.index(alias+' = nil --[[reviewed native interface-table staging]]'),alias,'__cleanup_'+delegate)
        reviews[0]['postHoistLiveness']=post_liveness
    remaining=list(todo)
    for _ in regions:remaining.remove(diagnostic)
    return result,remaining,[dict(nativeRead=hex(table_read),cacheStore=row['cacheProof']['cacheStore'],nativeCache=native,
        callContracts=call_reviews,aliases=reviews,executableLuaUnchanged=True,limits=row['scopeLimit'])]
