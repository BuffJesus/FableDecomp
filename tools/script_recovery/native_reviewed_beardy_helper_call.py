"""Restore two reviewed local beard-helper calls in BeardyBaldy Main."""
import hashlib
import re

CALL = re.compile(r'(?m)^(?P<indent> +)-- TODO\(native\): bVar6 = helper_E53670\(quest, me, \*\(this \+ 0x14\)\)\n(?P=indent)bVar6 = nil --\[\[unresolved native value\]\]$')


def restore(source, todo, calls, fn, helper, row, rdata, helper_binding):
    if row is None:
        return source, todo, calls
    def require(ok, reason):
        if not ok:
            raise ValueError('BeardyBaldy helper call: ' + reason)
    require(helper is not None and helper_binding.get('address', '').lower() == row['helperAddress'], 'helper registration changed')
    for native, evidence in ((fn, row['caller']), (helper, row['helper'])):
        require(hashlib.sha256(native['decompile'].encode()).hexdigest() == evidence['sourceSha256'], 'source changed')
        start,end=int(native['address'],16),int(native['bodyEndExclusive'],16)
        require(hashlib.sha256(rdata.bytes_at(start,end-start)).hexdigest() == evidence['nativeSha256'], 'native body changed')
    direct={c['site'].lower():c for c in fn['calls']}
    require(len(row['sites']) == 2 and all(direct.get(site,{}).get('target','').lower() == row['helperAddress'] for site in row['sites']),
            'native call sites changed')
    require(len(CALL.findall(source)) == 2, 'lifted helper calls changed')
    extra='helper_E53670: 1 helper operands for 0 parameters'
    missing='unlifted: bVar6 = helper_E53670(quest, me, *(this + 0x14))'
    require(todo.count(extra) == 2 and todo.count(missing) == 2, 'helper diagnostics changed')
    rewritten=CALL.sub(lambda match: match.group('indent') + 'bVar6 = helper_E53670(quest, me)', source)
    todo=[item for item in todo if item not in (extra, missing)]
    return rewritten,todo,[*calls,'helper_E53670']
