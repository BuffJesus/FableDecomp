"""Check retail queries on complete retail-built trees with independent size bounds."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EAX,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
from oracle_variable_tree_insertion import collect as trees,BASE,WIDTH,OWNER

def collect():
    raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(0x2000000,0x20000);stack=0x2010000;stop=0x200f000
    empty=bytearray(b'\x5a'*WIDTH);struct.pack_into('<I',empty,0xc34,0)
    states=[bytes(empty)]+[bytes.fromhex(snapshot)for row in trees()for snapshot in row['snapshots']];rows=[]
    def invoke(address,parameters):
        uc.mem_write(stack,struct.pack('<'+str(1+len(parameters))+'I',stop,*(v&0xffffffff for v in parameters)))
        uc.reg_write(UC_X86_REG_ECX,OWNER);uc.reg_write(UC_X86_REG_ESP,stack);uc.emu_start(address,stop,count=10000)
        assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+4*(1+len(parameters))
        return uc.reg_read(UC_X86_REG_EAX)
    for state in states:
        uc.mem_write(BASE,state)
        def word(address):return struct.unpack_from('<I',state,address-BASE)[0]
        nodes=[];pending=[word(OWNER+0xc34)]
        while pending:
            node=pending.pop()
            if not node:continue
            nodes.append((node,struct.unpack_from('<i',state,node-BASE+0x18)[0]));pending.extend([word(node+4),word(node+0x10)])
        requests={-0x80000000,-0x7fffffff,-1000,-8,-1,0,1,7,32,128,1000,0x7fffffff}
        for node,size in nodes:
            requests.update(v for v in [size-1,size,size+1]if -0x80000000<=v<=0x7fffffff)
        results=[]
        for request in sorted(requests):
            result=invoke(0xa64c10,[request]);eligible=[size for node,size in nodes if size>=request]
            if eligible:assert (result,min(eligible))in nodes
            else:assert result==0
            results.append((request,result))
        largest=invoke(0xa64c50,[]);smallest=invoke(0xa64c70,[])
        assert largest==(max(size for node,size in nodes)if nodes else 0)&0xffffffff
        assert smallest==(min(size for node,size in nodes)if nodes else 0)&0xffffffff
        assert bytes(uc.mem_read(BASE,WIDTH))==state
        rows.append(dict(state=state.hex(),queries=results,largest=largest,smallest=smallest))
    return rows

if __name__=='__main__':
    rows=collect();print('VARIABLE_TREE_QUERIES_RETAIL states=%d calls=%d'%(len(rows),sum(len(r['queries'])+2 for r in rows)))
