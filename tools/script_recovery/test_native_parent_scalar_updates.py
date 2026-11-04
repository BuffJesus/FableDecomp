import importlib.util
from pathlib import Path
import pytest

from tools.script_recovery import native_parent_scalar_updates as candidate
SOURCE='''void Main(void) {
  uint *cursor;
  cursor = (uint *)(*(int *)((int)this + 0x14) + 0x5c);
  *cursor = *cursor | 4;
}
'''
BODY=bytes.fromhex('568bf18b461483485c045ec3')
FIELDS={'0x5c':['PrizesWon','Int']}

def recover(source=SOURCE,body=BODY,fields=FIELDS):
    class Data:
        def bytes_at(self,start,size):
            assert start==0x1000 and size==len(body)
            return body
    return candidate.recover({'address':'0x1000','bodyEndExclusive':hex(0x1000+len(body))},source,Data(),fields)

@pytest.mark.parametrize('newline',['\n','\r\n'])
def test_parent_or_recovers_verified_store(newline):
    output,rows=recover(SOURCE.replace('\n',newline))
    assert len(rows)==1 and rows[0]['field']=='PrizesWon'
    assert rows[0]['native']['site']=='0x1006'
    assert '*(uint *)(*(int *)(this + 0x14) + 0x5c) = ' in output
    assert '*cursor = ' not in output

@pytest.mark.parametrize('extra',['consume(cursor);','return cursor;','cursor = other;','*cursor = 8;'])
def test_escaping_alias_stays_unresolved(extra):
    source=SOURCE.replace('\n}', '\n  '+extra+'\n}')
    assert recover(source)==(source,[])

@pytest.mark.parametrize('body',[
    '568bf18b461883485c045ec3', # wrong parent offset
    '568bf18b4614834860045ec3', # wrong field offset
    '568bf18b461483485c025ec3', # wrong mask
    '568bf18b461483605c045ec3', # AND, not OR
    '568bf98b461483485c045fc3', # unproven receiver
    '568bf131f68b461483485c045ec3', # overwritten receiver
    '568bf18b46149083485c045ec3', # nonadjacent load/store
    '568bf18b461483485c048b461483485c045ec3', # ambiguous stores
])
def test_native_mismatch_stays_unresolved(body):
    assert recover(body=bytes.fromhex(body))==(SOURCE,[])

@pytest.mark.parametrize('fields',[{}, {'0x5c':['PrizesWon','Thing']}, {'0x60':['PrizesWon','Int']}])
def test_field_type_and_offset_required(fields):
    assert recover(fields=fields)==(SOURCE,[])

def test_duplicate_source_store_stays_unresolved():
    pair='\n'.join(SOURCE.splitlines()[2:4])+'\n'
    source=SOURCE.replace('\n}', '\n'+pair+'}')
    assert recover(source)==(source,[])

def test_absent_pattern_does_not_read_native_bytes():
    assert candidate.recover({},'return;',None,{})==('return;',[])
