"""Pin runtime adapter ABI dispatch alongside the existing phase byte proofs."""
import hashlib,json
from pathlib import Path
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.guard_init import prove as init
from tools.script_recovery.guard_health import prove as main

def prove(data=None):
    data=data or RData();init(data);main(data)
    w=json.loads(Path(__file__).with_name('guard_capabilities_witness.json').read_text())
    raw=data.bytes_at(w['followAddress'],w['followSize'])
    if raw is None or hashlib.sha256(raw).hexdigest()!=w['followSha256']:raise ValueError('Guard native FollowThing dispatch changed')
    for offset,value in w['slotBytes'].items():
        if data.bytes_at(w['vtableAddress']+int(offset),4)!=bytes.fromhex(value):raise ValueError('Guard native API dispatch changed')
    return w
