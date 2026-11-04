"""Replay a prior comparison review before interpreting its repaired predicate.

The original AST remains unchanged. Only independently replayed comparison
edits are projected into current Lua offsets; no general inverse-pattern
acceptance or native/lifetime inference is supplied here.
"""
import hashlib


def replay(fn, phase, *, input, todo, calls, unit, owner, image, manifest, before, after):
    def require(ok, why):
        if not ok:
            raise ValueError('prior register health comparison: '+why)

    require(isinstance(input,dict) and set(input)=={'source','todo','calls'},'input schema')
    row=unit.get('reviewedRegisterHealthComparisons',{}).get(fn['address'].lower())
    require(row is not None,'missing review')
    require(input['todo']==todo and input['calls']==calls,'comparison changed non-source metadata')
    from tools.script_recovery.native_reviewed_register_health_comparisons import recover
    output,new_todo,new_calls,reviews=recover(input['source'],input['todo'],input['calls'],fn,row,
        unit,owner,image,manifest,before,after)
    require(output==phase and new_todo==todo and new_calls==calls and reviews,'replay output')
    require(all(r['native']['relation']=='ordered-greater' and r['native']['threshold']==0
        and r['native']['unordered'] is False for r in reviews),'native relation')
    return dict(source=input['source'],reviews=reviews,
        inputSha256=hashlib.sha256(input['source'].encode()).hexdigest(),
        outputSha256=hashlib.sha256(phase.encode()).hexdigest())


def prove_copy_after(fn,before,after,phase,context,options):
    def require(ok, why):
        if not ok:
            raise ValueError('prior register health comparison copy: '+why)

    require(isinstance(context,dict),'context schema')
    prior=replay(fn,phase,before=before,after=after,**context)
    from tools.script_recovery.native_health_copy_correspondence import prove
    proof=prove(fn,before,after,prior['source'],**options)
    reviews={int(r['site'],16):r for r in prior['reviews']}
    require(len(reviews)==len(prior['reviews']),'duplicate reviewed site')
    edits=[]
    for scope in proof['scopes']:
        site=int(scope['native']['call']['site'],16)
        if site not in reviews:
            continue
        review=reviews[site]
        require(scope['condition']==review['originalCondition'],'original condition')
        require(scope['condition']==scope['result']+' <= '+scope['threshold'],'original inverse shape')
        require(review['replacement']=='not ('+scope['result']+' > '+scope['threshold']+')','replacement shape')
        a=scope['end']-len(' then')-len(scope['condition']);b=a+len(scope['condition'])
        require(prior['source'][a:b]==scope['condition'],'original condition slice')
        edits.append((a,b,review['replacement']))
    require(len(edits)==len(reviews),'review/use inventory')
    edits.sort()
    require(all(a[1]<=b[0] for a,b in zip(edits,edits[1:])),'overlapping edits')
    expected=prior['source']
    for a,b,value in reversed(edits):expected=expected[:a]+value+expected[b:]
    require(expected==phase,'changes outside replayed comparisons')

    def project(position):
        require(all(not a<position<b for a,b,value in edits),'offset inside comparison')
        return position+sum(len(value)-(b-a) for a,b,value in edits if b<=position)

    mapped=[]
    for scope in proof['scopes']:
        new=dict(scope);review=reviews.get(int(scope['native']['call']['site'],16))
        new['start'],new['end']=project(scope['start']),project(scope['end'])
        expected_prefix=scope['prefix']
        if review is not None:
            suffix='if '+scope['condition']+' then'
            require(expected_prefix.endswith(suffix),'original prefix suffix')
            expected_prefix=expected_prefix[:-len(suffix)]+'if '+review['replacement']+' then'
            new['condition']=review['replacement']
            new['priorComparisonReview']=review
        new['prefix']=phase[new['start']:new['end']]
        require(new['prefix']==expected_prefix,'mapped Lua prefix')
        mapped.append(new)
    proof=dict(proof,scopes=mapped,priorRegisterComparison={k:v for k,v in prior.items() if k!='source'})
    return proof
