#pragma once
// Shared four-slot native movable-resource interface, from Ego_r DIA and retail vtables.
class CMovableResource {
public:
    virtual ~CMovableResource() {}
    virtual void RelocateData(long, void*, void*, long) = 0;
    virtual bool CanRelocateData(long) const;
    virtual unsigned long Verify() const;
};
