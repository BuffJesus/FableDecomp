"""Verify reset identity and actual linked provider; reject allocator mislabels."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE,pe_sections,va_to_off
ROOT=Path(__file__).resolve().parents[2]

def main():
    work=Path(tempfile.mkdtemp(prefix='inventory-trade-reset-'));env=check_one.env();env['INCLUDE']=str(ROOT/'rebuild/include')+';'+env['INCLUDE']
    source=ROOT/'rebuild/src/compiled/00/59/CTCInventoryTrade_ResetInventoryTradeItems_0059d9ad.cpp';obj=work/'provider.obj'
    result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work);assert result.returncode==0,result.stdout+result.stderr
    coff=Coff(obj);section,offset=coff.symbols['?ResetInventoryTradeItems@CTCInventoryTrade@@QAEXXZ'];body,relocs=coff.sections[section];patched=bytearray(body)
    assert len(relocs)==2
    for position,(symbol,kind)in relocs.items():
        assert symbol=='?RemoveAllItemsFromCategory@CTCInventoryTrade@@QAEXW4EInventoryCategoryIdentifier@NInventory@@@Z'and kind==20,(symbol,kind)
        struct.pack_into('<I',patched,position,(0x59cf49+struct.unpack_from('<i',body,position)[0]-0x59d9ad-position-4)&0xffffffff)
    raw=EXE.read_bytes();at=va_to_off(pe_sections(raw),0x59d9ad);assert len(body)==21 and bytes(patched)==raw[at:at+21]
    donor=(ROOT/'debug_build/ego_r.exe').read_bytes();at=va_to_off(pe_sections(donor),0x60c1a9);donor_body=bytearray(donor[at:at+21]);retail_body=bytearray(patched)
    for position in relocs:donor_body[position:position+4]=bytes(4);retail_body[position:position+4]=bytes(4)
    assert donor_body==retail_body,'donor reset body differs'
    test=ROOT/'rebuild/tests/00/59/CTCInventoryTrade_ResetInventoryTradeItems_0059d9ad_test.cpp';exe=work/'fixture.exe'
    result=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(exe),str(test)],env,work);assert result.returncode!=0,'omitted provider linked'
    result=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(exe),str(test),str(obj)],env,work);assert result.returncode==0,result.stdout+result.stderr
    result=subprocess.run([str(exe)],capture_output=True,text=True,timeout=30);assert result.returncode==0 and 'RESET_INVENTORY_TRADE_OK cases=4'in result.stdout,(result.returncode,result.stdout,result.stderr)
    (ROOT/'rebuild/compile-gate/inventory-trade-reset.json').write_text(json.dumps(dict(address='0059d9ad',grade='RELOCATION_MATCH',bytes=21,linked_cases=4,omitted_provider_checks=1,donor_identity='Ego_r 0060c1a9 CTCInventoryTrade::ResetInventoryTradeItems',mocked_boundaries='RemoveAllItemsFromCategory recording boundary; genuine linked reset provider'),indent=2)+'\n',encoding='utf-8')
    print('INVENTORY_TRADE_RESET_PASS cases=4 bytes=21 omitted=1')

if __name__=='__main__':main()
