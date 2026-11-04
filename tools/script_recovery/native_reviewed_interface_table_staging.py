"""Scratch candidate for sealed, unobservable interface-table cleanup staging."""
import hashlib,re
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,x86
from tools.script_recovery.annotate_interface_slots import VTABLE_BASE,load_slots
from tools.script_recovery.readable_lua import tokens

def classify(source,todo,fn,row,spec,image,snapshot_evidence,liveness,hoister,whole_liveness,*,copy_trace=None):
    if row is None:return source,list(todo),[]
    def require(ok,why):
        if not ok:raise ValueError('interface table staging '+fn['address']+': '+why)
    sha=lambda value:hashlib.sha256(value).hexdigest()
    require(getattr(image,'ok',False) and sha(image.data)==row['imageSha256'],'retail image changed')
    start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16)
    raw=image.bytes_at(start,end-start)
    require(raw is not None and sha(raw)==row['nativeSha256'],'native body changed')
    require(sha(fn['decompile'].encode())==row['sourceSha256'],'exported source changed')
    require(sha(source.encode())==row['luaPhaseSha256'],'Lua phase changed')
    require(row['abiContract']==dict(ordinaryMsvcCalleeSaved=True,callerFramePreserved=True,
            engineReentryVerified=False,exceptionsVerified=False,heapVerified=False),'unsupported ABI contract')
    require(all(word in row['scopeLimit'] for word in ['ABI','unverified','load-site']),'missing proof limits')
    require(re.fullmatch('[0-9a-f]{64}',row['acceptedEvidenceSha256']) is not None,'missing evidence seal')
    decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
    ins=list(decoder.disasm(raw,start));by={i.address:i for i in ins}
    require(ins and sum(i.size for i in ins)==len(raw),'incomplete native body')
    pointer_load,site,tail,call=(int(row[key],16) for key in ['pointerLoad','nativeRead','sharedTail','dispatch'])
    require(all(a in by for a in [pointer_load,site,tail,call]),'missing native endpoint')
    pointer,table,owner=row['pointerRegister'],row['tableRegister'],row['ownerRegister']
    require(pointer in ('ebx','ebp','esi','edi') and owner in ('ebx','ebp','esi','edi') and pointer!=owner,'invalid saved pointer/owner')
    require(table in ('eax','edx'),'unsupported table register')
    offset=4 if spec.entity else 0x40
    expected_load=pointer+', dword ptr ['+owner+' + '+(str(offset) if offset<10 else hex(offset))+']'
    require((by[pointer_load].mnemonic,by[pointer_load].op_str)==('mov',expected_load),'wrong interface load')
    provenance=snapshot_evidence(raw,start,interface_offset=offset)
    require(provenance['states'].get(site,{}).get(pointer)==('interface_snapshot',pointer_load),'unproved pointer load-site origin')
    sequence=[i for i in ins if site<=i.address<site+row['branchBytes']]
    expected=[('mov',table+', dword ptr ['+pointer+']'),('mov','ecx, '+pointer),('jmp',hex(tail))]
    require([(i.mnemonic,i.op_str) for i in sequence]==expected and sum(i.size for i in sequence)==row['branchBytes'],'table/receiver/branch changed')
    require(row['method']=='PauseAllNonScriptedEntities' and row['slot']=='0x5ec' and row['argument'] is False,'unsupported cleanup API')
    require(load_slots().get(0x5ec)==row['method'],'interface method changed')
    target=int(row['target'],16);witness=bytes.fromhex(row['calleeWitnessPrefix'])
    require(image.bytes_at(VTABLE_BASE+0x5ec,4)==target.to_bytes(4,'little'),'interface target changed')
    require(len(witness)>=16 and image.bytes_at(target,len(witness))==witness,'callee prefix changed')
    from tools.script_recovery.convert_quest_unit import callee_stack_words
    require(row['purgeWords']==1 and callee_stack_words(target)==1,'stack convention changed')
    tail_sequence=[i for i in ins if tail<=i.address<=call]
    require([(i.mnemonic,i.op_str) for i in tail_sequence]==[('push','0'),('call','dword ptr ['+table+' + 0x5ec]')],'shared false-pause tail changed')
    require(tail_sequence[0].address+tail_sequence[0].size==call,'noncontiguous tail')
    for i in ins:
        if i.group(x86.X86_GRP_JUMP):
            require(i.operands[0].type==x86.X86_OP_IMM,'unknown branch entry')
            dest=i.operands[0].imm
            require(not site<dest<site+row['branchBytes'] and not tail<dest<=call,'branch bypasses table or argument')
    alias,receiver=row['alias'],row['receiverAlias']
    require(all(re.fullmatch(r'[A-Za-z_]\w*',name) for name in [alias,receiver]) and alias!=receiver,'invalid aliases')
    diagnostic='unlifted: '+alias+' = *'+receiver
    require(row['diagnostic']==diagnostic and todo.count(diagnostic)==row['diagnosticMultiplicity'],'diagnostic multiplicity changed')
    fragment=row['nativeSourceFragment'];label='LAB_'+format(site,'08x');delegate='LAB_'+format(tail,'08x')
    require(fn['decompile'].count(fragment)==1 and re.fullmatch(re.escape(label)+r':\s*'+re.escape(alias)+r' = \*\(int \*\)'+re.escape(receiver)+r';\s*goto '+re.escape(delegate)+r';',fragment),'exported label/table/delegate changed')
    region=row['luaRegion'];require(source.count(region)==1,'ambiguous Lua region')
    pattern=re.compile(r'(?m)^(?P<indent>[ \t]*)-- '+label+r': \(native jump target\)\n(?P=indent)-- TODO\(native\): '+re.escape(diagnostic[10:])+r'\n(?P=indent)(?P<assignment>'+re.escape(alias)+r' = nil) --\[\[unresolved native value\]\]\n(?P=indent)-- TODO\(native\): goto '+delegate+r'\n')
    matches=list(pattern.finditer(region));require(len(matches)==1,'label/nil/delegate region changed')
    match=matches[0];offset=source.index(region);proof=liveness(source,offset+match.start('assignment'),alias)
    replacement=match[0].replace('-- TODO(native): '+diagnostic[10:],'-- reviewed native interface-table staging: '+diagnostic[10:],1).replace('--[[unresolved native value]]','--[[reviewed native interface-table staging]]',1)
    result=source[:offset+match.start()]+replacement+source[offset+match.end():]
    copy_reviews=[]
    if row.get('copies'):
        require(row['diagnosticMultiplicity']>=1+len(row['copies']),'too many reviewed copies')
        from tools.script_recovery.native_reviewed_tail_copy_staging import review_copies
        result,copy_reviews=review_copies(source,result,fn,row,image,copy_trace,liveness)
    significant=lambda text:[(t.lastgroup,t[0]) for t in tokens(text) if t.lastgroup not in ('space','comment','longcomment')]
    require(significant(result)==significant(source),'phase executable tokens changed')
    before,base_regions=hoister(source);after,new_regions=hoister(result)
    require(significant(before)==significant(after),'post-hoist executable tokens changed')
    require(json_normalized(new_regions)==json_normalized(base_regions),'post-hoist cleanup report changed')
    whole=whole_liveness(after,alias,'__cleanup_'+label)
    remaining=list(todo);remaining.remove(diagnostic)
    for _ in copy_reviews:remaining.remove(diagnostic)
    review=dict(nativeRead=row['nativeRead'],pointerLoad=row['pointerLoad'],dispatch=row['dispatch'],
        method=row['method'],argument=False,liveness=proof,postHoistLiveness=whole,executableLuaUnchanged=True,limits=row['scopeLimit'])
    if copy_reviews:review['copies']=copy_reviews
    return result,remaining,[review]

def json_normalized(value):
    import json
    return json.dumps(value,sort_keys=True).replace('reviewed native interface-table staging','unresolved native value')
