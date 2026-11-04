"""PlayWave reward-object token lookup and native horizontal position rotation.

F20CF9..D05: vector<CDefString> length from globals +A6C/+A70, stride four.
F20D08/D3A draw object index then angle. F20D1F..D2F build (14,0,5).
F20D59/D5F multiply the angle by the float 1/360 and store a float argument.
F20D6E invokes A14040; F20DA2..DCE adds all three center components.
F20E07 converts the selected CDefString; F20E2B creates an object with an
empty script name. The native helper, caller and lowered source are gated.
"""
import hashlib

PLAY_WAVE = 0x00F1EED0
NATIVE_SHA256 = 'e6c92c85661ca4ea5e0e31459825a85c68ef90a21fb10542cf6ebc7347ef72bc'
ROTATE = 0x00A14040
ROTATE_SIZE = 0x18b
ROTATE_SHA256 = '6c019921b6785843ded10f21808d9f392920a1f5aaae629646fdde6ecc6a6f0b'


def recover_arena_reward(source, fn, rdata):
    if int(fn['address'], 16) != PLAY_WAVE:
        return source
    for start, size, digest in [(PLAY_WAVE, int(fn['bodyEndExclusive'], 16) - PLAY_WAVE, NATIVE_SHA256),
                                 (ROTATE, ROTATE_SIZE, ROTATE_SHA256)]:
        raw = rdata.bytes_at(start, size)
        if raw is None or hashlib.sha256(raw).hexdigest() != digest:
            return source
    newline = '\r\n' if '\r\n' in source else '\n'
    text = source.replace('\r\n', '\n')
    edits = [
        ('if (*(int *)(this + 0xf4) / 100 < 6) {', 'if (*(int *)(this + 0xf4) < 600) {'),
        ('if (0 < *(int *)(this + 0xf4) / 100) goto LAB_00f20ce5;',
         'if (100 <= *(int *)(this + 0xf4)) goto LAB_00f20ce5;'),
        ('        uVar11 = ENGINE_Rand();\n        x_stk_cc = QUESTTHING_Empty();\n',
         '        uVar11 = ENGINE_Rand();\n        rewardIndex = uVar11 % ((iVar4 - iVar6) >> 2);\n'
         '        x_stk_cc = QUESTTHING_Empty();\n'),
        ('        xStack_f4 = 14.0;\n        f_stk_f0 = 0.0;\n        f_stk_ec = 5.0;\n', ''),
        ('        fStack_d8 = (float)(iVar12 % 0x168) * 0.0027777778450399637;\n'
         '        Math_CartesianToSpherical(xStack_f4,(int)fStack_d8);\n',
         '        rewardTurns = (iVar12 % 0x168) * 0.0027777778450399637;\n'
         '        rewardOffset = ENGINE_RotateVectorHorizontal(ENGINE_Vector3(14.0, 0.0, 5.0), rewardTurns);\n'),
        ('        xStack_e4._4_4_ = f_stk_f0 + pCVar13.y;\n'
         '        xStack_e4._0_4_ = (float)xStack_f4 + pCVar13.x;\n',
         '        rewardPosition = ENGINE_Vector3(rewardOffset.x + pCVar13.x, rewardOffset.y + pCVar13.y, rewardOffset.z + pCVar13.z);\n'),
        ('CDefString::operator_class_CCharString((CDefString *)(ENGINE_GlobalGameData(0xa6c) + (uVar11 % (uint)(iVar4 - iVar6 >> 2)) * 4),(int)&xStack_108);',
         'rewardDefinition = ENGINE_GlobalGameDataDefStringAt(0xa6c, rewardIndex);'),
        ('GSI->CreateObject(&xStack_68,&xStack_108,(C3DVector *)xStack_e4,&xStack_f8);',
         'GSI->CreateObject(&xStack_68,rewardDefinition,rewardPosition,&xStack_f8);'),
    ]
    if any(text.count(old) != 1 for old, _ in edits):
        return source
    if any(name in text for name in ('rewardIndex', 'rewardTurns', 'rewardOffset', 'rewardPosition', 'rewardDefinition')):
        return source
    for old, new in edits:
        text = text.replace(old, new)
    return text.replace('\n', newline)
