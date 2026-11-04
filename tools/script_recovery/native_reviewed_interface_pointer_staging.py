"""Review dead GSI receiver staging only beside a sealed, already restored API call."""
import hashlib,re
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,x86
from tools.script_recovery.annotate_interface_slots import VTABLE_BASE,load_slots
from tools.script_recovery.readable_lua import tokens

def classify(source,todo,fn,row,spec,image,resource_evidence,liveness):
 if row is None:return source,list(todo),[]
 def require(ok,why):
    if not ok:raise ValueError('interface pointer staging '+fn['address']+': '+why)
 digest=lambda data:hashlib.sha256(data).hexdigest()
 require(getattr(image,'ok',False) and digest(image.data)==row['imageSha256'],'retail image changed')
 start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16);raw=image.bytes_at(start,end-start)
 require(raw is not None and digest(raw)==row['nativeSha256'],'native extent changed')
 require(digest(fn['decompile'].encode())==row['sourceSha256'],'exported source changed')
 require(digest(source.encode())==row['luaPhaseSha256'],'lifted phase changed')
 require('prefix' in row['scopeLimit'] and 'unverified' in row['scopeLimit'] and 'ABI' in row['scopeLimit'],'missing proof limits')
 require(re.fullmatch('[0-9a-f]{64}',row['acceptedEvidenceSha256']) is not None,'missing bounded evidence seal')
 decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
 ins=list(decoder.disasm(raw,start));by={i.address:i for i in ins}
 require(ins and sum(i.size for i in ins)==len(raw),'incomplete native body')
 slot=int(row['slot'],16);target=int(row['target'],16)
 require(slot==0x5ec and row['method']=='PauseAllNonScriptedEntities' and load_slots().get(slot)==row['method'],'unsupported API')
 require(image.bytes_at(VTABLE_BASE+slot,4)==target.to_bytes(4,'little'),'interface target changed')
 witness=bytes.fromhex(row['calleeWitnessPrefix'])
 require(len(witness)>=16 and image.bytes_at(target,len(witness))==witness,'callee prefix changed')
 from tools.script_recovery.convert_quest_unit import callee_stack_words
 require(row['purgeWords']==1 and callee_stack_words(target)==1,'API stack convention changed')
 site,call=(int(row[key],16) for key in ('nativeRead','dispatch'))
 require(site in by and call in by and site<call,'missing native prefix endpoints')
 load=by[site];require(load.mnemonic=='mov' and len(load.operands)==2,'not a pointer load')
 dest,mem=load.operands;offset=4 if spec.entity else 0x40
 require(dest.type==x86.X86_OP_REG and dest.size==4 and mem.type==x86.X86_OP_MEM and mem.size==4
    and mem.mem.disp==offset and not mem.mem.index and not mem.mem.segment,'wrong interface field')
 pointer,owner=load.reg_name(dest.reg),load.reg_name(mem.mem.base)
 require(pointer in ('ebx','ebp','esi','edi') and owner in ('ebx','ebp','esi','edi') and pointer!=owner,'unanchored pointer/owner registers')
 require(any(i.mnemonic=='mov' and i.op_str==owner+', ecx' for i in ins[:12]),'missing entry thiscall origin')
 sequence=[i for i in ins if site<=i.address<=call]
 require(len(sequence)==6 and all(a.address+a.size==b.address for a,b in zip(sequence,sequence[1:])),'noncontiguous interface prefix')
 table=sequence[1].reg_name(sequence[1].operands[0].reg) if sequence[1].operands and sequence[1].operands[0].type==x86.X86_OP_REG else ''
 require(table in ('eax','edx') and table!=pointer,'unsupported volatile table register')
 argument=row['argument'];require(type(argument) is bool,'not a native boolean argument')
 save=sequence[4]
 require(save.mnemonic=='mov' and len(save.operands)==2 and save.operands[0].type==x86.X86_OP_MEM
    and save.operands[0].mem.base==x86.X86_REG_ESP and not save.operands[0].mem.index and not save.operands[0].mem.segment,'not a direct stack cache')
 cache_disp=save.operands[0].mem.disp
 require(row['cachePrefixEspOffset']==cache_disp-4 and row['cachePrefixEspOffset']>=0,'prefix-relative cache coordinate changed')
 expected=[('mov',pointer+', dword ptr ['+owner+' + '+hex(offset)+']' if offset>=10 else pointer+', dword ptr ['+owner+' + '+str(offset)+']'),
   ('mov',table+', dword ptr ['+pointer+']'),('push',str(int(argument))),('mov','ecx, '+pointer),
   ('mov','dword ptr [esp + '+hex(cache_disp)+'], '+pointer),('call','dword ptr ['+table+' + 0x5ec]')]
 require([(i.mnemonic,i.op_str) for i in sequence]==expected,'load/table/argument/cache/dispatch shape changed')
 facts=resource_evidence(image,fn,spec,include_pause=True);fact=facts.get(call)
 require(fact is not None and fact['family']=='interface' and fact['offset']==0 and fact['slot']==slot,'unproved interface receiver')
 # The existing CFG model forgets pending pushes at the intervening cache
 # write. The sealed contiguous prefix proves the argument independently.
 require(not fact['pushes'] or len(fact['pushes'])==1 and tuple(fact['pushes'][0])==('imm',int(argument)),
    'disagreeing API argument fact')
 for i in ins:
    if i.group(x86.X86_GRP_JUMP):
       require(i.operands[0].type==x86.X86_OP_IMM,'unknown prefix entry')
       require(not site<i.operands[0].imm<=call,'branch bypasses pointer load')
 alias,copy=row['alias'],row['copyAlias']
 require(all(re.fullmatch('[A-Za-z_]\\w*',name) for name in (alias,copy)) and alias!=copy,'invalid pointer aliases')
 diagnostic=row['diagnostic'];require(diagnostic=='unlifted: '+alias+' = *(this + '+str(offset)+')','diagnostic/field mismatch')
 require(todo.count(diagnostic)==1,'diagnostic multiplicity changed')
 fragment=row['nativeSourceFragment'];require(bool(fragment) and fn['decompile'].count(fragment)==1
    and alias+' = ' in fragment and 'this + '+str(offset) in fragment,'ambiguous exported pointer fragment')
 region=row['luaRegion'];require(source.count(region)==1,'ambiguous Lua pointer region')
 api='quest:'+row['method']+'('+str(argument).lower()+')'
 pattern=re.compile(r'(?m)^[ \t]*-- TODO\(native\): '+re.escape(diagnostic[10:])+r'\n[ \t]*(?P<assignment>'+re.escape(alias)+
    r' = nil) --\[\[unresolved native value\]\]\n[ \t]*'+re.escape(copy+' = '+alias)+r'\n[ \t]*'+re.escape(api)+r'\n')
 matches=list(pattern.finditer(region));require(len(matches)==1,'nil/copy/restored API span changed')
 match=matches[0];begin=source.index(region)+match.start();end=source.index(region)+match.end()
 proof=liveness(source,source.index(region)+match.start('assignment'),alias)
 replacement=match[0].replace('-- TODO(native): ','-- reviewed native interface-pointer staging: ',1).replace(
    '--[[unresolved native value]]','--[[reviewed native interface-pointer staging]]',1)
 out=source[:begin]+replacement+source[end:]
 significant=lambda text:[(token.lastgroup,token[0]) for token in tokens(text) if token.lastgroup not in ('space','comment','longcomment')]
 require(significant(out)==significant(source),'executable Lua changed')
 remaining=list(todo);remaining.remove(diagnostic)
 return out,remaining,[dict(nativeRead=row['nativeRead'],dispatch=row['dispatch'],interfaceOffset=offset,
    cachePrefixEspOffset=row['cachePrefixEspOffset'],method=row['method'],argument=argument,liveness=proof,
    executableLuaUnchanged=True,limits=row['scopeLimit'])]
