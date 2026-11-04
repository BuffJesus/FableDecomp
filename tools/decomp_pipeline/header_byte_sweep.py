"""Prove a shared-header edit leaves every includer TU's code unchanged.

  python header_byte_sweep.py snapshot <tag> <header.h> [...]   # before and after the edit
  python header_byte_sweep.py diff <tagA> <tagB>

`snapshot` compiles every rebuild/src/compiled TU that transitively includes one of the
headers (VC7.1 /O2 /MT, canonical rebuild/include) and records, per TU, a hash of each
named symbol's section bytes and relocations. Compiler-local `$L`/`$T` label names are
normalised (their numbering shifts whenever a header grows) and `.debug$S` is ignored
(it embeds the object path). `diff` lists TUs whose named-symbol code differs.
"""
import hashlib, json, re, sys, tempfile
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path
R = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(R / 'tools/decomp_pipeline'))
import check_one as V
from gate_game_component_abi import Coff
INC = R / 'rebuild/include'
STORE = R / 'rebuild/build/header_byte_sweep'


def includers(targets):
    memo = {}
    def incs(p):
        if p in memo:
            return memo[p]
        memo[p] = set()
        out = set()
        try:
            text = p.read_text(encoding='utf-8', errors='replace')
        except OSError:
            return out
        for name in re.findall(r'^\s*#\s*include\s*"([^"]+)"', text, re.M):
            q = next((q for q in (p.parent / name, INC / name) if q.is_file()), None)
            if q:
                out.add(q.name)
                out |= incs(q)
        memo[p] = out
        return out
    return [p for p in (R / 'rebuild/src/compiled').rglob('*.cpp') if targets & incs(p)]


def code_hashes(obj):
    c = Coff(obj)
    result = {}
    for name, (section, offset) in c.symbols.items():
        if name.startswith(('.', '$')) or section not in c.sections:
            continue
        body, rels = c.sections[section]
        relocs = sorted((site, '$LBL' if target.startswith('$') else target, kind)
                        for site, (target, kind) in rels.items())
        result[name] = hashlib.sha256(repr((offset, bytes(body), relocs)).encode()).hexdigest()
    return result


def snapshot(tag, headers):
    sources = includers(set(headers))
    print('sources', len(sources), flush=True)
    env = V.env()
    build = Path(tempfile.mkdtemp(prefix='header-byte-sweep-'))
    def one(src):
        obj = build / (hashlib.sha1(str(src).encode()).hexdigest()[:16] + '.obj')
        r = V.cl(['/nologo', '/c', '/O2', '/MT', '/I' + str(INC), '/Fo' + str(obj), str(src)], env, build)
        key = str(src.relative_to(R))
        return key, ('COMPILE_FAIL' if r.returncode else code_hashes(obj))
    with ThreadPoolExecutor(8) as pool:
        result = dict(pool.map(one, sources))
    STORE.mkdir(parents=True, exist_ok=True)
    (STORE / f'{tag}.json').write_text(json.dumps(result))
    print('snapshot', tag, len(result), 'compile failures', sum(v == 'COMPILE_FAIL' for v in result.values()))


def diff(a_tag, b_tag):
    a = json.loads((STORE / f'{a_tag}.json').read_text())
    b = json.loads((STORE / f'{b_tag}.json').read_text())
    changed = []
    for key in sorted(set(a) & set(b)):
        if a[key] != b[key]:
            names = sorted(n for n in set(a[key]) | set(b[key])
                           if not isinstance(a[key], dict) or not isinstance(b[key], dict)
                           or a[key].get(n) != b[key].get(n)) if isinstance(a[key], dict) and isinstance(b[key], dict) else ['COMPILE_FAIL']
            changed.append((key, names))
    print('compared', len(set(a) & set(b)), 'changed', len(changed))
    for key, names in changed:
        print(' ', key, names[:6])
    return changed


if __name__ == '__main__':
    if sys.argv[1] == 'snapshot':
        snapshot(sys.argv[2], sys.argv[3:])
    elif sys.argv[1] == 'diff':
        sys.exit(1 if diff(sys.argv[2], sys.argv[3]) else 0)
    else:
        raise SystemExit(__doc__)
