"""Route proven resource animation raw fifth bytes through the existing ABI helper."""
import hashlib,re
from tools.script_recovery.native_call_setup_ir import read_call_window
HELPER=(0x7e73d0,15,'b15918b5d8b85acb00708ee1f721e3297fac818c09fedf1a8ff14a30090a50bf')
RAW=('unsigned_byte',('memory',('constant',0x1375748)))
LITERALS={'0':0,'0x0':0,'false':0,'1':1,'0x1':1,'true':1}
def recover(source,fn,ordered,rdata):
    if not ordered:return source,[]
    if hashlib.sha256(rdata.bytes_at(HELPER[0],HELPER[1]) or b'').hexdigest()!=HELPER[2]:return source,[]
    changes=[];evidence=[]
    for pos,end,args,site,key,vt in ordered:
        if vt or str(site.get('target','')).lower()!='0x007e73d0' or len(args)!=9:continue
        if not re.fullmatch(r'(?:\((?:uint|byte|bool|char|undefined1)\))?\s*DAT_01375748',args[6].strip()):continue
        flags=[re.sub(r'^\((?:bool|uint|int|byte)\)\s*','',a.strip()) for a in args[2:6]+args[7:9]]
        if any(v not in LITERALS for v in flags):continue
        setup=read_call_window(rdata,int(fn['address'],16),fn['bodyExtent'],int(site['site'],16),argument_count=8)
        expected=[None,*[('constant',LITERALS[v]) for v in flags[:4]],RAW,*[('constant',LITERALS[v]) for v in flags[4:]]]
        if not setup or setup.target!=('constant',HELPER[0]) or len(setup.stack_arguments)!=8:continue
        if not all(value==expected[n] for n,value in enumerate(setup.stack_arguments) if n):continue
        if setup.ecx is None or setup.ecx[0]=='unknown':continue
        # The paired source head ends at args_start; retain the resource operand
        # and string slot. The lifter will require a tracked resource receiver.
        name=re.search(r'CScriptGameResourceObjectScriptedThingBase::\s*[_\w]*PlayAnimation[_\w]*\s*\($',source[:pos])
        if not name:continue
        new='CScriptGameResourceObjectScriptedThingBase::_PlayAnimationWithNativeArgument5_CScriptGameResourceObjectScriptedThingBase('
        changes.append((name.start(),end,new+', '.join(args[:6]+args[7:])))
        evidence.append({'site':site['site'],'function':fn['address'],'helper':hex(HELPER[0]),'raw_global':hex(0x1375748),'native_stack_arguments':setup.stack_arguments,'receiver':setup.ecx})
    for start,end,new in reversed(changes):source=source[:start]+new+source[end:]
    return source,evidence
