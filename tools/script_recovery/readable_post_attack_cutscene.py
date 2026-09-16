"""Recover the post-attack movie scope while retaining unreviewed surrounding code."""
import hashlib
from pathlib import Path
from tools.script_recovery.lift_native_lua import RData

SOURCE_SHA = '3baaf396bf79086a067a17cdae6bc3603b2d8291289370fa1e4c83630e6347f6'
NATIVE_SHA = '8dc72f018eb5e9a030d02a244b9b86416b83ae59c0864a3db40e0b5e9a3a13e7'


def lower(source, data=None):
    data = data or RData()
    if hashlib.sha256(data.bytes_at(0xdbeb20, 1095)).hexdigest() != NATIVE_SHA:
        raise ValueError('PostAttackStuff native bytes changed')
    for slot, target in ((0x118, 0x891ca0), (0x20, 0x89b5b0), (0x5c8, 0x89b110),
                         (0x5ec, 0x890ab0), (0x5cc, 0x88e500)):
        if int.from_bytes(data.bytes_at(0x1260f0c + slot, 4), 'little') != target:
            raise ValueError('PostAttackStuff API slot changed')
    for address, literal in ((0x1255174, 'HERO'), (0x12d9dc8, 'CS_OAKVALEINTRO_HESDEADJIM')):
        if data.string_at(address) != literal:
            raise ValueError('PostAttackStuff cutscene literal changed')
    if data.bytes_at(0x122d70e, 1) != b'\0':
        raise ValueError('PostAttackStuff movie class changed')
    start = source.index('                quest:SetStateBool("DadFound", true)')
    end = source.index('                ppVar9 = quest:GetThingWithScriptName(', start)
    if hashlib.sha256(source[start:end].encode()).hexdigest() != SOURCE_SHA:
        raise ValueError('PostAttackStuff movie draft changed')
    replacement = '''                quest:WithRetailResources(function(resources)
                    playPostAttackDadCutscene(quest, resources)
                end)
'''
    source = source[:start] + replacement + source[end:]
    helper = Path(__file__).with_name('post_attack_cutscene.lua').read_text()
    if helper.count('\nreturn ') != 1: raise ValueError('PostAttackStuff helper export changed')
    index = source.index('\nfunction PostAttackStuff(') + 1
    source = source[:index] + helper.rsplit('\nreturn ', 1)[0] + '\n\n' + source[index:]
    return source, dict(status='disabled post-attack cutscene scope recovered',
        nativeSha256=NATIVE_SHA, rawScopeSha256=SOURCE_SHA,
        implementation='playPostAttackDadCutscene',
        adapterEvidence='work/post_attack_scope_runtime_checks/result.json; work/post_attack_resource_integration/registration-result.json',
        remaining=['Post-attack adapter has isolated compiled checks; complete movie scope through the merged engine owner remains pending.',
                   'Surrounding lookup/distance/quest state and full native function comparison remain pending.',
                   'Engine scheduling, persistence, game installation and live cutscene validation remain pending.'])
