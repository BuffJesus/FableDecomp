"""Preserve the native idle-loop backedge after a BordelloLady conversation."""
import hashlib


POLL_ENTRY = '''        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        while true do
            if bVar5 then'''
CLEANUP = '''            ::FLOW_past_lab_00e40044::
            -- TODO(native): (*(code *)(*pppuVar28)[0x17b])();
            resources:DestroyMovie(xStack_190)'''
CLONED_HIT = '''            cVar6 = me:MsgIsHitByHero()
            if not cVar6 then
                bVar5 = me:MsgIsHitByAnySpecialAbilityFromHero()
                if bVar5 then
                    bVar5 = me:MsgIsHitByHeroSpecialAbility(0xe)
                    if not bVar5 then goto LAB_00e40183_c2 end
                end
            end
            goto FLOW_past_lab_00e40183_c2
            ::LAB_00e40183_c2::
            ::FLOW_past_lab_00e40183_c2::
                -- TODO(native): xStack_15c = uVar15 & 0xfffffffd;
            if 1 ~= 0 then'''
# The general tail copier now retains the native hero-hit else itself.
CLONED_HIT_WITH_ELSE = CLONED_HIT.replace(
    '                end\n            end\n            goto FLOW_past',
    '                end\n            else\n                goto LAB_00e40183_c2\n            end\n            goto FLOW_past', 1)
BACKEDGE = '''            ::LAB_00e402f9_c2::
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            goto FLOW_after_lab_00e40085'''


def restore(source, fn, row, rdata):
    if row is None:
        return source

    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed BordelloLady return {fn["address"]}: {why}')

    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    raw = rdata.bytes_at(start, end-start)
    require(raw is not None and hashlib.sha256(raw).hexdigest() == row['nativeSha256'],
            'native body changed')
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'],
            'exported source changed')
    require(set(row['nativeWindows']) == {'0x00e3ec80', '0x00e4006c', '0x00e400eb', '0x00e402f9'},
            'native window sites changed')
    for site, encoded in row['nativeWindows'].items():
        expected = bytes.fromhex(encoded)
        require(rdata.bytes_at(int(site, 16), len(expected)) == expected,
                f'native window changed at {site}')
    require('LADY_POLL' not in source and 'ladyHitAfterConversation' not in source,
            'restored identifiers already present')
    require(source.count(CLONED_HIT) + source.count(CLONED_HIT_WITH_ELSE) == 1,
            'lifted hit region changed')
    # Normalize exactly the independently restored native else before applying
    # the already reviewed hit flag/backedge repair; no broader drift is accepted.
    if CLONED_HIT_WITH_ELSE in source:
        source = source.replace(CLONED_HIT_WITH_ELSE, CLONED_HIT, 1)
    for block in (POLL_ENTRY, CLEANUP, CLONED_HIT, BACKEDGE, '    local alive = true\n'):
        require(source.count(block) == 1, 'lifted return path changed')
    source = source.replace('    local alive = true\n',
                            '    local ladyHitAfterConversation\n    local alive = true\n', 1)
    source = source.replace(POLL_ENTRY, POLL_ENTRY.replace('        while true do',
                            '        ::LADY_POLL::\n        while true do'), 1)
    source = source.replace(CLEANUP, CLEANUP.replace(
        '            -- TODO(native): (*(code *)(*pppuVar28)[0x17b])();',
        '            quest:PauseAllNonScriptedEntities(false)'), 1)
    hit = '            ladyHitAfterConversation = false\n' + CLONED_HIT
    hit = hit.replace('                end\n            end\n            goto FLOW_past',
                      '                end\n            else\n                goto LAB_00e40183_c2\n            end\n            goto FLOW_past', 1)
    hit = hit.replace('            ::LAB_00e40183_c2::\n',
                      '            ::LAB_00e40183_c2::\n            ladyHitAfterConversation = true\n', 1)
    hit = hit.replace('if 1 ~= 0 then', 'if ladyHitAfterConversation then')
    source = source.replace(CLONED_HIT, hit, 1)
    # The copied sibling tail must recheck interaction at E3EC80. Falling
    # through the enclosing while's exit starts a new movie without a talk.
    source = source.replace(BACKEDGE, BACKEDGE.replace('goto FLOW_after_lab_00e40085',
                            'if bVar5 then goto LAB_00e3f744 end\n            goto LADY_POLL'), 1)
    return source
