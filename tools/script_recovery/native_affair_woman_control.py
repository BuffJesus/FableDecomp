"""Recover pinned woman loop conditions and departure operands, not ownership."""
import hashlib
import json
import re
from pathlib import Path

from tools.script_recovery.lift_native_lua import RData


def recover(source, data=None):
    data = data or RData()
    witness = json.loads(Path(__file__).with_name('native_affair_woman_control_witness.json').read_text())
    raw = data.bytes_at(witness['address'], witness['size'])
    if raw is None or hashlib.sha256(raw).hexdigest() != witness['nativeSha256']:
        raise ValueError('AffairWoman loop/departure native bytes changed')
    if hashlib.sha256(source.encode()).hexdigest() != witness['draftSha256']:
        raise ValueError('AffairWoman loop/departure draft changed')
    for offset, target in witness['bindings'].items():
        if data.bytes_at(0x1260F0C+int(offset,16),4) != target.to_bytes(4,'little'):
            raise ValueError('AffairWoman camera/removal binding changed')
    for old, new in [
        ('cVar6 = extraout_AL_02', 'cVar6 = not alive'),
        ('while cVar6 == 0 do', 'while not cVar6 do'),
        ('cVar6 = extraout_AL_20', 'cVar6 = not alive'),
        ('quest:RemoveThing(r6)', 'quest:RemoveThing(me, false, true)')]:
        if source.count(old) != 1:
            raise ValueError('AffairWoman loop/departure correspondence changed')
        source = source.replace(old, new)
    source, count = re.subn(r'(?m)^(?P<i> *)me:GetPos\(\)\n(?P=i)cVar6 = quest:IsCameraPosOnScreen\(nil --\[\[missing\]\]\)',
        r'\g<i>cVar6 = quest:IsCameraPosOnScreen(me:GetPos())', source)
    if count != 2:
        raise ValueError('AffairWoman camera correspondence changed')
    return source, dict(witness, status='recovered', cameraQueries=count,
        remaining='resource/movie/named-Thing ownership and remaining body operands')
