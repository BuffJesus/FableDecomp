"""Interruption checkpoints in composed Lua; forced saves are serializer doubles."""
import copy

import pytest

from tools.script_recovery.test_book_reaction_lowering import generated
from tools.script_recovery.test_book_collection_sequence import Collection,GOOD_ORDER,EVIL_ORDER


class InterruptedCollection(Collection):
    def __init__(self,*args,**kwargs):
        self.fault=None
        super().__init__(*args,**kwargs)

    def macro(self, *args):
        super().macro(*args)
        name=args[1]
        reward='TEACHER_GIVES' in name
        if self.fault==('award' if reward else 'classroom'):
            self.stopped=True

    def frame(self,*args):
        if self.fault=='question' and self.pending_answers and any(e[0]=='question' for e in self.events):
            self.stopped=True
        return super().frame(*args)

    def persist_interrupted(self):
        # Deliberately bypass Collection.checkpoint's ReadingBook guard to inspect
        # OnPersist's schema. This is NOT evidence a real in-game save is allowed.
        assert not self.live
        self.mode,self.storage='save',{}
        self.root.globals().OnPersist(self.rq,'context')
        assert 'ReadingBook' not in self.storage
        return copy.deepcopy(dict(storage=self.storage,inventory=sorted(self.inventory)))


@pytest.mark.parametrize('kind,order,threshold',[
    ('GOOD',GOOD_ORDER,9),('EVIL',EVIL_ORDER,9),('KEY',GOOD_ORDER,25)])
@pytest.mark.parametrize('fault',['classroom','award'])
def test_macro_interruption_preserves_native_progress_and_cleanup(generated,kind,order,threshold,fault):
    _,roots,teachers=generated
    for root,teacher in zip(roots,teachers):
        h=InterruptedCollection(root,teacher);h.interact()
        for index in order[:threshold-1]:
            h.inventory.add(f'BOOK_{index}');h.interact()
        index=order[threshold-1]
        h.inventory.add(f'BOOK_{index}');h.events.clear();h.fault=fault
        assert h.resume()=='movie_closed'
        assert not h.live and not h.paused
        assert h.state['BooksDonated']==threshold and not h.inventory
        assert h.flags['BookDonated'][index]
        early=fault=='classroom'
        assert h.state['ReadingBook']==early
        # The early native exit does not call FixMovieSequenceCamera(false).
        # Movie destruction may reset engine camera state; this only records calls.
        assert h.camera==early
        assert len([e for e in h.events if e[0]=='award_request'])==(not early)
        assert len([e for e in h.events if e[0]=='thread'])==(not early)
        if kind=='KEY':assert h.state['HatRewarded'] and h.state['KeyRewarded']==(not early)
        else:assert h.state['HatRewarded']==(not early) and not h.state['KeyRewarded']
        saved=h.persist_interrupted()
        restored=Collection(root,teacher,saved)
        assert restored.flags==h.flags and not restored.state['ReadingBook']
        assert all(restored.state[k]==v for k,v in h.state.items() if k!='ReadingBook')
        if threshold==25:
            restored.interact()
            assert ('complete','V_BookCollecting',False,False,False) in restored.events
            assert not any(e[0]=='award_request' for e in restored.events)
            assert restored.state['KeyRewarded']==(not early)
        else:
            next_index=order[threshold]
            restored.inventory.add(f'BOOK_{next_index}');restored.interact()
            awards=[e for e in restored.events if e[0]=='award_request']
            assert awards==([('award_request',threshold+1,f'OBJECT_HERO_HAT_WIZARD_{kind}')] if early else [])


@pytest.mark.parametrize('index',[3,16])
def test_interrupted_unanswered_question_changes_no_donation_state(generated,index):
    _,roots,teachers=generated
    for root,teacher in zip(roots,teachers):
        h=InterruptedCollection(root,teacher);h.interact()
        h.events.clear();h.inventory.add(f'BOOK_{index}');h.pending_answers=2;h.fault='question'
        assert h.resume()=='movie_closed'
        assert h.state['BooksDonated']==h.state['GoodBooksDonated']==0
        assert not any(h.flags['BookDonated']) and h.flags['BookOwned'][index]
        assert h.inventory=={f'BOOK_{index}'} and h.state['LastBookRequested']==-1
        assert not h.live and not h.paused and not h.camera and not h.state['ReadingBook']
        assert not any(e[0] in ('take','macro','award_request','thread') for e in h.events)
        restored=Collection(root,teacher,h.persist_interrupted());restored.interact()
        assert restored.state['BooksDonated']==1 and not restored.inventory
