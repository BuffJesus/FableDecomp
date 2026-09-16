"""Guard Init and its four native by-value argument-consuming callees."""
import hashlib,json,struct
from pathlib import Path
from tools.script_recovery.lift_native_lua import RData

SOURCE='''function Init(quest, me)
    quest:WithRetailResources(function(resources)
        resources:InitializeGuardActor(me)
    end)
end
'''

def prove(data=None):
    data=data or RData();w=json.loads(Path(__file__).with_name('guard_init_witness.json').read_text())
    for row in [w,*w['consumers']]:
        raw=data.bytes_at(row['address'],row['size'])
        if raw is None or hashlib.sha256(raw).hexdigest()!=row['sha256']:raise ValueError('Guard Init/callee bytes changed')
    for row in w['consumers']:
        if data.bytes_at(w['vtable']+row['slot'],4)!=struct.pack('<I',row['address']):raise ValueError('Guard Init dispatch target changed')
    return w
