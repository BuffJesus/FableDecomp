#!/usr/bin/env python3
"""
gen_fse_bindings.py  --  ForgeFSE native-binding skeleton generator.

Turns reverse-engineering rows (name / address / demangled MSVC signature / scope)
into the 4-part ForgeFSE Sol2 binding skeleton:

    1. FableAPI.h        : typedef of the raw function pointer + `extern` declaration
    2. FableAPI.cpp      : global definition (= nullptr) + ASLR<>() resolution line
    3. LuaQuestState.cpp : a LuaQuestState method stub that calls the raw pointer
    4. LuaManager.cpp    : the questState_type["Name"] = &LuaQuestState::Name; line

The hand-written idiom this mirrors (read-only reference, DO NOT let the generator
touch these files -- it only PRINTS text for a human to paste):

    typedef unsigned long(__fastcall* tGetLanguage)(void);           // FableAPI.h
    extern tGetLanguage GetLanguage_API;                             // FableAPI.h
    tGetLanguage GetLanguage_API = nullptr;                          // FableAPI.cpp
    GetLanguage_API = ASLR<tGetLanguage>(0x415070);                  // FableAPI.cpp
    int LuaQuestState::GetGameLanguage() { ... GetLanguage_API() ...}  // LuaQuestState.cpp
    questState_type["GetGameLanguage"] = &LuaQuestState::GetGameLanguage; // LuaManager.cpp

INPUT
-----
A JSON list or a TSV. Each row needs:
    name      : short C++ method name to expose in Lua (e.g. IsHeroControlledByPlayer)
    address   : retail VA, hex string ("0x0088e980" or "0088e980")
    signature : the demangled MSVC signature, OR a mangled ?Name@Class@@... string
                (this tool can parse the common MSVC mangling itself -- see below)
    scope     : "Quest" (a CGameScriptInterface method -> LuaQuestState) or "Entity".
                Optional; defaults to Quest.

If `signature` is a raw mangled name (starts with '?') the generator demangles the
subset of MSVC mangling used by Fable's CGameScriptInterface vtable:
    ?Name@Class@@ (U|Q|A)(B|A)E <ret> <args...> @Z / XZ
      U/Q/A = virtual/public/... ;  B=const this, A=non-const this ; E=__thiscall
This is enough to recover: this-ptr constness, return type, and each arg type from
the primitive/known-class set below.

USAGE
-----
    python gen_fse_bindings.py --json rows.json
    python gen_fse_bindings.py --tsv  rows.tsv
    python gen_fse_bindings.py --selftest        # generates GetGameLanguage + friends
    python gen_fse_bindings.py --from-coverage coverage.tsv --name IsHeroControlledByPlayer

Every emitted stub carries // TODO(verify) markers where a human MUST confirm:
  - CCharString / C3DVector / CScriptThing marshalling (in vs out params),
  - non-trivial return types (CWideString, CScriptThing-by-value, std::map),
  - the vtable/this-pointer source (CGameScriptInterface vs a resolved sub-object).
"""

import argparse
import json
import re
import sys

# ---------------------------------------------------------------------------
# MSVC type-code -> (C++ type used in the typedef, how Lua marshals it)
# ---------------------------------------------------------------------------
# For the mangled-name path we only need the arg/return *type codes* that appear
# in Fable's CGameScriptInterface surface. Everything else -> TODO.

# demangled-token  ->  (cpp_type_in_typedef, lua_param_type_or_None, marshal_note)
TYPE_MAP = {
    # primitives
    "void":            ("void",            None,          None),
    "bool":            ("bool",            "bool",        None),
    "_N":              ("bool",            "bool",        None),   # mangled bool
    "int":             ("int",             "int",         None),
    "J":               ("long",            "int",         None),   # mangled long
    "unsigned long":   ("unsigned long",   "int",         None),
    "K":               ("unsigned long",   "int",         None),   # mangled unsigned long
    "unsigned int":    ("unsigned int",    "int",         None),
    "_K":              ("unsigned __int64","unsigned long long", None),  # unsigned __int64 (UID)
    "float":           ("float",           "float",       None),
    "M":               ("float",           "float",       None),   # mangled float
    "double":          ("double",          "double",      None),
    # known Fable classes (passed by ref in retail -> pointer in typedef)
    "CScriptThing":    ("const CScriptThing*", "const std::shared_ptr<CScriptThing>&",
                        "shared_ptr -> raw CScriptThing* (see Entity idiom)"),
    "CCharString":     ("const CCharString*", "const std::string&",
                        "wrap std::string in FableString, pass .get()"),
    "C3DVector":       ("const C3DVector*", "float x, float y, float z",
                        "build C3DVector{x,y,z}, pass &v"),
    "CWideString":     ("void* /*CWideString out*/", None,
                        "return-by-value CWideString -- NEEDS custom marshalling"),
}

