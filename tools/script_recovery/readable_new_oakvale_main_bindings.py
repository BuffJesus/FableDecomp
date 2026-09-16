"""Restore the missing sixteenth binding from the original Main caller."""
import hashlib
import json
import re
from tools.script_recovery.lift_native_lua import ROOT, RData

NATIVE_SHA = '12ffa76b8017eda86b362121f5d7c0995422368e43e94e03f8d38ba7be4e6198'
RAW_SCOPE_SHA = '19e04def088eeded99fe18a318100d88707fd85b3f16ad0cc1336936fae16ce3'


def lower(source, data=None):
    data = data or RData()
    if hashlib.sha256(data.bytes_at(0xdabac0, 2018)).hexdigest() != NATIVE_SHA:
        raise ValueError('New Oakvale Main native bytes changed')
    witness = json.loads((ROOT/'ghidra_out/script_recovery/new_oakvale_main_bindings_retail_bytes.json').read_text())
    if hashlib.sha256(data.bytes_at(0xdabac0, witness['bindingBlockSize'])).hexdigest().upper() != witness['bindingBlockSha256']:
        raise ValueError('New Oakvale binding witness changed')
    for binding in witness['bindings']:
        if data.string_at(int(binding['nameAddress'], 16)) != binding['name']:
            raise ValueError('New Oakvale binding name changed')
        site = int(binding['callbackStore'], 16)
        if data.bytes_at(site, 7) != b'\xc7\x47\x10'+int(binding['allocatorCallback'], 16).to_bytes(4, 'little'):
            raise ValueError('New Oakvale binding allocator changed')
    start = source.index('    if pCVar2 == nil then')
    end = source.index('    quest:FinalizeEntityBindings()', start)
    if hashlib.sha256(source[start:end].encode()).hexdigest() != RAW_SCOPE_SHA:
        raise ValueError('New Oakvale missing-binding draft changed')
    replacement = '    quest:AddEntityBinding("OVI_DeadFather", "NewOakValeIntro/Entities/OVI_DeadFather")\n'
    source = source[:start]+replacement+source[end:]
    old = 'quest:DeactivateQuest("Q__OakValeIntro_PostAttack", pCVar2)'
    if source.count(old) != 1: raise ValueError('New Oakvale deactivation draft changed')
    source = source.replace(old, 'quest:DeactivateQuest("Q__OakValeIntro_PostAttack", 0)')
    start = source.index('\nfunction Main('); end = source.index('\nfunction Init(', start)
    names = re.findall(r'quest:AddEntityBinding\("([^"]+)"', source[start:end])
    if names != [row['name'] for row in witness['bindings']] or len(names) != 16:
        raise ValueError('New Oakvale emitted binding order differs from native')
    return source, dict(status='sixteen ordered entity bindings recovered', nativeSha256=NATIVE_SHA,
        bindingNames=names, deactivationArgument=0,
        remaining=['FinalizeEntityBindings uses the runtime Lua allocator bridge; allocation failure and live scheduling need integration checks.',
                   'Main objective string scopes, initialization calls and spawned-thread lifecycle remain under review.'])
