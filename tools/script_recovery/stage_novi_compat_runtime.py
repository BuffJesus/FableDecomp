"""Snapshot reviewed runtime into a separate sidecar build; never edit the checkout."""
import argparse
import hashlib
import json
import shutil
from pathlib import Path

def replace_once(text, old, new):
    if text.count(old) != 1:
        raise ValueError('source anchor drift: '+old[:90])
    return text.replace(old,new,1)

def main():
    p=argparse.ArgumentParser()
    p.add_argument('--runtime',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    a=p.parse_args()
    if a.output.exists(): raise FileExistsError(a.output)
    a.output.mkdir(parents=True)
    inventory={}
    for folder in ['FableScriptExtender','Vendor']:
        for f in (a.runtime/folder).rglob('*'):
            if not f.is_file() or any(part in ['Release','Debug','.vs'] for part in f.relative_to(a.runtime).parts): continue
            relative=f.relative_to(a.runtime)
            dst=a.output/relative
            dst.parent.mkdir(parents=True,exist_ok=True)
            shutil.copy2(f,dst)
            inventory[relative.as_posix()]=hashlib.sha256(f.read_bytes()).hexdigest()
    shutil.copy2(a.runtime/'FableScriptExtender.sln',a.output)
    dll=a.output/'FableScriptExtender/dllmain.cpp'
    s=dll.read_text(encoding='utf-8-sig')
    marker='void __declspec(naked) MyHook() {'
    if s.count(marker)!=1: raise ValueError('hook anchor drift')
    s=s[:s.index(marker)] + '\n#include "NoviCompatibilityChain.inl"\n'
    for call in ['InstallMapResourceAliasHook();','LoadStartupMapResourceAlias();','RetailScriptShadowRunner::RunConfigured(g_fseBasePath);']:
        s=replace_once(s,call,'/* Local compatibility build: disabled '+call+' */')
    dll.write_text(s,encoding='utf-8')
    chain=Path(__file__).with_name('novi_compat_chain.inl')
    shutil.copy2(chain,a.output/'FableScriptExtender/NoviCompatibilityChain.inl')
    api=a.output/'FableScriptExtender/FableAPI.cpp'
    s=api.read_text(encoding='utf-8-sig')
    s=replace_once(s,'g_fseBasePath = std::string(dllPath) + "/FSE";',
                   'g_fseBasePath = std::string(dllPath) + "/NoviCompatibility";')
    api.write_text(s,encoding='utf-8')
    report=dict(source=str(a.runtime),source_hashes=inventory,
                chain_sha256=hashlib.sha256(chain.read_bytes()).hexdigest(),
                policy='Local-only independent quest hosts. Original FSE is loaded and chained, not replaced.')
    (a.output/'snapshot.json').write_text(json.dumps(report,indent=2))
    print(a.output)

if __name__=='__main__': main()
