#!/usr/bin/env python3
"""Apply/revert/check the GUI-side ModMenu hook in guiscripts.bnk.

Replaces the retail plain-text element script art\\gui\\gameface\\expandablemenu.lua
(body: "g_Menu = self") with tools/lua_mod/gui_modmenu_hook.lua. Backs up the
pristine bank to guiscripts.bnk.orig_backup on first apply.

Usage (from repo root):
  python tools/lua_mod/apply_gui_mod.py            # apply
  python tools/lua_mod/apply_gui_mod.py --check    # report state
  python tools/lua_mod/apply_gui_mod.py --revert   # restore pristine bank
"""
import os, shutil, sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import bnk_repack
import script_index

REPO = os.path.dirname(os.path.dirname(HERE))
BNK = os.path.join(REPO, "Fable2Recomp", "assets", "game", "data", "guiscripts.bnk")
BACKUP = BNK + ".orig_backup"
HOOK = os.path.join(HERE, "gui_modmenu_hook.lua")
TARGET = "scripts\\gameface\\guisetup.lua"


def entry_text(entries, name_sub):
    for e in entries:
        if name_sub.lower() in e["name"].lower():
            return e, script_index.entry_bytes(e)
    raise KeyError(name_sub)


def main():
    mode = sys.argv[1] if len(sys.argv) > 1 else "apply"
    if mode == "--revert":
        if not os.path.exists(BACKUP):
            print("no backup found:", BACKUP); return 1
        shutil.copy2(BACKUP, BNK)
        print("reverted guiscripts.bnk from", BACKUP)
        return 0

    base, entries = bnk_repack.read_bnk(BNK)
    e, text = entry_text(entries, TARGET)
    applied = b"ModMenu" in text
    if mode == "--check":
        print(f"entry {e['name']}: {e['dsz']} bytes, hook {'APPLIED' if applied else 'not applied'}")
        print("backup exists:", os.path.exists(BACKUP))
        return 0

    if applied:
        print("hook already applied; nothing to do")
        return 0
    if not os.path.exists(BACKUP):
        shutil.copy2(BNK, BACKUP)
        print("backed up pristine bank to", BACKUP)
    hook = open(HOOK, "rb").read()
    assert b"GUIStateAndConstants.lua" in hook, "hook must be the guisetup.lua reconstruction"
    bnk_repack.replace_entry(entries, TARGET, hook)
    bnk_repack.write_bnk(BNK, base, entries)
    print(f"applied: {TARGET} -> {len(hook)} bytes (was {len(text)})")
    # sanity: reread
    base2, entries2 = bnk_repack.read_bnk(BNK)
    _, text2 = entry_text(entries2, TARGET)
    assert text2 == hook, "reread mismatch!"
    print("verified: bank rereads OK,", len(entries2), "entries")
    return 0


if __name__ == "__main__":
    sys.exit(main())
