"""Use BordelloLady's stack resource for acquisition and cleanup."""
import hashlib

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG, X86_REG_ESP


SOURCE_CHANGES = (
    ('me:AcquireControl(4)', 'resources:TryAcquire(ladyResource, me, 4)', 5),
    ('me:AcquireControl(8)', 'resources:TryAcquire(ladyResource, me, 4)', 1),
    ('resources:PrepareResource(xStack_1a0)',
     'resources:PrepareResource(ladyResource)', 5),
    ('resources:PrepareResource(xStack_1a8)',
     'resources:PrepareResource(ladyResource)', 1),
)
MOVIE_RELEASE = ('-- TODO(native): (*(code *)(*pppuVar28)[0x17b])();\n'
                 '            resources:ReleaseResource(xStack_1a0)')
# callOrder label pairing (2026-10-02) gets this site's destructor kind right but keeps Ghidra's operand
MOVIE_RELEASE_PAIRED = ('-- TODO(native): (*(code *)(*pppuVar28)[0x17b])();\n'
                        '            resources:DestroyMovie(xStack_1a0)')
MOVIE_DESTROY = ('-- TODO(native): (*(code *)(*pppuVar28)[0x17b])();\n'
                 '            resources:DestroyMovie(xStack_190)')
HIT_POLL = '''            cVar6 = me:MsgIsHitByHero()
            if not cVar6 then
                bVar5 = me:MsgIsHitByAnySpecialAbilityFromHero()
                if bVar5 then
                    bVar5 = me:MsgIsHitByHeroSpecialAbility(0xe)
                    if not bVar5 then goto LAB_00e40183 end
                end
            else
                goto LAB_00e40183
            end
            goto FLOW_past_lab_00e40183
            ::LAB_00e40183::
            ::FLOW_past_lab_00e40183::'''
HIT_TEST = '''            ::FLOW_past_lab_00e40183::
                -- TODO(native): xStack_15c = uVar15 & 0xfffffffd;
            if 1 ~= 0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if not bVar5 then
                    resources:PrepareResource(resources:MemberResource("seh_Whore"))'''


def restore(source, fn, row, rdata):
    if row is None:
        return source

    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed BordelloLady resource lifetime {fn["address"]}: {why}')

    start = int(fn['address'], 16)
    end = int(fn['bodyEndExclusive'], 16)
    raw = rdata.bytes_at(start, end - start)
    require(raw is not None and hashlib.sha256(raw).hexdigest() == row['nativeSha256'],
            'native body changed')
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'],
            'exported source changed')
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(raw, start))
    require(instructions and instructions[-1].address + instructions[-1].size == end,
            'native disassembly incomplete')
    indexed = {ins.address: ins for ins in instructions}
    indirect = {call['site'].lower(): call for call in fn['indirectCalls']}
    direct = {call['site'].lower(): call for call in fn['calls']}
    for site in row['acquireSites']:
        address = int(site, 16)
        call = indirect.get(site.lower())
        push = indexed.get(address - 8)
        receiver = indexed.get(address - 6)
        require(call is not None and call['slot'] == '0x20'
                and push is not None and push.mnemonic == 'push'
                and push.operands[0].type == X86_OP_IMM
                and push.operands[0].imm == 4
                and receiver is not None and receiver.mnemonic == 'lea'
                and receiver.operands[0].type == X86_OP_REG
                and receiver.operands[1].type == X86_OP_MEM
                and receiver.operands[1].mem.base == X86_REG_ESP
                and receiver.operands[1].mem.disp == 0x1c,
                f'acquisition site changed at {site}')
    for site, target in row['localResourceCalls'].items():
        call = direct.get(site.lower())
        require(call is not None and call['target'].lower() == target.lower()
                and call['ecxStack'] + call['depth'] == 24,
                f'local resource operation changed at {site}')
    movie = direct.get(row['movieDestructorSite'].lower())
    require(movie is not None and movie['target'].lower() == '0x006e7b80'
            and movie['ecxStack'] + movie['depth'] == 40,
            'movie destructor changed')
    # The shared interruption exit releases the same frame+40 movie as the
    # normal exit. Ghidra reuses its printed stack name for a temporary answer
    # string; that string is not the destructor receiver.
    early_movie = direct.get('0x00e3f73b')
    require(early_movie is not None and early_movie['target'].lower() == '0x006e7b80'
            and early_movie['ecxStack'] + early_movie['depth'] == 40
            and rdata.bytes_at(0x00e3f737, 9).hex() == '8d4c2428e840848aff',
            'early movie destructor changed')
    early_cleanup = 'resources:DestroyMovie("TEXT_OBJECT_HERO_ANSWER_YES")'
    require(source.count(early_cleanup) == 1
            and source.count('xStack_190 = resources:StartMovie("")') == 1,
            'lifted early movie cleanup changed')
    # Native frame+0x13 records hit / damaging magic (excluding healing).
    # Its branch-local byte was constant-folded to the final textual write.
    # Restore after lifting so this control-flow fix cannot shift the legacy
    # operand guesses used by unrelated calls later in the function.
    for site, encoded in {
        0xE40113: 'ff5054', 0xE40118: '7569',
        0xE40140: 'ff92a8000000', 0xE40148: '7432',
        0xE4016A: '6a0e', 0xE40172: 'ff92a4000000',
        0xE4017A: '7407', 0xE4017C: 'c644241300',
        0xE40183: 'c644241301', 0xE401CD: '8a442413',
        0xE401D1: '84c0', 0xE401D3: '0f8420010000',
    }.items():
        ins = indexed.get(site)
        require(ins is not None and ins.bytes.hex() == encoded,
                f'hit flag instruction changed at {site:08x}')
    require(source.count(HIT_POLL) == 1 and source.count(HIT_TEST) == 1
            and 'ladyWasHit' not in source,
            'lifted hit flag changed')
    require(source.count('local ladyResource = x_stk_188') == 1,
            'constructed local resource changed')
    movie_release = MOVIE_RELEASE if MOVIE_RELEASE in source else MOVIE_RELEASE_PAIRED
    require(source.count(movie_release) == 1
            and source.count('resources:ReleaseResource(xStack_1a0)') == (2 if movie_release == MOVIE_RELEASE else 1),
            'lifted movie/resource cleanup changed')
    for old, _new, count in SOURCE_CHANGES:
        require(source.count(old) == count, f'lifted resource use changed: {old}')
    for old, new, count in SOURCE_CHANGES:
        source = source.replace(old, new, count)
    source = source.replace(movie_release, MOVIE_DESTROY, 1)
    source = source.replace(early_cleanup, 'resources:DestroyMovie(xStack_190)', 1)
    source = source.replace('resources:ReleaseResource(xStack_1a0)',
                            'resources:ReleaseResource(ladyResource)', 1)
    hit_poll = ('            local ladyWasHit = false\n' + HIT_POLL).replace(
        '            ::LAB_00e40183::\n',
        '            ::LAB_00e40183::\n            ladyWasHit = true\n')
    source = source.replace(HIT_POLL, hit_poll, 1)
    source = source.replace(HIT_TEST, HIT_TEST.replace('if 1 ~= 0 then', 'if ladyWasHit then'), 1)
    return source
