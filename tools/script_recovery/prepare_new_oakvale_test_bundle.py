#!/usr/bin/env python3
"""Prepare (but never deploy) a hashed, single-authority New Oakvale test payload."""
from __future__ import annotations
import argparse,hashlib,json,shutil
from pathlib import Path

def sha256(path:Path)->str:
 return hashlib.sha256(path.read_bytes()).hexdigest().upper()

def prepare(root:Path,forge_root:Path,output:Path)->dict:
 if output.exists():raise FileExistsError(f'refusing to overwrite existing bundle: {output}')
 fse_source=root/'refs/script_recovery/reconstructed/NewOakValeIntro/FSE';profile=root/'refs/script_recovery/new_oakvale_intro/runtime_playtest';dll=forge_root/'Release/FableScriptExtender.dll'
 if not fse_source.is_dir() or not profile.is_dir() or not dll.is_file():raise FileNotFoundError('validated FSE source, runtime profile, or Release DLL missing')
 shutil.copytree(fse_source,output/'FSE');shutil.copy2(dll,output/'FableScriptExtender.dll')
 # The reconstructed source remains disabled by default. A runnable bundle must
 # replace its standalone quest registry with the identity-preserving profile;
 # otherwise retail and Lua execute concurrently under two quest authorities.
 shutil.copy2(profile/'quests.lua',output/'FSE/quests.lua')
 override=(profile/'retail_override.lua').read_text(encoding='utf-8')
 disabled='enabled = false'
 if override.count(disabled)!=1:raise ValueError('runtime profile must contain exactly one disabled override switch')
 override=override.replace(disabled,'enabled = true',1)
 (output/'FSE/retail_override.lua').write_text(override,encoding='utf-8')
 quests=(output/'FSE/quests.lua').read_text(encoding='utf-8')
 if not __import__('re').search(r'Quests\s*=\s*\{\s*\}',quests):raise ValueError('runtime bundle would register a second quest authority')
 if 'allowUnverifiedDisposable = true' not in override or 'I UNDERSTAND THIS OVERRIDE MAY CORRUPT DISPOSABLE SAVES' not in override:raise ValueError('runtime override acknowledgement is incomplete')
 files=[]
 for path in sorted(x for x in output.rglob('*') if x.is_file()):
  files.append({'path':path.relative_to(output).as_posix(),'size':path.stat().st_size,'sha256':sha256(path)})
 manifest={'schema':'new-oakvale-test-bundle/0.2','deploymentPerformed':False,'sourceExeRequired':'retail Fable TLC','authority':{'mode':'identity-preserving-retail-override','nativeName':'Q_NewOakValeIntro','overrideEnabled':True,'standaloneQuestRegistered':False},'files':files,'fileCount':len(files),'dllSha256':sha256(output/'FableScriptExtender.dll'),'focus':['single-authority execution','Barrel Man broken-stock rebuke','final-barrel OBJECT_GOLD_1 delivery diagnostics','Bully marker run completion','Affair Wife route identity probes']}
 (output/'bundle_manifest.json').write_text(json.dumps(manifest,indent=2)+'\n',encoding='utf-8')
 return manifest

def main()->int:
 ap=argparse.ArgumentParser();ap.add_argument('--root',type=Path,default=Path(__file__).resolve().parents[2]);ap.add_argument('--forge-root',type=Path,required=True);ap.add_argument('--output',type=Path,required=True);a=ap.parse_args();print(json.dumps(prepare(a.root.resolve(),a.forge_root.resolve(),a.output.resolve()),indent=2));return 0
if __name__=='__main__':raise SystemExit(main())
