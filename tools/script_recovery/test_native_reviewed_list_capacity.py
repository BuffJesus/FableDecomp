"""A lowered list may replace capacity bookkeeping only with pinned native proof."""
from copy import deepcopy
import hashlib
import json
from pathlib import Path

import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_list_capacity import restore

ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def evidence():
    c = UnitConverter(ROOT/'refs/script_recovery/bandit_camp/translation_unit_typed.json')
    unit = json.loads((ROOT/'refs/script_recovery/bandit_camp/units/Q_BanditCamp.json').read_text())
    row = deepcopy(unit['reviewedListCapacityInit']['0x00d032f0'])
    source = ('undefined1 auStack_4 [4];\nundefined1 uStack_15;\n'
              'iStack_c = ENGINE_EmptyList();\n' + row['statement'] + '\n'
              'hero = GSI->GetHero();\niStack_c = CScriptThing::MsgGetThingsKilledGroups(hero);\n'
              'consume(ENGINE_ListWord(iStack_c, 0));\n')
    row['loweredSha256'] = hashlib.sha256(source.encode()).hexdigest()
    return source, c.native('0x00d032f0'), row, c.rdata


@pytest.mark.parametrize('newline',['\n','\r\n'])
def test_only_initializer_is_removed(evidence,newline):
    source,fn,row,rdata = evidence
    source = source.replace('\n',newline)
    assert restore(source,fn,row,rdata) == source.replace(row['statement'],'',1)
    assert restore(source,fn,None,rdata) == source


@pytest.mark.parametrize('key',['nativeSha256','sourceSha256','loweredSha256'])
def test_stale_hash_is_rejected(evidence,key):
    source,fn,row,rdata = evidence;row=deepcopy(row);row[key]='0'*64
    with pytest.raises(ValueError,match='changed'):restore(source,fn,row,rdata)


@pytest.mark.parametrize('old,new',[
    ('consume(', 'use(auStack_4);\nconsume('),
    ('consume(', 'use(uStack_15);\nconsume('),
    ('ENGINE_EmptyList()', 'different()'),
    ('MsgGetThingsKilledGroups', 'OtherMessage'),
    ('(int)&uStack_15,0', '(int)&uStack_15,1'),
    ('undefined1 auStack_4 [4]', 'undefined1 auStack_4 [8]'),
])
def test_unproven_source_is_rejected_even_with_refreshed_hash(evidence,old,new):
    source,fn,row,rdata=evidence;row=deepcopy(row);source=source.replace(old,new)
    row['loweredSha256']=hashlib.sha256(source.encode()).hexdigest()
    with pytest.raises(ValueError):restore(source,fn,row,rdata)


@pytest.mark.parametrize('site,old,new',[
    (0x4502eb,'8bc1','33c0'),  # callee is not the proven store helper
    (0xd032fb,'33ed','8bed'),  # zero register no longer defined as zero
    (0xd0341d,'8d4c242c','8d4c2430'),  # capacity not adjacent to end
    (0xd0343b,'8d4c241c','8d4c2420'),  # message receives another stack slot
    (0xd03442,'ff92dc000000','ff92e0000000'),  # different message slot
])
def test_changed_native_semantics_are_rejected(evidence,site,old,new):
    source,fn,row,rdata=evidence;row=deepcopy(row)
    class Changed:
        def bytes_at(self,start,size):
            raw=rdata.bytes_at(start,size)
            if start<=site and site+len(bytes.fromhex(old))<=start+size:
                at=site-start;assert raw[at:at+len(bytes.fromhex(old))]==bytes.fromhex(old)
                raw=raw[:at]+bytes.fromhex(new)+raw[at+len(bytes.fromhex(old)):]
            return raw
    changed=Changed();start=int(fn['address'],16);size=int(fn['bodyEndExclusive'],16)-start
    row['nativeSha256']=hashlib.sha256(changed.bytes_at(start,size)).hexdigest()
    if hex(site) in row['nativeInstructions']:row['nativeInstructions'][hex(site)]=new
    with pytest.raises(ValueError):restore(source,fn,row,changed)


def test_missing_native_witness_and_ambiguous_call_are_rejected(evidence):
    source,fn,row,rdata=evidence;row=deepcopy(row);del row['nativeInstructions']['0xd03425']
    with pytest.raises(ValueError,match='witness incomplete'):restore(source,fn,row,rdata)
    row=evidence[2];fn=deepcopy(fn)
    fn['calls'] += [c for c in fn['calls'] if int(c['target'],16)==0x4502eb]
    with pytest.raises(ValueError,match='ambiguous'):restore(source,fn,row,rdata)
