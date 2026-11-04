"""Exact residual self-copy recovery and stale evidence rejection."""
from copy import deepcopy
import pytest
from tools.script_recovery.test_native_woman_message_dispatch import review_input
from tools.script_recovery.native_reviewed_stack_aliases import restore

def test_residual_self_copies(review_input):
    captured,row=review_input
    source=captured['source']
    assert source.count('pCVar9 = unaff_EBP;')==2
    assert source.count('unaff_EBP = pCVar9;')==1
    result=restore(source,captured['fn'],row,captured['rd'])
    assert 'pCVar9 = unaff_EBP;' not in result
    assert 'unaff_EBP = pCVar9;' not in result
    assert result.count('pCVar9 = womanSelfPointer;')==2

@pytest.mark.parametrize('fault',('missing_copy','extra_copy','missing_join','extra_join','copy_count','join_count','first_reload','second_reload','self_spill'))
def test_residual_self_evidence_fails_closed(review_input,fault):
    captured,row=review_input;row=deepcopy(row);source=captured['source']
    copy='pCVar9 = unaff_EBP;';join='unaff_EBP = pCVar9;'
    if fault=='missing_copy':source=source.replace(copy,'',1)
    elif fault=='extra_copy':source+='\n'+copy
    elif fault=='missing_join':source=source.replace(join,'',1)
    elif fault=='extra_join':source+='\n'+join
    elif fault in ('copy_count','join_count'):
        needle=copy if fault=='copy_count' else join
        index=next(i for i,(old,new) in enumerate(row['edits']) if old==needle)
        row['editCounts'][index]+=1
    else:
        site={'first_reload':0xecfbc7,'second_reload':0xed0699,'self_spill':0xecf814}[fault]
        key=next(k for k in row['nativeInstructions'] if int(k,16)==site)
        row['nativeInstructions'][key]='90'
    with pytest.raises(ValueError,match='reviewed stack alias'):
        restore(source,captured['fn'],row,captured['rd'])
