"""Generate a disabled BookTrader candidate from checked native ownership maps."""
from __future__ import annotations

import argparse
import hashlib
import json
import re
import sys
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.benchmark_lifter import LuaSyntaxChecker
from tools.script_recovery.native_book_trader_lifetime import map_book_trader_lifetime
from tools.script_recovery.generate_affair_man_resource_candidate import Rewrite, verified_movies
from tools.script_recovery.native_book_trader_conversation import verify as verify_conversation

LIFTED = ROOT / 'refs/script_recovery/lifted/NewOakValeIntro'
DRAFT = LIFTED / 'FSE/NewOakValeIntro/Entities/NOVI_BookTrader.lua'
DRAFT_SHA = '6fa14a054e654386ba6831550d4f193dfc1c3866eee11b435b826813fc100dfc'


def verified_hit_scope(data):
    witness = json.loads(Path(__file__).with_name('native_book_trader_hit_scope_witness.json').read_text())
    raw = data.bytes_at(int(witness['nativeRegion'], 16), witness['nativeSize'])
    if (raw is None or hashlib.sha256(raw).hexdigest() != witness['nativeSha256']
            or data.string_at(0x125D1C8) != 'SCRIPT_NAME_HERO'
            or hashlib.sha256(witness['oldLua'].encode()).hexdigest() != witness['oldLuaSha256']):
        raise ValueError('BookTrader hit scope evidence changed')
    return witness


