"""Retail Guard control/movie event inventory and CFG ownership validation."""
from dataclasses import asdict
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from tools.script_recovery.guard_health import prove
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.native_resource_lifetime import check_resource_lifetimes

def inventory(data=None):
    data=data or RData();w=prove(data)
    decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
    instructions=list(decoder.disasm(data.bytes_at(w['mainAddress'],w['mainSize']),w['mainAddress']))
    roles={'0x99a380':('start','resource'),'0x7e74d0':('end','resource'),'0x99a430':('end','resource'),
           '0xcd23b9':('use','resource'),'0xcd2770':('use','resource'),'0x7e7490':('use','resource'),
           '0x7e7390':('use','resource'),'0x7e7450':('use','resource'),'0x7e7320':('use','resource'),
           '0x6e7b60':('start','movie'),'0x6e7b80':('end','movie')}
    known={i.address:(0,'hero') for i in instructions if i.mnemonic=='call' and '+ 0x118]' in i.op_str}
    events={};rows=[];selections={}
    for site,offset in ((0xdadd9c,32),(0xdaddb2,32),(0xdaddc5,32),(0xdade2e,100)):
        ins=next(i for i in instructions if i.address==site)
        if ins.mnemonic!='lea' or ins.op_str!=f'ecx, [esp + {hex(offset)}]':raise ValueError('Guard shared movie selection changed')
        selections[site]=('movie',offset)
    for site in (0xdaca0f,0xdada50,0xdade37):
        ins=next(i for i in instructions if i.address==site)
        if ins.mnemonic!='lea' or ins.op_str!='ecx, [esp + 0x10]':raise ValueError('Guard resource receiver changed')
        selections[site]=('resource',16)
    for ins in instructions:
        if ins.mnemonic!='call':continue
        acquisition='+ 0x20]' in ins.op_str
        if ins.op_str not in roles and not acquisition:continue
        setup=read_call_window(data,w['mainAddress'],w['mainSize'],ins.address,known,argument_count=3 if acquisition else 0)
        if setup is None:raise ValueError('Guard event setup missing '+hex(ins.address))
        operation,kind=('use','resource') if acquisition else roles[ins.op_str]
        identity=setup.stack_arguments[1] if acquisition else setup.ecx
        if ins.address==0xdac776:
            # This window includes the prologue; normalize to the post-prologue
            # ESP used by all other call windows and the native health fixtures.
            if identity!=('stack',-328):raise ValueError('Guard prologue resource offset changed')
            identity=('stack',16)
        if ins.address==0xdadbef:kind='movie' # inline derived movie constructor follows base ctor
        if ins.address in (0xdade32,0xdaca22,0xdada5a,0xdade3b):resource=('register','ecx')
        elif identity[0]=='stack':resource=(kind,identity[1])
        else:raise ValueError('Guard event identity missing '+hex(ins.address)+' '+str(identity))
        events[ins.address]=(operation,resource)
        rows.append({'address':ins.address,'operation':operation,'object':resource,'setup':asdict(setup)})
    result=check_resource_lifetimes(instructions,events,receiver_register='ecx',selections=selections)
    return {'nativeMainSha256':w['mainSha256'],'events':rows,'cfgLifetimeProved':result,
            'selections':selections,'limits':['CFG tracks object lifetime, not pause flags, engine callback errors or scheduler teardown.']}
