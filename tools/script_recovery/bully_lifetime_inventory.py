"""Bully retail event inventory and path-sensitive lifetime checks; no Lua edits."""
import hashlib
import json
from dataclasses import asdict
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from tools.script_recovery.lift_native_lua import RData,ROOT
from tools.script_recovery.bully_initial_phases import recover
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.native_resource_lifetime import check_resource_lifetimes

def inventory(data=None):
    data=data or RData();_,w=recover(data)
    decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
    instructions=list(decoder.disasm(data.bytes_at(w['mainAddress'],w['mainSize']),w['mainAddress']))
    known={i.address:(0,'hero') for i in instructions if i.mnemonic=='call' and '+ 0x118]' in i.op_str}
    known.update({i.address:(8,'cstring') for i in instructions if i.mnemonic=='call' and i.op_str=='0x99ebf0'})
    roles={'0x99a380':('start','resource'),'0x7e72a0':('start','resource'),
        '0x7e74d0':('end','resource'),'0x99a430':('end','resource'),
        '0xcd23b9':('use','resource'),'0xcd2770':('use','resource'),
        '0x7e7490':('use','resource'),'0x7e7390':('use','resource'),
        '0x7e7450':('use','resource'),'0x7e72f0':('use','resource'),'0x7e73d0':('use','resource'),
        '0x6e7b60':('start','movie'),'0x6e7b80':('end','movie')}
    rows=[];events={};selections={}
    # Three predecessor LEAs supply the shared movie destructor at DBBD7E.
    for site,offset in ((0xdbb9dd,224),(0xdbbceb,200),(0xdbbd77,240)):
        instruction=next(i for i in instructions if i.address==site)
        if instruction.mnemonic!='lea' or instruction.op_str!=f'ecx, [esp + {hex(offset)}]':
            raise ValueError('Bully shared movie receiver selection changed')
        selections[site]=('movie',offset)
    for ins in instructions:
        if ins.mnemonic!='call':continue
        acquisition='+ 0x20]' in ins.op_str
        movie_start='+ 0x5c8]' in ins.op_str;pause='+ 0x5ec]' in ins.op_str
        if ins.op_str not in roles and not acquisition and not movie_start and not pause and ins.address not in (0xdbb56a,0xdbcce6):continue
        count=3 if acquisition else (2 if ins.address==0xdbb56a or movie_start else (1 if pause else 0))
        setup=read_call_window(data,w['mainAddress'],w['mainSize'],ins.address,known,argument_count=count)
        if setup is None:raise ValueError('Bully event setup unavailable '+hex(ins.address))
        if movie_start:
            operation,kind='use','movie';identity=setup.stack_arguments[1]
            constructor=max(i.address for i in instructions if i.address<ins.address and i.mnemonic=='call' and i.op_str=='0x99ebf0')
            key=read_call_window(data,w['mainAddress'],w['mainSize'],constructor,argument_count=2)
            if key is None or key.stack_arguments!=(('constant',0x122d70e),('constant',0xffffffff)) or data.bytes_at(0x122d70e,1)!=b'\0':
                raise ValueError('Bully native movie key changed')
        elif pause:
            flag=setup.stack_arguments[0]
            if flag not in (('constant',0),('constant',1)):raise ValueError('Bully pause flag not literal')
            operation,kind=('start' if flag[1] else 'end'),'pause';identity=('stack',0)
        elif acquisition:
            operation,kind='use','resource';identity=setup.stack_arguments[1]
        elif ins.address==0xdbb56a:
            operation,kind='start','victim';identity=setup.stack_arguments[0]
        elif ins.address==0xdbcce6:
            operation,kind='end','victim';identity=setup.ecx
        else:
            operation,kind=roles[ins.op_str];identity=setup.ecx
        if ins.address==0xdbbd7e:resource=('register','ecx')
        elif identity[0]=='stack':resource=(kind,identity[1])
        else:raise ValueError('Bully event identity not proved '+hex(ins.address)+' '+str(identity))
        events[ins.address]=(operation,resource)
        rows.append({'address':ins.address,'operation':operation,'object':resource,'setup':asdict(setup)})
    result=check_resource_lifetimes(instructions,events,receiver_register='ecx',selections=selections)
    return {'events':rows,'cfgLifetimeProved':result,'selections':selections,
            'nativeMainSha256':w['mainSha256'],'limits':['Stack offsets are reviewed native frame locals; no implication of Lua local scope or runtime scheduler teardown.','No generated actor changes; movie input-map/actor ownership and raw pause policy still require independent consumer proof.']},instructions,events,selections

if __name__=='__main__':
    report,_,_,_=inventory();out=ROOT/'work/bully_converter';out.mkdir(exist_ok=True)
    (out/'LIFETIME_INVENTORY.json').write_text(json.dumps(report,indent=2)+'\n')
    print('CFG lifetime proof:',report['cfgLifetimeProved'],'events:',len(report['events']))
