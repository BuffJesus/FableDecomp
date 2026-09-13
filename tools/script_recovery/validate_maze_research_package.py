"""Offline Maze Research contract, assets, disabled staging and trace checks."""
import argparse
import hashlib
import io
import json
from pathlib import Path
import sys
import unittest

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT))
from lupa import LuaRuntime
from tools.script_recovery import test_maze_research_reference

def validate(forge):
    port=ROOT/'refs/script_recovery/maze_research'
    staged=ROOT/'refs/script_recovery/reconstructed/MazeResearch/FSE'
    evidence=port/'runtime_evidence'
    checks={}
    def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
    suite=unittest.defaultTestLoader.loadTestsFromModule(test_maze_research_reference)
    stream=io.StringIO();result=unittest.TextTestRunner(stream=stream,verbosity=2).run(suite)
    checks['reference_and_staged_traces']=result.wasSuccessful()
    lua=LuaRuntime();files=sorted(staged.rglob('*.lua'))
    for path in files:
        loaded=lua.eval('load')(path.read_text(encoding='utf-8'),str(path))
        if isinstance(loaded,tuple):raise ValueError(f'{path}: {loaded}')
    checks['all_lua_parses']=len(files)==7
    for name in ('quest.lua','runtime_adapter.lua'):
        checks['reference_copy_'+name]=sha(port/'reference'/name)==sha(staged/'MazeResearch'/name)
    macros=json.loads((evidence/'installed-macros-20260912.json').read_text())
    checks['both_retail_macros']=len(macros['macros'])==2
    for macro in macros['macros']:
        normal,skip=macro['Macro'],macro['SkipCond']
        checks[macro['name']+'_flag_order']=(normal.index('SetFlag UNLIMBO,TRUE') < normal.index('FadeThingIn GoodSword,2')
            and skip.index('SetFlag UNLIMBO,TRUE') < skip.index('DoScriptFrame 2') < skip.index('Remove GoodSword'))
    placements=json.loads((evidence/'installed-placements-20260912.json').read_text())
    checks['seven_placements']=len(placements['records'])==7
    checks['placement_source_unchanged']=sha(Path(placements['path']))==placements['tng_sha256']
    script_bin=Path(placements['path']).parents[2]/'CompiledDefs/script.bin'
    checks['macro_source_unchanged']=sha(script_bin)==macros['scriptBinSha256']
    native_counts={name:len(json.loads((evidence/f'native-{name}-cases-20260912.json').read_text())['cases'])
                   for name in ('history','grave')}
    checks['native_case_inventory']=native_counts=={'history':11,'grave':20}
    runtime=forge/'FableScriptExtender'
    sources=[Path(__file__),Path(test_maze_research_reference.__file__),*files,
             *runtime.glob('*.h'),*runtime.glob('*.cpp'),*evidence.glob('native-*.json')]
    return {'passed':all(checks.values()),'scope':'Offline source package; engine calls doubled, no live playback approval',
            'checks':checks,'native_cases':native_counts,'test_log':stream.getvalue(),
            'inputs':[{'path':str(p.resolve()),'sha256':sha(p)} for p in sorted(sources)]}

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--forge-root',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args()
    # Never overwrite an earlier validation record.
    if args.output.exists():raise SystemExit('Output already exists')
    report=validate(args.forge_root)
    args.output.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({'passed':report['passed'],'checks':report['checks'],'native_cases':report['native_cases']}))
    raise SystemExit(0 if report['passed'] else 1)
