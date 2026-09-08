// Ego_r.pdb names DistPushed at +0x0c. Retail compares it with the
// shared push-distance threshold at 0x01383744.
extern float g_OnHeroPushAngryDistance;

struct CTCOnHeroPush {
    unsigned char _base_0x00[0x0c];
    float DistPushed;

    bool IsAngry() const;
};

bool CTCOnHeroPush::IsAngry() const {
    return DistPushed > g_OnHeroPushAngryDistance;
}