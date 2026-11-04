"""Restore the two Singing Stones name-and-offset camera calls."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
OLD = (
    'quest:CameraUseCameraPoint(r1, nil --[[missing]], 0x0, 0x0, -1.0)',
    'quest:CameraUseCameraPoint(nil --[[missing]], nil --[[missing]], 0x0, 0x0, -1.0)',
)
NEW = 'quest:CameraUseCameraPoint("CAM_Stones", nil, nil, -1.0, 0, -1)'
DIAGNOSTICS = (
    'CameraUseCameraPoint: missing spTarget (4 args for 5 params)',
    'CameraUseCameraPoint: missing spCamPoint, spTarget (3 args for 5 params)',
)


def restore(source, todo, calls, fn, row, rdata, sidecar):
    if row is None:
        return source, todo, calls

    def require(ok, reason):
        if not ok:
            raise ValueError('SingingStones camera: ' + reason)

    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    require(start == 0x00ED2BB0 and
            hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'],
            'reviewed source changed')
    require(hashlib.sha256(rdata.bytes_at(start, end-start)).hexdigest() == row['nativeSha256'],
            'native body changed')
    native = fn['decompile']
    require(native.count('"CAM_Stones"') == 2 and
            native.count(' + 0x670))') == 2 and
            native.count('(C3DVector_bv *)0x0,(void *)0x0,-1.0,0,-1)') == 2,
            'native camera calls changed')
    report_path = ROOT / sidecar['buildReport']
    report = json.loads(report_path.read_text())
    require(hashlib.sha256(report_path.read_bytes()).hexdigest() == sidecar['buildReportSha256'],
            'sidecar build report changed')
    require(hashlib.sha256((ROOT / report['patch']).read_bytes()).hexdigest() == report['patch_sha256'] and
            hashlib.sha256(Path(report['candidate_dll']).read_bytes()).hexdigest() == report['candidate_dll_sha256'],
            'sidecar patch or DLL changed')
    binding = ROOT / sidecar['path'] / 'FableScriptExtender'
    expected = {
        'LuaManager.cpp': 'sol::object, sol::object, float, int, int',
        'LuaQuestState.h': 'const std::string& camPointName, sol::object offset, sol::object rotation',
        'LuaQuestState.cpp': 'fsCamPoint.get(), pOffset, pRotation, duration, i1, i2',
        'GameInterface.cpp': 'CameraUseCameraPoint_NameAndOffset_API = (tCameraUseCameraPoint2)pVTable[412]; // 0x670',
    }
    for name, needle in expected.items():
        path = binding / name
        if name in report['source_hashes']:
            require(hashlib.sha256(path.read_bytes()).hexdigest() == report['source_hashes'][name],
                    'sidecar source changed: ' + name)
        else:
            relative = 'FableScriptExtender/' + name
            if relative in report['source_hashes']:
                require(hashlib.sha256(path.read_bytes()).hexdigest() == report['source_hashes'][relative],
                        'sidecar source changed: ' + name)
        require(needle in path.read_text(), 'sidecar binding changed: ' + name)
    require(source.count(OLD[0]) == 1 and source.count(OLD[1]) == 1,
            'lifted camera call shape changed')
    for diagnostic in DIAGNOSTICS:
        require(todo.count(diagnostic) == 1, 'diagnostic changed: ' + diagnostic)
        todo.remove(diagnostic)
    for old in OLD:
        source = source.replace(old, NEW)
    return source, todo, calls
