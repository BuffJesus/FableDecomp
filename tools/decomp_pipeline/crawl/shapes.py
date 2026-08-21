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
        cur = o + _SIZE.get(ty.rstrip("* "), 4)
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

def emit_int_compare(b, lt):
    """`cmp; sbb eax,eax; neg eax` is UNSIGNED LESS-THAN (it materialises CF), NOT `!=`.
    The equality idiom is `sub; neg; sbb; inc`. The return type must be `int`: a `bool`
    return compiles to `setb` instead of the sbb/neg pair."""
    if lt:
        return ("// Free unsigned comparison, __stdcall (`ret 8`): returns a < b.\n"
                "extern \"C\" int __stdcall Compare(unsigned int a, unsigned int b)"
                " { return a < b; }\n"), "Compare"
    return ("// Free int comparison, __stdcall (`ret 8`): returns a == b.\n"
            "extern \"C\" bool __stdcall Compare(int a, int b) { return a == b; }\n"), "Compare"

def emit_field_compare(b, disp, lt, disp_b=None):
    """lt: `cmp; sbb; neg` == UNSIGNED `a->f < b->f` (int return; bool gives `setb`).
    else: `sub; neg; sbb; inc` == `a->f == b->f`.
    The two operands often sit at DIFFERENT offsets (comparing unlike types, e.g. an
    iterator against a container end), so each side gets its own struct."""
    if disp_b is None:
        disp_b = disp
    ty = "unsigned int" if lt else "int"
    op = "<" if lt else "=="
    ret = "int" if lt else "bool"
    return ("// Free __stdcall comparison: a->[0x%x] %s b->[0x%x].\n"
            "#pragma pack(push,1)\n"
            "struct A {\n%s};\n"
            "struct B {\n%s};\n"
            "#pragma pack(pop)\n"
            "extern \"C\" %s __stdcall Compare(A* a, B* b)"
            " { return a->field %s b->field; }\n"
            % (disp, op, disp_b, _layout([(disp, ty, "field")]),
               _layout([(disp_b, ty, "field")]), ret, op)), "Compare"

def emit_self_field_ge(b, d1, d2):
    """`cmp` + `sbb eax,eax` + `inc eax` == unsigned >= of two members (int return)."""
    return ("// `return this->[0x%x] >= this->[0x%x];` (unsigned: cmp / sbb / inc).\n"
            "#pragma pack(push,1)\n"
            "struct T {\n%s    int AtLeast();\n};\n"
            "#pragma pack(pop)\n"
            "int T::AtLeast() { return this->f_%x >= this->f_%x; }\n"
            % (d1, d2, _layout([(d1, "unsigned int", "f_%x" % d1),
                                (d2, "unsigned int", "f_%x" % d2)]), d1, d2)), "AtLeast"

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


# ---------------- second gapscan round ----------------
def emit_virt_forward_subobj(b, disp, part, slot):
    """`this->sub->Virt(&this->part, 0)` -- sub-object POINTER at this+disp, the
    argument is the ADDRESS of an embedded member (`add eax,part`), plus a 0 flag."""
    return ("// Forward to vtable slot %d of the sub-object pointer at this+0x%x, passing\n"
            "// the address of the embedded member at this+0x%x and a 0 flag.\n"
            "#pragma pack(push,1)\n"
            "struct Part { int dummy; };\n"
            "struct Sub {\n%s    virtual void Do(Part* p, int flag);\n};\n"
            "struct T {\n%s    void Run();\n};\n"
            "#pragma pack(pop)\n"
            "void T::Run() { this->sub->Do(&this->part, 0); }\n"
            % (slot // 4, disp, part, _vdecls(slot),
               _layout([(disp, "Sub*", "sub"), (part, "Part", "part")]))), "Run"

