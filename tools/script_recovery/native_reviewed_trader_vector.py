"""Recover a reviewed TraderEscort local marker vector with deterministic scope."""
import hashlib
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
ERASE = '-- TODO(native): Std_Vector_Erase_Range(&xStack_30,(int)xStack_30,(int)x_stk_2c);'
DIAGNOSTIC = 'unlifted: Std_Vector_Erase_Range(&xStack_30,(int)xStack_30,(int)x_stk_2c);'


def restore(source, todo, calls, fn, row, rdata):
    if row is None:
        return source, todo, calls

    def require(ok, reason):
        if not ok:
            raise ValueError('reviewed trader vector: ' + reason)

    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    require(start == int(row['address'], 16) and end == int(row['end'], 16), 'extent changed')
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'], 'export changed')
    raw = rdata.bytes_at(start, end-start)
    require(raw is not None and hashlib.sha256(raw).hexdigest() == row['nativeSha256'], 'native body changed')
    for address, entry in row['dependencies'].items():
        raw = rdata.bytes_at(int(address, 16), entry['length'])
        require(raw is not None and hashlib.sha256(raw).hexdigest() == entry['sha256'], 'native dependency changed')
    direct = {c['site'].lower(): c for c in fn['calls']}
    require(direct.get('0x00e060ac', {}).get('target', '').lower() == '0x008ac9d0', 'clear call changed')
    require(direct.get('0x00e063fe', {}).get('target', '').lower() == '0x008ac970', 'exit destructor changed')
    for entry in row['sidecar'].values():
        path = ROOT / entry['path']
        require(hashlib.sha256(path.read_bytes()).hexdigest() == entry['sha256'], 'sidecar source changed')
    require(todo.count(DIAGNOSTIC) == 1, 'clear diagnostic changed')
    source = _recover(source, row['intermediateSha256'])
    require(hashlib.sha256(source.encode()).hexdigest() == row['recoveredSha256'], 'recovered shape changed')
    return source, [x for x in todo if x != DIAGNOSTIC], [*calls, 'WithRetailResources', 'NewThingPositionList',
                                                      'Clear', 'FillScriptName', 'Count', 'GetPos']


def _recover(source, expected):
    if hashlib.sha256(source.encode()).hexdigest() != expected:
        raise ValueError('reviewed trader vector: intermediate changed')

    def replace(old, new, count=1):
        nonlocal source
        if source.count(old) != count:
            raise ValueError('reviewed trader vector: list operation changed')
        source = source.replace(old, new)

    replace(ERASE, 'surprisePositions:Clear()')
    replace('xStack_30 = quest:GetAllThingsWithScriptName("M_BalverineSurprise")',
            'surprisePositions:FillScriptName("M_BalverineSurprise")')
    replace('#xStack_30', 'surprisePositions:Count()', 2)
    replace('xStack_30[(iVar9) / 0xc + 1]:GetPos()', 'surprisePositions:GetPos((iVar9) / 0xc)')
    replace('xStack_30[uVar10 + 1]:GetPos()', 'surprisePositions:GetPos(uVar10)')
    replace(', xStack_30\n', '\n')
    if 'xStack_30' in source:
        raise ValueError('reviewed trader vector: unresolved vector use')
    lines = source.splitlines()
    if lines[0] != 'function WatchForSurprisingBalverines(quest)' or lines[-1] != 'end':
        raise ValueError('reviewed trader vector: function boundary changed')
    lines = [lines[0], '    quest:WithRetailResources(function(resources)',
             '        local surprisePositions = resources:NewThingPositionList()',
             *['    ' + line for line in lines[1:-1]], '    end)', lines[-1]]
    return '\n'.join(lines) + '\n'
