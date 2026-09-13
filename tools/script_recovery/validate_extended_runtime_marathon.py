"""Verify this offline marathon's preserved failures, passing proofs and install hashes."""
import argparse,hashlib,io,json
from pathlib import Path
import sys,unittest
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT))
from tools.script_recovery import test_guardian_sister_reference

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def validate():
    checks={};inputs={Path(__file__)}
    def data(relative):
        p=ROOT/relative;inputs.add(p);return json.loads(p.read_text())
    def report(relative):
        r=data(relative);folder=(ROOT/relative).parent
        valid=r['passed'] and all(sha(Path(p['path']))==p['sha256'] for p in r['inputs'])
        valid=valid and all(sha(folder/p['path'])==p['sha256'] for p in r['outputs'])
        return r,valid
    asan,checks['asan_report_and_hashes']=report('work/runtime-marathon-state-conversation-asan-20260912/result.json')
    checks['asan_case_inventory']=asan['asan'] and asan['scenarios']==6 and asan['state_contract_groups']==3 and asan['ambient_contract_groups']==3 and asan['conversation_lines_passed']
    regression,checks['regression_report_and_hashes']=report('work/runtime-marathon-verified-regression-20260912/result.json')
    checks['regression_inventory']=regression['entity_cases']==14 and regression['matching_events']==255 and regression['guardian_entity_cases']==28 and regression['guardian_entity_events']==1377 and regression['staged_root_binding_cases']==3
    old_calls=ROOT/'work/runtime-marathon-final-regression-20260912/conversation-lines.lua'
    current_calls=ROOT/'work/runtime-marathon-verified-regression-20260912/conversation-lines.lua'
    inputs.update((old_calls,current_calls));checks['asan_calls_match_fresh_generation']=sha(old_calls)==sha(current_calls)
    old=ROOT/'work/runtime-entity-vm-asan-repro-20260912'
    failed=[]
    for name in ('unregister','clear','shutdown','reinitialize','replace','process_exit'):
        p=old/(name+'.log');inputs.add(p);failed.append('ERROR: AddressSanitizer: heap-use-after-free' in p.read_text())
    checks['six_preserved_vm_uaf_reproductions']=all(failed)
    for name,path,pattern in [
        ('borrowed_type_error','work/runtime-ambient-borrowed-proof-20260912/ambient.log','sol: no matching function call'),
        ('borrowed_helper_overread','work/runtime-ambient-borrowed-fixed-20260912/ambient.log','ERROR: AddressSanitizer: heap-buffer-overflow')]:
        p=ROOT/path;inputs.add(p);checks[name]=pattern in p.read_text()
    native_audio=data('refs/script_recovery/new_oakvale_intro/runtime_evidence/native-audio-update-gates-20260912.json')
    audio_repeat=data('work/native-audio-update-gates-repeat-20260912.json')
    checks['91_repeatable_native_audio_cases']=len(native_audio['cases'])==91 and native_audio==audio_repeat
    native_sister=data('refs/script_recovery/guardian_sister_info/runtime_evidence/native-root-lifecycle-20260912.json')
    sister_repeat=data('work/native-guardian-sister-root-repeat-20260912.json')
    checks['19_repeatable_sister_native_cases']=len(native_sister['cases'])==19 and native_sister==sister_repeat
    stream=io.StringIO();suite=unittest.defaultTestLoader.loadTestsFromModule(test_guardian_sister_reference)
    tests=unittest.TextTestRunner(stream=stream,verbosity=2).run(suite)
    checks['sister_reference_and_adapter']=tests.wasSuccessful()
    port=ROOT/'refs/script_recovery/guardian_sister_info'
    inputs.update((port/'reference').glob('*.lua'));inputs.add(Path(test_guardian_sister_reference.__file__))
    assets=data('refs/script_recovery/guardian_sister_info/runtime_evidence/installed-assets-reviewed-20260912.json')
    checks['sister_asset_inputs_unchanged']=all(sha(Path(p['path']))==p['sha256'] for p in assets['sources'])
    checks['sister_asset_inventory']=len(assets['macro']['Macro'])==101 and len(assets['macro']['SkipCond'])==9 and len(assets['requested_text_keys'])==14 and len(assets['placements'])==19
    checks['missing_markers_explicitly_recorded']=assets['missing_authored_placements']==['MK_GSI_FAN1END','MK_GSI_FAN2END']
    game=Path('C:/Programs/Steam/steamapps/common/Fable The Lost Chapters')
    deployment=data('refs/script_recovery/new_oakvale_intro/runtime_evidence/deployment-v23-20260912.json')
    checks['all_25_installed_files_still_v23']=len(deployment['files'])==25 and all(sha(game/p['path'])==p['after'].lower() for p in deployment['files'])
    checks['native_exe_still_pinned']=sha(game/'Fable.exe')==native_audio['exe_sha256']==native_sister['exe_sha256']
    build_log=ROOT/'work/runtime-marathon-verified-release-20260912/build.log';inputs.add(build_log)
    build_text=build_log.read_text(encoding='utf-8',errors='replace')
    checks['release_build_completed']='FableScriptExtender.vcxproj ->' in build_text and 'error ' not in build_text.lower()
    candidate=Path('D:/Code/ForgeFSE-retail-shadow/Release/FableScriptExtender.dll');inputs.add(candidate)
    checks['candidate_is_uninstalled']=sha(candidate)!=sha(game/'FableScriptExtender.dll')
    for path in ['work/runtime-20260912-video-comparison/prove_audio_update_gates.py',
                 'work/runtime-20260912-video-comparison/prove_guardian_sister_root.py',
                 'tools/script_recovery/audit_guardian_sister_assets.py',
                 'work/runtime-entity-vm-asan-repro-20260912/LuaManager-before.h',
                 'work/runtime-ambient-borrowed-fixed-20260912/LuaQuestState-before-pointer-fix.cpp']:
        inputs.add(ROOT/path)
    return dict(passed=all(checks.values()),scope=__doc__,checks=checks,test_log=stream.getvalue(),
        candidate_dll_sha256=sha(candidate),
        limits=['Game/engine API boundaries are doubled in compiled and native replay checks.',
                'No live crash, audible volume issue, save-file round trip or streaming behavior is claimed fixed.',
                'Guardian Sister entity Main and two authored fan-exit placements remain unresolved.'],
        inputs=[{'path':str(p.resolve()),'sha256':sha(p)} for p in sorted(inputs)])

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    if a.output.exists():raise FileExistsError(a.output)
    result=validate();a.output.write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({'passed':result['passed'],'checks':result['checks'],'candidate_dll_sha256':result['candidate_dll_sha256']}))
    raise SystemExit(0 if result['passed'] else 1)
