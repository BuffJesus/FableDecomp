"""Read-only external Windows capture of retail Escape eligibility and fade state.

No injection, hooks, engine calls, key injection, or process-memory writes.
Offsets apply only to the executable hash validated by the native-byte proof.
"""
import argparse
import ctypes as c
from ctypes import wintypes as w
import hashlib
import json
from pathlib import Path
import struct
import time


class EscapeObserver:
    """Own message-only Raw Input sink; never consumes the game's input queue.

    https://learn.microsoft.com/en-us/windows/win32/inputdev/using-raw-input
    Only Escape transitions are retained. Other keyboard events are discarded.
    """
    def __init__(self, user):
        self.user = user
        self.down = False
        class Device(c.Structure):
            _fields_ = [("page", w.USHORT), ("usage", w.USHORT),
                        ("flags", w.DWORD), ("target", w.HWND)]
        class Header(c.Structure):
            _fields_ = [("type", w.DWORD), ("size", w.DWORD),
                        ("device", w.HANDLE), ("param", w.WPARAM)]
        self.Device = Device
        self.header_size = c.sizeof(Header)
        user.CreateWindowExW.argtypes = [w.DWORD, w.LPCWSTR, w.LPCWSTR, w.DWORD,
                                        c.c_int, c.c_int, c.c_int, c.c_int,
                                        w.HWND, w.HMENU, w.HINSTANCE, c.c_void_p]
        user.CreateWindowExW.restype = w.HWND
        user.RegisterRawInputDevices.argtypes = [c.POINTER(Device), w.UINT, w.UINT]
        user.RegisterRawInputDevices.restype = w.BOOL
        user.GetRawInputData.argtypes = [w.HANDLE, w.UINT, c.c_void_p, c.POINTER(w.UINT), w.UINT]
        user.GetRawInputData.restype = w.UINT
        user.PeekMessageW.argtypes = [c.POINTER(w.MSG), w.HWND, w.UINT, w.UINT, w.UINT]
        user.DispatchMessageW.argtypes = [c.POINTER(w.MSG)]
        user.DispatchMessageW.restype = w.LPARAM
        user.DestroyWindow.argtypes = [w.HWND]
        self.window = user.CreateWindowExW(0, "STATIC", "Retail Escape observer", 0,
                                           0, 0, 0, 0, w.HWND(-3), None, None, None)
        if not self.window:
            raise c.WinError(c.get_last_error())
        device = Device(1, 6, 0x100, self.window)  # INPUTSINK; never NOLEGACY.
        if not user.RegisterRawInputDevices(c.byref(device), 1, c.sizeof(Device)):
            user.DestroyWindow(self.window)
            raise c.WinError(c.get_last_error())

    def poll(self):
        events = []
        msg = w.MSG()
        while self.user.PeekMessageW(c.byref(msg), self.window, 0, 0, 1):
            if msg.message == 0xFF:  # WM_INPUT, read before dispatch/default cleanup.
                size = w.UINT()
                self.user.GetRawInputData(w.HANDLE(msg.lParam), 0x10000003, None,
                                          c.byref(size), self.header_size)
                data = c.create_string_buffer(size.value)
                got = self.user.GetRawInputData(w.HANDLE(msg.lParam), 0x10000003,
                                                data, c.byref(size), self.header_size)
                if got != 0xFFFFFFFF and got >= self.header_size + 16:
                    raw = data.raw
                    if struct.unpack_from("<I", raw)[0] == 1:
                        _, flags, _, key = struct.unpack_from("<HHHH", raw, self.header_size)
                        if key == 0x1B:
                            self.down = not bool(flags & 1)
                            events.append({"message_tick": msg.time, "down": self.down})
            self.user.DispatchMessageW(c.byref(msg))
        return events

    def close(self):
        device = self.Device(1, 6, 1, None)  # REMOVE from this process only.
        self.user.RegisterRawInputDevices(c.byref(device), 1, c.sizeof(self.Device))
        self.user.DestroyWindow(self.window)


