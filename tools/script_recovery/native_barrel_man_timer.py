"""Verify the phase-zero CTimer owner, cached ID, and all cleanup paths."""
import hashlib
import json
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from tools.script_recovery.native_barrel_man_resources import verify as verify_resource
from tools.script_recovery.native_bounded_switch import resolve
from tools.script_recovery.native_resource_lifetime import check_single_resource_lifetime


def verify(function,data,witness=None):
    resource=verify_resource(function,data)
    w=witness or json.loads(Path(__file__).with_name('native_barrel_man_timer_witness.json').read_text())
    for region in w['profiles']+w['regions']:
        raw=data.bytes_at(region['address'],region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest()!=region['sha256']:
            raise ValueError('Barrel timer native instructions changed')
    expected=[(0xDB620B,'start'),(0xDB6289,'use'),(0xDB62A9,'use'),(0xDB64E7,'end'),(0xDB6AC4,'end'),(0xDB6ADB,'end')]
    if [(e['site'],e['operation']) for e in w['events']]!=expected or w['stack']!=68:
        raise ValueError('Barrel timer event coverage changed')
    decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
    instructions=list(decoder.disasm(data.bytes_at(resource['address'],resource['size']),resource['address']))
    events={e['site']:(e['operation'],('stack',68)) for e in w['events']}
    if not check_single_resource_lifetime(instructions,events,indirect_branches=resolve(instructions,data)):
        raise ValueError('Barrel timer lifetime CFG changed')
    return dict(w,status='verified',semantics='Constructor registers a timer through current global interface; object stores returned ID. Movement caches that ID in EBP, sets2 and polls signed remaining>0. Every constructed path deregisters once through current global interface.',
                remaining='Compose owned timer scope into phase-zero Lua; do not substitute timer ID zero or cache the global receiver.')
