"""Generate a separate, DISABLED resource-aware NOVI_AffairMan Lua candidate.

The converter draft (`Entities/NOVI_AffairMan.lua`) models every husband control
operation through the cached entity handle (`me:AcquireControl`, blocking `me:Speak`,
...).  Retail instead owns ONE `CScriptGameResourceObjectScriptedThingBase` local
(baseline stack+16) for the whole Main scope: constructed once, prepared three
times, acquired at six sites, used by every speech/task/animation/move/clear call
and ten temporary Thing getters, and destroyed exactly once on every constructed
path.  That map is `native_affair_man_resources_witness.json`, re-verified here
against the native bytes before anything is rewritten.

This tool rewrites the draft into explicit `LuaRetailResources` calls inside a
`quest:WithRetailResources` scope, keeping one resource id across construction,
preparation, acquisition attempts, actions, task queries and deterministic
release.  It writes to `candidates/`, never into the registered package, and the
result depends on the UNAPPLIED extension under `work/man_resource_extension`
(MoveToPosition, PlayAnimation, ClearCommands, ClearAllActions,
ThingIsDistanceFromPositionOver). Movie starts, destruction and pause use the
existing resource bridge after re-verifying the native movie witness and CFG.
Rewrites are counted against native events or reviewed expanded-Lua layout;
a mismatch raises instead of emitting a partially converted file.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import re
import sys
from dataclasses import asdict
from pathlib import Path

from capstone import Cs, CS_ARCH_X86, CS_MODE_32

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from tools.script_recovery.lift_native_lua import RData, load_manifest  # noqa: E402
from tools.script_recovery.native_post_attack_resources import map_affair_man_resources  # noqa: E402
from tools.script_recovery.native_affair_man_speech import map_affair_man_speech  # noqa: E402
from tools.script_recovery.benchmark_lifter import LuaSyntaxChecker  # noqa: E402
from tools.script_recovery.native_call_setup_ir import read_call_window  # noqa: E402
from tools.script_recovery.native_resource_lifetime import check_single_resource_lifetime  # noqa: E402

LIFTED = ROOT / 'refs/script_recovery/lifted/NewOakValeIntro'
DRAFT = LIFTED / 'FSE/NewOakValeIntro/Entities/NOVI_AffairMan.lua'
TRANSLATION_UNIT = ROOT / 'refs/script_recovery/new_oakvale_intro/translation_unit.json'
MAIN_ADDRESS = '0x00DB09E0'
RESOURCE = 'man_resource'
THING = 'man_thing'
MOVIE = 'man_movie'
EXTENSION_METHODS = ('MoveToPosition', 'PlayAnimation', 'ClearCommands', 'ClearAllActions',
                     'ThingIsDistanceFromPositionOver', 'ReadAnimationArgument5', 'IsHitByHeroExceptAbility',
                     'ThingsAreWithinDistance', 'FaceThing', 'AddConversationPerson', 'AddConversationLine')
ANIMATION_BYTE = 0x01375748

HEADER = '''-- DISABLED CANDIDATE: resource-aware NOVI_AffairMan generated from the converter draft
-- plus the verified native resource map (native_affair_man_resources_witness.json).
-- Not registered, not installed. Requires the unapplied LuaRetailResources extension:
-- {methods}.
-- One retail resource local ({resource}) spans Main: constructed after the entry
-- termination check, prepared/acquired at the three native sites, used by every
-- speech, task query, animation, move and clear call. Normal exits release at the
-- native join; errors use scope Close, including movie destruction and unpause.
-- Speech is the native non-waiting wrapper followed by the retail task poll.
'''

WRAPPER = '''
function Main(quest, me)
    -- Normal exits release at DB1D92's join; Close is a fallback for Lua errors.
    quest:WithRetailResources(function(resources)
        __resource_main(quest, me, resources)
    end)
end
'''


class Rewrite:
    """Counted textual rewrite; the count must equal the native event count."""

    def __init__(self, name, pattern, replacement, expected, *, flags=0):
        self.name, self.pattern, self.replacement, self.expected, self.flags = name, pattern, replacement, expected, flags

    def apply(self, source):
        result, count = re.subn(self.pattern, self.replacement, source, flags=self.flags)
        if count != self.expected:
            raise ValueError(f'{self.name}: draft has {count} sites, native witness has {self.expected}')
        return result, count


def native_inputs(rdata=None):
    unit = json.loads(TRANSLATION_UNIT.read_text(encoding='utf-8-sig'))
    fn = next(f for f in unit['functions'] if f['address'].upper() == MAIN_ADDRESS.upper())
    return fn, rdata or RData(), load_manifest()


def verified_witnesses(fn, rdata, manifest):
    resources = map_affair_man_resources(fn, rdata)
    if not resources or resources[0].get('status') != 'mapped':
        raise ValueError('husband resource witness rejected: ' + json.dumps(resources)[:200])
    resources = resources[0]
    for key in ('lifetimeStatus', 'temporaryThingStatus'):
        if 'verified' not in resources.get(key, ''):
            raise ValueError(f'husband resource witness lacks {key}')
    speech = map_affair_man_speech(fn, rdata, manifest)
    if not speech or speech[0].get('status') != 'mapped':
        raise ValueError('husband speech witness rejected: ' + json.dumps(speech)[:200])
    return resources, speech[0]


def verify_hit_pause(fn, rdata):
    """DB0C88 pushes true for the hit movie's game-interface slot 379 call."""
    setup = read_call_window(rdata, int(MAIN_ADDRESS, 16), fn['size'],
                             0xDB0C8C, argument_count=1)
    game = ('memory', ('address', ('register', 'esi'), 4))
    target = ('memory', ('address', ('memory', ('address', game, 0)), 0x5EC))
    if (setup is None or setup.ecx != game or setup.target != target
            or setup.stack_arguments != (('constant', 1),)):
        raise ValueError('native hit movie pause operands changed')