def emit_zero_init_ret_this(b, count, nargs):
    """`mov eax,ecx; mov [eax+k],0 ...; ret` -- zero N leading dwords and return this."""
    fields = "".join("    int f%d;\n" % i for i in range(count))
    body = "".join("    this->f%d = 0;\n" % i for i in range(count))
    p = ", ".join("int a%d" % i for i in range(nargs))
    return ("// Zero the first %d dword members and return `this`. __fastcall this=ecx.\n"
            "#pragma pack(push,1)\n"
            "struct T {\n%s    T* Init(%s);\n};\n"
            "#pragma pack(pop)\n"
            "T* T::Init(%s) {\n%s    return this;\n}\n"
            % (count, fields, p, p, body)), "Init"

def emit_is_null(b, disp):
    return ("// `return this->ptr == 0;` (xor/test/sete). __fastcall this=ecx.\n"
            "#pragma pack(push,1)\n"
            "struct T {\n%s    bool IsNull();\n};\n"
            "#pragma pack(pop)\n"
            "bool T::IsNull() { return this->ptr == 0; }\n"
            % _layout([(disp, "int", "ptr")])), "IsNull"

def emit_member_add(b, disp, delta):
    """`mov eax,[ecx+d]; add eax,k` -- pointer member advanced by a constant."""
    return ("// `return this->[0x%x] + 0x%x;` -- member pointer advanced by a constant.\n"
            "#pragma pack(push,1)\n"
            "struct T {\n%s    char* Advance();\n};\n"
            "#pragma pack(pop)\n"
            "char* T::Advance() { return this->ptr + 0x%x; }\n"
            % (disp, delta, _layout([(disp, "int", "ptr")]).replace("int ptr", "char* ptr"),
               delta)), "Advance"

def emit_arg_add(b, delta):
    return ("// Free __fastcall-free adder: `return (char*)a + 0x%x;` (`ret`, cdecl).\n"
            "extern \"C\" char* Advance(char* a) { return a + 0x%x; }\n"
            % (delta, delta)), "Advance"

def emit_subptr_inc(b, d1, d2):
    """`mov eax,[ecx+d1]; inc dword [eax+d2]` -- bump a counter through a member ptr."""
    return ("// `++this->sub->counter;` -- sub-object pointer at this+0x%x, counter at\n"
            "// sub+0x%x. __fastcall this=ecx.\n"
            "#pragma pack(push,1)\n"
            "struct Sub {\n%s};\n"
            "struct T {\n%s    void Bump();\n};\n"
            "#pragma pack(pop)\n"
            "void T::Bump() { ++this->sub->counter; }\n"
            % (d1, d2, _layout([(d2, "int", "counter")]),
               _layout([(d1, "Sub*", "sub")]))), "Bump"

def emit_int_and_byte_clear(b, d1, d2):
    return ("// Clear the dword at this+0x%x and the byte flag at this+0x%x.\n"
            "#pragma pack(push,1)\n"
            "struct T {\n%s    void Clear();\n};\n"
            "#pragma pack(pop)\n"
            "void T::Clear() { this->count = 0; this->flag = false; }\n"
            % (d1, d2, _layout([(d1, "int", "count"), (d2, "bool", "flag")]))), "Clear"