def eligibility(gui, busy):
    """0x48DE44 with receiver GUI+0x18; verified against native emulation."""
    blockers = []
    if busy:
        blockers.append("global_24_104")
    if gui[0xF6]:
        for condition, name in [
            (not gui[0xF8], "scene_not_skippable"),
            (gui[0xF7], "already_skipped"),
            (gui[0x94] & 0x48, "subtitle_flags_0x48"),
            (not gui[0x100], "skip_timer_inactive"),
            (gui[0x10D], "skip_timer_phase_0x10D"),
        ]:
            if condition:
                blockers.append(name)
    # EASkipCutScene additionally requires inCutscene before calling eligibility.
    return {"predicate": not blockers, "event_eligible": bool(gui[0xF6]) and not blockers,
            "blockers": blockers, "inCutscene": gui[0xF6]}


def snapshot_audio_listener(read, core, delta=0):
    """Decode only the native core/mixer classes used by C1C49D.

    C0A590 returns core+38. C1C49D reads mixer+8/+14/+20 as position,
    forward and up. These are game coordinates, before backend Y/Z exchange.
    The +885 byte is one update gate, not proof that this path executed.
    """
    result = {"core": core, "complete": False, "atomic": False}

    def fetch(address, size):
        if not address:
            raise ValueError("null_pointer")
        data = read(address, size)
        if len(data) != size:
            raise ValueError("short_read")
        return data

    try:
        raw = fetch(core, 0x3C)
        result["core_raw"] = raw.hex()
        if struct.unpack_from("<I", raw)[0] != 0x12B1E24 + delta:
            raise ValueError("unknown_core_vtable")
        mixer = struct.unpack_from("<I", raw, 0x38)[0]
        result["mixer"] = mixer
        raw = fetch(mixer, 0x2C)
        result["mixer_raw"] = raw.hex()
        if struct.unpack_from("<I", raw)[0] != 0x12B2748 + delta:
            raise ValueError("unknown_mixer_vtable")
        result.update(position=struct.unpack_from("<fff", raw, 8),
                      forward=struct.unpack_from("<fff", raw, 0x14),
                      up=struct.unpack_from("<fff", raw, 0x20),
                      update_gate_0x885=fetch(mixer + 0x885, 1)[0], complete=True)
    except (OSError, ValueError) as exc:
        result["partial_reason"] = str(exc)
    return result


def snapshot_audio_update_gate(read, mixer, delta=0):
    """C10ABD -> mixer+7BC; C1C700 ctor pins proxy class and +19 gate."""
    result = {'complete': False, 'atomic': False}
    try:
        def fetch(address, size):
            if not address: raise ValueError('null_pointer')
            data = read(address, size)
            if len(data) != size: raise ValueError('short_read')
            return data
        def word(address): return struct.unpack('<I', fetch(address, 4))[0]
        if word(mixer) != 0x12B2748 + delta:
            raise ValueError('unknown_mixer_vtable')
        proxy = word(mixer + 0x7BC)
        raw = fetch(proxy, 0x1A)
        if struct.unpack_from('<I', raw)[0] != 0x12B3290 + delta:
            raise ValueError('unknown_proxy_vtable')
        result.update(proxy=proxy, raw=raw.hex(), proxy_gate_0x19=raw[0x19],
                      proxy_mode_0x18=raw[0x18], complete=True)
        # C33590 -> C3EA60: capture the actual input to the constructor's
        # 1/2/4 gate decision, without guessing its enum from endpoint settings.
        try:
            backend = word(0x143BB5C + delta)
            mode_object = word(backend + 0x188) if backend else 0
            if not backend: raise ValueError('null_backend')
            result.update(native_mode_code=word(mode_object+4) if mode_object else 0x80000000,
                          mode_object=mode_object, mode_complete=True)
            # C3F110 records selected mode and channel count separately from
            # C53340's translated DirectSound configuration. Fallback can differ.
            try:
                if word(backend) != 0x12B5BC0 + delta:
                    raise ValueError('unknown_output_backend_vtable')
                output_raw = fetch(backend+0x1B8, 8)
                selected, channels = struct.unpack('<II', output_raw)
                result.update(output_selected_mode=selected, output_channel_count=channels,
                              output_raw=output_raw.hex(), output_complete=True)
            except (OSError, ValueError) as exc:
                result.update(output_complete=False, output_partial_reason=str(exc))
        except (OSError, ValueError) as exc:
            result.update(mode_complete=False, mode_partial_reason=str(exc))
    except (OSError, ValueError) as exc:
        result['partial_reason'] = str(exc)
    return result