def verified_movies(fn, rdata, *, witness_file='native_affair_movies_witness.json'):
    """Recheck the existing two-movie witness independently of annotated C text."""
    witness = json.loads(Path(__file__).with_name(witness_file).read_text())
    if hashlib.sha256(fn['decompile'].encode()).hexdigest() != witness['sourceSha256']:
        raise ValueError('movie native source changed')
    for item in [witness] + witness['callees']:
        raw = rdata.bytes_at(int(item['address'], 16), item['size'])
        if raw is None or hashlib.sha256(raw).hexdigest() != item['bytesSha256']:
            raise ValueError('movie native bytes changed')
    if (rdata.bytes_at(0x1260F0C + 0x5C8, 4) != (0x89B110).to_bytes(4, 'little')
            or rdata.bytes_at(0x122D70E, 1) != b'\0'):
        raise ValueError('movie API binding or empty class changed')
    events = {}
    for event in witness['events']:
        site = event['setup']['address']
        setup = read_call_window(rdata, int(witness['address'], 16), witness['setupSize'],
                                 site, argument_count=2 if event['operation'] == 'start' else 0)
        if setup is None or json.loads(json.dumps(asdict(setup))) != event['setup']:
            raise ValueError('movie native operands changed')
        resource = event['resource']
        events[site] = (event['operation'], tuple(resource) if isinstance(resource, list) else resource)
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(rdata.bytes_at(int(witness['address'], 16), witness['size']),
                                       int(witness['address'], 16)))
    if not check_single_resource_lifetime(instructions, events,
            receiver_register=witness.get('receiverRegister'),
            selections={int(site, 16): resource for site, resource in witness.get('selections', {}).items()}):
        raise ValueError('movie native lifetimes overlap or fail cleanup')
    return {'status': 'native-operands-and-single-active-lifetime-verified',
            'starts': [hex(e['setup']['address']) for e in witness['events'] if e['operation'] == 'start'],
            'nativeDestructors': sum(e['operation'] == 'end' for e in witness['events']),
            'className': '', 'bytesSha256': witness['bytesSha256']}


def verified_hit_scope(rdata):
    witness = json.loads(Path(__file__).with_name('native_affair_man_hit_scope_witness.json').read_text())
    raw = rdata.bytes_at(int(witness['nativeRegion'], 16), witness['nativeSize'])
    if (raw is None or hashlib.sha256(raw).hexdigest() != witness['nativeSha256']
            or hashlib.sha256(witness['oldLua'].encode()).hexdigest() != witness['oldLuaSha256']):
        raise ValueError('native hit scope evidence changed')
    return witness


def verified_cached_things(rdata):
    witness = json.loads(Path(__file__).with_name('native_affair_man_cached_things_witness.json').read_text())
    raw = rdata.bytes_at(witness['address'], witness['size'])
    if raw is None or hashlib.sha256(raw).hexdigest() != witness['sha256']:
        raise ValueError('cached Thing native body changed')
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(raw, witness['address']))
    for thing in witness['things']:
        if rdata.string_at(thing['stringAddress']) != thing['name']:
            raise ValueError('cached Thing script name changed')
        for key, count in (('lookup', 2), ('destroy', 0)):
            expected = thing[key]
            actual = read_call_window(rdata, witness['address'], witness['size'], expected['address'], argument_count=count)
            if actual is None or json.loads(json.dumps(asdict(actual))) != expected:
                raise ValueError('cached Thing native operands changed')
        events = {thing['lookup']['address']: ('start', thing['stackSlot']),
                  thing['destroy']['address']: ('end', thing['stackSlot'])}
        if not check_single_resource_lifetime(instructions, events):
            raise ValueError('cached Thing lifetime changed')
    return witness


