"""Read-only upstream registration audit; method presence is not semantic parity."""
import argparse
import hashlib
import json
import re
import subprocess
from pathlib import Path

def main():
    p = argparse.ArgumentParser()
    p.add_argument('--upstream', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    root = Path(__file__).resolve().parents[2]
    source = a.upstream/'FableScriptExtender/LuaManager.cpp'
    requirements = root/'refs/script_recovery/new_oakvale_intro/api_requirements.json'
    text = source.read_text(encoding='utf-8-sig')
    bindings = {scope:set(re.findall(variable+r'\["(\w+)"\]', text)) for scope,variable in
                [('Quest','questState_type'),('Entity','cscriptThing_type')]}
    rows = []
    for row in json.loads(requirements.read_text())['requirements']:
        scope, name = row.get('bindingScope'), row.get('proposedForgeCall')
        if scope not in bindings:
            continue
        rows.append(dict(scope=scope, method=name, registered=name in bindings[scope],
                         scripts=sorted({e['script'] for e in row.get('nativeEvidence',[]) if 'script' in e})))
    result = dict(upstream_commit=subprocess.check_output(['git','-C',str(a.upstream),'rev-parse','HEAD'],text=True).strip(),
                  hashes={str(f):hashlib.sha256(f.read_bytes()).hexdigest() for f in [source,requirements]},
                  requirements=rows, missing=[r for r in rows if not r['registered']],
                  stock_compatible=False,
                  limits=['Registration-name audit only; existing names do not prove matching signatures or behavior.',
                          'Input inventory covers reviewed native requirements; this is not an exhaustive dynamic Lua call analysis.',
                          'Source revision checked, no distributed upstream DLL tested.'])
    a.output.parent.mkdir(parents=True,exist_ok=True)
    with a.output.open('x',encoding='utf-8') as f:
        json.dump(result,f,indent=2)
    print(f"{len(result['missing'])} missing registrations; report: {a.output}")

if __name__ == '__main__':
    main()
