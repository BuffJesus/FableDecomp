"""Live IDirect3DDevice9 census: launch retail Fable.exe under dbgeng (pybag) and record every device
method's callers. Complements the static lower bound in d3d9_device_census.py.

How: breakpoint on the instruction in CRenderManagerCore::Init that stores the new device
(0xA0A955 `mov [esi+0x3CF0], edi`). There, read the device vtable and set a breakpoint on each of
the 119 methods (names from d3d9.h). Each hit records the return address ([esp]); a method's
breakpoint is removed after BUDGET hits so the game keeps running at speed. Results are written to
--out every few seconds.

Safety: DEBUG_PROCESS_DETACH_ON_EXIT is set, so killing this tracer leaves the game running. It
does not touch game files. Launch only when the shared install is free.
"""
from __future__ import annotations

import argparse
import collections
import json
import os
import struct
import sys
import time
from pathlib import Path

os.environ["WINDBG_DIR"] = r"C:\Windows\System32"
os.environ["_NT_SYMBOL_PATH"] = ""
sys.stdout.reconfigure(line_buffering=True)

from pybag import UserDbg  # noqa: E402
from pybag.dbgeng import core as DbgEng  # noqa: E402

sys.path.insert(0, str(Path(__file__).resolve().parent))
from d3d9_device_census import d3d9_interfaces  # noqa: E402

INSTALL = Path(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters")
DEVICE_STORE = 0xA0A955          # CRenderManagerCore::Init: mov [esi+0x3CF0], edi
DEVICE_STORE_BYTES = bytes.fromhex("89bef03c0000")
IMAGE_FILE_MACHINE_I386 = 0x14C
RENDER_CORE_GLOBAL = 0x13CB508   # CRenderManagerCore* (live-verified 2026-10-16: [+4] == [+0x3CF0] == device)
BUDGET = 3000


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("--seconds", type=int, default=150)
    ap.add_argument("--out", required=True)
    ap.add_argument("--attach", action="store_true",
                    help="attach to the running Fable.exe (Steam relaunches a debugger-created one) and "
                         "read the device from [[0x1436E18]+0x3CF0]; load-time Create* calls are missed")
    args = ap.parse_args()
    names = d3d9_interfaces()["IDirect3DDevice9"]

    dbg = UserDbg()
    if args.attach:
        pids = dbg.pids_by_name("Fable.exe")
        if len(pids) != 1:
            sys.exit(f"expected one Fable.exe, found {pids}")
        dbg.attach(pids[0][0])
    else:
        os.chdir(INSTALL)
        dbg.create(str(INSTALL / "Fable.exe"))
    dbg._client._cli.AddProcessOptions(0x00000001)  # DEBUG_PROCESS_DETACH_ON_EXIT
    dbg._control.SetEffectiveProcessorType(IMAGE_FILE_MACHINE_I386)
    try:
        trace(dbg, args, names)
    finally:
        # A crash without detach leaves every game thread suspended (seen 2026-10-16).
        try:
            dbg.detach()
            print("detached; game left running")
        except Exception as e:
            print("detach failed:", e)


def trace(dbg, args, names) -> None:
    got = dbg.read(DEVICE_STORE, len(DEVICE_STORE_BYTES))
    if bytes(got) != DEVICE_STORE_BYTES:
        sys.exit(f"unexpected bytes at {DEVICE_STORE:#x}: {bytes(got).hex()}")

    state = {"device": None, "vtable": None, "methods": {}, "t_device": None}
    hits = collections.Counter()
    sites = collections.defaultdict(collections.Counter)
    bp_ids = {}

    def u32(addr):
        return struct.unpack("<I", bytes(dbg.read(addr, 4)))[0]

    def on_method(bp, d, addr):
        sp = d.reg.get_sp() & 0xFFFFFFFF
        ret = u32(sp)
        for n in state["methods"][addr]:
            hits[n] += 1
            sites[n][ret] += 1
        if hits[state["methods"][addr][0]] >= BUDGET:
            try:
                d.bd(bp_ids[addr])
            except Exception:
                pass
        return DbgEng.DEBUG_STATUS_GO

    def on_store(bp, d):
        regs = d.reg
        dev = regs.edi & 0xFFFFFFFF
        vt = u32(dev)
        state.update(device=dev, vtable=vt, t_device=time.time())
        slots = [u32(vt + 4 * i) for i in range(len(names))]
        by_addr = collections.defaultdict(list)
        for i, a in enumerate(slots):
            by_addr[a].append(names[i])
        state["methods"] = dict(by_addr)
        for a in by_addr:
            bp_ids[a] = d.bp(a, handler=lambda b, dd, a=a: on_method(b, dd, a))
        print(f"device {dev:#x} vtable {vt:#x}: {len(by_addr)} distinct method bodies hooked")
        return DbgEng.DEBUG_STATUS_GO

    if args.attach:
        class _R:  # minimal stand-in so on_store can read the live device
            pass
        core = u32(RENDER_CORE_GLOBAL)
        if not core or u32(core + 4) != u32(core + 0x3CF0):
            sys.exit(f"render core {core:#x} does not hold one device at +4/+0x3CF0")
        fake = _R(); fake.edi = u32(core + 0x3CF0)
        class _D:
            reg = fake
            bp = dbg.bp
        on_store(None, _D)
    else:
        dbg.bp(DEVICE_STORE, handler=on_store, oneshot=True)

    def dump(final=False):
        data = {
            "device": state["device"] and f"{state['device']:08X}",
            "vtable": state["vtable"] and f"{state['vtable']:08X}",
            "budget": BUDGET,
            "final": final,
            "methods": {n: {"hits": hits[n], "budget_exhausted": hits[n] >= BUDGET,
                            "call_sites": {f"{r:08X}": c for r, c in sites[n].most_common()}}
                        for n in names if hits[n]},
            "shared_bodies": {f"{a:08X}": v for a, v in state["methods"].items() if len(v) > 1},
        }
        Path(args.out).write_text(json.dumps(data, indent=1))

    t0 = time.time()
    last = 0.0
    while time.time() - t0 < args.seconds:
        try:
            dbg.go(timeout=2)  # pybag wait() takes SECONDS (it multiplies by 1000)
        except Exception as e:  # wait timeout or a non-breakpoint event
            msg = str(e)
            if "not accessible" in msg:
                print("target gone:", msg[:120])
                break
            if "timeout" not in msg.lower() and "E_PENDING" not in msg:
                print("event:", msg[:200])
                try:
                    dbg.go_nothandled(timeout=2)
                except Exception:
                    pass
        if time.time() - last > 5:
            last = time.time()
            dump()
            print(f"t={time.time() - t0:5.0f}s methods={sum(1 for n in names if hits[n])} "
                  f"hits={sum(hits.values())}")
    dump(final=True)
    for a, i in bp_ids.items():
        try:
            dbg.bc(i)
        except Exception:
            pass


if __name__ == "__main__":
    main()
