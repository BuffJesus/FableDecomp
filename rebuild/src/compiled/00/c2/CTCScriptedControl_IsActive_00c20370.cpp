struct CTCScriptedControl {
    char pad[0xc];
    char m_field0c;
};

extern bool __fastcall CTCScriptedControl_Compare(CTCScriptedControl* self, CTCScriptedControl* other);

int __fastcall CTCScriptedControl_IsActive(CTCScriptedControl* self, CTCScriptedControl* other)
{
    if (self->m_field0c == other->m_field0c) {
        if (CTCScriptedControl_Compare(self, other))
            return 1;
    }
    return 0;
}