def event_counts(witness):
    counts = {}
    for event in witness['events']:
        counts[event['name']] = counts.get(event['name'], 0) + 1
    return counts


def rewrites(witness, speech):
    counts = event_counts(witness)
    things = witness['temporaryThings']
    # Eight getters feed the quest GetHealth slot (vtable 1056/4 = 264, the runtime's GetHealth_API);
    # two feed the __fastcall distance helper 0x00CBE45C with the 2.0 m home radius.
    health_things = [t for t in things if t['querySetup']['target'][0] == 'memory']
    distance_things = [t for t in things if t['querySetup'] ['target'] == ['constant', 0x00CBE45C]]
    if len(health_things) + len(distance_things) != len(things):
        raise ValueError('unexpected temporary Thing query target')
    if any(t['querySetup']['stack_arguments'] != [['constant', 0x40000000]] for t in distance_things):
        raise ValueError('home distance query radius changed')
    if len(speech['calls']) != counts['speak']:
        raise ValueError('speech witness and resource witness disagree on speak sites')
    for call in speech['calls']:
        if (call['selection'], call['listen'], call['sound2D'], call['overFade']) != (0, False, True, False):
            raise ValueError('speech operands changed')
    for event in witness['events']:
        if event['name'] == 'acquire' and event['setup']['stack_arguments'] != [['register', 'ebp'], ['stack', 16], ['constant', 4]]:
            raise ValueError('acquisition operands changed')
        if event['name'] == 'animation' and event['setup']['stack_arguments'][5] != [
                'unsigned_byte', ['memory', ['constant', ANIMATION_BYTE]]]:
            raise ValueError('animation argument 5 is no longer the reviewed dynamic byte')
    indent = r'(?P<i>[ \t]*)'
    return [
        Rewrite('hit movie pause',
                r'(quest:StartMovieSequence\(\)\n[ \t]*)quest:PauseAllNonScriptedEntities\(false\)',
                r'\g<1>quest:PauseAllNonScriptedEntities(true)', 1),
        Rewrite('dead byte casts', r'[ \t]*uVar6 = SUB41\((?:pCVar13|me),0\)\n', '', 3),
        # Two native stack slots are never live together. Expanded Lua has 33
        # cleanup tails from 12 native destructor sites; counts are layout guards.
        Rewrite('movie start', r'quest:StartMovieSequence\(\)',
                f'{MOVIE} = resources:StartMovie("")', 2),
        Rewrite('movie end', r'quest:EndMovieSequence\(\)',
                f'resources:DestroyMovie({MOVIE})', 33),
        Rewrite('movie pause', r'quest:PauseAllNonScriptedEntities\(', 'resources:Pause(', 35),
        # Resource address take (`&ppuStack_140`) is the native `this` for the local; nothing to emit.
        Rewrite('resource address take', r'[ \t]*-- TODO\(native\): pppuVar27 = &ppuStack_140;\n', '', 1),
        # has_resource (CD23B9) / reset (CD2770) pair == runtime PrepareResource. The decompiler
        # mislabels CD23B9 as C3DMeshInfo::HasPhysicsMesh (bsim label; address identity is authoritative).
        Rewrite('preparation', indent + r'-- TODO\(native\): bVar4 = C3DMeshInfo::HasPhysicsMesh\([^\n]*\n(?P=i)if bVar4 then\n(?P=i)end\n',
                rf'\g<i>resources:PrepareResource({RESOURCE})\n', counts['has_resource']),
        Rewrite('acquisition', r'me:AcquireControl\(4\)', f'resources:TryAcquire({RESOURCE}, me, 4)', counts['acquire']),
        # Native 7E7390 does not wait; the draft's following IsPerformingScriptTask loop is the retail wait.
        # The wrapper's return value is unused in the draft (r3..r10 are never read).
        Rewrite('speech', r'r\d+ = me:Speak\(', f'resources:Speak({RESOURCE}, ', counts['speak']),
        Rewrite('task query', r'me:IsPerformingScriptTask\(\)', f'resources:IsPerformingScriptTask({RESOURCE})', counts['task_query']),
        Rewrite('clear actions', r'me:ClearAllActions\(\)', f'resources:ClearAllActions({RESOURCE})', counts['clear_actions']),
        Rewrite('clear commands', r'me:ClearCommands\(\)', f'resources:ClearCommands({RESOURCE})', counts['clear_commands']),
        Rewrite('animation', r'me:PlayAnimation\(', f'resources:PlayAnimation({RESOURCE}, ', counts['animation']),
        Rewrite('animation byte', r'\bDAT_01375748\b', 'resources:ReadAnimationArgument5()', counts['animation']),
        Rewrite('movement', r'me:MoveToPosition\(', f'resources:MoveToPosition({RESOURCE}, ', counts['move']),
        # GetScriptThing -> query -> destructor, each proven sequential, so one Thing slot suffices.
        Rewrite('health Thing', indent + r'-- TODO\(native\): uVar\d+ = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_[^\n]*\n(?P=i)fVar18 = quest:GetHealth\(me\)\n',
                rf'\g<i>{THING} = resources:NewThingFromResource({RESOURCE})\n\g<i>fVar18 = resources:ThingHealth({THING})\n\g<i>resources:DestroyThing({THING})\n',
                len(health_things)),
        Rewrite('home distance Thing', indent + r'bVar4 = me:IsDistanceFromPositionOver\(native_arg_man_home_position, 2\.0\)\n',
                rf'\g<i>{THING} = resources:NewThingFromResource({RESOURCE})\n\g<i>bVar4 = resources:ThingIsDistanceFromPositionOver({THING}, native_arg_man_home_position, 2.0)\n\g<i>resources:DestroyThing({THING})\n',
                len(distance_things)),
        # Construction follows the entry termination check (DB0A32 bypasses it, DB0A3C constructs).
        Rewrite('construction', r'(function __resource_main\(quest, me, resources\)\n(?P<l>    local [^\n]*\n)    local alive = true\n    alive = quest:NewScriptFrame\(me\)\n    alive = not quest:IsActiveThreadTerminating\(\)\n    if alive then\n)',
                rf'\g<1>        {RESOURCE} = resources:NewResource()\n', 1),
        # Every constructed path reaches DB1D8E/DB1D92 once; the draft joins them at this label.
        Rewrite('destruction', r'(        ::LAB_00db1d8e::\n)', rf'\g<1>        resources:ReleaseResource({RESOURCE})\n', 1),
    ]


