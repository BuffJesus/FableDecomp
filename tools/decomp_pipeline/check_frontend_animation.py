#!/usr/bin/env python3
"""Check the recovered animation rules and their visual-checkpoint bridge."""
import json
import sys
from check_cgame_play import ROOT, parity, run


def main():
    env = parity.env()
    for name in ("check_ui_colour.py", "check_ui_swapping.py",
                 "check_frontend_random.py", "check_frontend_fade.py",
                 "check_sprite_visibility.py", "check_frontend_hierarchy.py"):
        print(run([sys.executable, ROOT / "tools/decomp_pipeline" / name], env).strip())
    directory = ROOT / "work/frontend_animation_check"
    directory.mkdir(parents=True, exist_ok=True)
    obj, exe = directory / "behavior.obj", directory / "behavior.exe"
    run([parity.CL_EXE, "/nologo", "/c", "/W3", "/MT", "/GS", "/O2", "/Oy",
         "/Fo" + str(obj), ROOT / "rebuild/tests/integration/FrontendAnimation_test.cpp"], env)
    run([parity.VC / "bin/link.exe", "/nologo", "/subsystem:console",
         "/out:" + str(exe), obj, "kernel32.lib"], env)
    behavior = run([exe], env).strip()
    if behavior != "FABLETLC_FRONTEND_ANIMATION PASS":
        raise RuntimeError("Animation fixture pass marker missing")
    print(behavior)
    (directory / "report.json").write_text(json.dumps({"accepted": True,
        "behavior": behavior, "scope": "recovered decisions/colour plus queued fade bridge compared with native coastal hierarchies"}, indent=2) + "\n")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
