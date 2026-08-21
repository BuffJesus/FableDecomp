#!/usr/bin/env python3
"""Shape classifiers shared by shape_author.py (landed/baked rows) and gap_author.py
(functions discovered inside over-captured rows).

`classify(body)` -> (source_cpp, leaf_symbol) or None. Every emitted source is still
byte-verified by verify_and_land, so a misclassification simply fails to land.
"""
import struct


# ---------------- shape classification ----------------
def pad(n):
    return "    char pad[0x%x];\n" % n if n else ""

def ret_args(b):
    """trailing `ret` / `ret imm16` -> number of stack-arg bytes the callee pops."""
    if b.endswith(b"\xc3"):
        return 0
    if len(b) >= 3 and b[-3] == 0xc2:
        return struct.unpack_from("<H", b, len(b) - 2)[0]
    return None

def params(nbytes):
    return ", ".join("int a%d" % i for i in range(nbytes // 4))

def emit_empty(b):
    n = ret_args(b); p = params(n)
    return ("// Empty member hook. __fastcall this=ecx.\n"
            "struct T { void Hook(%s); };\n"
            "void T::Hook(%s) {}\n" % (p, p)), "Hook"

def emit_const(b, expr, ty):
    n = ret_args(b); p = params(n)
    return ("// Constant-return member: returns %s. __fastcall this=ecx.\n"
            "struct T { %s Get(%s); };\n"
            "%s T::Get(%s) { return %s; }\n" % (expr, ty, p, ty, p, expr)), "Get"

def emit_getter(b, ty, disp):
    n = ret_args(b); p = params(n)
    return ("// Member getter at this+0x%x. __fastcall this=ecx. pack(1) keeps the field at\n"
            "// the retail byte offset (natural alignment would shift an odd one).\n"
            "#pragma pack(push,1)\n"
            "struct T {\n%s    %s field;\n    %s Get(%s);\n};\n"
            "#pragma pack(pop)\n"
            "%s T::Get(%s) { return this->field; }\n" % (disp, pad(disp), ty, ty, p, ty, p)), "Get"

def emit_addr(b, disp):
    n = ret_args(b); p = params(n)
    return ("// Address-of-member accessor (`lea`) at this+0x%x. __fastcall this=ecx.\n"
            "#pragma pack(push,1)\n"
            "struct Sub { int dummy; };\n"
            "struct T {\n%s    Sub field;\n    Sub* GetAddr(%s);\n};\n"
            "#pragma pack(pop)\n"
            "Sub* T::GetAddr(%s) { return &this->field; }\n" % (disp, pad(disp), p, p)), "GetAddr"

def emit_setter(b, ty, disp):
    return ("// Member setter at this+0x%x: one stack arg, `ret 4`. __fastcall this=ecx.\n"
            "#pragma pack(push,1)\n"
            "struct T {\n%s    %s field;\n    void Set(%s value);\n};\n"
            "#pragma pack(pop)\n"
            "void T::Set(%s value) { this->field = value; }\n" % (disp, pad(disp), ty, ty, ty)), "Set"

def emit_sub_forward(b, disp):
    return ("// Forwarder to a member of the sub-object pointer at this+0x%x. VC7.1 tail-calls\n"
            "// a void member->void member forward, so retail is `mov ecx,[ecx+d]; jmp rel32`.\n"
            "#pragma pack(push,1)\n"
            "struct Sub { void Run(); };\n"
            "struct T {\n%s    Sub* sub;\n    void Run();\n};\n"
            "#pragma pack(pop)\n"
            "void T::Run() { this->sub->Run(); }\n" % (disp, pad(disp))), "Run"

def emit_tail_jmp(b):
    return ("// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.\n"
            "extern \"C\" void ForwardTarget();\n"
            "extern \"C\" void Forward() { ForwardTarget(); }\n"), "Forward"

def emit_global_getter(b):
    return ("// Global getter: `mov eax,[g]; ret`. Free function, no `this`.\n"
            "extern int g_value;\n"
            "extern \"C\" int GetGlobal() { return g_value; }\n"), "GetGlobal"

def emit_pair_destroy(b, o1, o2):
    """destroy the member at o2, then TAIL-call the one at o1 (std::pair _Dest_val)."""
    # o2 is released first, o1 is tail-called; either may sit lower in the object.
    if o1 < o2:
        lo_off, lo_name, hi_off, hi_name = o1, "first", o2, "second"
    else:
        lo_off, lo_name, hi_off, hi_name = o2, "second", o1, "first"
    layout = ("    char lead[0x%x];\n" % lo_off if lo_off else "")
    layout += "    Part %s;\n" % lo_name
    gap = hi_off - lo_off - 1
    layout += ("    char gap[0x%x];\n" % gap if gap else "")
    layout += "    Part %s;\n" % hi_name
    return ("// std::pair _Dest_val: release the member at this+0x%x, then TAIL-call the one\n"
            "// at this+0x%x. __fastcall this=ecx. pack(1) pins both offsets.\n"
            "#pragma pack(push,1)\n"
            "struct Part { void Release(); };\n"
            "struct Pair {\n%s    void Destroy();\n};\n"
            "#pragma pack(pop)\n"
            "void Pair::Destroy() {\n    this->second.Release();\n    this->first.Release();\n}\n"
            % (o2, o1, layout)), "Destroy"

def emit_addr_of_arg(b, disp):
    return ("// Forward the ADDRESS of the stack argument to a member of the sub-object at\n"
            "// this+0x%x. __fastcall this=ecx, value=stack (ret 4).\n"
            "#pragma pack(push,1)\n"
            "struct Sub { void Add(int* value); };\n"
            "struct T {\n%s    Sub sub;\n    void Add(int value);\n};\n"
            "#pragma pack(pop)\n"
            "void T::Add(int value) { this->sub.Add(&value); }\n" % (disp, pad(disp))), "Add"

def emit_outparam_getter(b, word):
    field = "width" if word == 0 else "height"
    return ("// Getter that materialises a temporary, fills it through an out-param member\n"
            "// call, and returns one word of it. __fastcall this=ecx, no args.\n"
            "struct Rect { int width; int height; };\n"
            "struct T {\n    Rect* GetRect(Rect* out);\n    int Get();\n};\n"
            "int T::Get() {\n    Rect r;\n    return this->GetRect(&r)->%s;\n}\n" % field), "Get"

def emit_char_setter(b, disp):
    return ("// Byte-member setter at this+0x%x from a stack arg. __fastcall this=ecx (ret 4).\n"
            "#pragma pack(push,1)\n"
            "struct T {\n%s    char field;\n    void Set(char value);\n};\n"
            "#pragma pack(pop)\n"
            "void T::Set(char value) { this->field = value; }\n" % (disp, pad(disp))), "Set"

def emit_subptr_setter(b, d1, d2):
    return ("// Store a stack arg into a field of the sub-object POINTER at this+0x%x\n"
            "// (field at sub+0x%x). __fastcall this=ecx (ret 4).\n"
            "#pragma pack(push,1)\n"
            "struct Sub {\n%s    int field;\n};\n"
            "struct T {\n%s    Sub* sub;\n    void Set(int value);\n};\n"
            "#pragma pack(pop)\n"
            "void T::Set(int value) { this->sub->field = value; }\n"
            % (d1, d2, pad(d2), pad(d1))), "Set"

def emit_iter_step(b, disp):
    what = "next" if disp == 0 else "prev"
    return ("// List iterator step: `this->node = this->node->%s;`. __fastcall this=ecx.\n"
            "#pragma pack(push,1)\n"
            "struct Node {\n%s    Node* link;\n};\n"
            "struct Iter { Node* node; void Step(); };\n"
            "#pragma pack(pop)\n"
            "void Iter::Step() { this->node = this->node->link; }\n" % (what, pad(disp))), "Step"

def emit_delete_if_set(b):
    return ("// Null-guarded virtual destroy (vtable slot 0, flag arg 1).\n"
            "// __fastcall pointer=ecx; the callee is the object's own slot 0.\n"
            "struct Obj { virtual void Destroy(int flags); };\n"
            "extern \"C\" void __fastcall DeleteIfSet(Obj* p) { if (p) p->Destroy(1); }\n"), "DeleteIfSet"


# ---------------- 2026-08-20 shape classes (gapscan drain) ----------------
_SIZE = {"char": 1, "bool": 1, "int": 4, "float": 4}

def _layout(fields, ptr_types=None):
    """fields = [(offset, ctype, name)]; emit a pack(1) body with explicit pads."""
    out, cur = "", 0
    for o, ty, nm in sorted(fields):
        if o > cur:
            out += "    char pad_%x[0x%x];\n" % (cur, o - cur)
        out += "    %s %s;\n" % (ty, nm)
        cur = o + _SIZE.get(ty.rstrip("*"), 4)
    return out

def emit_zero_store(b, disp, ty):
    """`mov [ecx+d], 0` -- clear one member, no args."""
    return ("// Clear the member at this+0x%x. __fastcall this=ecx.\n"
            "#pragma pack(push,1)\n"
            "struct T {\n%s    void Clear();\n};\n"
            "#pragma pack(pop)\n"
            "void T::Clear() { this->field = 0; }\n"
            % (disp, _layout([(disp, ty, "field")]))), "Clear"

def emit_two_bool_clear(b, d1, d2):
    """`xor al,al; mov [ecx+d1],al; mov [ecx+d2],al` -- clear two byte flags."""
    return ("// Clear the two byte flags at this+0x%x / this+0x%x. __fastcall this=ecx.\n"
            "#pragma pack(push,1)\n"
            "struct T {\n%s    void Clear();\n};\n"
            "#pragma pack(pop)\n"
            "void T::Clear() { this->flag_a = false; this->flag_b = false; }\n"
            % (d1, d2, _layout([(d1, "bool", "flag_a"), (d2, "bool", "flag_b")]))), "Clear"

def _vdecls(slot):
    """slot = vtable BYTE offset -> filler virtuals so the target lands on that slot."""
    return "".join("    virtual void slot%d();\n" % i for i in range(slot // 4))

def _sub_layout(disp):
    return _layout([(disp, "Sub*", "sub")])

def emit_virt_forward_this(b, disp, slot):
    """`this->sub->Virt(this)` -- forward to a virtual of the sub-object POINTER at
    this+disp, passing `this` as the argument. Retail keeps `this` in eax across the
    `mov ecx,[eax+d]` that clobbers ecx."""
    return ("// Forward to vtable slot %d of the sub-object pointer at this+0x%x, passing\n"
            "// `this` as the argument. __fastcall this=ecx.\n"
            "#pragma pack(push,1)\n"
            "struct T;\n"
            "struct Sub {\n%s    virtual void Do(T* p);\n};\n"
            "struct T {\n%s    void Run();\n};\n"
            "#pragma pack(pop)\n"
            "void T::Run() { this->sub->Do(this); }\n"
            % (slot // 4, disp, _vdecls(slot), _sub_layout(disp))), "Run"

def emit_virt_forward_deref(b, disp, slot):
    """`this->sub->Virt(*p)` -- VC7.1 tail-jumps it, rewriting the incoming stack slot."""
    return ("// Tail-forward to vtable slot %d of the sub-object pointer at this+0x%x,\n"
            "// passing the DEREFERENCED stack argument (VC7.1 rewrites the incoming arg\n"
            "// slot and `jmp`s). __fastcall this=ecx.\n"
            "#pragma pack(push,1)\n"
            "struct Sub {\n%s    virtual void Do(int v);\n};\n"
            "struct T {\n%s    void Do(int* p);\n};\n"
            "#pragma pack(pop)\n"
            "void T::Do(int* p) { this->sub->Do(*p); }\n"
            % (slot // 4, disp, _vdecls(slot), _sub_layout(disp))), "Do"

def emit_ptr_compare(b, ne):
    op = "!=" if ne else "=="
    return ("// Free pointer comparison, __stdcall (`ret 8`): returns a %s b.\n"
            "extern \"C\" bool __stdcall Compare(void* a, void* b) { return a %s b; }\n"
            % (op, op)), "Compare"

def emit_int_compare(b, ne):
    op = "!=" if ne else "=="
    return ("// Free int comparison, __stdcall (`ret 8`): returns a %s b.\n"
            "extern \"C\" bool __stdcall Compare(int a, int b) { return a %s b; }\n"
            % (op, op)), "Compare"

def emit_field_compare(b, disp, ne):
    op = "!=" if ne else "=="
    return ("// Free __stdcall comparison of the member at +0x%x of two objects: a %s b.\n"
            "#pragma pack(push,1)\n"
            "struct T {\n%s};\n"
            "#pragma pack(pop)\n"
            "extern \"C\" bool __stdcall Compare(T* a, T* b) { return a->field %s b->field; }\n"
            % (disp, op, _layout([(disp, "int", "field")]), op)), "Compare"

def emit_self_field_ge(b, d1, d2):
    """`cmp` + `sbb eax,eax` + `inc eax` == unsigned >= of two members."""
    return ("// `return this->[0x%x] >= this->[0x%x];` (unsigned: cmp / sbb / inc).\n"
            "#pragma pack(push,1)\n"
            "struct T {\n%s    bool AtLeast();\n};\n"
            "#pragma pack(pop)\n"
            "bool T::AtLeast() { return this->f_%x >= this->f_%x; }\n"
            % (d1, d2, _layout([(d1, "int", "f_%x" % d1), (d2, "int", "f_%x" % d2)]),
               d1, d2)), "AtLeast"

def emit_assign_ret_self(b):
    """`mov eax,ecx; mov ecx,[esp+4]; mov [eax],ecx; ret 4` -- store then return *this."""
    return ("// Assign the stack argument to the member at this+0 and return *this.\n"
            "// __fastcall this=ecx (ret 4).\n"
            "struct T {\n    int field;\n    T& Assign(int value);\n};\n"
            "T& T::Assign(int value) { this->field = value; return *this; }\n"), "Assign"

def emit_return_this(b):
    n = ret_args(b); p = params(n)
    return ("// Returns `this` unchanged. __fastcall this=ecx.\n"
            "struct T { T* Self(%s); };\n"
            "T* T::Self(%s) { return this; }\n" % (p, p)), "Self"


def classify(b):
    # ---- 2026-08-20 classes ----
    # `mov byte [ecx+d], 0` / `mov dword [ecx+d], 0`
    if b[0] in (0xc6, 0xc7) and b[1] in (0x41, 0x81) and b.endswith(b"\xc3"):
        d = b[2] if b[1] == 0x41 else struct.unpack_from("<I", b, 2)[0]
        i = 3 if b[1] == 0x41 else 6
        if b[0] == 0xc6 and b[i:i + 1] == b"\x00" and i + 2 == len(b):
            return emit_zero_store(b, d, "char")
        if b[0] == 0xc7 and b[i:i + 4] == b"\x00\x00\x00\x00" and i + 5 == len(b):
            return emit_zero_store(b, d, "int")
    # xor al,al; mov [ecx+d1],al; mov [ecx+d2],al; ret
    if len(b) == 9 and b[0:2] == b"\x32\xc0" and b[2] == 0x88 and b[3] == 0x41 \
            and b[5] == 0x88 and b[6] == 0x41 and b[8] == 0xc3:
        return emit_two_bool_clear(b, b[4], b[7])
    # mov eax,ecx; mov ecx,[eax+d]; mov edx,[ecx]; push eax; call [edx+slot]; ret
    if len(b) == 12 and b[0:2] == b"\x8b\xc1" and b[2] == 0x8b and b[3] == 0x48 \
            and b[5:8] == b"\x8b\x11\x50" and b[8] == 0xff and b[9] == 0x52 \
            and b[11] == 0xc3:
        return emit_virt_forward_this(b, b[4], b[10])
    # mov edx,[esp+4]; mov ecx,[ecx+d]; mov edx,[edx]; mov eax,[ecx]; mov [esp+4],edx; jmp [eax]
    if len(b) == 17 and b[0:4] == b"\x8b\x54\x24\x04" and b[4] == 0x8b and b[5] == 0x49 \
            and b[7:9] == b"\x8b\x12" and b[9:11] == b"\x8b\x01" \
            and b[11:15] == b"\x89\x54\x24\x04" and b[15:17] == b"\xff\x20":
        return emit_virt_forward_deref(b, b[6], 0)
    # mov ecx,[esp+4]; mov edx,[esp+8]; xor eax,eax; cmp ecx,edx; sete al; ret 8
    if b == b"\x8b\x4c\x24\x04\x8b\x54\x24\x08\x33\xc0\x3b\xca\x0f\x94\xc0\xc2\x08\x00":
        return emit_ptr_compare(b, False)
    # mov eax,[esp+4]; cmp eax,[esp+8]; sbb eax,eax; neg eax; ret 8   -> a != b
    if b == b"\x8b\x44\x24\x04\x3b\x44\x24\x08\x1b\xc0\xf7\xd8\xc2\x08\x00":
        return emit_int_compare(b, True)
    # a->f == b->f: mov eax,[esp+4]; mov eax,[eax+d]; mov ecx,[esp+8]; sub eax,[ecx+d];
    #               neg eax; sbb eax,eax; inc eax; ret 8
    if b[0:4] == b"\x8b\x44\x24\x04" and len(b) > 8 and b[4] == 0x8b and b[5] in (0x00, 0x40):
        d = 0 if b[5] == 0x00 else b[6]
        i = 6 if b[5] == 0x00 else 7
        if b[i:i + 4] == b"\x8b\x4c\x24\x08" and b[i + 4] == 0x2b and b[i + 5] in (0x01, 0x41):
            d2 = 0 if b[i + 5] == 0x01 else b[i + 6]
            j = i + 6 if b[i + 5] == 0x01 else i + 7
            if d == d2 and b[j:] == b"\xf7\xd8\x1b\xc0\x40\xc2\x08\x00":
                return emit_field_compare(b, d, False)
    # a->f != b->f: mov eax,[esp+4]; mov ecx,[eax+d]; mov edx,[esp+8]; cmp ecx,[edx+d];
    #               sbb eax,eax; neg eax; ret 8
    if b[0:4] == b"\x8b\x44\x24\x04" and len(b) > 8 and b[4] == 0x8b and b[5] in (0x08, 0x48):
        d = 0 if b[5] == 0x08 else b[6]
        i = 6 if b[5] == 0x08 else 7
        if b[i:i + 4] == b"\x8b\x54\x24\x08" and b[i + 4] == 0x3b and b[i + 5] in (0x0a, 0x4a):
            d2 = 0 if b[i + 5] == 0x0a else b[i + 6]
            j = i + 6 if b[i + 5] == 0x0a else i + 7
            if d == d2 and b[j:] == b"\x1b\xc0\xf7\xd8\xc2\x08\x00":
                return emit_field_compare(b, d, True)
    # mov eax,[ecx+d1]; cmp eax,[ecx+d2]; sbb eax,eax; inc eax; ret
    if len(b) == 10 and b[0] == 0x8b and b[1] == 0x41 and b[3] == 0x3b and b[4] == 0x41 \
            and b[6:10] == b"\x1b\xc0\x40\xc3":
        return emit_self_field_ge(b, b[2], b[5])
    # mov eax,ecx; mov ecx,[esp+4]; mov [eax],ecx; ret 4
    if b == b"\x8b\xc1\x8b\x4c\x24\x04\x89\x08\xc2\x04\x00":
        return emit_assign_ret_self(b)
    # mov eax,ecx; ret / ret imm16   -> returns `this`
    if b[0:2] == b"\x8b\xc1" and (b[2:] == b"\xc3" or (len(b) == 5 and b[2] == 0xc2)):
        return emit_return_this(b)
    # byte setter: mov al,[esp+4]; mov [ecx+d],al; ret 4
    if b[0:4] == b"\x8a\x44\x24\x04" and b[4] == 0x88 and b[5] in (0x41, 0x81) and b[-3] == 0xc2:
        core = b[4:-3]
        if (b[5] == 0x41 and len(core) == 3) or (b[5] == 0x81 and len(core) == 6):
            return emit_char_setter(b, core[2] if b[5] == 0x41 else struct.unpack_from("<I", core, 2)[0])
    # sub-pointer setter: mov eax,[ecx+d1]; mov ecx,[esp+4]; mov [eax+d2],ecx; ret 4
    if b[0] == 0x8b and b[1] in (0x41, 0x81) and b[-3] == 0xc2:
        i = 3 if b[1] == 0x41 else 6
        d1 = b[2] if b[1] == 0x41 else struct.unpack_from("<I", b, 2)[0]
        if b[i:i + 4] == b"\x8b\x4c\x24\x04" and b[i + 4] == 0x89 and b[i + 5] in (0x48, 0x88):
            j = i + 6
            d2 = b[j] if b[i + 5] == 0x48 else struct.unpack_from("<I", b, j)[0]
            j += 1 if b[i + 5] == 0x48 else 4
            if j == len(b) - 3:
                return emit_subptr_setter(b, d1, d2)
    # iterator step: mov eax,[ecx]; mov edx,[eax+d]; mov [ecx],edx; ret
    if b[0:2] == b"\x8b\x01" and b[2] == 0x8b and b[3] in (0x10, 0x50) and b.endswith(b"\x89\x11\xc3"):
        if b[3] == 0x10 and len(b) == 7:
            return emit_iter_step(b, 0)
        if b[3] == 0x50 and len(b) == 8:
            return emit_iter_step(b, b[4])
    if b == b"\x85\xc9\x74\x06\x8b\x01\x6a\x01\xff\x10\xc3":
        return emit_delete_if_set(b)
    # pair _Dest_val: push esi; mov esi,ecx; lea ecx,[esi+o2]; call; <first>; pop esi; jmp
    if len(b) >= 19 and b[0:3] == b"\x56\x8b\xf1" and b[3] == 0x8d and b[4] in (0x4e, 0x8e):
        o2 = b[5] if b[4] == 0x4e else struct.unpack_from("<I", b, 5)[0]
        i = 6 if b[4] == 0x4e else 9
        if b[i] == 0xe8:
            i += 5
            if b[i:i + 4] == b"\x8b\xce\x5e\xe9" and len(b) == i + 8:
                return emit_pair_destroy(b, 0, o2)
            if b[i] == 0x8d and b[i + 1] in (0x4e, 0x8e):
                o1 = b[i + 2] if b[i + 1] == 0x4e else struct.unpack_from("<I", b, i + 2)[0]
                j = i + 3 if b[i + 1] == 0x4e else i + 6
                if b[j:j + 2] == b"\x5e\xe9" and len(b) == j + 6:
                    return emit_pair_destroy(b, o1, o2)
    # lea eax,[esp+4]; push eax; add ecx,disp; call; ret 4
    if len(b) in (17, 19) and b[0:5] == b"\x8d\x44\x24\x04\x50" and b[5] in (0x81, 0x83) and b[6] == 0xc1:
        d = b[7] if b[5] == 0x83 else struct.unpack_from("<I", b, 7)[0]
        i = 8 if b[5] == 0x83 else 11
        if b[i] == 0xe8 and b[i + 5:] == b"\xc2\x04\x00":
            return emit_addr_of_arg(b, d)
    # sub esp,8; lea eax,[esp]; push eax; call; mov eax,[eax(+4)]; add esp,8; ret
    if b[0:6] == b"\x83\xec\x08\x8d\x04\x24" and b[6] == 0x50 and b[7] == 0xe8:
        rest = b[12:]
        if rest == b"\x8b\x00\x83\xc4\x08\xc3":
            return emit_outparam_getter(b, 0)
        if rest == b"\x8b\x40\x04\x83\xc4\x08\xc3":
            return emit_outparam_getter(b, 1)
    # shapes that end in a tail jump (no `ret` at all)
    if len(b) == 11 and b[0:2] == b"\x8b\x89" and b[6] == 0xe9:
        return emit_sub_forward(b, struct.unpack_from("<I", b, 2)[0])
    if len(b) == 8 and b[0:2] == b"\x8b\x49" and b[3] == 0xe9:
        return emit_sub_forward(b, b[2])
    if len(b) == 5 and b[0] == 0xe9:
        return emit_tail_jmp(b)
    if len(b) == 6 and b[0] == 0xa1 and b[5] == 0xc3:
        return emit_global_getter(b)
    n = ret_args(b)
    if n is None:
        return None
    core = b[:-1] if b.endswith(b"\xc3") else b[:-3]
    if core == b"":
        return emit_empty(b)
    if core == b"\x32\xc0":
        return emit_const(b, "false", "bool")
    if core == b"\xb0\x01":
        return emit_const(b, "true", "bool")
    if core == b"\x33\xc0":
        return emit_const(b, "0", "int")
    if core == b"\x83\xc8\xff":
        return emit_const(b, "-1", "int")
    # disp-0 member accessors: `mov eax,[ecx]` / `mov al,[ecx]` / `fld dword [ecx]`
    if len(core) == 2 and core[1] == 0x01 and core[0] in (0x8b, 0x8a, 0xd9):
        return emit_getter(b, {0x8b: "int", 0x8a: "char", 0xd9: "float"}[core[0]], 0)
    if len(core) == 5 and core[0] == 0xb8:
        return emit_const(b, "0x%x" % struct.unpack_from("<I", core, 1)[0], "int")
    # member accessors: opcode + modrm(reg=eax, rm=ecx) + disp8/disp32
    ACC = {0x8b: "int", 0x8a: "char", 0xd9: "float"}
    if len(core) in (3, 6) and core[1] in (0x41, 0x81):
        ok8 = core[1] == 0x41 and len(core) == 3
        ok32 = core[1] == 0x81 and len(core) == 6
        if ok8 or ok32:
            d = core[2] if ok8 else struct.unpack_from("<I", core, 2)[0]
            if core[0] == 0x8d:
                return emit_addr(b, d)
            if core[0] in ACC:
                return emit_getter(b, ACC[core[0]], d)
    # setter: mov eax,[esp+4]; mov [ecx+disp],eax; ret 4
    if n == 4 and core.startswith(b"\x8b\x44\x24\x04"):
        rest = core[4:]
        ok8 = len(rest) == 3 and rest[1] == 0x41
        ok32 = len(rest) == 6 and rest[1] == 0x81
        if rest[:1] == b"\x89" and (ok8 or ok32):
            d = rest[2] if ok8 else struct.unpack_from("<I", rest, 2)[0]
            return emit_setter(b, "int", d)
    return None

