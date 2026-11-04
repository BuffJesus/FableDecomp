from pathlib import Path
from copy import deepcopy
import hashlib
import json
import re
import pytest
from tools.script_recovery import native_reviewed_hit_health_lifetimes as CLASSIFIER
from tools.script_recovery.lua_staging_liveness import prove_unused_staging
from tools.script_recovery.test_native_reviewed_interface_table_staging import FixtureImage
from lupa.lua54 import LuaRuntime

W = Path(__file__).resolve().parent/'fixtures/hit_health/talking_trader'
sha = lambda raw: hashlib.sha256(raw).hexdigest()
read = lambda name: json.loads((W/name).read_text())


class Image(FixtureImage):
    def string_at(self, address):
        return 'SCRIPT_NAME_HERO' if address == 0x125d1c8 else None


@pytest.fixture
def inputs(monkeypatch):
    # Retail image access is forbidden: this evidence packet must be portable.
    from tools.script_recovery import lift_native_lua
    monkeypatch.setattr(lift_native_lua.RData, '__init__', lambda *a, **kw: (_ for _ in ()).throw(AssertionError('retail image opened')))
    return dict(source=(W/'phase.lua').read_text(), todo=read('phase_todo.json'), calls=read('phase_calls.json'),
                fn=read('native_function.json'), row=read('candidate_row.json'), unit=read('candidate_unit.json'),
                owner='TalkingTrader1', image=Image([(int(r['address'], 16), bytes.fromhex(r['bytes'])) for r in read('image_parts.json')]),
                liveness=prove_unused_staging)


def run(inputs):
    return CLASSIFIER.recover(**inputs)


def repin(inputs):
    row = inputs['row']
    row['luaPhaseSha256'] = sha(inputs['source'].encode())
    row['scope']['luaPrefixSha256'] = sha(row['scope']['oldLuaPrefix'].encode())


def change_prefix(inputs, old, new):
    scope = inputs['row']['scope']
    prefix = scope['oldLuaPrefix']
    assert prefix.count(old) == 1
    changed = prefix.replace(old, new, 1)
    assert inputs['source'].count(prefix) == 1
    inputs['source'] = inputs['source'].replace(prefix, changed, 1)
    scope['oldLuaPrefix'] = changed
    repin(inputs)


def test_exact_phase_recovers_native_health_lifetime_and_numeric_flag(inputs):
    old = deepcopy({k: inputs[k] for k in ('todo', 'calls', 'fn', 'row', 'unit')})
    out, todo, calls, reviews = run(inputs)
    assert todo == inputs['todo'] and len(calls) == len(inputs['calls'])-3 and len(reviews) == 1
    assert out.count('resources:IsHitByHeroExceptAbilityAndHealthAbove(me, 14, 0.0) and 1 or 0') == 1
    assert reviews[0]['nativeRoles']['booleanHealthPaths'] == 32
    assert reviews[0]['registeredEntryFrame']['maskInitializerDominates']
    assert [r['alias'] for r in reviews[0]['deadScratch']] == ['cVar7', 'fret_01']
    assert all(old[k] == inputs[k] for k in old)
    LuaRuntime().compile(out)


def test_absent_metadata_preserves_every_schema(inputs):
    inputs['row'] = None
    assert run(inputs) == (inputs['source'], inputs['todo'], inputs['calls'], [])


@pytest.mark.parametrize('fault', ['native', 'source', 'phase', 'calls', 'owner', 'vtable', 'function', 'method', 'ability', 'threshold',
    'literalAddress', 'missingProfile', 'badProfile', 'badProfileBytes', 'frame', 'scopeHash', 'prefixHash', 'objects', 'mask',
    'sourceCalls', 'flagAlias', 'queryAlias', 'healthAlias', 'wrongCallIndex', 'inlineBreakCount', 'thisDefinition', 'actorDefinition',
    'falseExit', 'maskOffset', 'flagOffset', 'healthSlot', 'healthSize', 'healthTarget', 'healthHash', 'healthVtable', 'thresholdAddress', 'diagnostics', 'labels'])
