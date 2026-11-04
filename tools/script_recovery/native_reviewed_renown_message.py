"""Recover the retail renown level message table indexed by hero level."""
import hashlib
from capstone import Cs,CS_ARCH_X86,CS_MODE_32

OLD = 'quest:HeroReceiveMessageFromGuildMaster("", "TEXT_QST_080_RENOWN_LEVEL_SEVEN", true, true)'
EXPECTED = ['TEXT_QST_080_RENOWN_LEVEL_'+x for x in ('ONE','TWO','THREE','FOUR','FIVE','SIX','SEVEN')]


def restore(source, fn, row, rdata):
    if row is None:
        return source
    def require(ok, reason):
        if not ok:
            raise ValueError('renown message table: '+reason)
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest()==row['sourceSha256'],'source changed')
    begin,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16)
    raw=rdata.bytes_at(begin,end-begin)
    require(hashlib.sha256(raw).hexdigest()==row['nativeSha256'],'native body changed')
    require(row['messages']==EXPECTED and all(fn['decompile'].count('"'+key+'"')==1 for key in EXPECTED),
            'native message literals changed')
    require('&CStack_24 + iVar2' in fn['decompile'] and 'if (iVar2 < 8)' in fn['decompile'],
            'native indexed message changed')
    indirect={x['site'].lower():x for x in fn['indirectCalls']}
    require(indirect.get(row['renownCallSite'],{}).get('slot')=='0x448', 'message call changed')
    ins={i.address:(i.mnemonic,i.op_str) for i in Cs(CS_ARCH_X86,CS_MODE_32).disasm(raw,begin)}
    expected={
        0x00cf0343:('call','dword ptr [edx + 0x288]'),
        0x00cf034c:('mov','edi, eax'),
        0x00cf0385:('cmp','edi, 8'),
        0x00cf03ba:('lea','eax, [esp + edi*4 + 0x20]'),
        0x00cf03bf:('call','dword ptr [edx + 0x448]'),
    }
    require(all(ins.get(site)==value for site,value in expected.items()),'index instructions changed')
    require(source.count(OLD)==1 and source.count('iVar2 = quest:GetHeroRenownLevel()')==1,
            'lifted message source changed')
    header='function Tutorial_RenownLevelIncrease(quest)\n'
    require(source.count(header)==1,'lifted function header changed')
    table='    local renownMessages = {\n'+''.join(
        '        ['+str(i)+'] = "'+key+'",\n' for i,key in enumerate(EXPECTED,1))+'    }\n'
    source=source.replace(header,header+table,1)
    new='quest:HeroReceiveMessageFromGuildMaster("", renownMessages[iVar2] or "TEXT_QST_080_RENOWN_LEVEL_SEVEN", true, true)'
    return source.replace(OLD,new,1)
