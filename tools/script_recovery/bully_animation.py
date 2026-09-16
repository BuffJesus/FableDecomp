"""Preserve the native opinion-animation raw fifth flag and all seven arguments."""
import hashlib
import json
from pathlib import Path
from tools.script_recovery.bully_initial_phases import recover
from tools.script_recovery.lift_native_lua import RData

def prove(data=None):
    data=data or RData();recover(data)
    witness=json.loads(Path(__file__).with_name('bully_animation_witness.json').read_text())
    for entry in witness['ranges']:
        if hashlib.sha256(data.bytes_at(entry['address'],entry['size'])).hexdigest()!=entry['sha256']:
            raise ValueError('Native animation forwarding/storage changed')
    return witness

def lower(source,data=None):
    data=data or RData();prove(data)
    old=', false, false, false, true, resources:ReadAnimationArgument5(), false)'
    if source.count(old)!=2:raise ValueError('Bully animation argument correspondence changed')
    source=source.replace(old,', false, false, false, true, false, false)')
    if source.count('resources:PlayAnimation(bully_control,')!=2:raise ValueError('Bully animation calls changed')
    source=source.replace('resources:PlayAnimation(bully_control,','resources:PlayAnimationWithNativeArgument5(bully_control,')
    return source,{'sites':[0xdbc710,0xdbc758],'globalByte':0x1375748,
        'helper':0x7e73d0,'expertMethod':0x9049f0,'actionConstructor':0x9034f0,
        'rawByteStorage':{'load':0x903536,'store':0x903548,'actionOffset':29},
        'bytesSha256':{hex(a):hashlib.sha256(data.bytes_at(a,n)).hexdigest() for a,n in ((0x7e73d0,15),(0x9049f0,250),(0x9034f0,117))},
        'semantics':'Construct CString, load raw byte, call with 7 flags or skip empty expert, destroy CString. No normalization assumed.',
        'limits':'Raw byte is preserved through action storage; eventual action consumer not yet traced.'}
