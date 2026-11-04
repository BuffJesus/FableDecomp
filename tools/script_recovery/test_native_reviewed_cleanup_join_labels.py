"""Reject stale or unresolved cleanup joins while preserving address comments."""
from copy import deepcopy
import hashlib
import json
from pathlib import Path
import re
import pytest
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_cleanup_join_labels import classify
R=Path(__file__).resolve().parents[2]

@pytest.fixture(scope='module')
def evidence():
    unit=json.loads((R/'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    c=UnitConverter(R/'refs/script_recovery/bordello/translation_unit_typed.json')
    source=(R/'refs/script_recovery/lifted/Bordello/draft/FSE/V_Bordello/native_quest_helpers.lua').read_text()
    source=re.search(r'^function helper_E3E720[^\n]*\n.*?^end$',source,re.M|re.S)[0]
    return source,c.native('0x00e3e720'),unit['reviewedCleanupJoinLabels']['0x00e3e720'],c.rdata

def test_review_changes_only_diagnostic_and_keeps_native_address(evidence):
    source,fn,row,rd=evidence;todo=['other','label LAB_00e3ea29']
    updated,remaining,review=classify(source,todo,fn,row,rd)
    assert updated==source and remaining==['other'] and todo==['other','label LAB_00e3ea29']
    assert review[0]['addressCommentRetained'] and '-- LAB_00e3ea29:' in updated
    assert classify(source,todo,fn,None,rd)==(source,todo,[])

@pytest.mark.parametrize('field',['nativeSha256','sourceSha256','luaBodySha256','address'])
def test_stale_identity_or_body_evidence_fails(evidence,field):
    source,fn,row,rd=evidence;row=deepcopy(row);row[field]='0'*64
    with pytest.raises(ValueError,match='reviewed cleanup join'):
        classify(source,['label LAB_00e3ea29'],fn,row,rd)

@pytest.mark.parametrize('change',['live_jump','changed_release','missing_comment'])
def test_changed_lua_requires_new_review(evidence,change):
    source,fn,row,rd=evidence
    if change=='live_jump':source=source.replace('    local resources', '    goto LAB_00e3ea29\n    local resources',1)
    elif change=='changed_release':source=source.replace('resources:ReleaseResource(xStack_10)','resources:ReleaseResource(xStack_1c)',1)
    else:source=source.replace('-- LAB_00e3ea29: (native jump target)','-- removed')
    with pytest.raises(ValueError,match='structured Lua body changed'):
        classify(source,['label LAB_00e3ea29'],fn,row,rd)

def test_changed_receiver_metadata_fails(evidence):
    source,fn,row,rd=evidence;fn=deepcopy(fn)
    next(c for c in fn['calls'] if c['site'].lower()=='0x00e3ea36')['ecxStack']=-28
    with pytest.raises(ValueError,match='receiver metadata changed'):
        classify(source,['label LAB_00e3ea29'],fn,row,rd)

def test_stale_edge_certificate_fails(evidence):
    source,fn,row,rd=evidence;row=deepcopy(row);row['joins'][0]['edges'][0]['bytes']='7436'
    with pytest.raises(ValueError,match='cleanup edge changed'):
        classify(source,['label LAB_00e3ea29'],fn,row,rd)

def test_diagnostic_inventory_fails_closed(evidence):
    source,fn,row,rd=evidence
    with pytest.raises(ValueError,match='diagnostic changed'):
        classify(source,[],fn,row,rd)
