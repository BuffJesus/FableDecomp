"""Verify the suffix CString retained through the Villager talk conversation."""
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from tools.script_recovery.native_villager_control_resource import verify as verify_control
from tools.script_recovery.native_resource_lifetime import check_single_resource_lifetime
from tools.script_recovery.native_call_setup_ir import read_call_window


def verify(data):
    control=verify_control(data)
    literals={0x12D86E4:'_MALE',0x12D86DC:'_FEMALE',
              0x12D86B4:'TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS',
              0x12D8694:'TEXT_QST_048_VILLAGER_SPOKEN_TO'}
    if any(data.string_at(address)!=text for address,text in literals.items()):
        raise ValueError('Villager talk key literal changed')
    decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
    instructions=list(decoder.disasm(data.bytes_at(control['address'],control['size']),control['address']))
    at={i.address:i for i in instructions}
    # All instructions are pinned by the owning-control witness. These call
    # identities and receiver selections distinguish the suffix from temporaries.
    calls={0xDAE3F7:0x99E4B0,0xDAE43D:0x99EFE0,0xDAE52A:0x99F570,
           0xDAE580:0x99F570,0xDAE602:0x99EAE0,0xDAEA44:0x99EAE0}
    for site,target in calls.items():
        if at[site].mnemonic!='call' or at[site].op_str!=hex(target):raise ValueError('Villager talk key call changed')
    identity=('stack',24)
    for site,constructor,output in ((0xDAE52A,0xDAE51F,52),(0xDAE580,0xDAE575,60)):
        setup=read_call_window(data,control['address'],control['size'],site,{constructor:(8,'prefix')},argument_count=1)
        if (setup is None or setup.ecx!=('stack',output) or setup.edx!=('result',constructor,'prefix')
                or setup.stack_arguments!=(identity,)):
            raise ValueError('Villager suffix concatenation operands changed')
    events={site:('start' if site==0xDAE3F7 else 'end' if site in (0xDAE602,0xDAEA44) else 'use',identity) for site in calls}
    selections={site:identity for site in (0xDAE416,0xDAE42E,0xDAEA40)}
    for site in selections:
        if at[site].mnemonic!='lea' or at[site].op_str!='ecx, [esp + 0x18]':raise ValueError('Villager talk cleanup receiver changed')
    if not check_single_resource_lifetime(instructions,events,receiver_register='ecx',selections=selections):
        raise ValueError('Villager talk suffix lifetime changed')
    return dict(status='verified-talk-suffix-lifetime',controlSha256=control['sha256'],
                identity=identity,events={hex(k):v for k,v in events.items()},literals={hex(k):v for k,v in literals.items()},
                remaining='Concatenation result/prefix temporary order and host owned-string composition remain.')