# mangled arg-type single/multi tokens (very small subset, enough for CGSI)
MANGLE_ARG = {
    "_N": "bool", "H": "int", "J": "long", "K": "unsigned long",
    "M": "float", "N": "double", "_K": "unsigned __int64", "X": "void",
}


def norm_addr(a):
    a = a.strip().lower()
    if a.startswith("0x"):
        a = a[2:]
    return "0x" + a.lstrip("0").rjust(0) if a.lstrip("0") else "0x0", int(a, 16)


# ---------------------------------------------------------------------------
# MSVC demangler (tiny, targeted at ?Name@Class@@ <access><this-const>E <ret><args>@Z)
# ---------------------------------------------------------------------------
def demangle(mangled):
    """Return dict(name, cls, thiscall, const_this, ret, args[list]) or None."""
    # ?Name@Class@@ <access-run> <ret> <args> @Z / XZ
    # access-run for a member fn is 3 chars: [UQAIMV] (access/virtual) + [ABEFGH] (this cv)
    #   + E (=__thiscall).  e.g. UBE = public-virtual / const-this / __thiscall.
    m = re.match(r"^\?([A-Za-z0-9_]+)@([A-Za-z0-9_]+)@@([UQAIMVEFG])([ABCDEFGH])(E?)(.*)$", mangled)
    if not m:
        return None
    name, cls, acc0, acc_cv, thiscall_c, rest = m.groups()
    const_this = acc_cv in ("B", "D", "F", "H")  # B=const,D=const volatile, etc.
    thiscall = thiscall_c == "E"
    body = rest
    # Return type is first code; args follow until end.
    ret, argstr = _take_type(body)
    args = []
    while argstr and not argstr.startswith("@") and argstr not in ("XZ", "Z", ""):
        if argstr.startswith("XZ") or argstr == "Z":
            break
        t, argstr = _take_type(argstr)
        if t is None:
            break
        if t == "void":  # XZ terminator
            break
        args.append(t)
    return dict(name=name, cls=cls, thiscall=thiscall, const_this=const_this,
                ret=ret, args=args)


def _take_type(s):
    """Consume one MSVC type code from front of s -> (readable_type, rest)."""
    if not s:
        return None, ""
    # class ref:  AB V CScriptThing @@  (const ref) / AAV...@@  (non-const ref) / ?AV..@@ (by value)
    mref = re.match(r"^(\?A|AB|AA|PA|PB)?V([A-Za-z0-9_]+)@@", s)
    if mref:
        return mref.group(2), s[mref.end():]
    # single-char primitive
    c = s[0]
    if c in MANGLE_ARG:
        return MANGLE_ARG[c], s[1:]
    if s.startswith("_N") or s.startswith("_K"):
        return MANGLE_ARG[s[:2]], s[2:]
    # unknown
    return None, s[1:]


