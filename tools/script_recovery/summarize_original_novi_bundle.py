import hashlib
import json
from pathlib import Path

ROOT = Path('work/new-oakvale-original-fse-20260912')
for folder in sorted(p for p in ROOT.iterdir() if p.is_dir()):
    dll = folder / 'FableScriptExtender.dll'
    if dll.exists():
        print(folder.name, dll.stat().st_size, hashlib.sha256(dll.read_bytes()).hexdigest())

for name in ('local-readiness.json', 'integrity.json'):
    p = ROOT / name
    if p.exists():
        print(name, json.loads(p.read_text(encoding='utf-8')))
