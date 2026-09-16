"""Wife's two argument-animation CString/raw-byte call scopes."""
import hashlib,json
from pathlib import Path
from tools.script_recovery.lift_native_lua import RData

def prove(data=None):
    d=data or RData();w=json.loads(Path(__file__).with_name('wife_animation_witness.json').read_text())
    for row in w['regions']:
        if hashlib.sha256(d.bytes_at(row['address'],row['size'])).hexdigest()!=row['sha256']:raise ValueError('Wife animation native bytes changed')
    for address,key in w['literals'].items():
        if d.string_at(int(address,16))!=key:raise ValueError('Wife animation literal changed')
    return w

def lower(source):
    w=prove()
    for key in ('ST_ARGUING_POINT_AWAY','ST_ARGUING_POINT_AT'):
        old=f'resources:PlayAnimation(wife_resource, "{key}", false, false, false, true, resources:ReadAnimationArgument5(), false, false)'
        if source.count(old)!=1:raise ValueError('Wife animation source correspondence changed')
        source=source.replace(old,f'resources:PlayWifeArgumentAnimation(wife_resource, "{key}")')
    return source,w