# ---------------------------------------------------------------------------
# Signature -> resolved binding spec
# ---------------------------------------------------------------------------
def resolve(row):
    name = row["name"]
    addr_disp, addr_int = norm_addr(row["address"])
    scope = row.get("scope", "Quest")
    sig = row.get("signature", "")

    todo = []
    demangled = None
    if sig.startswith("?"):
        demangled = demangle(sig)
    if demangled is None and sig.startswith("?"):
        todo.append("could not demangle signature -- fill typedef by hand")

    if demangled:
        ret = demangled["ret"]
        args = demangled["args"]
        const_this = demangled["const_this"]
    else:
        # allow explicit ret/args in the row
        ret = row.get("returnType", "void")
        args = row.get("params", [])
        const_this = row.get("constThis", True)

    if ret is None:
        ret = "void"
        todo.append("return: demangler could not read return type -- fill by hand")
    ret_cpp, ret_lua, ret_note = TYPE_MAP.get(ret, (ret + " /*?*/", None,
                                                    "unknown return type"))
    if ret_note:
        todo.append(f"return: {ret_note}")

    # Free functions (e.g. NLocalisation::GetLanguage) are __fastcall with no this-ptr.
    is_free = scope == "Free"

    # build typedef param list (this-ptr first for __thiscall CGSI methods)
    tdef_params = []
    lua_params = []
    call_args = []
    if not is_free:
        tdef_params.append("CGameScriptInterfaceBase* pInterface")  # often needs resolved sub-obj
        call_args.append("m_pGameInterface")
    for i, a in enumerate(args):
        cpp, lua, note = TYPE_MAP.get(a, (a + "* /*?*/", None, f"unknown arg type '{a}'"))
        pn = f"arg{i}"
        if a == "CScriptThing":
            tdef_params.append(f"const CScriptThing* {pn}")
            lua_params.append(f"const std::shared_ptr<CScriptThing>& {pn}")
            call_args.append(f"{pn}.get()")
            todo.append(f"arg{i}: {note}")
        elif a == "CCharString":
            tdef_params.append(f"const CCharString* {pn}")
            lua_params.append(f"const std::string& {pn}")
            call_args.append(f"fs_{pn}.get()")
            todo.append(f"arg{i}: {note}")
        elif a == "C3DVector":
            tdef_params.append(f"const C3DVector* {pn}")
            lua_params.append(f"float {pn}_x, float {pn}_y, float {pn}_z")
            call_args.append(f"&v_{pn}")
            todo.append(f"arg{i}: {note}")
        else:
            tdef_params.append(f"{cpp} {pn}")
            lua_params.append(f"{cpp} {pn}")
            call_args.append(pn)
            if note:
                todo.append(f"arg{i}: {note}")

    return dict(name=name, addr=addr_disp, scope=scope, const_this=const_this,
                ret_cpp=ret_cpp, ret_lua=ret_lua, args=args, is_free=is_free,
                tdef_params=tdef_params, lua_params=lua_params, call_args=call_args,
                todo=todo)


def emit(spec):
    n = spec["name"]
    tname = f"t{n}"
    apiname = f"{n}_API"
    callconv = "__fastcall" if spec["is_free"] else "__thiscall"
    tdef = f"typedef {spec['ret_cpp']}({callconv}* {tname})({', '.join(spec['tdef_params'])});"

    header = "\n".join([
        f"// --- ForgeFSE addition: {n} @ retail {spec['addr']} ---",
        f"// TODO(verify): confirm this-ptr source. CGameScriptInterface methods are called",
        f"//   on *g_pDSTGame (or m_pGameInterface); sub-object methods need a resolver.",
        tdef,
        f"extern {tname} {apiname};",
    ])

    cpp_def = f"{tname} {apiname} = nullptr; // ForgeFSE addition"
    cpp_res = f"    {apiname} = ASLR<{tname}>({spec['addr']}); // {n}"

    # method body
    ret_cpp = spec["ret_cpp"]
    has_ret = ret_cpp != "void"
    method_ret = spec["ret_cpp"] if not spec["ret_lua"] else _lua_ret_cpp(spec)
    lua_sig = ", ".join(spec["lua_params"])

    guard = f"!{apiname}" if spec["is_free"] else f"!m_pGameInterface || !{apiname}"
    body_lines = [f"{method_ret} LuaQuestState::{n}({lua_sig}) {{",
                  f"    if ({guard}) {{",
                  f'        Log("!!! ERROR: {n} - API function not available!");',
                  f"        return{_default_ret(method_ret)};",
                  f"    }}"]
    # marshal FableString / C3DVector locals
    for i, a in enumerate(spec["args"]):
        if a == "CCharString":
            body_lines.append(f"    FableString fs_arg{i}(arg{i}.c_str());")
        if a == "C3DVector":
            body_lines.append(f"    const C3DVector v_arg{i} = {{ arg{i}_x, arg{i}_y, arg{i}_z }};")
    call = f"{apiname}({', '.join(spec['call_args'])})"
    if has_ret:
        body_lines.append(f"    return static_cast<{method_ret}>({call}); // TODO(verify) return marshalling")
    else:
        body_lines.append(f"    {call};")
    body_lines.append("}")
    method = "\n".join(body_lines)

    reg = f'    questState_type["{n}"] = &LuaQuestState::{n}; // ForgeFSE addition'

    decl = f"    {method_ret} {n}({lua_sig}); // ForgeFSE addition @ {spec['addr']}"

    todo = "\n".join(f"//   - {t}" for t in spec["todo"]) or "//   (none flagged)"

    return dict(header=header, cpp_def=cpp_def, cpp_res=cpp_res,
                method=method, reg=reg, decl=decl, todo=todo, name=n)


