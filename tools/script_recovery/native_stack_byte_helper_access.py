"""Derive supported helper access summaries from complete original instruction shapes.

The receiver and its CString control/data allocations must be valid, disjoint
typed objects. Buffer/control frees retain their explicit external ABI boundary.
"""
import hashlib
import struct
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from capstone import x86 as x


def summarize(image,caller,metadata):
    number=lambda value:str(value) if 0<=value<10 else hex(value)
    def require(ok,why):
        if not ok:raise ValueError('native helper access: '+why)
    md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
    proofs={}
    def decode(name):
        row=metadata[name];address=int(row['address'],16);size=row['size']
        raw=image.bytes_at(address,size)
        require(raw is not None and len(raw)==size,'missing '+name)
        instructions=list(md.disasm(raw,address))
        require(instructions and sum(i.size for i in instructions)==size,'incomplete '+name)
        proofs[name]=dict(address=hex(address),size=size,sha256=hashlib.sha256(raw).hexdigest())
        return address,instructions
    def shape(ins,expected,name):
        require([(i.mnemonic,i.op_str) for i in ins]==expected,'unsupported '+name+' instructions')
    wrapper,wi=decode('termination')
    getter,gi=decode('getter');predicate,pi=decode('predicate')
    parent=metadata['parentOffset'];state=metadata['stateOffset'];flag=metadata['flagOffset']
    slot=metadata['parentVtableSlot'];table=int(metadata['entityVtable'],16)
    require(slot>=0 and slot%4==0,'unaligned parent vtable slot')
    pointer=image.bytes_at(table+slot,4)
    require(pointer is not None and int.from_bytes(pointer,'little')==getter,'registered parent getter mismatch')
    shape(wi,[('mov','eax, dword ptr [ecx]'),('call','dword ptr [eax + '+number(slot)+']'),
        ('mov','ecx, eax'),('jmp',hex(predicate))],'termination wrapper')
    shape(gi,[('mov','eax, dword ptr [ecx + '+number(parent)+']'),('ret','')],'registered parent getter')
    shape(pi,[('mov','eax, dword ptr [ecx + '+number(state)+']'),('test','eax, eax'),
        ('jne',hex(predicate+10)),('xor','al, al'),('ret',''),
        ('mov','al, byte ptr [eax + '+number(flag)+']'),('ret','')],'termination predicate')
    constructor,ci=decode('constructor')
    require(len(ci)>=3,'short constructor')
    shape(ci[:3],[('push','esi'),('mov','esi, ecx'),('mov','dword ptr [esi], 0')],'constructor prefix')
    require(ci[-1].mnemonic=='ret' and ci[-1].op_str=='8','constructor argument ABI changed')
    destructor,di=decode('destructor');release,ri=decode('release')
    counter=int(metadata['instanceCounter'],16)
    # The counter is in virtual .data padding. Check its writable PE mapping;
    # do not reinterpret absent file bytes as an initialized native DWORD.
    header=struct.unpack_from('<I',image.data,0x3c)[0]
    sections=struct.unpack_from('<H',image.data,header+6)[0]
    optional=struct.unpack_from('<H',image.data,header+20)[0]
    counter_sections=[]
    for index in range(sections):
        at=header+24+optional+40*index
        virtual,rva,rawsize=struct.unpack_from('<III',image.data,at+8)
        flags=struct.unpack_from('<I',image.data,at+36)[0]
        if image.base+rva<=counter and counter+4<=image.base+rva+max(virtual,rawsize):
            counter_sections.append(flags)
    require(len(counter_sections)==1 and counter_sections[0]&0x80000000,
            'instance counter outside writable virtual image')
    shape(ci[3:4],[('inc','dword ptr ['+hex(counter)+']')],'constructor instance counter')
    shape(di,[('call',hex(release)),('dec','dword ptr ['+hex(counter)+']'),('ret','')],'destructor')
    free_text=int(metadata['freeText'],16);free_control=int(metadata['freeControl'],16)
    require(free_text!=free_control,'free boundary identity collision')
    # Supported packed CString control: data0,length4,flags8,tag12,refs13.
    shape(ri,[
        ('push','edi'),('mov','edi, ecx'),('mov','eax, dword ptr [edi]'),('test','eax, eax'),
        ('je',hex(release+85)),('mov','edx, dword ptr [eax + 0xd]'),('dec','edx'),
        ('mov','ecx, edx'),('test','ecx, ecx'),('mov','dword ptr [eax + 0xd], edx'),
        ('jg',hex(release+79)),('push','esi'),('mov','esi, dword ptr [edi]'),('test','esi, esi'),
        ('je',hex(release+78)),('mov','eax, dword ptr [esi]'),('test','eax, eax'),
        ('je',hex(release+50)),('push','eax'),('call',hex(free_text)),('add','esp, 4'),
        ('mov','dword ptr [esi], 0'),('mov','edx, dword ptr [esi + 8]'),
        ('and','edx, 0x80000000'),('push','esi'),('mov','dword ptr [esi + 4], 0'),
        ('mov','dword ptr [esi + 8], edx'),('call',hex(free_control)),('add','esp, 4'),
        ('pop','esi'),('mov','dword ptr [edi], 0'),('pop','edi'),('ret','')],'CString release')
    # Derive the caller's invariant script receiver, including epilogue restores.
    start=int(caller['address'],16);end=int(caller['bodyEndExclusive'],16)
    raw=image.bytes_at(start,end-start);ins=list(md.disasm(raw,start))
    require(ins and sum(i.size for i in ins)==len(raw),'incomplete caller')
    anchors=[(k,i) for k,i in enumerate(ins) if i.mnemonic=='mov' and i.op_str=='ebx, ecx']
    require(len(anchors)==1 and anchors[0][0]<12,'ambiguous caller receiver origin')
    anchor,_=anchors[0]
    require(all(not i.group(x.X86_GRP_JUMP) and not i.group(x.X86_GRP_CALL) and
        x.X86_REG_ECX not in i.regs_access()[1] for i in ins[:anchor]),'caller receiver clobbered before anchor')
    alias_regs={x.X86_REG_EBX,x.X86_REG_BX,x.X86_REG_BL,x.X86_REG_BH}
    for k,i in enumerate(ins):
        if k==anchor:continue
        if alias_regs.intersection(i.regs_access()[1]):
            require(i.mnemonic=='pop' and i.op_str=='ebx','caller receiver overwritten')
            shape(ins[k+1:k+3],[('add','esp, 8'),('ret','8')],'receiver restore epilogue')
    return dict(
        contracts={constructor:dict(kind='constructor-overwrite',width=4),
            destructor:dict(kind='receiver-object',receiverBytes=4,stackArguments=0,callerStackAccess='none'),
            wrapper:dict(kind='nonstack-receiver',stackArguments=0,callerStackAccess='none')},
        nonstackRegisters=[x.X86_REG_EBX],nativeProofs=proofs,
        receiverOrigin=dict(instruction=hex(ins[anchor].address),register='ebx',origin='entry thiscall ECX'),
        effects=dict(constructor='first full DWORD zero before any read/call/branch',
            destructor='four-byte CString receiver; separate packed control17/data heap objects; global instance counter',
            termination='read-only registered parent/state/thread chain; no caller stack arguments'),
        limits='Valid disjoint script/CString/heap objects required. CRT free bodies and allocator correctness remain external boundaries. No exception/unwind or gameplay claim.')
