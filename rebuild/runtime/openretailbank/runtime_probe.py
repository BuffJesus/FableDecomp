import shutil
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, r'D:/Documents/FableTLC/tools')
sys.path.insert(0, r'D:/Documents/FableTLC/tools/decomp_pipeline')
import verify_and_land as V

ROOT = Path(r'D:/Documents/FableTLC')
e = V.env()
work = V.WORK_ROOT / 'runtimeprobe'
shutil.rmtree(work, ignore_errors=True)
work.mkdir(parents=True, exist_ok=True)

srcs = [
    ROOT / 'rebuild/src/compiled/00/9a/CBankFileManager_OpenRetailBank_009a8840.cpp',
    ROOT / 'rebuild/runtime/openretailbank/ring_thunks.cpp',
    ROOT / 'rebuild/runtime/openretailbank/runtime_globals.cpp',
    ROOT / 'rebuild/runtime/openretailbank/runtime_helpers.cpp',
]
manifest = (ROOT / 'rebuild/runtime/openretailbank/link_manifest.tsv').read_text(
    encoding='utf-8').splitlines()[1:]
for line in manifest:
    fields = line.split('\t')
    if fields[3] in ('landed-byte-exact', 'faithful-behavior'):
        srcs.append(ROOT / fields[4])
srcs.append(ROOT / 'rebuild/runtime/openretailbank/runtime_probe.cpp')

objects = []
failed = []
for source in srcs:
    obj = work / (source.stem + '.obj')
    cp = V.cl(['/nologo', '/c', '/O2', '/Oy', '/W3', f'/Fo{obj}', str(source)], e)
    if cp.returncode or not obj.exists():
        failed.append((source.name, (cp.stdout or '')[-500:]))
    else:
        objects.append(obj)

print(f'compiled {len(objects)}/{len(srcs)} objs; {len(failed)} failed')
for name, detail in failed:
    print('  FAIL', name, detail)
if failed:
    raise SystemExit(1)

exe = work / 'oab_probe.exe'
link = subprocess.run(
    [str(V.VC / 'bin' / 'link.exe'), '/nologo', '/SUBSYSTEM:CONSOLE',
     f'/out:{exe}'] + [str(obj) for obj in objects],
    capture_output=True, text=True, env=e)
output = (link.stdout or '') + '\n' + (link.stderr or '')
if link.returncode:
    print(output)
    raise SystemExit(link.returncode)

try:
    run = subprocess.run(
        [str(exe)] + sys.argv[1:], capture_output=True, text=True,
        timeout=15)
except subprocess.TimeoutExpired as timeout:
    print(timeout.stdout or '', end='')
    print(timeout.stderr or '', end='')
    print('PROBE timeout after 15 seconds')
    raise SystemExit(124)
print(run.stdout, end='')
print(run.stderr, end='')
raise SystemExit(run.returncode)