def _lua_ret_cpp(spec):
    # map engine return to the C++/Lua-facing return type
    if spec["ret_cpp"] in ("bool",):
        return "bool"
    if spec["ret_cpp"] in ("long", "unsigned long", "int", "unsigned int"):
        return "int"
    if spec["ret_cpp"] == "float":
        return "float"
    return spec["ret_cpp"]


def _default_ret(t):
    if t == "void":
        return ""
    if t == "bool":
        return " false"
    if t in ("int", "long", "unsigned long", "float", "double"):
        return " -1"
    return " {}"


def render(spec_row):
    e = emit(resolve(spec_row))
    out = []
    out.append(f"{'='*78}\n  {e['name']}\n{'='*78}")
    out.append("---- FableAPI.h (typedef + extern) ----------------------------------")
    out.append(e["header"])
    out.append("\n---- FableAPI.cpp (global def) --------------------------------------")
    out.append(e["cpp_def"])
    out.append("---- FableAPI.cpp (inside InitializeFableAPI) ----------------------")
    out.append(e["cpp_res"])
    out.append("\n---- LuaQuestState.h (declaration) ---------------------------------")
    out.append(e["decl"])
    out.append("\n---- LuaQuestState.cpp (method) ------------------------------------")
    out.append(e["method"])
    out.append("\n---- LuaManager.cpp (registration) ---------------------------------")
    out.append(e["reg"])
    out.append("\n---- TODO(verify) --------------------------------------------------")
    out.append(e["todo"])
    return "\n".join(out)


# ---------------------------------------------------------------------------
def load_tsv(path):
    rows = []
    with open(path, encoding="utf-8") as f:
        header = f.readline().rstrip("\n").split("\t")
        for line in f:
            parts = line.rstrip("\n").split("\t")
            rows.append(dict(zip(header, parts)))
    return rows


def from_coverage(path, want_name):
    """Pull one row out of coverage.tsv (address<TAB>mangled...) by exposed name."""
    with open(path, encoding="utf-8") as f:
        for line in f:
            p = line.rstrip("\n").split("\t")
            if len(p) < 2:
                continue
            addr, mangled = p[0], p[1]
            m = re.match(r"^\?([A-Za-z0-9_]+)@", mangled)
            if m and m.group(1) == want_name:
                return dict(name=want_name, address=addr, signature=mangled, scope="Quest")
    return None


SELFTEST_ROWS = [
    # KNOWN hand-written example (GetGameLanguage) -- but note: NLocalisation::GetLanguage
    # is a free function (__fastcall no-args), NOT a CGSI method. We model it explicitly.
    dict(name="GetGameLanguage", address="0x415070",
         returnType="unsigned long", params=[], scope="Free"),
    # KNOWN CGSI method with a CCharString& arg -> good demangle test
    dict(name="IsRegionLoadedAndPreloaded", address="0x00890550",
         signature="?IsRegionLoadedAndPreloaded@CGameScriptInterface@@UBE_NABVCCharString@@@Z"),
    # no-arg bool getter
    dict(name="IsHeroControlledByPlayer", address="0x0088e980",
         signature="?IsHeroControlledByPlayer@CGameScriptInterface@@UBE_NXZ"),
    # ScriptThing arg
    dict(name="IsEntityUnderScriptedControl", address="0x008960c0",
         signature="?IsEntityUnderScriptedControl@CGameScriptInterface@@UBE_NABVCScriptThing@@@Z"),
]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--json")
    ap.add_argument("--tsv")
    ap.add_argument("--from-coverage")
    ap.add_argument("--name", action="append", default=[])
    ap.add_argument("--selftest", action="store_true")
    args = ap.parse_args()

    rows = []
    if args.selftest:
        rows = SELFTEST_ROWS
    elif args.json:
        rows = json.load(open(args.json, encoding="utf-8"))
    elif args.tsv:
        rows = load_tsv(args.tsv)
    elif args.from_coverage:
        for nm in args.name:
            r = from_coverage(args.from_coverage, nm)
            if r:
                rows.append(r)
            else:
                print(f"// NOT FOUND in coverage: {nm}", file=sys.stderr)
    else:
        ap.error("need --json / --tsv / --from-coverage / --selftest")

    for r in rows:
        print(render(r))
        print()


if __name__ == "__main__":
    main()
