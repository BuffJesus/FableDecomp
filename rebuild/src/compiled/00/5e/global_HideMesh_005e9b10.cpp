// HideMesh @ 0x005e9b10
// mov eax,[ecx]; push 0; call [eax+0x70]; ret
// Virtual call: this->SetMeshVisible(false), vtable slot at byte 0x70 (index 28).
struct CMeshEntity {
    virtual void v00();  virtual void v01();  virtual void v02();  virtual void v03();
    virtual void v04();  virtual void v05();  virtual void v06();  virtual void v07();
    virtual void v08();  virtual void v09();  virtual void v10();  virtual void v11();
    virtual void v12();  virtual void v13();  virtual void v14();  virtual void v15();
    virtual void v16();  virtual void v17();  virtual void v18();  virtual void v19();
    virtual void v20();  virtual void v21();  virtual void v22();  virtual void v23();
    virtual void v24();  virtual void v25();  virtual void v26();  virtual void v27();
    virtual void SetVisible(int bVisible);   // slot 28 => byte 0x70
};

void __fastcall HideMesh(CMeshEntity* self)
{
    self->SetVisible(0);
}