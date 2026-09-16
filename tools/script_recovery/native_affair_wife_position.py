"""Map both native position branches without guessing the missing-actor fallback."""
import hashlib
import json
import re
from pathlib import Path


def map_affair_wife_position(function, rdata):
    w=json.loads(Path(__file__).with_name('native_affair_wife_position_witness.json').read_text())
    if str(function.get('address','')).lower()!=w['address'].lower():
        return []
    if hashlib.sha256(function.get('decompile','').encode()).hexdigest()!=w['sourceSha256']:
        return [{'status':'rejected','reason':'position source changed'}]
    for region in w['regions']:
        raw=rdata.bytes_at(region['address'],region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest()!=region['sha256']:
            return [{'status':'rejected','reason':'position native evidence changed'}]
    return [dict(w,status='mapped')]


def recover_affair_wife_position(function, source, rdata):
    evidence = map_affair_wife_position(function, rdata)
    if not evidence or evidence[0]['status'] != 'mapped':
        return source, evidence
    if hashlib.sha256(source.encode()).hexdigest() != 'c577da4d72d3240feb433337e30566599e1471aad60998607a7677a08e121e4d':
        return source, [{'status': 'rejected', 'reason': 'position annotated source changed'}]
    branch = (r'if \(piStack_9c == \(int \*\)0x0\) \{\s*'
              r'pCVar9 = \(CScriptThing \*\)&DAT_0143e8e0;\s*}\s*else \{\s*'
              r'pCVar9 = \(CScriptThing \*\)\(\*\*\(code \*\*\)\(\*piStack_9c \+ 0x18\)\)\(\);\s*}')
    operands = (r'&stack0xffffff40,pCVar9,\s*'
                r'\(char \*\)0x40000000,\(CCharString \*\)0x1,\(CCharString \*\)0x0,\(CCharString \*\)0x1,\s*'
                r'\(bool\)SUB41\(pCVar8,0\),\(bool\)SUB41\(pCVar27,0\)')
    if (len(re.findall(branch, source)) != 1 or len(re.findall(operands, source)) != 1
            or source.count('GSI->GetThingWithScriptName(&local_native_cached_husband,"NOVI_AffairMan")') != 1):
        return source, [{'status': 'rejected', 'reason': 'position/movement source correspondence changed'}]
    source = re.sub(branch, 'pCVar9 = GSI->RetailThingPosition(local_native_cached_husband);', source)
    source = re.sub(operands, '&stack0xffffff40,pCVar9,2.0,1,0,1', source)
    evidence[0].update(status='recovered', loweringStatus='position and five movement operands recovered',
        runtimeRequirement='proposed RetailThingPosition global binding',
        lifetimeLimitation='cached Thing and movement resource ownership remain draft diagnostics')
    return source, evidence
