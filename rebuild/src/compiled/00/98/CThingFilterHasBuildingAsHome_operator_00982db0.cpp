struct CThing;
struct CThingAICreature_HomeView {
    CThing* GetHomeBuilding() const;
};

struct CThingFilter_HasBuildingAsHome {
    CThing* HomeBuilding;
    bool operator()(const CThingAICreature_HomeView* creature) const;
};

bool CThingFilter_HasBuildingAsHome::operator()(
    const CThingAICreature_HomeView* creature) const {
    return creature->GetHomeBuilding() == HomeBuilding;
}