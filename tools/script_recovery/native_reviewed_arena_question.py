"""Restore the original Arena end-round question argument order after lifting.

At F2104A..F21071 pending yes/no/empty/true arguments cross a helper call.
GetEndRoundQuestion consumes its hidden CString result slot and returns it in
EAX; GSI+1C8 receives that question first. The lifted call loses this operand
and shifts the rest. Original caller/helper bytes and exact lifted call gate
this recovery. The returned text is evaluated once before the UI call.
"""
import hashlib

PLAY_WAVE=0x00F1EED0
CALLER_SHA256='e6c92c85661ca4ea5e0e31459825a85c68ef90a21fb10542cf6ebc7347ef72bc'
HELPER_SHA256='b27e9860e22f1341a249fd14f967fc08ca3e0c0fa6803592397155dc56fa9ad1'
OLD='quest:GiveHeroYesNoQuestion("TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_YES", "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_NO", "", pvVar8, (pQuestionText ~= 0))'
NEW='quest:GiveHeroYesNoQuestion(pQuestionText, "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_YES", "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_NO", "", true)'

def recover_arena_question(source,fn,rdata):
    if int(fn['address'],16)!=PLAY_WAVE:return source
    for start,size,digest in ((PLAY_WAVE,int(fn['bodyEndExclusive'],16)-PLAY_WAVE,CALLER_SHA256),(0xF21590,0xAB,HELPER_SHA256)):
        raw=rdata.bytes_at(start,size)
        if raw is None or hashlib.sha256(raw).hexdigest()!=digest:return source
    for address,text in ((0x12FA5D8,'TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_YES'),(0x12FA608,'TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_NO')):
        if rdata.string_at(address)!=text:return source
    if rdata.bytes_at(0x122D70E,1)!=b'\0':return source
    if source.count(OLD)!=1 or source.count('pQuestionText = GetEndRoundQuestion(quest)')!=1:return source
    if sum(line.strip()==OLD for line in source.splitlines())!=1:return source
    return source.replace(OLD,NEW,1)
