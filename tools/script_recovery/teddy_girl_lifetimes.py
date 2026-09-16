"""Native TeddyGirl control/movie/retained-Thing and presented CString scopes."""
from dataclasses import asdict
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from tools.script_recovery.teddy_girl_health import prove
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.native_resource_lifetime import check_resource_lifetimes

def inventory(data=None):
    data=data or RData();w=prove(data);decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
    instructions=list(decoder.disasm(data.bytes_at(w['mainAddress'],w['mainSize']),w['mainAddress']))
    roles={'0x99a380':('start','control'),'0x7e74d0':('end','control'),'0x99a430':('end','control'),
           '0xcd23b9':('use','control'),'0xcd2770':('use','control'),'0x7e7490':('use','control'),
           '0x7e7390':('use','control'),'0x7e7450':('use','control'),'0x7e7300':('use','control'),
           '0x6e7b60':('start','movie'),'0x6e7b80':('end','movie')}
    known={i.address:(0,'hero') for i in instructions if i.mnemonic=='call' and '+ 0x118]' in i.op_str}
    selections={}
    for n,ins in enumerate(instructions):
        if ins.mnemonic=='jmp' and ins.op_str in ('0xdafa96','0xdb05d5'):
            previous=instructions[n-1]
            if previous.mnemonic!='lea' or not previous.op_str.startswith('ecx, [esp + '):raise ValueError('TeddyGirl movie receiver selector changed')
            selections[previous.address]=('movie',previous.operands[1].mem.disp)
    selections[0xdb05ce]=('movie',184)
    selections[0xdafa8f]=('movie',200)
    events={};rows=[]
    special={0xdaf110:('start','thing',2),0xdafbd1:('start','thing',2),
             0xdb049c:('end','thing',0),0xdb05e7:('end','thing',0),0xdafc94:('end','thing',0),0xdb0589:('end','thing',0),
             0xdaf134:('start','presented',0),0xdb0445:('end','presented',0),0xdb05de:('end','presented',0)}
    for ins in instructions:
        if ins.mnemonic!='call':continue
        acquisition='+ 0x20]' in ins.op_str
        if ins.op_str not in roles and not acquisition and ins.address not in special:continue
        operation,kind,count=special[ins.address] if ins.address in special else ((*('use','control'),3) if acquisition else (*roles[ins.op_str],0))
        setup=read_call_window(data,w['mainAddress'],w['mainSize'],ins.address,known,argument_count=count)
        if setup is None:raise ValueError('TeddyGirl event setup missing '+hex(ins.address))
        identity=setup.stack_arguments[1] if acquisition else (setup.stack_arguments[0] if count==2 else setup.ecx)
        if ins.address==0xdaf0dc:
            if identity!=('stack',16):raise ValueError('TeddyGirl initial constructor prologue changed')
            identity=('stack',20)
        if ins.address==0xdaf262:kind='movie'
        if ins.address in (0xdafa96,0xdb05d5):resource=('register','ecx')
        elif identity[0]=='stack':resource=(kind,identity[1])
        else:raise ValueError('TeddyGirl resource identity not proved '+hex(ins.address)+' '+str(identity))
        events[ins.address]=(operation,resource);rows.append({'address':ins.address,'operation':operation,'object':resource,'setup':asdict(setup)})
    proof=check_resource_lifetimes(instructions,events,receiver_register='ecx',selections=selections)
    return {'nativeMainSha256':w['mainSha256'],'events':rows,'selections':selections,'cfgLifetimeProved':proof,
            'limits':['Temporary health Things are independently validated by x87/getter/destructor execution.',
                      'CFG ownership does not establish pause flags, callback errors or scheduler teardown.']}
