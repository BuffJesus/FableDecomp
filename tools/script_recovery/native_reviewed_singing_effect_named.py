"""Recover SingingStone effect position and script name through the sidecar ABI."""
import hashlib
import json
import struct
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
HIT = 'quest:CreateEffectAtPos("MARKTELEPORTER", xStack_c, 0.0, false)'
PRIOR = 'quest:CreateEffectAtPos("MARKTELEPORTER", pCVar4, 0.0, false, pCVar3)'
POSITION_TODOS = (
    '-- TODO(native): xStack_18 = (int *)(pCVar4.z + 1.0);',
    '-- TODO(native): xStack_18._4_4_ = pCVar4.y;',
    '-- TODO(native): xStack_18._0_4_ = pCVar4.x;',
)
POSITION_DIAGNOSTICS = (
    'unlifted: xStack_18 = (int *)(pCVar4.z + 1.0);',
    'unlifted: xStack_18._4_4_ = pCVar4.y;',
    'unlifted: xStack_18._0_4_ = pCVar4.x;',
)


def restore(source, todo, calls, fn, row, rdata, sidecar):
    if row is None:
        return source, todo, calls

    def require(ok, reason):
        if not ok:
            raise ValueError('SingingStone named effect: ' + reason)

    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    require(start == 0x00ED3370 and
            hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'],
            'reviewed source changed')
    require(hashlib.sha256(rdata.bytes_at(start, end-start)).hexdigest() == row['nativeSha256'],
            'native body changed')
    native = fn['decompile']
    require(native.count('"StonesEffect"') == 2 and native.count('"MARKTELEPORTER"') == 2 and
            native.count('_DAT_0122ded8') == 2 and
            native.count(' + 400))') == 2 and
            'auStack_c._8_4_ = *(float *)&pCVar4->field_0x8 + _DAT_0122ded8;' in native and
            'piStack_10 = (int *)(*(float *)&pCVar4->field_0x8 + _DAT_0122ded8);' in native,
            'native effect call changed')
    require(struct.unpack('<f', rdata.bytes_at(0x0122ded8, 4))[0] == 1.0,
            'native position offset changed')
    sidecar_path = ROOT / sidecar['path']
    report_path = ROOT / sidecar['buildReport']
    report = json.loads(report_path.read_text())
    require(hashlib.sha256(report_path.read_bytes()).hexdigest() == sidecar['buildReportSha256'],
            'sidecar build report changed')
    require(hashlib.sha256((ROOT / report['patch']).read_bytes()).hexdigest() == report['patch_sha256'] and
            hashlib.sha256(Path(report['candidate_dll']).read_bytes()).hexdigest() == report['candidate_dll_sha256'],
            'sidecar patch or DLL changed')
    expected = {
        'FableScriptExtender/LuaQuestState.h': 'sol::optional<std::string> scriptName',
        'FableScriptExtender/LuaQuestState.cpp': 'FableString fsScript(scriptName.value_or("").c_str());',
        'FableScriptExtender/LuaManager.cpp': 'return self.CreateEffectAtPos(effectName, position, angle, independent, alwaysUpdate, scriptName);',
        'FableScriptExtender/ManifestFunctionRegistry.generated.h': '{"scriptName", "sol::optional<std::string>", true}',
    }
    for relative, needle in expected.items():
        path = sidecar_path / relative
        require(hashlib.sha256(path.read_bytes()).hexdigest() == report['source_hashes'][relative] and
                needle in path.read_text(), 'sidecar binding changed: ' + relative)

    require(source.count(HIT) == 1 and source.count(PRIOR) == 1,
            'lifted effect calls changed')
    for marker, diagnostic in zip(POSITION_TODOS, POSITION_DIAGNOSTICS):
        require(source.count(marker) == 1 and todo.count(diagnostic) == 1,
                'lifted position changed')
        source = source.replace('    ' + marker + '\n', '')
        todo.remove(diagnostic)
    source = source.replace(HIT, 'quest:CreateEffectAtPos("MARKTELEPORTER", xStack_c, 0.0, false, false, "StonesEffect")')
    source = source.replace(PRIOR,
        'quest:CreateEffectAtPos("MARKTELEPORTER", {x = pCVar4.x, y = pCVar4.y, z = pCVar4.z + 1.0}, 0.0, false, false, "StonesEffect")')
    return source, todo, calls
