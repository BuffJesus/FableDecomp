#!/usr/bin/env python3
"""Syntax-check a Lua file before injecting it (apply_mod embeds SOURCE, no compile
step — a parse error there silently kills the whole hook at game load, e.g. the mod
menu just stops working). Uses lupa's bundled Lua `load`.

Usage: py -3.12 tools/lua_mod/luacheck.py tools/lua_mod/modmenu.lua
(lupa is installed under Python 3.12 on this machine: `py -3.12 -m pip install lupa`)
"""
import sys


def check(path: str) -> bool:
    import lupa
    L = lupa.LuaRuntime()
    src = open(path, encoding="utf-8").read()
    ok, err = L.eval("function(s) local f,e=load(s); return (f~=nil), e end")(src)
    if ok:
        print(f"OK: {path} parses")
        return True
    print(f"SYNTAX ERROR in {path}:\n  {err}")
    return False


if __name__ == "__main__":
    if len(sys.argv) != 2:
        raise SystemExit("usage: luacheck.py <file.lua>")
    sys.exit(0 if check(sys.argv[1]) else 1)