def test_stale_or_ambiguous_evidence_refuses(inputs, fault):
    row = inputs['row']; scope = row['scope']
    if fault == 'native': row['nativeExtentSha256'] = '0'*64
    elif fault == 'source': row['sourceSha256'] = '0'*64
    elif fault == 'phase': row['luaPhaseSha256'] = '0'*64
    elif fault == 'calls': row['callsSha256'] = '0'*64
    elif fault == 'owner': inputs['owner'] = 'Other'
    elif fault == 'vtable': row['entityVtable'] = '0x1'
    elif fault == 'function': inputs['unit']['entities']['TalkingTrader1']['functions']['Main']['address'] = '0x1'
    elif fault == 'method': row['method'] = 'IsHitByHeroExceptAbility'
    elif fault == 'ability': row['ability'] = 13
    elif fault == 'threshold': row['minimumHealth'] = 1
    elif fault == 'literalAddress': row['heroLiteral'] = '0x125d1c9'
    elif fault == 'missingProfile': row['helperBodies'].pop()
    elif fault == 'badProfile': row['helperBodies'][0]['sha256'] = '0'*64
    elif fault == 'badProfileBytes': row['helperBodies'][0]['bytes'] = '90'
    elif fault == 'frame': row['frameDepth'] += 4
    elif fault == 'scopeHash': scope['nativeSha256'] = '0'*64
    elif fault == 'prefixHash': scope['luaPrefixSha256'] = '0'*64
    elif fault == 'objects': scope['objectOffsets'][0] += 4
    elif fault == 'mask': scope['temporaryMask'] = 14
    elif fault == 'sourceCalls': scope['sourceCalls'][0]['site'] = '0x1'
    elif fault in ('flagAlias', 'queryAlias', 'healthAlias'): scope[fault] = 'escape'
    elif fault == 'wrongCallIndex': scope['callListIndex'] += 1
    elif fault == 'inlineBreakCount': row['analysisInlineBreaks'] += 1
    elif fault == 'thisDefinition': row['thisDefinition'] = '0xecbdfb'
    elif fault == 'actorDefinition': row['actorDefinition'] = '0xecbdf9'
    elif fault == 'falseExit': scope['falseExit'] = '0xecc850'
    elif fault == 'maskOffset': scope['maskOffset'] += 4
    elif fault == 'flagOffset': scope['flagOffset'] += 1
    elif fault == 'healthSlot': row['healthApi']['slot'] += 4
    elif fault == 'healthSize': row['healthApi']['size'] += 1
    elif fault == 'healthTarget': row['healthApi']['target'] = '0x890401'
    elif fault == 'healthHash': row['healthApi']['sha256'] = '0'*64
    elif fault == 'healthVtable': row['healthApi']['vtable'] = '0x1260f10'
    elif fault == 'thresholdAddress': row['thresholdAddress'] = '0x122dee0'
    elif fault == 'diagnostics': scope['diagnostics'] = ['invented']
    elif fault == 'labels': scope['labels'][1] = scope['labels'][0]
    with pytest.raises(ValueError): run(inputs)


@pytest.mark.parametrize('fault', ['wrongAbility', 'wrongHeroBranch', 'wrongSpecialBranch', 'wrongHealthReceiver', 'wrongHealthActor',
    'wrongHealthOrder', 'wrongFlag', 'extraCall', 'extraWrite', 'externalJump', 'observableQuery', 'observableHealth',
    'conditionalQueryRead', 'conditionalHealthRead', 'closure', 'duplicateLocal'])
def test_unsafe_lua_refuses_after_rehash(inputs, fault):
    if fault == 'wrongAbility': change_prefix(inputs, 'SpecialAbility(0xe)', 'SpecialAbility(13)')
    elif fault == 'wrongHeroBranch': change_prefix(inputs, 'if not cVar7 then\n', 'if cVar7 then\n')
    elif fault == 'wrongSpecialBranch': change_prefix(inputs, 'if not cVar7 then goto', 'if cVar7 then goto')
    elif fault == 'wrongHealthReceiver': change_prefix(inputs, 'quest:GetHealth(me)', 'other:GetHealth(me)')
    elif fault == 'wrongHealthActor': change_prefix(inputs, 'quest:GetHealth(me)', 'quest:GetHealth(other)')
    elif fault == 'wrongHealthOrder': change_prefix(inputs, 'fret_01 <= 0.0', 'fret_01 > 0.0')
    elif fault == 'wrongFlag': change_prefix(inputs, 'c_stk_d1 = 1', 'c_stk_d1 = 0')
    elif fault == 'extraCall': change_prefix(inputs, 'cVar7 = me:MsgIsHitByHero()', 'cVar7 = me:MsgIsHitByHero()\nquest:Observe()')
    elif fault == 'extraWrite': change_prefix(inputs, 'cVar7 = me:MsgIsHitByHero()', 'cVar7 = me:MsgIsHitByHero()\nglobal = 1')
    else:
        marker = '            if c_stk_d1 ~= 0 then'
        inserts = dict(externalJump='goto LAB_00ecc5ce', observableQuery='quest:Observe(cVar7)', observableHealth='quest:Observe(fret_01)',
            conditionalQueryRead='if value then\nquest:Observe(cVar7)\nend', conditionalHealthRead='if value then\nquest:Observe(fret_01)\nend',
            closure='local function hidden() return cVar7 end', duplicateLocal='local cVar7')
        inputs['source'] = inputs['source'].replace(marker, inserts[fault]+'\n'+marker)
        repin(inputs)
    with pytest.raises(ValueError): run(inputs)


