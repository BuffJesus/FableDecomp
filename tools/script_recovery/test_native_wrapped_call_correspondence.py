from pathlib import Path
import json
import pytest
from tools.script_recovery import native_wrapped_call_correspondence as SPACING
from tools.script_recovery import native_health_copy_correspondence as COPY
P=Path(__file__).resolve().parent/'fixtures/beardy_ordered_comparison'
def inputs():
 p=json.loads((P/'phase.json').read_text());return dict(fn=p['fn'],before=p['before'],after=p['after'],phase=(P/'phase.lua').read_text())
def test_original_default_rejection_retained():
 with pytest.raises(ValueError,match='binding/normalization correspondence'):COPY.prove(**inputs())
def test_original_full_twenty_four_use_inventory_with_explicit_spacing_certificate():
 d=inputs();r=COPY.prove(**d,allow_wrapped_call_spacing=True);p=r['wrappedCallSpacing'];assert r['nativeSites']==r['emittedUses']==24 and p['joinedSpacingChanges']==1 and p['originalSpacingChanges']==0 and p['allOriginalCallMetadataExact'] and p['literalsAndCommentsOpaque']
@pytest.mark.parametrize('value',[1,None,'true',[],{}])
def test_non_boolean_option_rejected(value):
 with pytest.raises(ValueError,match='wrapped call spacing option'):COPY.prove(**inputs(),allow_wrapped_call_spacing=value)
@pytest.mark.parametrize('text',['"literal ) ( text"',"'(' // comment ) (\n",'/* comment ) ( */',r'"escaped \" ) ("'])
def test_literals_and_comments_opaque(text):assert SPACING.canonical_call_spacing(text)==(text,0)
@pytest.mark.parametrize('left,right',[('f(x) (y)','f(x)(y)'),('f(x)\t(y)','f(x)(y)'),('f(x) \t (y)','f(x)(y)')])
def test_only_adjacent_parenthesis_horizontal_spacing_canonicalizes(left,right):assert SPACING.canonical_call_spacing(left)==(right,1)
@pytest.mark.parametrize('left,right',[('a + +b','a++b'),('a > > b','a>>b'),('"a) (b"','"a)(b"'),('/* ) ( */','/* )( */'),('f(x)\n(y)','f(x)(y)'),('f(x) (z)','f(x)(y)')])
def test_token_literal_comment_and_newline_changes_remain_distinct(left,right):assert SPACING.canonical_call_spacing(left)[0]!=SPACING.canonical_call_spacing(right)[0]
@pytest.mark.parametrize('text',['"unterminated',"'x",'/* incomplete', 'R"(raw)"','u8R"(raw)"','"line\nbreak"'])
def test_unsupported_lexical_forms_reject(text):
 with pytest.raises(ValueError):SPACING.canonical_call_spacing(text)
def test_supplied_joined_binding_must_match_original():
 d=inputs();raw=d['fn']['decompile'];n=SPACING.bind_st0_results(SPACING.join_wrapped_statements(raw))
 with pytest.raises(ValueError,match='joined binding input'):SPACING.prove(d['fn'],n+';')