def generate(out_dir=None, *, draft_path=DRAFT, rdata=None):
    fn, rdata, manifest = native_inputs(rdata)
    witness, speech = verified_witnesses(fn, rdata, manifest)
    verify_hit_pause(fn, rdata)
    movies = verified_movies(fn, rdata)
    hit_scope = verified_hit_scope(rdata)
    cached_things = verified_cached_things(rdata)
    draft = draft_path.read_text(encoding='utf-8')
    # These SUB41 results are unused decompiler temporaries. Fail closed if a
    # future draft consumes uVar6, rather than silently removing a live value.
    for line in draft.splitlines():
        code = line.split('--', 1)[0].strip()
        if code.startswith('local '):
            continue
        rhs = re.sub(r'^uVar6\s*=\s*', '', code)
        if re.search(r'\buVar6\b', rhs):
            raise ValueError('uVar6 is no longer a dead temporary')
    if draft.count('function Main(quest, me)\n') != 1 or 'function Init(quest, me)' not in draft:
        raise ValueError('draft layout changed')
    source = draft.replace('function Main(quest, me)\n', 'local function __resource_main(quest, me, resources)\n', 1)
    source, count = re.subn(r'(local function __resource_main\(quest, me, resources\)\n    local )', rf'\g<1>{RESOURCE}, {THING}, {MOVIE}, ', source)
    if count != 1:
        raise ValueError('Main local declaration not found')
    applied = {}
    for rewrite in rewrites(witness, speech):
        source, applied[rewrite.name] = rewrite.apply(source)
    source, applied['hit string scope'] = Rewrite('hit string scope', re.escape(hit_scope['oldLua']),
        '                native_arg_man_hit = resources:IsHitByHeroExceptAbility(me, 14)\n', 1).apply(source)
    source, applied['hit mask reload'] = Rewrite('hit mask reload',
        r'                uVar14 = native_arg_man_cleanup_mask\n', '', 1).apply(source)
    actor_rewrites = [
        Rewrite('cached Thing lookup', r'quest:GetThingWithScriptName\(', 'resources:NewThingFromScriptName(', 2),
        Rewrite('cached Thing alive', r'\((r[12]) ~= nil and \1:IsAlive\(\)\)', r'resources:ThingAlive(\1)', 2),
        Rewrite('cached Thing distance', r'quest:IsDistanceBetweenThingsUnder\(me, (r[12]),',
                r'resources:ThingsAreWithinDistance(me, \1,', 3),
        # Two native cleanup-facing sites pass false; the draft omitted its default.
        Rewrite('cached Thing facing default', r'quest:EntitySetFacingAngleTowardsThing\((r[12]), me\)',
                r'resources:FaceThing(\1, me, false)', 2),
        Rewrite('cached Thing facing', r'quest:EntitySetFacingAngleTowardsThing\(', 'resources:FaceThing(', 4),
        Rewrite('cached Thing conversation person', r'quest:AddPersonToConversation\(uVar8, r1\)',
                'resources:AddConversationPerson(uVar8, r1)', 2),
        Rewrite('cached Thing conversation line', r'quest:AddLineToConversation\((uVar8, native_arg_\w+, (?:r1, me|me, r1), false)\)',
                r'resources:AddConversationLine(\1)', 6),
        Rewrite('cached Thing destruction', r'(        ::LAB_00db1d7c::\n)',
                r'\g<1>        resources:DestroyThing(r2)\n        resources:DestroyThing(r1)\n', 1),
    ]
    for rewrite in actor_rewrites:
        source, applied[rewrite.name] = rewrite.apply(source)
    remaining = sorted(set(re.findall(r'me:(AcquireControl|Speak|IsPerformingScriptTask|PlayAnimation|MoveToPosition|ClearAllActions|ClearCommands)\b', source)))
    if remaining:
        raise ValueError('cached-control calls remain: ' + ', '.join(remaining))
    if 'quest:GetHealth(me)' in source:
        raise ValueError('cached health query remains')
    header = HEADER.format(methods=', '.join(EXTENSION_METHODS), resource=RESOURCE)
    first, rest = source.split('\n', 2)[0], source.split('\n', 2)[2]
    if not first.startswith('-- Generated native draft: NOVI_AffairMan'):
        raise ValueError('draft header changed')
    source = header + rest
    source = source.rstrip('\n') + '\n' + WRAPPER
    syntax = LuaSyntaxChecker().check({'NOVI_AffairMan.resource_candidate.lua': source})
    if not syntax.get('ok'):
        raise ValueError('candidate syntax rejected: ' + json.dumps(syntax))
    report = {
        'schema': 'affair-man-resource-candidate/0.1',
        'status': 'disabled-candidate-not-registered',
        'draft': str(draft_path.relative_to(ROOT)).replace('\\', '/'),
        'draftSha256': hashlib.sha256(draft.encode()).hexdigest(),
        'nativeAddress': witness['address'], 'nativeSize': witness['size'],
        'nativeBytesSha256': witness['bytesSha256'], 'sourceSha256': witness['sourceSha256'],
        'resourceEvents': event_counts(witness), 'temporaryThings': len(witness['temporaryThings']),
        'lifetimeStatus': witness['lifetimeStatus'], 'temporaryThingStatus': witness['temporaryThingStatus'],
        'rewrites': applied, 'syntax': syntax,
        'hitMoviePause': {'callAddress': '0x00DB0C8C', 'value': True,
                         'status': 'native-operands-verified'},
        'cleanupStatus': 'normal join verified natively; Lua errors use scope fallback for resources, movies and pause',
        'movies': movies,
        'hitScope': {k: v for k, v in hit_scope.items() if k != 'oldLua'},
        'cachedThings': cached_things,
        'requiresExtension': list(EXTENSION_METHODS),
        'openGaps': [
            'animation argument accessor is proposed and tested offline, but not integrated into the runtime',
            'bounded live-entry storage is implemented in the combined runtime proposal but remains unapplied',
            'proposed bridge passes MSVC x86/sol tests with engine doubles; full runtime DLL and gameplay validation remain pending',
            'the draft TODO(native) diagnostics carried over unchanged',
        ],
    }
    if out_dir is not None:
        out_dir = Path(out_dir)
        out_dir.mkdir(parents=True, exist_ok=True)
        (out_dir / 'NOVI_AffairMan.resource_candidate.lua').write_text(source, encoding='utf-8')
        (out_dir / 'NOVI_AffairMan.resource_candidate.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    return source, report


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument('--out', type=Path, default=LIFTED / 'candidates')
    args = parser.parse_args()
    _, result = generate(args.out)
    print(json.dumps({k: result[k] for k in ('status', 'rewrites', 'syntax', 'openGaps')}, indent=2))
