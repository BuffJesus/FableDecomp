#!/usr/bin/env bash
# Lift harness (module): compile a multi-function .cpp with VC7.1, then diff EACH
# function's .text bytes against its retail bytes. A per-method MATCH proves the
# lifted C++ recompiles to the original machine code, function by function.
#
# Usage: bash build_module_diff.sh <source.cpp> <expected.tsv>
#   expected.tsv: lines of  <methodNameSubstring>\t<space-separated hex>
set -u
SRC="$1"; EXP="$2"
SCRIPT_DIR=$(cd "$(dirname "$0")" && pwd)
LIFT_ROOT=$(cd "$SCRIPT_DIR/.." && pwd)
BUILD="$LIFT_ROOT/proofs/build"
mkdir -p "$BUILD"
VC="D:/Tools/vc71"
OBJDUMP="C:/Users/Cornelio/AppData/Local/Microsoft/WinGet/Packages/BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe/mingw64/bin/objdump.exe"
OBJ="$BUILD/$(basename "${SRC%.cpp}").obj"
CLOUT="$BUILD/_cl_out.txt"
OD="$BUILD/_od.txt"

# compile 32-bit via PowerShell (avoids MSYS mangling of /flags); same flags as the
# proven single-function harness: /nologo /c /O2 /Oy.
SRCW=$(cygpath -w "$SRC" 2>/dev/null || echo "$SRC")
OBJW=$(cygpath -w "$OBJ" 2>/dev/null || echo "$OBJ")
powershell.exe -NoProfile -Command "\$env:PATH='$VC\\bin;'+\$env:PATH; \$env:INCLUDE='$VC\\include'; \$env:LIB='$VC\\lib'; & '$VC\\bin\\cl.exe' /nologo /c /O2 /Oy /Fo'$OBJW' '$SRCW'" >"$CLOUT" 2>&1
if [ ! -f "$OBJ" ]; then echo "COMPILE FAILED:"; cat "$CLOUT"; exit 1; fi

# per-symbol bytes from objdump -d: gather hex columns under each "<sym>:" until next sym
"$OBJDUMP" -d "$OBJ" 2>/dev/null > "$OD"
python - "$EXP" "$OD" <<'PY'
import re, sys
exp={}
for l in open(sys.argv[1],encoding='utf-8'):
    if not l.strip() or l.startswith('#'): continue
    name,hx=l.rstrip('\n').split('\t')
    exp[name]=hx.strip().lower()

od=open(sys.argv[2],encoding='utf-8',errors='replace').read().splitlines()
funcs={}; cur=None
for l in od:
    m=re.match(r'^[0-9a-f]+ <(.+?)>:',l)
    if m: cur=m.group(1); funcs[cur]=[]; continue
    if cur is None: continue
    mm=re.match(r'^\s+[0-9a-f]+:\s+((?:[0-9a-f]{2} )+)',l)
    if mm: funcs[cur]+= mm.group(1).split()

npass=nfail=0
for name,hx in exp.items():
    # find the objdump symbol whose demangled/decorated name contains the method
    sym=[s for s in funcs if name in s]
    got=' '.join(funcs[sym[0]]) if sym else '(symbol not found)'
    ok = (got==hx)
    print(("  MATCH  " if ok else "  DIFF   ")+f"{name:16} lifted={got}")
    if not ok and sym: print(f"           retail={hx}")
    npass+=ok; nfail+=not ok
print(f"\n{npass}/{npass+nfail} functions byte-identical")
sys.exit(1 if nfail else 0)
PY
