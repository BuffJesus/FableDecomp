"""Attach the native-verified DeadFather library to readable entity entry points."""
import hashlib
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.dead_father_candidate import generate as compose, DRAFT


def generate(*,draft_path=DRAFT):
    source, evidence=compose()
    if hashlib.sha256(Path(draft_path).read_text().encode()).hexdigest()!=evidence['draftSha256']:
        raise ValueError('DeadFather converter draft changed')
    call='quest:RegisterBoundAliveCondition(me)'
    if source.count(call)!=1:raise ValueError('DeadFather entry condition changed')
    source=source.replace(call,'quest:RegisterBoundAliveCondition()')
    source += """
function Init(quest, me) DeadFatherInit(quest, me) end
function Main(quest, me) DeadFatherMain(quest, me) end
function OnPredicateFail(quest, me) DeadFatherOnPredicateFail(quest, me) end
"""
    LuaRuntime().execute('assert(load(...))',source)
    return source,dict(enabled=False,entryCondition=evidence['conditionEvidence'],evidence=evidence,
        candidateSha256=hashlib.sha256(source.encode()).hexdigest(),integration='work/dead_father_converter/INTEGRATION.md',
        pending=['Four adapters require merged runtime registration and owner validation.',
                 'Live raw animation byte is forwarded; downstream noncanonical-byte interpretation remains unproven.',
                 'Pre-return failed lookup ownership, condition/scheduler and save/load/gameplay validation.'])
