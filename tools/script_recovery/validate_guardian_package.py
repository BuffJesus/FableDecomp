"""Validate disabled Guardian staging against compiled checks and local evidence.

Reads installed files only. No launch, install, activation, or save writes.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
from lupa import LuaRuntime

ROOT=Path(__file__).resolve().parents[2]
NAME='QS_GuardianTrophyDealerInfo'
ENTITY='GTDI_Maze'
V23='c3900b3db85423e1a7e4035825b265225e4892eb26522b92580aa2a055539d31'

def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()

def registry(path):
    lua=LuaRuntime();lua.execute(path.read_text(encoding='utf-8-sig'))
    def native(value):
        if hasattr(value,'items'):return {k:native(v) for k,v in value.items()}
        return value
    return native(lua.globals().RetailOverrides),native(lua.globals().Quests)

def validate(runtime_report,game):
    evidence=ROOT/'refs/script_recovery/guardian_trophy_dealer_info/runtime_evidence'
    staged=ROOT/'refs/script_recovery/reconstructed/GuardianTrophyDealerInfo/FSE'
    package=staged/'GuardianTrophyDealerInfo'
    checks={};inputs={Path(__file__),runtime_report}
    report=json.loads(runtime_report.read_text())
    checks['compiled_checks_passed']=report.get('passed') is True
    checks['compiled_inventory']=[report.get(k) for k in ('guardian_entity_cases','guardian_entity_events',
        'guardian_watcher_cases','guardian_persistence_cases','guardian_root_contract_cases')]==[28,1377,10,8,1]
    checks['compiled_inputs_unchanged']=all(sha(Path(p['path']))==p['sha256'] for p in report['inputs'])
    checks['compiled_outputs_unchanged']=all(sha(runtime_report.parent/p['path'])==p['sha256'] for p in report['outputs'])
    files=sorted(staged.rglob('*.lua'));inputs.update(files)
    lua=LuaRuntime()
    for path in files:
        loaded=lua.eval('load')(path.read_text(encoding='utf-8'),str(path))
        if isinstance(loaded,tuple):raise ValueError(f'Parse failed: {path}: {loaded}')
    checks['six_lua_files_parse']=len(files)==6
    compiled_paths={Path(p['path']).resolve() for p in report['inputs']}
    checks['all_package_files_in_compiled_hashes']=all(p.resolve() in compiled_paths for p in files)
    root_proof=evidence/'native-root-cases-20260912.json';inputs.add(root_proof)
    root_cases=json.loads(root_proof.read_text())['cases']
    root_results=[]
    for case in root_cases:
        vm=LuaRuntime();vm.globals().package.path=staged.as_posix()+'/?.lua;'+vm.globals().package.path
        vm.execute((package/'GuardianTrophyDealerInfo.lua').read_text())
        root=vm.eval('''function(initial)
            local q={piece=initial,events={}}
            local function record(...) q.events[#q.events+1]={...} end
            function q:SetStateBool(key,v) assert(key=='PieceOver');self.piece=v;record('piece_over',v) end
            function q:AddEntityBinding(name,path,flags)
                assert(path=='GuardianTrophyDealerInfo/Entities/GTDI_Maze');record('bind',name,flags)
            end
            function q:FinalizeEntityBindings() record('finalize') end
            function q:CreateThread(name,options) record('spawn',name,options.region) end
            function q:GetActiveQuestName() record('active');return 'QS_GuardianTrophyDealerInfo' end
            function q:SetQuestCardObjective(...) record('objective',...) end
            Init(q);Main(q);assert(q.piece==false);return q.events
        end''')(case['initial'])
        root_results.append([list(row.values()) for row in root.values()]==case['adapter_events'])
    checks['two_native_root_comparisons']=len(root_cases)==2 and all(root_results)
    for name in ('quest.lua','runtime_adapter.lua'):
        reference=evidence.parent/'reference'/name;inputs.add(reference)
        checks['reference_copy_'+name]=sha(reference)==sha(package/name)
    overrides,legacy=registry(staged/'retail_override.lua')
    _,legacy=registry(staged/'quests.lua')
    checks['disabled']=overrides['enabled'] is False and overrides['allowUnverifiedDisposable'] is False and overrides['disposableSaveAcknowledgement']==''
    checks['no_legacy_registration']=not legacy
    entries=overrides['entries'];entry=entries[1];entity=entry['entity_scripts'][1]
    checks['one_override_one_entity']=len(entries)==1 and len(entry['entity_scripts'])==1
    checks['override_identity']=entry['nativeName']==NAME and entry['mode']=='override' and entry['file']=='GuardianTrophyDealerInfo/GuardianTrophyDealerInfo'
    checks['entity_identity']=entity=={'name':ENTITY,'id':69,'file':'GuardianTrophyDealerInfo/Entities/GTDI_Maze'}
    corpus_path=ROOT/'refs/script_recovery/seed_corpus/corpus_index.json';inputs.add(corpus_path)
    corpus=json.loads(corpus_path.read_text())
    packages=corpus if isinstance(corpus,list) else corpus['packages']
    seed=next(p for p in packages if p['name']=='GuardianTrophyDealerInfo')['registry'][0]
    checks['seed_identity_agrees']=seed['key']==NAME and seed['file']==entry['file'] and seed['entities']==[entity]
    registry_path=ROOT/'ghidra_out/quest_registry_table.tsv';inputs.add(registry_path)
    checks['native_registry_name']=sum(line.startswith(NAME+'\t') for line in registry_path.read_text().splitlines())==1
    qst=game/'data/Levels/FinalAlbion.qst';inputs.add(qst)
    checks['single_inactive_native_registration']=re.findall(r'(?m)^\s*AddQuest\("'+NAME+r'",\s*(TRUE|FALSE)\);',qst.read_text())==['FALSE']
    conflicts=[];examined=[]
    configs=[p for p in (ROOT/'refs/script_recovery/reconstructed').rglob('retail_override.lua') if p.parent!=staged]
    configs += [game/'FSE/retail_override.lua']
    for path in configs:
        inputs.add(path);other,_=registry(path);examined.append(str(path))
        for row in (other or {}).get('entries',{}).values():
            if row.get('nativeName')==NAME:conflicts.append({'path':str(path),'quest':NAME})
            for e in row.get('entity_scripts',{}).values():
                if e.get('id')==69 or e.get('name')==ENTITY:conflicts.append({'path':str(path),'entity':e})
    for path in [p.with_name('quests.lua') for p in configs]:
        inputs.add(path);_,other=registry(path);examined.append(str(path))
        for key,row in (other or {}).items():
            if key==NAME or row.get('file')==entry['file']:conflicts.append({'path':str(path),'legacy':key})
            for e in row.get('entity_scripts',{}).values():
                if e.get('id')==69 or e.get('name')==ENTITY:conflicts.append({'path':str(path),'legacy_entity':e})
    checks['no_current_registry_conflicts']=not conflicts
    card_path=evidence/'installed-card-component-20260912.json';inputs.add(card_path)
    card=json.loads(card_path.read_text())
    checks['card_assets_unchanged']=all(sha(game/p)==h for p,h in card['source_sha256'].items())
    macros_path=evidence/'installed-macros-20260912.json';inputs.add(macros_path)
    macros=json.loads(macros_path.read_text())
    checks['macro_asset_unchanged']=sha(game/'data/CompiledDefs/script.bin')==macros['script_sha256']
    placements_path=evidence/'installed-placements-20260912.json';inputs.add(placements_path)
    placements=json.loads(placements_path.read_text())
    checks['placement_assets_unchanged']=all(sha(Path(p['path']))==p['sha256'] for p in placements['files'])
    dlls=[game/'FableScriptExtender.dll',game/'FSE/FableScriptExtender.dll'];inputs.update(dlls)
    checks['installed_dlls_still_v23']=all(sha(p)==V23 for p in dlls)
    inputs.update(evidence.glob('native-*.json'))
    return dict(passed=all(checks.values()),scope=__doc__,checks=checks,conflicts=conflicts,
        examined_registries=examined,
        limits=['Native engine APIs and scheduler are doubled in compiled checks.',
                'Root replay covers successful allocations; native allocation failure is not a Lua allocation contract.',
                'Entity ID 69 comes from the seed Lua registry, not a native numeric entity ID.',
                'Conflict checks cover the listed installed and reconstructed profiles at these hashes.',
                'Requires newer uninstalled runtime APIs; live scheduler, streaming, save/load and playback remain unverified.'],
        inputs=[{'path':str(p.resolve()),'sha256':sha(p)} for p in sorted(inputs)])

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--runtime-report',type=Path,required=True)
    parser.add_argument('--game-root',type=Path,default=Path('C:/Programs/Steam/steamapps/common/Fable The Lost Chapters'))
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args()
    if args.output.exists():raise FileExistsError(args.output)
    result=validate(args.runtime_report,args.game_root)
    args.output.write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({'passed':result['passed'],'checks':result['checks'],'conflicts':result['conflicts']}))
    raise SystemExit(0 if result['passed'] else 1)