def snapshot_audio_backend(read, mixer, delta=0, max_voices=128):
    """Read C1C460/C13700 voice array and C33FD0's backend objects.

    Fields are native offsets, not a claim that a voice is audible. Reads are
    non-atomic; retain raw bytes and stop on invalid/oversized arrays.
    """
    result = {"voices": [], "complete": False, "atomic": False}
    def fetch(address, size):
        if not address:
            raise ValueError("null_pointer")
        data = read(address, size)
        if len(data) != size:
            raise ValueError("short_read")
        return data
    def word(data, offset=0):
        return struct.unpack_from("<I", data, offset)[0]
    try:
        if word(fetch(mixer, 4)) != 0x12B2748 + delta:
            raise ValueError("unknown_mixer_vtable")
        array = fetch(mixer + 0x224, 0x14)
        count, base = word(array, 4), word(array, 0x10)
        result.update(count=count, array_raw=array.hex())
        if count > max_voices:
            raise ValueError("voice_limit")
        for index in range(count):
            wrapper = fetch(base + index * 0x2C, 0x2C)
            voice, backend = word(wrapper, 4), word(wrapper, 0xC)
            entry = dict(index=index, wrapper_raw=wrapper.hex(), voice=voice,
                         backend=backend, wrapper_gate_0x25=wrapper[0x25])
            result["voices"].append(entry)
            if voice:
                raw = fetch(voice, 0x4C)
                entry.update(voice_raw=raw.hex(), source_position=struct.unpack_from("<fff", raw, 0x38),
                             relative=raw[0x44], voice_gate_0x48=word(raw, 0x48))
            if voice and backend and entry['voice_gate_0x48'] and wrapper[0x25]:
                raw = fetch(backend, 0x1DC)
                entry.update(backend_raw=raw.hex(), backend_vtable=word(raw),
                             submitted_position=struct.unpack_from('<fff', raw, 0x158),
                             local_position=struct.unpack_from('<fff', raw, 0xFC),
                             distance=struct.unpack_from('<f', raw, 0x12C)[0],
                             distance_factor=struct.unpack_from('<f', raw, 0x130)[0],
                             directional_factor=struct.unpack_from('<f', raw, 0x134)[0],
                             cone_parameters=struct.unpack_from('<IIf', raw, 0x190),
                             dirty_flags=word(raw, 0x1D8))
        result['complete'] = True
    except (OSError, ValueError) as exc:
        result['partial_reason'] = str(exc)
    return result


