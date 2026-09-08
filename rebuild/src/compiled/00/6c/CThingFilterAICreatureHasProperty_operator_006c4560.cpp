struct CThingAICreature_PropertyView {
    virtual void ReservedVirtual0();
    virtual unsigned long GetProperty() const;
};

struct CThingFilter_AICreatureHasProperty {
    unsigned long Property;
    bool operator()(const CThingAICreature_PropertyView* creature) const;
};

bool CThingFilter_AICreatureHasProperty::operator()(
    const CThingAICreature_PropertyView* creature) const {
    return creature->GetProperty() == Property;
}