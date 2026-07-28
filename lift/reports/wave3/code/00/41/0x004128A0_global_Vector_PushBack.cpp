struct VectorOverlay {
    std::byte _pad0[0x4];
    CWideString* m_finish;      // +0x04
    CWideString* m_endOfStorage; // +0x08
};

static_assert(offsetof(VectorOverlay, m_finish) == 0x4);
static_assert(offsetof(VectorOverlay, m_endOfStorage) == 0x8);

void __thiscall Vector_PushBack(VectorOverlay* self, CWideString* value)
{
    CWideString* finish = self->m_finish;

    if (finish != self->m_endOfStorage) {
        if (finish != nullptr) {
            CWideString::CWideString(finish, value);
        }
        self->m_finish = reinterpret_cast<CWideString*>(
            reinterpret_cast<unsigned char*>(self->m_finish) + 4
        );
        return;
    }

    Std_Vector_Insert_Range_Copies(finish, value, &value, 1, 1);
}