def snapshot_audio_samples(read, manager, delta=0, max_emitters=128,
                           max_samples=256, budget_seconds=0.02, clock=time.monotonic):
    """Bounded, non-atomic observation of native emitter/sample lists.

    Layout: A3ECB0/AAC580, AACCC0/AAB850. Never follows other vtables.
    Concurrent deletion may invalidate a read; report a partial snapshot.
    """
    result = {"emitters": [], "complete": False, "atomic": False}
    deadline = clock() + budget_seconds
    samples = 0

    def fetch(address, size):
        if clock() >= deadline:
            raise ValueError("time_budget")
        if not address:
            raise ValueError("null_pointer")
        data = read(address, size)
        if len(data) != size:
            raise ValueError("short_read")
        return data

    def word(data, offset=0):
        return struct.unpack_from("<I", data, offset)[0]

    def nodes(head, size):
        seen = set()
        node = word(fetch(head, size))
        while node != head:
            if node in seen:
                raise ValueError("list_cycle")
            seen.add(node)
            data = fetch(node, size)
            yield node, data
            node = word(data)

    try:
        raw = fetch(manager, 0x80)
        if word(raw) != 0x129D2C4 + delta:
            raise ValueError("unknown_manager_vtable")
        for node, data in nodes(word(raw, 0x4C), 12):
            if len(result["emitters"]) >= max_emitters:
                raise ValueError("emitter_limit")
            address = word(data, 8)
            emitter = fetch(address, 0x20)
            if word(emitter) != 0x129E6EC + delta:
                raise ValueError("unknown_emitter_vtable")
            entry = {"address": address, "node": node, "raw": emitter.hex(),
                     "position": struct.unpack_from("<fff", emitter, 8),
                     "spatial": emitter[0x18], "samples": []}
            result["emitters"].append(entry)
            for sample_node, sample_data in nodes(word(emitter, 0x14), 16):
                if samples >= max_samples:
                    raise ValueError("sample_limit")
                payload = word(sample_data, 8)
                sample = fetch(payload, 0x24)
                if word(sample) != 0x129E67C + delta:
                    raise ValueError("unknown_sample_vtable")
                entry["samples"].append({
                    "address": payload, "node": sample_node, "raw": sample.hex(),
                    "handle": word(sample, 8), "bank": word(sample, 0xC),
                    "spatial": sample[0x14], "sound_in_2d": sample[0x15],
                    "sample_id": word(sample, 0x18)})
                samples += 1
        result["complete"] = True
    except (OSError, ValueError) as exc:
        result["partial_reason"] = str(exc)
    result["sample_count"] = samples
    return result


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--pid", type=int, required=True)
    ap.add_argument("--output", type=Path, required=True)
    ap.add_argument("--seconds", type=float, default=600)
    ap.add_argument("--proof", type=Path, default=Path(__file__).resolve().parents[2] / "refs/script_recovery/new_oakvale_intro/runtime_evidence/retail-skip-gate-emulation-20260912.json")
    args = ap.parse_args()
    kernel = c.WinDLL("kernel32", use_last_error=True)
    user = c.WinDLL("user32", use_last_error=True)
    kernel.OpenProcess.argtypes = [w.DWORD, w.BOOL, w.DWORD]
    kernel.OpenProcess.restype = w.HANDLE
    kernel.ReadProcessMemory.argtypes = [w.HANDLE, c.c_void_p, c.c_void_p, c.c_size_t, c.POINTER(c.c_size_t)]
    kernel.ReadProcessMemory.restype = w.BOOL
    kernel.CloseHandle.argtypes = [w.HANDLE]
    kernel.CreateToolhelp32Snapshot.argtypes = [w.DWORD, w.DWORD]
    kernel.CreateToolhelp32Snapshot.restype = w.HANDLE
    kernel.GetTickCount64.restype = c.c_ulonglong
    kernel.WaitForSingleObject.argtypes = [w.HANDLE, w.DWORD]
    kernel.WaitForSingleObject.restype = w.DWORD
    user.GetForegroundWindow.restype = w.HWND
    user.GetWindowThreadProcessId.argtypes = [w.HWND, c.POINTER(w.DWORD)]

    class Module(c.Structure):
        _fields_ = [("dwSize", w.DWORD), ("th32ModuleID", w.DWORD),
                    ("th32ProcessID", w.DWORD), ("GlblcntUsage", w.DWORD),
                    ("ProccntUsage", w.DWORD), ("modBaseAddr", c.c_void_p),
                    ("modBaseSize", w.DWORD), ("hModule", w.HMODULE),
                    ("szModule", w.WCHAR * 256), ("szExePath", w.WCHAR * 260)]

    for name in ["Module32FirstW", "Module32NextW"]:
        getattr(kernel, name).argtypes = [w.HANDLE, c.POINTER(Module)]
        getattr(kernel, name).restype = w.BOOL
    snapshot = kernel.CreateToolhelp32Snapshot(0x08 | 0x10, args.pid)
    if snapshot == c.c_void_p(-1).value:
        raise c.WinError(c.get_last_error())
    modules = []
    try:
        entry = Module()
        entry.dwSize = c.sizeof(Module)
        ok = kernel.Module32FirstW(snapshot, c.byref(entry))
        if not ok:
            raise c.WinError(c.get_last_error())
        while ok:
            modules.append({"name": entry.szModule, "path": entry.szExePath,
                            "base": entry.modBaseAddr, "size": entry.modBaseSize})
            ok = kernel.Module32NextW(snapshot, c.byref(entry))
    finally:
        kernel.CloseHandle(snapshot)
    exe = next(m for m in modules if m["name"].lower() == "fable.exe")
    proof = args.proof
    expected = json.loads(proof.read_text())["executable_sha256"]
    actual = hashlib.sha256(Path(exe["path"]).read_bytes()).hexdigest()
    if actual != expected:
        raise RuntimeError("Executable differs from validated retail bytes")
    delta = exe["base"] - 0x400000
    # SYNCHRONIZE | QUERY_LIMITED_INFORMATION | VM_READ; no write rights.
    process = kernel.OpenProcess(0x101010, False, args.pid)
    if not process:
        raise c.WinError(c.get_last_error())

    def read(address, size):
        if not address:
            raise ValueError("Null pointer")
        data = c.create_string_buffer(size)
        count = c.c_size_t()
        if not kernel.ReadProcessMemory(process, address, data, size, c.byref(count)) or count.value != size:
            raise OSError("ReadProcessMemory failed", hex(address), c.get_last_error())
        return data.raw

    def u32(address):
        return struct.unpack("<I", read(address, 4))[0]

    args.output.parent.mkdir(parents=True, exist_ok=True)
    keyboard = EscapeObserver(user)
    try:
        with args.output.open("x", encoding="utf-8", buffering=1) as out:
            out.write(json.dumps({"type": "provenance", "pid": args.pid, "modules": modules,
                                  "exe_sha256": actual, "interval_seconds": 0.05,
                                  "access": "read-only"}) + "\n")
            deadline = time.monotonic() + args.seconds
            next_audio_snapshot = 0
            while time.monotonic() < deadline and kernel.WaitForSingleObject(process, 0) == 0x102:
                events = keyboard.poll()
                row = {"tick": kernel.GetTickCount64() & 0xFFFFFFFF,
                       "escape_down": keyboard.down, "escape_events": events}
                foreground = w.DWORD()
                user.GetWindowThreadProcessId(user.GetForegroundWindow(), c.byref(foreground))
                row["foreground_pid"] = foreground.value
                try:
                    gui_ptr = u32(0x13B8790 + delta)
                    gui = read(gui_ptr, 0x298)
                    busy = u32(u32(u32(0x13B86A0 + delta) + 0x24) + 0x104)
                    gsi = u32(0x143E8F8 + delta)
                    display = u32(gsi + 0xC)
                    fade = read(display + 0xBC, 0x40)
                    row.update(gui=gui_ptr, gui_bytes=gui.hex(), busy=busy,
                               eligibility=eligibility(gui, busy), gsi=gsi, display=display,
                               fade_bytes=fade.hex(), gui_mode=struct.unpack_from("<I", gui, 0x294)[0],
                               skip_duration=struct.unpack_from("<f", gui, 0x104)[0],
                               skip_elapsed=struct.unpack_from("<f", gui, 0x110)[0])
                    draw = struct.unpack_from("<I", gui, 0x168)[0]
                    if draw:
                        draw_bytes = read(draw, 0x2C)
                        main_game = struct.unpack_from("<I", draw_bytes, 0x10)[0]
                        row.update(draw_object=draw, draw_bytes=draw_bytes.hex(),
                                   render_display=u32(main_game + 0x28))
                    audio = u32(0x13B8394 + delta)
                    if audio:
                        audio_vtable = u32(audio)
                        row.update(audio_manager=audio, audio_vtable=audio_vtable,
                                   audio_create_emitter=u32(audio_vtable + 0x10))
                        if audio_vtable == 0x129D2C4 + delta:
                            manager_bytes = read(audio, 0x80)
                            row.update(audio_manager_bytes=manager_bytes.hex(),
                                       audio_script_2d_position=struct.unpack_from("<fff", manager_bytes, 0x74))
                            core = struct.unpack_from("<I", manager_bytes, 0x40)[0]
                            if core:
                                row.update(audio_core=core, audio_core_vtable=u32(core))
                                row["audio_listener"] = snapshot_audio_listener(read, core, delta)
                            if time.monotonic() >= next_audio_snapshot:
                                row["audio_samples"] = snapshot_audio_samples(read, audio, delta)
                                listener = row.get('audio_listener', {})
                                if listener.get('complete'):
                                    row['audio_backend'] = snapshot_audio_backend(read, listener['mixer'], delta)
                                    row['audio_update_gate'] = snapshot_audio_update_gate(read, listener['mixer'], delta)
                                next_audio_snapshot = time.monotonic() + 0.5
                    # Native C57460 uses this loader; C565F0 sets +10C on attempt.
                    # +104 chooses built-in proc lookup in C574B0. These fields
                    # identify a loader path, not the active playback device.
                    loader = 0x143BDB0 + delta
                    row.update(openal_loader=loader, openal_module=u32(loader + 4),
                               openal_builtin_proc_flag=read(loader + 0x104, 1)[0],
                               openal_attempted=read(loader + 0x10C, 1)[0])
                except (OSError, ValueError) as exc:
                    row["read_error"] = str(exc)
                out.write(json.dumps(row) + "\n")
                time.sleep(0.05)
    finally:
        keyboard.close()
        kernel.CloseHandle(process)


if __name__ == "__main__":
    main()