def verified_facing(data):
    witness = json.loads(Path(__file__).with_name('native_book_trader_facing_witness.json').read_text())
    for region in witness['regions']:
        raw = data.bytes_at(int(region['address'], 16), region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest() != region['sha256']:
            raise ValueError('BookTrader facing operands or ABI changed')
    if data.string_at(int(witness['scriptNameAddress'], 16)) != witness['scriptName']:
        raise ValueError('BookTrader facing target changed')
    return witness


def generate(out_dir=None, *, draft_path=DRAFT, rdata=None):
    unit = json.loads((ROOT / 'refs/script_recovery/new_oakvale_intro/translation_unit.json').read_text())
    fn = next(f for f in unit['functions'] if f['address'].lower() == '0x00db3fa0')
    data = rdata or RData()
    mapped = map_book_trader_lifetime(fn, data)
    if not mapped or mapped[0].get('status') != 'mapped':
        raise ValueError('BookTrader native lifetime rejected: ' + repr(mapped))
    lifetime = mapped[0]
    movies = verified_movies(fn, data, witness_file='native_book_trader_movies_witness.json')
    hit_scope = verified_hit_scope(data)
    facing = verified_facing(data)
    conversation = verify_conversation(data)
    draft = Path(draft_path).read_text(encoding='utf-8')
    if hashlib.sha256(draft.encode()).hexdigest() != DRAFT_SHA:
        raise ValueError('BookTrader draft changed; review native-to-Lua correspondence')
    counts = Counter(event['name'] for event in lifetime['resourceEvents'])
    # The seven calls use the nonwaiting native speech wrapper with hero,
    # literal text, selection0/listenfalse/sound2Dtrue/overFadefalse.
    setups = {s['site']: s['setup'] for s in lifetime['setups']}
    texts = []
    for event in lifetime['resourceEvents']:
        if event['name'] == 'speak':
            args = setups[event['site']]['stack_arguments']
            if args[0][0] != 'result' or args[1][0] != 'constant' or args[2:] != [
                    ['constant', 0], ['constant', 0], ['constant', 1], ['constant', 0]]:
                raise ValueError('BookTrader speech operands changed')
            texts.append(data.string_at(args[1][1]))
    lua_texts = re.findall(r'pcVar19 = "([^"]+)"\n\s+pCVar8 = quest:GetHero\(\)\n\s+r\d+ = me:Speak', draft)
    # Structured Lua places the insufficient-cash branch before the purchase
    # branch; native address order is different. Unique speech literals identify
    # their call sites without assuming basic-block layout order is execution order.
    if Counter(texts) != Counter(lua_texts):
        raise ValueError('BookTrader speech literals differ from retail bytes')
    source = draft.replace('function Main(quest, me)\n', 'local function __resource_main(quest, me, resources)\n', 1)
    applied = {}
    edits = [
        Rewrite('scoped conversation setup',
                r'(?m)^(?P<i> *)uVar9 = quest:AddNewConversation\(me, false, false\)\n'
                r'(?P=i)uVar12 = quest:GetHero\(\)\n'
                r'(?P=i)quest:AddPersonToConversation\(uVar9, uVar12\)',
                r'\g<i>uVar9 = quest:StartConversationWithHero(me, false, false)', 1),
        Rewrite('scoped conversation line',
                r'(?m)^(?P<i> *)native_arg_book_listener = quest:GetHero\(\)\n'
                r'(?P=i)quest:AddLineToConversation\(uVar9, "TEXT_QST_048_TRADER_ROLL_UP", me, native_arg_book_listener, false\)',
                r'\g<i>quest:AddConversationLineToHero(uVar9, "TEXT_QST_048_TRADER_ROLL_UP", me, false)', 1),
        Rewrite('hit string scope', re.escape(hit_scope['oldLua']),
                '            bVar4 = resources:IsHitByHeroExceptAbility(me, 14)\n', 1),
        Rewrite('prepare', r'(?m)^(?P<i> *)-- TODO\(native\): bVar4 = C3DMeshInfo::HasPhysicsMesh[^\n]+\n(?P=i)if bVar4 then\n(?P=i)end',
                r'\g<i>resources:PrepareResource(book_resource)', counts['has_resource']),
        Rewrite('acquire', r'me:AcquireControl\(', 'resources:TryAcquire(book_resource, me, ', counts['acquire']),
        Rewrite('speak', r'me:Speak\(', 'resources:Speak(book_resource, ', counts['speak']),
        Rewrite('task query', r'me:IsPerformingScriptTask\(\)', 'resources:IsPerformingScriptTask(book_resource)', counts['task_query']),
        Rewrite('movement', r'me:MoveToPosition\(', 'resources:MoveToPosition(book_resource, ', counts['move']),
        Rewrite('animation', r'me:PlayAnimation\(', 'resources:PlayAnimation(book_resource, ', counts['animation']),
        Rewrite('live animation byte', r'\bDAT_01375748\b', 'resources:ReadAnimationArgument5()', 1),
        Rewrite('movie start', r'quest:StartMovieSequence\(\)', 'book_movie = resources:StartMovie("")', 2),
        Rewrite('movie end', r'quest:EndMovieSequence\(\)', 'resources:DestroyMovie(book_movie); book_movie = nil', 26),
        Rewrite('movie pause', r'quest:PauseAllNonScriptedEntities\(', 'resources:Pause(', 28),
        Rewrite('health temporary', r'(?m)^(?P<i> *)(?P<result>\w+) = quest:GetHealth\(me\)$',
                r'\g<i>book_thing = resources:NewThingFromResource(book_resource)\n'
                r'\g<i>\g<result> = resources:ThingHealth(book_thing)\n'
                r'\g<i>resources:DestroyThing(book_thing); book_thing = nil', 7),
        Rewrite('distance temporary', r'(?m)^(?P<i> *)bVar4 = \(me ~= nil and me:IsDistanceFromPositionOver\(native_arg_book_home, fVar23\)\)$',
                r'\g<i>book_thing = resources:NewThingFromResource(book_resource)\n'
                r'\g<i>bVar4 = resources:ThingIsDistanceFromPositionOver(book_thing, native_arg_book_home, fVar23)\n'
                r'\g<i>resources:DestroyThing(book_thing); book_thing = nil', 1),
        Rewrite('early resource exits', r'(?m)^(?P<i> *)return$', r'\g<i>release_book()\n\g<i>return', 3),
        Rewrite('common resource exit', r'(        ::LAB_00db4f5a::\n)', r'\g<1>        release_book()\n', 1),
        Rewrite('resource construction', r'(    if alive then\n)(        alive = not quest:IsActiveThreadTerminating\(\))',
                r'\g<1>        book_resource = resources:NewResource()\n\g<2>', 1),
    ]
    for edit in edits:
        source, applied[edit.name] = edit.apply(source)
    snap_flags = iter(facing['snapFlags'])
    source, applied['facing snap flags'] = Rewrite('facing snap flags',
        r'quest:EntitySetFacingAngleTowardsThing\(me, uVar9\)',
        lambda match: 'quest:EntitySetFacingAngleTowardsThing(me, uVar9, ' +
                      str(next(snap_flags)).lower() + ')', 3).apply(source)
    source, applied['scoped named facing'] = Rewrite('scoped named facing',
        r'(?m)^(?P<i> *)uVar9 = quest:GetThingWithScriptName\("NOVI_Theresa"\)\n'
        r'(?P=i)quest:EntitySetFacingAngleTowardsThing\(me, uVar9, (?P<snap>true|false)\)',
        r'\g<i>quest:FaceThingByScriptName(me, "NOVI_Theresa", \g<snap>)', 3).apply(source)
    source = source.replace('local function __resource_main(quest, me, resources)\n', '''local function __resource_main(quest, me, resources)
    local book_resource, book_thing, book_movie
    local function release_book()
        assert(book_movie == nil and book_thing == nil, "BookTrader cleanup order changed")
        resources:ReleaseResource(book_resource)
        book_resource = nil
    end
''', 1)
    source = '-- DISABLED resource-aware BookTrader candidate; see candidate report for remaining gaps.\n' + source
    source += '''
function Main(quest, me)
    quest:WithRetailResources(function(resources)
        __resource_main(quest, me, resources)
    end)
end
'''
    if re.search(r'me:(?:AcquireControl|Speak|IsPerformingScriptTask|PlayAnimation|MoveToPosition)\(', source):
        raise ValueError('BookTrader cached control operation remains')
    syntax = LuaSyntaxChecker().check({'BookTrader.resource_candidate.lua': source})
    if not syntax['ok']:
        raise ValueError('BookTrader candidate syntax rejected: ' + repr(syntax))
    report = dict(schema='book-trader-resource-candidate/0.1', status='disabled-incomplete',
        draftSha256=DRAFT_SHA, candidateSha256=hashlib.sha256(source.encode()).hexdigest(),
        rewrites=applied, nativeResourceEvents=dict(counts), nativeTemporaryThings=8,
        lifetimeStatus=lifetime['lifetimeStatus'], movies=movies, syntax=syntax,
        hitScope={key: value for key, value in hit_scope.items() if key != 'oldLua'},
        facingEvidence=facing,
        conversationEvidence=conversation,
        namedThingScopes={'count': 3, 'order': 'name construction, lookup, facing, Thing destruction, name destruction',
                          'adapter': 'tools/script_recovery/scythe_thing_adapters.inc:FaceThingByScriptName'},
        requiresExtension=['MoveToPosition', 'PlayAnimation', 'ReadAnimationArgument5', 'ThingIsDistanceFromPositionOver', 'IsHitByHeroExceptAbility'],
        requiresQuestExtension=['FaceThingByScriptName', 'AddConversationLineToHero', 'StartConversationWithHero'],
        openGaps=['Standalone entry predicate integration remains pending; the readable builder restores it separately.',
                  'Runtime extension is unapplied; full DLL/gameplay validation is pending.'])
    if out_dir is not None:
        out_dir = Path(out_dir)
        out_dir.mkdir(parents=True, exist_ok=True)
        (out_dir / 'NOVI_BookTrader.resource_candidate.lua').write_text(source, encoding='utf-8')
        (out_dir / 'NOVI_BookTrader.resource_candidate.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    return source, report


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--out', type=Path, default=LIFTED / 'candidates')
    args = parser.parse_args()
    _, report = generate(args.out)
    print(json.dumps(report, indent=2))