def emit_guarded_virt(b, slot, arg):
    return ("// Null-guarded virtual call: `if (p) p->Slot%d(%d);` __fastcall p=ecx.\n"
            "struct Obj {\n%s    virtual void Call(int flags);\n};\n"
            "extern \"C\" void __fastcall CallIfSet(Obj* p) { if (p) p->Call(%d); }\n"
            % (slot // 4, arg, _vdecls(slot), arg)), "CallIfSet"

def emit_outparam_pair_copy(b, d1, d2):
    """`mov edx,[ecx+d1]; mov eax,[esp+4]; mov [eax],edx; mov ecx,[ecx+d2]; mov [eax+4],ecx`
    -- copy two members into a caller-supplied 2-dword struct."""
    return ("// Copy the members at this+0x%x / this+0x%x into the caller's 2-dword struct.\n"
            "// __fastcall this=ecx, out=stack (ret 4).\n"
            "#pragma pack(push,1)\n"
            "struct Out { int a; int b; };\n"
            "struct T {\n%s    void Get(Out* out);\n};\n"
            "#pragma pack(pop)\n"
            "void T::Get(Out* out) {\n    out->a = this->first;\n    out->b = this->second;\n}\n"
            % (d1, d2, _layout([(d1, "int", "first"), (d2, "int", "second")]))), "Get"

def emit_global_times_ten(b):
    """`mov eax,[g]; lea eax,[eax+eax*4]; add eax,eax` -- the strength-reduced `g * 10`."""
    return ("// `return g * 10;` -- VC7.1 strength-reduces to `lea eax,[eax+eax*4]; add eax,eax`.\n"
            "extern int g_value;\n"
            "extern \"C\" int TimesTen() { return g_value * 10; }\n"), "TimesTen"

def emit_refptr_copy(b):
    """`mov eax,ecx; mov ecx,[esp+4]; mov edx,[ecx]; mov [eax],edx; mov edx,[ecx+4];
    test edx,edx; mov [eax+4],edx; je +2; inc dword [edx]` -- copy an intelligent
    pointer (target + refcount) and bump the refcount when there is one."""
    return ("// Intelligent-pointer copy: take the target and the refcount block from the\n"
            "// source, then bump the count when the block exists. VC7.1 schedules the\n"
            "// second store ahead of the branch. __fastcall this=ecx (ret 4).\n"
            "struct RefCount { int count; };\n"
            "struct Ptr {\n    void* target;\n    RefCount* refs;\n};\n"
            "struct T {\n    void* target;\n    RefCount* refs;\n"
            "    T* Copy(const Ptr& other);\n};\n"
            "T* T::Copy(const Ptr& other) {\n"
            "    this->target = other.target;\n"
            "    this->refs = other.refs;\n"
            "    if (this->refs) ++this->refs->count;\n"
            "    return this;\n}\n"), "Copy"

def emit_is_not_null(b, disp):
    return ("// `return this->ptr != 0;` (xor/test/setne). __fastcall this=ecx.\n"
            "#pragma pack(push,1)\n"
            "struct T {\n%s    bool IsSet();\n};\n"
            "#pragma pack(pop)\n"
            "bool T::IsSet() { return this->ptr != 0; }\n"
            % _layout([(disp, "int", "ptr")])), "IsSet"

def emit_self_virtual_tail(b, slot):
    """`mov eax,[ecx]; jmp [eax+slot]` -- a non-virtual member whose whole body is a call
    to one of its OWN virtuals, which VC7.1 tail-calls."""
    return ("// Non-virtual member whose entire body calls one of its own virtuals; VC7.1\n"
            "// tail-jumps through the vtable (slot %d). __fastcall this=ecx.\n"
            "struct T {\n%s    virtual void Target();\n    void Run();\n};\n"
            "void T::Run() { this->Target(); }\n"
            % (slot // 4, _vdecls(slot))), "Run"

def emit_empty_noopt(b):
    """`push ebp; mov ebp,esp; push ecx; mov [ebp-4],ecx; mov esp,ebp; pop ebp; ret`
    -- an empty member from an UNOPTIMISED translation unit."""
    return ("// Empty member function from an unoptimised TU: retail keeps the frame and\n"
            "// the `this` spill. Needs `#pragma optimize(\"\",off)`, which the verifier's\n"
            "// pragma sweep supplies. __fastcall this=ecx.\n"
            "#pragma optimize(\"\",off)\n"
            "struct T { void Hook(); };\n"
            "void T::Hook() {}\n"), "Hook"

def emit_vtbl_dtor(b):
    """`mov dword [ecx], offset vftable; ret` -- the destructor of a polymorphic class
    with nothing to destroy still re-installs the vptr. The vtable address is a DATA
    relocation, so one source serves every class in the family."""
    return ("// Destructor of a polymorphic class with no members to release: MSVC still\n"
            "// re-installs the vptr (`mov dword ptr [ecx], offset vftable`). The vtable\n"
            "// address is a data relocation. __fastcall this=ecx.\n"
            "struct T {\n    virtual void Method() {}\n    ~T();\n};\n"
            "T::~T() {}\n"), "~T"

def emit_virt_forward_this_arg(b, disp, slot):
    """`this->sub->Virt(this, a)` -- as emit_virt_forward_this but with one stack arg,
    which retail parks in esi across the call."""
    return ("// Forward to vtable slot %d of the sub-object pointer at this+0x%x, passing\n"
            "// `this` and the stack argument. __fastcall this=ecx (ret 4).\n"
            "#pragma pack(push,1)\n"
            "struct T;\n"
            "struct Sub {\n%s    virtual void Do(T* p, int a);\n};\n"
            "struct T {\n%s    void Run(int a);\n};\n"
            "#pragma pack(pop)\n"
            "void T::Run(int a) { this->sub->Do(this, a); }\n"
            % (slot // 4, disp, _vdecls(slot), _sub_layout(disp))), "Run"

def emit_construct_helper(b):
    """`mov ecx,[esp+4]; call Init; mov eax,[esp+4]; ret 4` -- run a member on the
    argument and hand the argument back."""
    return ("// Construct-and-return helper: run a member function on the argument and\n"
            "// return the argument. __stdcall (ret 4); the callee takes this in ecx.\n"
            "struct T { void Init(); };\n"
            "extern \"C\" T* __stdcall Construct(T* p) { p->Init(); return p; }\n"), "Construct"

def emit_float_global_getter(b):
    return ("// Float global getter: `fld dword ptr [g]; ret`. Free function, no `this`.\n"
            "extern float g_value;\n"
            "extern \"C\" float GetGlobal() { return g_value; }\n"), "GetGlobal"

def emit_free_zero_init(b, count):
    fields = "".join("    int f%d;\n" % i for i in range(count))
    body = "".join("    p->f%d = 0;\n" % i for i in range(count))
    return ("// __stdcall free initialiser: zero the first %d dword members of the argument.\n"
            "#pragma pack(push,1)\n"
            "struct T {\n%s};\n"
            "#pragma pack(pop)\n"
            "extern \"C\" void __stdcall Init(T* p) {\n%s}\n"
            % (count, fields, body)), "Init"

def emit_assign_arg2_ret_self(b):
    return ("// Store the SECOND stack argument at this+0 and return `this`; the first is\n"
            "// unused. __fastcall this=ecx (ret 8).\n"
            "struct T {\n    int field;\n    T* Init(int unused, int value);\n};\n"
            "T* T::Init(int unused, int value) { this->field = value; return this; }\n"), "Init"

def emit_pair_init(b):
    return ("// Store both stack arguments at this+0 / this+4 and return `this`.\n"
            "// __fastcall this=ecx (ret 8).\n"
            "struct T {\n    int first;\n    int second;\n    T* Init(int a, int b);\n};\n"
            "T* T::Init(int a, int b) {\n    this->first = a;\n    this->second = b;\n"
            "    return this;\n}\n"), "Init"

def emit_member_vector_size(b, disp):
    """`mov ecx,[ecx+d]; mov eax,[ecx+4]; sub eax,[ecx]; sar eax,2` -- element count of a
    vector reached through a member pointer."""
    return ("// Element count of the vector reached through the member pointer at this+0x%x\n"
            "// (`(last - first) >> 2`). __fastcall this=ecx.\n"
            "#pragma pack(push,1)\n"
            "struct Vec { int* first; int* last; };\n"
            "struct T {\n%s    int Size();\n};\n"
            "#pragma pack(pop)\n"
            "int T::Size() { return this->vec->last - this->vec->first; }\n"
            % (disp, _layout([(disp, "Vec*", "vec")]))), "Size"

def emit_free_byte_clear(b):
    return ("// __stdcall free function: clear the byte pointed to by the SECOND argument.\n"
            "extern \"C\" void __stdcall Clear(int unused, char* p) { *p = 0; }\n"), "Clear"

def emit_subptr_setter0(b):
    """`mov ecx,[ecx]; mov eax,[esp+4]; mov [ecx],eax; ret 4` -- store through this->p."""
    return ("// Store the stack argument through the pointer at this+0 (`this->p->field`).\n"
            "// __fastcall this=ecx (ret 4).\n"
            "struct Sub { int field; };\n"
            "struct T { Sub* sub; void Set(int value); };\n"
            "void T::Set(int value) { this->sub->field = value; }\n"), "Set"


def classify(b):
    # ---- 2026-08-20 classes, fourth round ----
    # mov edx,[ecx+d1]; mov eax,[esp+4]; mov [eax],edx; mov ecx,[ecx+d2]; mov [eax+4],ecx; ret 4
    if b[0] == 0x8b and b[1] in (0x51, 0x91) and b.endswith(b"\x89\x48\x04\xc2\x04\x00"):
        d1 = b[2] if b[1] == 0x51 else struct.unpack_from("<I", b, 2)[0]
        i = 3 if b[1] == 0x51 else 6
        if b[i:i + 4] == b"\x8b\x44\x24\x04" and b[i + 4:i + 6] == b"\x89\x10" \
                and b[i + 6] == 0x8b and b[i + 7] in (0x49, 0x89):
            d2 = b[i + 8] if b[i + 7] == 0x49 else struct.unpack_from("<I", b, i + 8)[0]
            j = i + 9 if b[i + 7] == 0x49 else i + 12
            if j == len(b) - 6:
                return emit_outparam_pair_copy(b, d1, d2)
    # mov eax,[g]; lea eax,[eax+eax*4]; add eax,eax; ret   -> g * 10
    if len(b) == 11 and b[0] == 0xa1 and b[5:11] == b"\x8d\x04\x80\xd1\xe0\xc3":
        return emit_global_times_ten(b)
    # intelligent-pointer copy (target + refcount, bump when present)
    if b == b"\x8b\xc1\x8b\x4c\x24\x04\x8b\x11\x89\x10\x8b\x51\x04\x85\xd2\x89\x50\x04" \
            b"\x74\x02\xff\x02\xc2\x04\x00":
        return emit_refptr_copy(b)
    # mov edx,[ecx+d]; xor eax,eax; test edx,edx; setne al; ret
    if b[0] == 0x8b and b[1] in (0x11, 0x51, 0x91) and b.endswith(b"\x33\xc0\x85\xd2\x0f\x95\xc0\xc3"):
        d = 0 if b[1] == 0x11 else (b[2] if b[1] == 0x51 else struct.unpack_from("<I", b, 2)[0])
        i = 2 if b[1] == 0x11 else (3 if b[1] == 0x51 else 6)
        if len(b) == i + 8:
            return emit_is_not_null(b, d)
    # mov eax,[ecx]; jmp [eax+slot]  -- tail call through this object's own vtable
    if len(b) == 5 and b[0:2] == b"\x8b\x01" and b[2:4] == b"\xff\x60":
        return emit_self_virtual_tail(b, b[4])
    # push ebp; mov ebp,esp; push ecx; mov [ebp-4],ecx; mov esp,ebp; pop ebp; ret
    if b == b"\x55\x8b\xec\x51\x89\x4d\xfc\x8b\xe5\x5d\xc3":
        return emit_empty_noopt(b)
    # ---- 2026-08-20 classes, third round ----
    # mov dword [ecx], offset vftable; ret   -- dtor of a polymorphic class
    if len(b) == 7 and b[0:2] == b"\xc7\x01" and b[6] == 0xc3:
        return emit_vtbl_dtor(b)
    # xor eax,eax; inc eax; ret   -- `return 1;` (size-optimised)
    if b == b"\x33\xc0\x40\xc3":
        return emit_const(b, "1", "int")
    # push imm8; pop eax; ret   -- small constant return
    if len(b) == 4 and b[0] == 0x6a and b[2] == 0x58 and b[3] == 0xc3:
        return emit_const(b, "%d" % b[1], "int")
    # push esi; mov esi,[esp+8]; mov eax,ecx; mov ecx,[eax+d]; mov edx,[ecx];
    # push esi; push eax; call [edx+slot]; pop esi; ret 4
    if len(b) == 21 and b[0] == 0x56 and b[1:5] == b"\x8b\x74\x24\x08" \
            and b[5:7] == b"\x8b\xc1" and b[7] == 0x8b and b[8] == 0x48 \
            and b[10:14] == b"\x8b\x11\x56\x50" and b[14:16] == b"\xff\x52" \
            and b[17] == 0x5e and b[18] == 0xc2:
        return emit_virt_forward_this_arg(b, b[9], b[16])
    # mov ecx,[esp+4]; call rel32; mov eax,[esp+4]; ret 4  -- construct-and-return helper
    if len(b) == 16 and b[0:4] == b"\x8b\x4c\x24\x04" and b[4] == 0xe8 \
            and b[9:13] == b"\x8b\x44\x24\x04" and b[13] == 0xc2:
        return emit_construct_helper(b)
    # fld dword [global]; ret
    if len(b) == 7 and b[0:2] == b"\xd9\x05" and b[6] == 0xc3:
        return emit_float_global_getter(b)
    # mov eax,[esp+4]; xor ecx,ecx; zero 3 dwords through it; ret 4
    if b == b"\x8b\x44\x24\x04\x33\xc9\x89\x08\x89\x48\x04\x89\x48\x08\xc2\x04\x00":
        return emit_free_zero_init(b, 3)
    # mov eax,ecx; mov ecx,[esp+8]; mov [eax],ecx; ret 8  -- keep 2nd arg, return this
    if b == b"\x8b\xc1\x8b\x4c\x24\x08\x89\x08\xc2\x08\x00":
        return emit_assign_arg2_ret_self(b)
    # mov edx,[esp+8]; mov eax,ecx; mov ecx,[esp+4]; mov [eax],ecx; mov [eax+4],edx; ret 8
    if b == b"\x8b\x54\x24\x08\x8b\xc1\x8b\x4c\x24\x04\x89\x08\x89\x50\x04\xc2\x08\x00":
        return emit_pair_init(b)
    # mov ecx,[ecx+d]; mov eax,[ecx+4]; sub eax,[ecx]; sar eax,2  -- vector size via a member
    if len(b) == 11 and b[0] == 0x8b and b[1] == 0x49 and b[3:5] == b"\x8b\x41" \
            and b[5] == 0x04 and b[6:8] == b"\x2b\x01" and b[8:11] == b"\xc1\xf8\x02" \
            or (len(b) == 12 and b[0] == 0x8b and b[1] == 0x49 and b[3:5] == b"\x8b\x41"
                and b[5] == 0x04 and b[6:8] == b"\x2b\x01" and b[8:11] == b"\xc1\xf8\x02"
                and b[11] == 0xc3):
        if len(b) == 12:
            return emit_member_vector_size(b, b[2])
    # mov eax,[esp+8]; mov byte [eax],0; ret 8
    if b == b"\x8b\x44\x24\x08\xc6\x00\x00\xc2\x08\x00":
        return emit_free_byte_clear(b)
    # ---- 2026-08-20 classes, second round ----
    # mov eax,ecx; mov ecx,[eax+d]; mov edx,[ecx]; push 0; add eax,part; push eax;
    # call [edx+slot32]; ret
    if len(b) == 20 and b[0:2] == b"\x8b\xc1" and b[2] == 0x8b and b[3] == 0x48 \
            and b[5:9] == b"\x8b\x11\x6a\x00" and b[9:11] == b"\x83\xc0" \
            and b[12] == 0x50 and b[13:15] == b"\xff\x92" and b[19] == 0xc3:
        return emit_virt_forward_subobj(b, b[4], b[11], struct.unpack_from("<I", b, 15)[0])
    # mov eax,ecx; [xor ecx,ecx;] zero N consecutive dwords; ret [imm16]
    if b[0:2] == b"\x8b\xc1":
        reg = b[2:4] == b"\x33\xc9"      # xor ecx,ecx + `mov [eax+k],ecx` stores
        i, n, expect = (4 if reg else 2), 0, 0
        while i < len(b):
            if reg and expect == 0 and b[i:i + 2] == b"\x89\x08":
                i += 2
            elif reg and expect and b[i:i + 3] == b"\x89\x48" + bytes([expect]):
                i += 3
            elif not reg and expect == 0 and b[i:i + 6] == b"\xc7\x00\x00\x00\x00\x00":
                i += 6
            elif not reg and expect and b[i:i + 7] == b"\xc7\x40" + bytes([expect]) + b"\x00\x00\x00\x00":
                i += 7
            else:
                break
            n += 1; expect += 4
        if n >= 1 and i < len(b) and (b[i:] == b"\xc3" or (b[i] == 0xc2 and len(b) == i + 3)):
            return emit_zero_init_ret_this(b, n, 0 if b[i] == 0xc3
                                           else struct.unpack_from("<H", b, i + 1)[0] // 4)
    # mov edx,[ecx+d]; xor eax,eax; test edx,edx; sete al; ret
    if b[0] == 0x8b and b[1] in (0x11, 0x51, 0x91) and b.endswith(b"\x33\xc0\x85\xd2\x0f\x94\xc0\xc3"):
        d = 0 if b[1] == 0x11 else (b[2] if b[1] == 0x51 else struct.unpack_from("<I", b, 2)[0])
        i = 2 if b[1] == 0x11 else (3 if b[1] == 0x51 else 6)
        if len(b) == i + 8:
            return emit_is_null(b, d)
    # mov eax,[ecx+d]; add eax,imm8; ret
    if len(b) in (6, 7) and b[0] == 0x8b and b[1] in (0x01, 0x41):
        d = 0 if b[1] == 0x01 else b[2]
        i = 2 if b[1] == 0x01 else 3
        if b[i:i + 2] == b"\x83\xc0" and len(b) == i + 4 and b[i + 3] == 0xc3:
            return emit_member_add(b, d, b[i + 2])
    # mov eax,[esp+4]; add eax,imm8; ret
    if len(b) == 8 and b[0:4] == b"\x8b\x44\x24\x04" and b[4:6] == b"\x83\xc0" and b[7] == 0xc3:
        return emit_arg_add(b, b[6])
    # mov eax,[ecx+d1]; inc dword [eax+d2]; ret
    if len(b) == 7 and b[0] == 0x8b and b[1] == 0x41 and b[3] == 0xff and b[4] == 0x40 \
            and b[6] == 0xc3:
        return emit_subptr_inc(b, b[2], b[5])
    # xor eax,eax; mov [ecx+d1],eax; mov [ecx+d2],al; ret
    if len(b) == 9 and b[0:2] == b"\x33\xc0" and b[2] == 0x89 and b[3] == 0x41 \
            and b[5] == 0x88 and b[6] == 0x41 and b[8] == 0xc3:
        return emit_int_and_byte_clear(b, b[4], b[7])
    # test ecx,ecx; je +7; mov eax,[ecx]; push 1; call [eax+slot]; ret
    if len(b) == 12 and b[0:4] == b"\x85\xc9\x74\x07" and b[4:8] == b"\x8b\x01\x6a\x01" \
            and b[8] == 0xff and b[9] == 0x50 and b[11] == 0xc3:
        return emit_guarded_virt(b, b[10], 1)
    # mov ecx,[ecx]; mov eax,[esp+4]; mov [ecx],eax; ret 4
    if b == b"\x8b\x09\x8b\x44\x24\x04\x89\x08\xc2\x04\x00":
        return emit_subptr_setter0(b)
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
            if b[j:] == b"\xf7\xd8\x1b\xc0\x40\xc2\x08\x00":
                return emit_field_compare(b, d, False, d2)
    # a->f != b->f: mov eax,[esp+4]; mov ecx,[eax+d]; mov edx,[esp+8]; cmp ecx,[edx+d];
    #               sbb eax,eax; neg eax; ret 8
    if b[0:4] == b"\x8b\x44\x24\x04" and len(b) > 8 and b[4] == 0x8b and b[5] in (0x08, 0x48):
        d = 0 if b[5] == 0x08 else b[6]
        i = 6 if b[5] == 0x08 else 7
        if b[i:i + 4] == b"\x8b\x54\x24\x08" and b[i + 4] == 0x3b and b[i + 5] in (0x0a, 0x4a):
            d2 = 0 if b[i + 5] == 0x0a else b[i + 6]
            j = i + 6 if b[i + 5] == 0x0a else i + 7
            if b[j:] == b"\x1b\xc0\xf7\xd8\xc2\x08\x00":
                return emit_field_compare(b, d, True, d2)
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

