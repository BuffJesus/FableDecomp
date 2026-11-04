"""Recover PlayWave's unsigned backward spawn index and null-marker string.

F1F356 LEA EAX,[EBX-1] and F1FE6F DEC EAX wrap at 32 bits. The
following CMP EAX,-1 selects count-1. F1F315/F1F321 copies the established
empty CCharString global when the furthest marker is absent. Require the
reviewed full native body and every exported source occurrence.
"""
import hashlib
START=0xF1EED0
SHA='e6c92c85661ca4ea5e0e31459825a85c68ef90a21fb10542cf6ebc7347ef72bc'
def recover_arena_spawn_selection(source,fn,rdata):
 if int(fn['address'],16)!=START:return source
 raw=rdata.bytes_at(START,int(fn['bodyEndExclusive'],16)-START)
 if not raw or hashlib.sha256(raw).hexdigest()!=SHA:return source
 edits=[
 ('x_stk_104 = (int *)(iVar4 + -1);','x_stk_104 = (int *)((iVar4 + -1) & 0xffffffff);',1),
 ('x_stk_104 = (int *)((int)x_stk_104 + -1);','x_stk_104 = (int *)(((int)x_stk_104 + -1) & 0xffffffff);',1),
 ('x_stk_104 = (int *)(iVar6 + -1);','x_stk_104 = (int *)((iVar6 + -1) & 0xffffffff);',2),
 ('CCharString::CCharString(xStack_a0,(CCharString *)&DAT_0143e8ec);','xStack_a0 = ENGINE_EmptyString();',1),
 ]
 if any(source.count(old)!=count for old,new,count in edits):return source
 for old,new,count in edits:source=source.replace(old,new)
 return source
