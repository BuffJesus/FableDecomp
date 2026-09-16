"""Emit the same native-checked deed helpers for quest and entity callers."""
import hashlib
from pathlib import Path
from tools.script_recovery.native_oakvale_deed_helpers import prove

SHARED_SHA='e91abe8c08ee0122072641134040a8cbb40d5494584222842b3de3b3f37ba0ba'
QUEST_SHA='c33072f5f766facbd8b7351f2e5a0dd1d6a6b2040cc4226f0cdc215682013eda'


def lower(source,shared=False):
    start=0 if shared else source.index('\nfunction AddGoodDeed(')+1
    end=len(source) if shared else source.index('\nfunction destructor(',start)+1
    expected=SHARED_SHA if shared else QUEST_SHA
    if hashlib.sha256(source[start:end].encode()).hexdigest()!=expected:raise ValueError('Oakvale deed draft changed')
    evidence=prove();body=Path(__file__).with_name('oakvale_deed_bodies.lua').read_text()
    if body.count('\nreturn { good = addGoodDeed, bad = addBadDeed }')!=1:raise ValueError('Deed helper exports changed')
    body=body.rsplit('\nreturn ',1)[0]
    if shared:
        body+='\nreturn { AddGoodDeed = addGoodDeed, AddBadDeed = addBadDeed }\n'
    else:
        body+='''
function AddGoodDeed(quest)
    addGoodDeed(quest)
end

function AddBadDeed(quest, deed)
    addBadDeed(quest, nil, deed)
end

'''
    return source[:start]+body+source[end:],dict(status='structured native deed helper composition',
        rawSha256=expected,native=evidence,shared=shared,
        improvements=['Read SCRIPT_DEF morality on every call','Signed 32-bit counter wrap','Atomic objective CString lifetimes'],
        limits='Native whole helpers and compiled atomic adapters have separate checks; final engine scheduling/persistence/gameplay remain unverified.')
