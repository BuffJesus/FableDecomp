"""Replay actual sibling-tail copies to associate emitted health uses with native calls.

This supplies source correspondence, not native operation or liveness proof.
All inputs must come from the same captured lifter phase.
"""
import re
from collections import Counter
from tools.script_recovery.convert_quest_unit import _text_order_sites
from tools.script_recovery.native_evidence_lowering import normalise_typed_decompile,bind_st0_results,join_wrapped_statements
from tools.script_recovery.native_goto_scopes import duplicate_sibling_tails

def prove(fn,before,after,phase,*,health_slot=0x420,getter_target=0x7e7490,cleanup_target=0x4aa840,prior_health=None,getter_slot=None,inline_member_name=None,inline_cleanup_profiles=None,allow_zero_staging=False,allow_wrapped_call_spacing=False,prior_comparison=None):
    def require(ok,why):
        if not ok:raise ValueError('health copy correspondence: '+why)
    if prior_comparison is not None:
        require(prior_health is None,'prior comparison/hit composition unsupported')
        from tools.script_recovery.native_prior_register_health_comparisons import prove_copy_after
        options=dict(health_slot=health_slot,getter_target=getter_target,cleanup_target=cleanup_target,
            getter_slot=getter_slot,inline_member_name=inline_member_name,
            inline_cleanup_profiles=inline_cleanup_profiles,allow_zero_staging=allow_zero_staging,
            allow_wrapped_call_spacing=allow_wrapped_call_spacing)
        return prove_copy_after(fn,before,after,phase,prior_comparison,options)
    # Bind results while the full original call inventory still exists.
    # Normalization may rewrite unrelated calls (for example CRT rand).
    require(getter_slot is None or type(getter_slot)is int and 0<=getter_slot<=0x1000 and getter_slot%4==0,'getter slot profile')
    require(inline_member_name is None or re.fullmatch(r'[A-Za-z_]\w*',inline_member_name),'inline member profile')
    require(type(allow_zero_staging)is bool,'zero staging option')
    cleanup_profiles={} if inline_cleanup_profiles is None else {int(k,16) if isinstance(k,str) else k:v for k,v in inline_cleanup_profiles.items()}
    require(all(type(k)is int and v.get('kind')=='inline-Thing-release' and set(v)=={'kind','callback','delete','base'} for k,v in cleanup_profiles.items()),'inline cleanup profile schema')
    prior=None;consumed=set()
    if prior_health is not None:
        from tools.script_recovery.native_prior_hit_health import replay
        prior=replay(fn,phase,**prior_health);consumed=prior['sites']
    normalized=bind_st0_results(join_wrapped_statements(fn['decompile']))
    require(type(allow_wrapped_call_spacing)is bool,'wrapped call spacing option')
    spacing=None
    if allow_wrapped_call_spacing:
        from tools.script_recovery.native_wrapped_call_correspondence import prove as prove_spacing
        spacing=prove_spacing(fn,normalized)
    else:
        require(bind_st0_results(normalise_typed_decompile(normalized))==bind_st0_results(normalise_typed_decompile(fn['decompile'])), 'binding/normalization correspondence')
    ordered=_text_order_sites(normalized,fn);original=_text_order_sites(fn['decompile'],fn)
    require(ordered is not None and original is not None,'source call alignment')
    require([s[3]['site'] for s in ordered]==[s[3]['site'] for s in original],'source call order')
    native={}
    for index,(a,b,args,metadata,_,_) in enumerate(ordered):
        if metadata.get('slot')!=hex(health_slot):continue
        line=normalized[normalized.rfind('\n',0,a)+1:a]
        assignment=re.match(r'\s*(fret_\w+)\s*=',line)
        require(assignment is not None and 0<index<len(ordered)-1,'health result assignment')
        result=assignment[1]
        require(result not in native,'duplicate native result')
        getter,cleanup=ordered[index-1],ordered[index+1]
        if int(metadata['site'],16) not in consumed:
            require(getter[3].get('target','').lower()==f'0x{getter_target:08x}' if getter_slot is None else getter[3].get('slot')==hex(getter_slot),'producer/cleanup identities')
            profile=cleanup_profiles.get(int(metadata['site'],16))
            if profile is None:require(cleanup[3].get('target','').lower()==f'0x{cleanup_target:08x}','producer/cleanup identities')
            else:
                sequence=ordered[index+1:index+4];require(len(sequence)==3 and [int(s[3]['site'],16) for s in sequence]==[int(profile[k],16) for k in ['callback','delete','base']],'inline cleanup original source sequence')
                require(sequence[0][3].get('slot')=='0x4' and sequence[1][3].get('target','').lower()=='0x00bfe9bc' and sequence[2][3].get('target','').lower()=='0x0099a2e0','inline callback/delete/base source identities')
        require(len(args)==2 and re.fullmatch(r'[A-Za-z_]\w*',args[1]),'source actor')
        native[result]=dict(result=result,call=metadata,getter=getter[3],cleanup=cleanup[3],sourceArguments=args)
    require(bool(native),'native health inventory')
    require(set(cleanup_profiles)<={int(p['call']['site'],16) for p in native.values()},'unused inline cleanup profile')
    pattern=re.compile(r'^\s*(fret_\w+) = GSI->GetHealth\((\w+)\);$')
    self_pattern=re.compile(r'^\s*(fret_\w+) = GSI->GetHealth\((\(CScriptThing \*\)\(this \+ 8\))\);$')
    def uses(lines):
        found=[]
        for ordinal,line in enumerate(lines):
            match=pattern.fullmatch(line)
            if match is None and prior is not None:
                candidate=self_pattern.fullmatch(line)
                if candidate is not None and candidate[1] in native and int(native[candidate[1]]['call']['site'],16) in consumed:match=candidate
            if match:found.append(dict(line=ordinal,result=match[1],actor=match[2]))
            elif 'GetHealth' in line:require(False,'unsupported AST health expression')
        return found
    original_uses=uses(before);emitted=uses(after)
    require(Counter(u['result'] for u in original_uses)==Counter(native.keys()),'original AST/native inventory')
    self_mappings=[]
    for u in original_uses:
        original=native[u['result']];site=int(original['call']['site'],16)
        if u['actor']==original['sourceArguments'][1]:continue
        require(prior is not None and site in consumed and u['actor']=='(CScriptThing *)(this + 8)' and prior['roles'][site]==dict(result=u['result'],actor='me'),'original AST actor')
        self_mappings.append(dict(site=site,nativeSourceActor=original['sourceArguments'][1],astActor=u['actor'],luaActor='me',evidence='Independently replayed registered native self/query roles and exact prior Lua prefix'))
    trace=[];replayed=duplicate_sibling_tails(before,copy_trace=trace)
    require(replayed==after,'actual copy replay')
    expected=Counter(u['result'] for u in original_uses);copied_indices={}
    for entry in trace:
        copies=uses(entry['copiedStatements']);expected.update(u['result'] for u in copies)
        start=entry['outputStartIndex'];end=start+entry['outputStatementCount']
        for index,u in enumerate(emitted):
            if start<=u['line']<end:
                require(index not in copied_indices,'overlapping copied uses')
                copied_indices[index]=entry['target']
        require(Counter(u['result'] for u in emitted if start<=u['line']<end)==Counter(u['result'] for u in copies),'copy output inventory')
    require(Counter(u['result'] for u in emitted)==expected,'emitted copy inventory')
    prefix=re.compile(r'(?m)^(?P<i> *)(?P<output>\w+) = resources:ScriptThing\((?P<resource>\w+)\)\n(?P=i)(?P<actor>\w+) = (?P=output)\n(?P=i)(?P<result>\w+) = quest:GetHealth\((?P=actor)\)\n(?P=i)(?P<threshold>\w+) = 0\.0\n(?P=i)if (?P<condition>[^\n]+) then')
    if inline_member_name is not None:
        member=re.escape('resources:MemberResource("'+inline_member_name+'")')
        prefix=re.compile(r'(?m)^(?P<i> *)(?P<output>\w+) = resources:ScriptThing\((?P<resource>'+member+r')\)\n(?P=i)(?P<result>\w+) = quest:GetHealth\((?P<actor>\w+)\)\n(?P=i)(?P<threshold>\w+) = 0\.0\n(?P=i)if (?P<condition>[^\n]+) then')
    if allow_zero_staging:
        require(inline_member_name is not None and bool(cleanup_profiles),'zero staging needs explicit inline cleanup/member profiles')
        member=re.escape('resources:MemberResource("'+inline_member_name+'")')
        prefix=re.compile(r'(?m)^(?P<i> *)(?P<output>\w+) = resources:ScriptThing\((?P<resource>'+member+r')\)\n(?P=i)(?P<result>\w+) = quest:GetHealth\((?P<actor>\w+)\)\n(?P=i)(?P<threshold>\w+) = 0\.0\n(?P<staging>(?:(?P=i)(?P<stagingAlias>\w+) = nil\n(?P=i)(?P=stagingAlias) = 0\n)?)' + r'(?P=i)if (?P<condition>[^\n]+) then')
    matches=list(prefix.finditer(phase))
    all_native=len(native)
    if prior is not None:
        health=re.compile(r'(?m)^ *(fret_\w+) = quest:GetHealth\(([A-Za-z_]\w*)\)$')
        original_lua=list(health.finditer(prior['source']))
        require(len(original_lua)==prior['source'].count('quest:GetHealth(')==len(emitted),'prior full Lua health inventory')
        for m,u in zip(original_lua,emitted):
            site=int(native[u['result']]['call']['site'],16)
            if site in consumed:
                role=prior['roles'][site]
                require(m[1]==u['result']==role['result'] and m[2]==role['actor'],'prior proved native-self Lua consumer')
            else:require(m[1]==u['result'] and m[2]==u['actor'],'prior Lua/AST health consumer')
        removed=[u for u in emitted if int(native[u['result']]['call']['site'],16) in consumed]
        require(len(removed)==len(consumed)==1,'prior consumed emitted inventory')
        oldprefix=list(prefix.finditer(prior['source']))
        require([m[0] for m in oldprefix]==[m[0] for m in matches],'remaining resource prefixes preserved')
        emitted=[u for u in emitted if int(native[u['result']]['call']['site'],16) not in consumed]
        native={key:value for key,value in native.items() if int(value['call']['site'],16) not in consumed}
    require(len(matches)==phase.count('quest:GetHealth(')==len(emitted),'Lua prefix inventory')
    rows=[]
    for ordinal,(m,u) in enumerate(zip(matches,emitted)):
        require(m['result']==u['result'] and m['actor']==u['actor'],'Lua/AST consumer')
        require(inline_member_name is None or m['output']==m['actor'],'inline member output/actor')
        require(m['result'] in native,'unknown native result')
        require(m['condition'] in (m['threshold']+' < '+m['result'],m['result']+' <= '+m['threshold']),'Lua condition shape')
        require(m['resource'] not in {m['output'],m['actor'],m['result'],m['threshold']} and len({m['actor'],m['result'],m['threshold']})==3 and m['output'] not in {m['result'],m['threshold']},'Lua role overlap')
        if allow_zero_staging:
            staging=m['staging'];profile=cleanup_profiles.get(int(native[m['result']]['call']['site'],16))
            require(not staging or profile is not None and m['stagingAlias'] not in {m['output'],m['actor'],m['result'],m['threshold']},'zero staging scope/alias')
        rows.append(dict(start=m.start(),end=m.end(),native=native[m['result']],output=m['output'],actor=m['actor'],result=m['result'],resource=m['resource'],threshold=m['threshold'],condition=m['condition'],prefix=m[0],distinctThingAliases=sorted({m['output'],m['actor']}),copiedContinuation=copied_indices.get(ordinal)))
        if allow_zero_staging:rows[-1]['retainedStaging']=m['staging'];rows[-1]['inlineCleanupProfile']=cleanup_profiles.get(int(native[m['result']]['call']['site'],16))
    result=dict(scopes=rows,nativeSites=len(native),emittedUses=len(emitted),copyTrace=trace)
    if spacing is not None:result['wrappedCallSpacing']=spacing
    if prior is not None:result['priorHealth']=dict(originalNativeSites=all_native,consumedSites=sorted(consumed),proof=prior['proof'])
    if self_mappings:result['priorHealth']['checkedSelfExpressions']=self_mappings
    return result

