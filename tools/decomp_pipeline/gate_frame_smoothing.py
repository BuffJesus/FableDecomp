#!/usr/bin/env python3
"""Link the three recovered frame-smoothing methods and exercise the live ring."""
import subprocess
import tempfile
from pathlib import Path
import verify_and_land as V

ROOT = Path(__file__).resolve().parents[2]
SOURCES = [
    "rebuild/src/compiled/00/49/CFrameRateSmoother_AddFrameTime_0049b9e0.cpp",
    "rebuild/src/compiled/00/49/CFrameRateSmoother_CalculateSmoothedTime_0049b9a0.cpp",
    "rebuild/src/compiled/00/49/CFrameRateSmoother_GetSmoothedTime_0049b990.cpp",
    "rebuild/tests/00/49/CFrameRateSmoother_ring_integration_test.cpp",
]

def main():
    env = V.env()
    with tempfile.TemporaryDirectory(prefix="fable-frame-ring-") as directory:
        work = Path(directory)
        exe = work / "ring.exe"
        build = subprocess.run(
            [str(V.VC / "bin/cl.exe"), "/nologo", "/O2", "/Oy", "/W3", "/MT",
             f"/Fe{exe}", *[str(ROOT / source) for source in SOURCES]],
            cwd=work, env=env, capture_output=True, text=True)
        if build.returncode:
            print(build.stdout + build.stderr)
            return 1
        run = subprocess.run([str(exe)], cwd=work, env=env, capture_output=True, text=True)
        print(run.stdout + run.stderr, end="")
        return 0 if run.returncode == 0 and "FABLETLC_FRAME_SMOOTHER_RING PASS" in run.stdout else 1

if __name__ == "__main__":
    raise SystemExit(main())
