"""Validate a local compatibility payload; launch only with --launch.

Does not install, replace, delete or patch files in the game directory.
"""
import argparse,csv,datetime,hashlib,json,re,shutil,subprocess
from pathlib import Path

def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def validate(bundle,game):
    manifest=json.loads((bundle/'manifest.json').read_text())
    for name,digest in manifest['files'].items():
        target=(bundle/name).resolve()
        if not target.is_relative_to(bundle.resolve()) or sha(target)!=digest:
            raise ValueError('Payload hash mismatch: '+name)
    if sha(game/'Fable.exe')!=manifest['game_exe_sha256']:
        raise ValueError('Unsupported Fable.exe')
    text=(game/'data/Levels/FinalAlbion.qst').read_text(encoding='utf-8-sig')
    if re.search(r'^\s*AddQuest\s*\(\s*"NewOakValeIntro"',text,re.M):
        raise ValueError('Duplicate legacy NewOakValeIntro activation exists in FinalAlbion.qst')
    return manifest

def main():
    p=argparse.ArgumentParser()
    p.add_argument('--bundle',type=Path,default=Path(__file__).resolve().parent)
    p.add_argument('--game-dir',type=Path,required=True)
    p.add_argument('--launch',action='store_true')
    p.add_argument('--save-dir',type=Path,default=Path.home()/'Documents/My Games/Fable/Saves')
    a=p.parse_args();bundle=a.bundle.resolve();game=a.game_dir.resolve()
    validate(bundle,game)
    if not a.launch:
        print('Preflight passed. No game launched and no installed files changed.');return
    proc=subprocess.run(['tasklist','/FI','IMAGENAME eq Fable.exe','/FO','CSV','/NH'],capture_output=True,text=True,check=True,creationflags=0x08000000)
    if any(row and row[0].lower()=='fable.exe' for row in csv.reader(proc.stdout.splitlines())):
        raise RuntimeError('Close the existing game before launching a separate test')
    session=bundle/'runs'/datetime.datetime.now().strftime('%Y%m%d-%H%M%S-%f')
    session.mkdir(parents=True,exist_ok=False)
    if not a.save_dir.is_dir():raise FileNotFoundError('Specify the actual Fable save directory with --save-dir')
    shutil.copytree(a.save_dir,session/'save-backup')
    backup={str(f.relative_to(a.save_dir)):sha(f) for f in a.save_dir.rglob('*') if f.is_file()}
    (session/'save-backup.json').write_text(json.dumps(dict(source=str(a.save_dir.resolve()),files=backup),indent=2))
    for name in ['FSE','NoviCompatibility']:
        log=bundle/name/'FableScriptExtender.log'
        if log.exists():shutil.copy2(log,session/(name+'-previous.log'))
    command=[str(bundle/'NoviLocalLauncher.exe'),str(game/'Fable.exe'),str(game),
             str(bundle/'FableScriptExtender.dll'),str(bundle/'NoviCompatibility.dll')]
    result=subprocess.run(command,capture_output=True,text=True,timeout=120)
    (session/'launch.log').write_text(result.stdout+result.stderr)
    result.check_returncode()
    validate(bundle,game)
    launch_info=json.loads(result.stdout.strip().splitlines()[-1])
    with (session/'observer.log').open('w') as observer_log:
        observer=subprocess.Popen([__import__('sys').executable,str(bundle/'observe_retail_cutscene.py'),
                                   '--pid',str(launch_info['pid']),'--seconds','1800',
                                   '--proof',str(bundle/'observer-proof.json'),
                                   '--output',str(session/'observer.jsonl')],
                                  stdout=observer_log,stderr=subprocess.STDOUT,creationflags=0x08000000)
    (session/'observer-process.json').write_text(json.dumps({'pid':observer.pid,'game_pid':launch_info['pid']}))
    print(result.stdout.strip())
    print('Save backup: '+str(session/'save-backup'))
    print('Read-only audio/cutscene observer started; output: '+str(session/'observer.jsonl'))
    print('Original FSE and the add-on are loaded from this test folder. Installed v23 files are unchanged.')

if __name__=='__main__':main()