@pytest.mark.parametrize('field', ['ecxStack', 'pushedStack'])
def test_forged_constructor_coordinates_refuse_after_correspondence_repin(inputs, field):
    row = inputs['row']; scope = row['scope']; metadata = scope['sourceCalls'][0]['metadata']
    site = metadata['site']
    fncall = next(c for c in inputs['fn']['calls'] if c['site'] == site)
    for target in (metadata, fncall):
        if field == 'ecxStack': target[field] += 4
        else: target[field] = [0, 0]
    # Constructor pushed arguments are checked by source literal and native roles;
    # exported stack snapshots must also retain the expected non-stack values.
    with pytest.raises(ValueError): run(inputs)


def mutate_native(inputs, address, old, new):
    fn = inputs['fn']; start = int(fn['address'], 16); end = int(fn['bodyEndExclusive'], 16)
    image = inputs['image']; raw = bytearray(image.bytes_at(start, end-start)); at = address-start
    assert bytes(raw[at:at+len(old)]) == old and old != new
    raw[at:at+len(old)] = new
    parts = []
    for base, length, offset in image.raw_secs:
        data = bytes(raw) if base == start and length == end-start else image.data[offset:offset+length]
        parts.append((base, data))
    inputs['image'] = Image(parts)
    row = inputs['row']; scope = row['scope']
    row['nativeExtentSha256'] = sha(raw)
    scope['nativeSha256'] = sha(raw[int(scope['start'], 16)-start:int(scope['endExclusive'], 16)-start])


@pytest.mark.parametrize('address,old,new', [
    (0xecc54d, 'c8d12501', 'c9d12501'), (0xecc54b, 'ff', 'fe'),
    (0xecc554, '70', '6c'), (0xecc56a, 'cd', 'ce'), (0xecc55d, '3c', '38'),
    (0xecc5bd, '0e', '0f'), (0xecc5cc, '75', '74'), (0xecc5d0, '04', '08'),
    (0xecc5d3, '55', '56'), (0xecc5d6, '20040000', '24040000'),
    (0xecc5dc, 'dcde2201', 'e0de2201'), (0xecc5db, '1d', '15'),
    (0xecc5e3, '1b', '1a'), (0xecc5f0, '00', '01'), (0xecc5fc, '58', '60'),
    (0xecc622, 'fe', 'f6'), (0xecbdfd, '08', '0c'), (0xecbdfa, 'f1', 'f0'),
    (0xecbf5c, '57', '90'), (0xecbdff, 'db', 'd9'), (0xecbf12, '3c', '38'),
])
def test_semantically_wrong_native_bytes_refuse_after_rehash(inputs, address, old, new):
    mutate_native(inputs, address, bytes.fromhex(old), bytes.fromhex(new))
    with pytest.raises(ValueError): run(inputs)


@pytest.mark.parametrize('address,old,new', [(0xecc622, 'fe', 'fa'), (0xecc553, '24', '64')])
def test_equivalent_native_variants_remain_supported(inputs, address, old, new):
    mutate_native(inputs, address, bytes.fromhex(old), bytes.fromhex(new))
    assert len(run(inputs)[3]) == 1


@pytest.mark.parametrize('replacement', ['resources = other', 'me = other', 'quest = other', 'local me = other', 'local quest = other'])
def test_receiver_rebinding_refuses_after_lua_rehash(inputs, replacement):
    inputs['source'] = inputs['source'].replace('            if c_stk_d1 ~= 0 then', replacement+'\n            if c_stk_d1 ~= 0 then')
    repin(inputs)
    with pytest.raises(ValueError): run(inputs)
