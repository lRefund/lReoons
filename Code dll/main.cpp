#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdint>
#include <cstring>
#include <thread>
#include <atomic>
#include <sstream>
#include <iomanip>
#include "offsets.h"
using namespace Offsets;
typedef void* (*il2cpp_domain_get_t)();
typedef void** (*il2cpp_domain_get_assemblies_t)(void*, size_t*);
typedef void* (*il2cpp_assembly_get_image_t)(void*);
typedef const char* (*il2cpp_image_get_name_t)(void*);
typedef void* (*il2cpp_class_from_name_t)(void*, const char*, const char*);
typedef void* (*il2cpp_class_get_field_from_name_t)(void*, const char*);
typedef void (*il2cpp_field_static_get_value_t)(void*, void*);
typedef void (*il2cpp_field_static_set_value_t)(void*, void*);
typedef void* (*il2cpp_thread_attach_t)(void*);
namespace Il2Cpp {
inline uintptr_t Base = 0;
inline il2cpp_domain_get_t domain_get = nullptr;
inline il2cpp_domain_get_assemblies_t domain_get_assemblies = nullptr;
inline il2cpp_assembly_get_image_t assembly_get_image = nullptr;
inline il2cpp_image_get_name_t image_get_name = nullptr;
inline il2cpp_class_from_name_t class_from_name = nullptr;
inline il2cpp_class_get_field_from_name_t class_get_field = nullptr;
inline il2cpp_field_static_get_value_t field_get = nullptr;
inline il2cpp_field_static_set_value_t field_set = nullptr;
inline il2cpp_thread_attach_t thread_attach = nullptr;
inline void* GameImage = nullptr;
template<typename T> T Read(uintptr_t a) { __try { return *(T*)a; } __except(1) { return T{}; } }
template<typename T> void Write(uintptr_t a, T v) { __try { *(T*)a = v; } __except(1) {} }
inline bool Valid(uintptr_t p) { __try { volatile char c = *(char*)p; (void)c; return p != 0; } __except(1) { return false; } }
bool Init() {
    HMODULE h = GetModuleHandleA("GameAssembly.dll");
    if (!h) return false;
    Base = (uintptr_t)h;
#define R(n) n = (il2cpp_##n##_t)GetProcAddress(h, "il2cpp_" #n); if(!n) return false
    R(domain_get); R(domain_get_assemblies); R(assembly_get_image); R(image_get_name);
    R(class_from_name); R(thread_attach);
#undef R
    class_get_field = (il2cpp_class_get_field_from_name_t)GetProcAddress(h, "il2cpp_class_get_field_from_name");
    field_get = (il2cpp_field_static_get_value_t)GetProcAddress(h, "il2cpp_field_static_get_value");
    field_set = (il2cpp_field_static_set_value_t)GetProcAddress(h, "il2cpp_field_static_set_value");
    if (!class_get_field || !field_get || !field_set) return false;
    auto dom = domain_get(); if (!dom) return false;
    thread_attach(dom);
    size_t cnt = 0;
    auto asms = domain_get_assemblies(dom, &cnt);
    if (!asms) return false;
    for (size_t i = 0; i < cnt; i++) {
        auto img = assembly_get_image(asms[i]); if (!img) continue;
        auto nm = image_get_name(img);
        if (nm && strcmp(nm, "Assembly-CSharp.dll") == 0) { GameImage = img; break; }
    }
    return GameImage != nullptr;
}
void* FindClass(const char* ns, const char* name) { return GameImage ? class_from_name(GameImage, ns, name) : nullptr; }
template<typename T> T GetStatic(void* cls, const char* f) {
    if (!cls) return T{}; auto fld = class_get_field(cls, f); if (!fld) return T{};
    T v{}; field_get(fld, &v); return v;
}
template<typename T> void SetStatic(void* cls, const char* f, T v) {
    if (!cls) return; auto fld = class_get_field(cls, f); if (!fld) return;
    field_set(fld, &v);
}
int ListCount(uintptr_t l) { if (!Valid(l)) return 0; return Read<int>(l + 0x18); }
uintptr_t ListItem(uintptr_t l, int i) {
    if (!Valid(l)) return 0; uintptr_t items = Read<uintptr_t>(l + 0x10);
    if (!Valid(items)) return 0; if (i < 0 || i >= Read<int>(l + 0x18)) return 0;
    return Read<uintptr_t>(items + 0x20 + (uintptr_t)i * 8);
}
}
using namespace Il2Cpp;
static void* g_inGameClass = nullptr;
struct State {
    bool infiniteCash=0,infiniteHealth=0,instantCooldowns=0,pauseWeapons=0;
    bool debugOptions=0,sandbox=0,autoPlay=0,blockAutoPlay=0;
    bool freeTowers=0,lockHeroPurchases=0,lockTowerPurchases=0,autoAbility=0;
    bool freezeSpawner=0,infiniteRange=0,godMode=0,doubleCash=0;
    bool noLivesLost=0,infinitePierce=0,maxDamage=0,fastForward=0;
};
static State& S() { static State s; return s; }
static bool GameInit() {
    if (!Il2Cpp::Init()) return false;
    g_inGameClass = Il2Cpp::FindClass("", "InGame");
    if (!g_inGameClass) g_inGameClass = Il2Cpp::FindClass("Assets.Scripts.Unity.UI_New.InGame", "InGame");
    return g_inGameClass != nullptr;
}
static uintptr_t GetInGame() { return g_inGameClass ? (uintptr_t)GetStatic<void*>(g_inGameClass, "instance") : 0; }
static uintptr_t GetBridge() { uintptr_t ig = GetInGame(); return (ig && Valid(ig)) ? Read<uintptr_t>(ig + InGame::Bridge) : 0; }
static uintptr_t GetSim() { uintptr_t br = GetBridge(); return (br && Valid(br)) ? Read<uintptr_t>(br + UnityToSim::SimPtr) : 0; }
static uintptr_t GetTowerMgr() { uintptr_t s = GetSim(); return (s && Valid(s)) ? Read<uintptr_t>(s + Simulation::TowerManager) : 0; }
static uintptr_t GetBloonMgr() { uintptr_t s = GetSim(); return (s && Valid(s)) ? Read<uintptr_t>(s + Simulation::BloonManager) : 0; }
static uintptr_t GetMap() { uintptr_t s = GetSim(); return (s && Valid(s)) ? Read<uintptr_t>(s + Simulation::MapPtr) : 0; }
static uintptr_t GetSpawner() { uintptr_t m = GetMap(); return (m && Valid(m)) ? Read<uintptr_t>(m + MapObj::Spawner) : 0; }
template<typename F> static void ForTower(F fn) {
    uintptr_t tm = GetTowerMgr(); if (!tm) return;
    uintptr_t l = Read<uintptr_t>(tm + TowerMgr::Towers);
    int c = ListCount(l);
    for (int i = 0; i < c; i++) { uintptr_t t = ListItem(l, i); if (t && Valid(t)) fn(t); }
}
template<typename F> static void ForBloon(F fn) {
    uintptr_t bm = GetBloonMgr(); if (!bm) return;
    uintptr_t l = Read<uintptr_t>(bm + BloonMgr::Bloons);
    int c = ListCount(l);
    for (int i = 0; i < c; i++) { uintptr_t b = ListItem(l, i); if (b && Valid(b)) fn(b); }
}
template<typename F> static void ForAbility(uintptr_t tower, F fn) {
    uintptr_t l = Read<uintptr_t>(tower + Tower::Abilities);
    int c = ListCount(l);
    for (int i = 0; i < c; i++) { uintptr_t a = ListItem(l, i); if (a && Valid(a)) fn(a); }
}
template<typename F> static void ForWeapon(uintptr_t tower, F fn) {
    uintptr_t l = Read<uintptr_t>(tower + Tower::Weapons);
    int c = ListCount(l);
    for (int i = 0; i < c; i++) { uintptr_t w = ListItem(l, i); if (w && Valid(w)) fn(w); }
}
template<typename F> static void ForAllAbilities(F fn) { ForTower([&fn](uintptr_t t) { ForAbility(t, fn); }); }
template<typename F> static void ForAllWeapons(F fn) { ForTower([&fn](uintptr_t t) { ForWeapon(t, fn); }); }
static int TowerCount() { uintptr_t tm=GetTowerMgr(); return tm ? ListCount(Read<uintptr_t>(tm+TowerMgr::Towers)) : 0; }
static int BloonCount() { uintptr_t bm=GetBloonMgr(); return bm ? ListCount(Read<uintptr_t>(bm+BloonMgr::Bloons)) : 0; }
static uintptr_t TowerAt(int i) { uintptr_t tm=GetTowerMgr(); return tm ? ListItem(Read<uintptr_t>(tm+TowerMgr::Towers),i) : 0; }
static uintptr_t BloonAt(int i) { uintptr_t bm=GetBloonMgr(); return bm ? ListItem(Read<uintptr_t>(bm+BloonMgr::Bloons),i) : 0; }
static void Mk_SellAll() { uintptr_t br=GetBridge(); if(br) ((void(*)(void*,void*))(Base+RVA::BridgeR::SellAllTowers))((void*)br,nullptr); }
static void Mk_SellAllProps() { uintptr_t br=GetBridge(); if(br) ((void(*)(void*,void*))(Base+RVA::BridgeR::SellAllProps))((void*)br,nullptr); }
static void Mk_ResetAbilityCDs() { uintptr_t br=GetBridge(); if(br) ((void(*)(void*,void*))(Base+RVA::BridgeR::ResetAbilityCooldowns))((void*)br,nullptr); }
static void Mk_ClearAllAbilityCDs() { ForAllAbilities([](uintptr_t a) { Write<float>(a+Ability::Cooldown,0); }); }
static void Mk_ActivateAllAbilities() { ForAllAbilities([](uintptr_t a) { ((void(*)(void*,void*))(Base+RVA::AbilityR::Activate))((void*)a,nullptr); }); }
static void Mk_DeactivateAllAbilities() { ForAllAbilities([](uintptr_t a) { ((void(*)(void*,void*))(Base+RVA::AbilityR::Deactivate))((void*)a,nullptr); }); }
static void Mk_PauseWeapons() { auto& s=S(); s.pauseWeapons=!s.pauseWeapons; uintptr_t sim=GetSim(); if(sim) Write<bool>(sim+Simulation::PauseWeapons,s.pauseWeapons); }
static void Mk_InstantCooldowns() { auto& s=S(); s.instantCooldowns=!s.instantCooldowns; uintptr_t sim=GetSim(); if(sim) Write<bool>(sim+Simulation::InstantCooldowns,s.instantCooldowns); }
static void Mk_ToggleWeapons() { uintptr_t br=GetBridge(); if(br) ((void(*)(void*,void*))(Base+RVA::BridgeR::ToggleWeapons))((void*)br,nullptr); }
static void Mk_SetAllWeaponRate(float r) { ForAllWeapons([r](uintptr_t w) { Write<float>(w+Weapon::Rate,r); }); }
static void Mk_SetAllWeaponCD(float c) { ForAllWeapons([c](uintptr_t w) { Write<float>(w+Weapon::Cooldown,c); }); }
static void Mk_EnableAllWeapons() { ForAllWeapons([](uintptr_t w) { Write<bool>(w+Weapon::Enabled,true); }); }
static void Mk_DisableAllWeapons() { ForAllWeapons([](uintptr_t w) { Write<bool>(w+Weapon::Enabled,false); }); }
static void Mk_SetAllWorth(double v) { ForTower([v](uintptr_t t) { Write<double>(t+Tower::Worth,v); }); }
static void Mk_SetAllDmgDealt(double v) { ForTower([v](uintptr_t t) { Write<double>(t+Tower::DamageDealt,v); }); }
static void Mk_ResetDamages() { uintptr_t br=GetBridge(); if(br) ((void(*)(void*,void*))(Base+RVA::BridgeR::ResetDamages))((void*)br,nullptr); }
static void Mk_MaxGeraldo() { uintptr_t br=GetBridge(); if(br) ((void(*)(void*,void*))(Base+RVA::BridgeR::MaxGeraldoStock))((void*)br,nullptr); }
static void Mk_SetAllAbilityCD(float c) { ForAllAbilities([c](uintptr_t a) { Write<float>(a+Ability::Cooldown,c); }); }
static void Mk_SetAllAbilityMaxCD(float c) { ForAllAbilities([c](uintptr_t a) { Write<float>(a+Ability::CooldownMax,c); }); }
static void Mk_AllAbilitiesReady() { ForAllAbilities([](uintptr_t a) { Write<float>(a+Ability::Cooldown,0); Write<bool>(a+Ability::CanActivate,true); }); }
static void Mk_SetAllCashEarned(double v) { ForTower([v](uintptr_t t) { Write<double>(t+Tower::CashEarned,v); }); }
static void Mk_SetAllOwner(int o) { ForTower([o](uintptr_t t) { Write<int>(t+Tower::Owner,o); }); }
static void Mk_SetAllSelectable(bool v) { ForTower([v](uintptr_t t) { Write<bool>(t+Tower::IsSelectable,v); }); }
static void Mk_DeleteAll() { uintptr_t ig=GetInGame(); if(ig) ((void(*)(void*,void*))(Base+RVA::InGameR::DeleteAllTowers))((void*)ig,nullptr); }
static void Mk_LockHeroPurchases() { auto& s=S(); s.lockHeroPurchases=!s.lockHeroPurchases; uintptr_t sim=GetSim(); if(sim) ((void(*)(void*,bool,void*))(Base+RVA::Sim::LockHeroPurchases))((void*)sim,s.lockHeroPurchases,nullptr); }
static void Mk_LockTowerPurchases() { auto& s=S(); s.lockTowerPurchases=!s.lockTowerPurchases; uintptr_t sim=GetSim(); if(sim) ((void(*)(void*,bool,void*))(Base+RVA::Sim::LockTowerPurchases))((void*)sim,s.lockTowerPurchases,nullptr); }
static void Mk_AutoAbility() { auto& s=S(); s.autoAbility=!s.autoAbility; }
static void Mk_FreeTowers() { auto& s=S(); s.freeTowers=!s.freeTowers; }
static void Mk_TowerSell(int i) { uintptr_t t=TowerAt(i); if(t) { uintptr_t sim=GetSim(); if(sim) ((void(*)(void*,void*,int,void*))(Base+RVA::Sim::SellTower))((void*)sim,(void*)t,0,nullptr); } }
static void Mk_TowerSetWorth(int i, double v) { uintptr_t t=TowerAt(i); if(t) Write<double>(t+Tower::Worth,v); }
static void Mk_TowerSetDmg(int i, double v) { uintptr_t t=TowerAt(i); if(t) Write<double>(t+Tower::DamageDealt,v); }
static void Mk_TowerClearCD(int i) { uintptr_t t=TowerAt(i); if(t) ForAbility(t, [](uintptr_t a) { Write<float>(a+Ability::Cooldown,0); Write<bool>(a+Ability::CanActivate,true); }); }
static void Mk_TowerActivateAbility(int i) { uintptr_t t=TowerAt(i); if(t) ForAbility(t, [](uintptr_t a) { ((void(*)(void*,void*))(Base+RVA::AbilityR::Activate))((void*)a,nullptr); }); }
static void Mk_TowerDisableWeapons(int i) { uintptr_t t=TowerAt(i); if(t) ForWeapon(t, [](uintptr_t w) { Write<bool>(w+Weapon::Enabled,false); }); }
static void Mk_TowerEnableWeapons(int i) { uintptr_t t=TowerAt(i); if(t) ForWeapon(t, [](uintptr_t w) { Write<bool>(w+Weapon::Enabled,true); }); }
static void Mk_TowerSetWeaponRate(int i, float r) { uintptr_t t=TowerAt(i); if(t) ForWeapon(t, [r](uintptr_t w) { Write<float>(w+Weapon::Rate,r); }); }
static void Mk_TowerSetSelectable(int i, bool v) { uintptr_t t=TowerAt(i); if(t) Write<bool>(t+Tower::IsSelectable,v); }
static void Mk_TowerDestroy(int i) { uintptr_t t=TowerAt(i); if(t) ((void(*)(void*,void*))(Base+RVA::TowerR::Destroy))((void*)t,nullptr); }
static void Mk_AbilitiesChanged() { uintptr_t sim=GetSim(); if(sim) ((void(*)(void*,void*))(Base+RVA::Sim::AbilitiesChanged))((void*)sim,nullptr); }
static void Mk_SetAllWeaponCDZero() { ForAllWeapons([](uintptr_t w) { Write<float>(w+Weapon::Cooldown,0); }); }
static void Mk_TowerUpgrade(int i) { uintptr_t t=TowerAt(i); if(t) ((void(*)(void*,void*))(Base+RVA::TowerR::Upgrade))((void*)t,nullptr); }
static void Mk_TowerAddCash(int i, double v) { uintptr_t t=TowerAt(i); if(t) ((void(*)(void*,double,void*))(Base+RVA::TowerR::AddCash))((void*)t,v,nullptr); }
static void Mk_TowerSetTarget(int i, int type) { uintptr_t t=TowerAt(i); if(t) ((void(*)(void*,int,void*))(Base+RVA::TowerR::SetTargetType))((void*)t,type,nullptr); }
static void Mk_SpawnAllDarts() { ForAllWeapons([](uintptr_t w) { ((void(*)(void*,void*))(Base+RVA::WeaponR::SpawnDart))((void*)w,nullptr); }); }
static void Mk_UpgradeParagon(int i) { uintptr_t t=TowerAt(i); if(!t) return; uintptr_t tm=GetTowerMgr(); if(tm) ((void(*)(void*,void*,void*))(Base+RVA::TowerMgrR::UpgradeTowerParagon))((void*)tm,(void*)t,nullptr); }
static void Mk_TowerMgrSell(int i) { uintptr_t t=TowerAt(i); if(!t) return; uintptr_t tm=GetTowerMgr(); if(tm) ((void(*)(void*,void*,void*))(Base+RVA::TowerMgrR::SellTower))((void*)tm,(void*)t,nullptr); }
static void Mk_TowerMgrDestroy(int i) { uintptr_t t=TowerAt(i); if(!t) return; uintptr_t tm=GetTowerMgr(); if(tm) ((void(*)(void*,void*,void*))(Base+RVA::TowerMgrR::DestroyTower))((void*)tm,(void*)t,nullptr); }
static void Mk_TowerSellDirect(int i) { uintptr_t t=TowerAt(i); if(t) ((void(*)(void*,void*))(Base+RVA::TowerR::Sell))((void*)t,nullptr); }
static void V_SetCash(double c) { uintptr_t sim=GetSim(); if(sim) ((void(*)(void*,double,int,void*))(Base+RVA::Sim::SetCash))((void*)sim,c,-1,nullptr); }
static void V_AddCash(double c) { uintptr_t ig=GetInGame(); if(ig) ((void(*)(void*,double,void*))(Base+RVA::InGameR::CheatSetCash))((void*)ig,c,nullptr); }
static void V_InfiniteCash() { auto& s=S(); s.infiniteCash=!s.infiniteCash; }
static void V_SetHealth(float h) { uintptr_t sim=GetSim(); if(sim) ((void(*)(void*,float,bool,void*))(Base+RVA::Sim::SetHealth))((void*)sim,h,false,nullptr); }
static void V_SetHealthDirect(float h) { uintptr_t sim=GetSim(); if(sim) ((void(*)(void*,float,void*))(Base+RVA::Sim::set_Health))((void*)sim,h,nullptr); }
static void V_InfiniteHealth() { auto& s=S(); s.infiniteHealth=!s.infiniteHealth; }
static void V_ResetHealth() { uintptr_t sim=GetSim(); if(sim) ((void(*)(void*,void*))(Base+RVA::Sim::ResetHealth))((void*)sim,nullptr); }
static void V_InfiniteHealthMode() { uintptr_t sim=GetSim(); if(sim) ((void(*)(void*,void*))(Base+RVA::Sim::ResetToInfinitHealth))((void*)sim,nullptr); }
static void V_SetMaxHealth(float h) { uintptr_t sim=GetSim(); if(sim) ((void(*)(void*,float,void*))(Base+RVA::Sim::set_MaxHealth))((void*)sim,h,nullptr); }
static void V_SetShield(float v) { uintptr_t sim=GetSim(); if(sim) ((void(*)(void*,float,void*))(Base+RVA::Sim::set_Shield))((void*)sim,v,nullptr); }
static void V_SetRevives(int v) { uintptr_t sim=GetSim(); if(sim) ((void(*)(void*,int,void*))(Base+RVA::Sim::set_Revives))((void*)sim,v,nullptr); }
static void V_TakeDmg(float d) { uintptr_t sim=GetSim(); if(sim) ((void(*)(void*,float,void*))(Base+RVA::Sim::TakeDamage))((void*)sim,d,nullptr); }
static void V_DoubleCash() { auto& s=S(); s.doubleCash=!s.doubleCash; }
static void V_NoLivesLost() { auto& s=S(); s.noLivesLost=!s.noLivesLost; uintptr_t sim=GetSim(); if(sim) Write<bool>(sim+Simulation::NoLivesLost,s.noLivesLost); }
static void V_SetCashSpent(double v) { uintptr_t sim=GetSim(); if(sim) Write<double>(sim+Simulation::CashSpent,v); }
static void V_HasCashChanged(bool v) { uintptr_t sim=GetSim(); if(sim) Write<bool>(sim+Simulation::HasCashChanged,v); }
static void V_HasHealthChanged(bool v) { uintptr_t sim=GetSim(); if(sim) Write<bool>(sim+Simulation::HasHealthChanged,v); }
static void V_SetBloonOverride(int v) { uintptr_t sim=GetSim(); if(sim) Write<int>(sim+Simulation::BloonLivesLostOverride,v); }
static void V_GodMode() { auto& s=S(); s.godMode=!s.godMode; }
static void V_SetMaxSoftcap(float v) { uintptr_t sim=GetSim(); if(sim) ((void(*)(void*,float,void*))(Base+RVA::Sim::set_MaxHealth))((void*)sim,v,nullptr); }
static void V_DebugOptions() { auto& s=S(); s.debugOptions=!s.debugOptions; uintptr_t sim=GetSim(); if(sim) Write<bool>(sim+Simulation::DebugOptions,s.debugOptions); }
static void V_SetTierCount(int v) { uintptr_t sim=GetSim(); if(sim) Write<int>(sim+Simulation::CachedTierCount,v); }
static void V_RemoveCash(double c) { uintptr_t sim=GetSim(); if(sim) ((void(*)(void*,double,int,void*))(Base+RVA::Sim::RemoveCash))((void*)sim,c,-1,nullptr); }
static void V_AddCashBridge(double c) { uintptr_t br=GetBridge(); if(br) ((void(*)(void*,double,int,void*))(Base+RVA::BridgeR::AddCash))((void*)br,c,-1,nullptr); }
static void V_SetCashBridge(double c) { uintptr_t br=GetBridge(); if(br) ((void(*)(void*,double,int,void*))(Base+RVA::BridgeR::SetCash))((void*)br,c,-1,nullptr); }
static void V_SendCash(int player) { uintptr_t sim=GetSim(); if(sim) ((void(*)(void*,int,double,void*))(Base+RVA::Sim::SendCash))((void*)sim,player,1000.0,nullptr); }
static void V_AddSharedCash(double c) { uintptr_t sim=GetSim(); if(sim) ((void(*)(void*,double,void*))(Base+RVA::Sim::AddSharedCash))((void*)sim,c,nullptr); }
static void V_CheatSetHealth(float h) { uintptr_t ig=GetInGame(); if(ig) ((void(*)(void*,float,void*))(Base+RVA::InGameR::CheatSetHealth))((void*)ig,h,nullptr); }
static void V_SetStartingHealth(float v) { uintptr_t sim=GetSim(); if(sim) Write<float>(sim+Simulation::StartingHealth,v); }
static void V_SetSoftcapMod(float v) { uintptr_t sim=GetSim(); if(sim) Write<float>(sim+Simulation::SoftcapPctMod,v); }
static void V_SetMaxShield(float v) { uintptr_t sim=GetSim(); if(sim) Write<float>(sim+Simulation::MaxShield,v); }
static void W_DestroyAllBloons() { uintptr_t sim=GetSim(); if(sim) ((void(*)(void*,void*))(Base+RVA::Sim::DestroyAllBloons))((void*)sim,nullptr); }
static void W_DestroyAllBloonsIG() { uintptr_t ig=GetInGame(); if(ig) ((void(*)(void*,void*))(Base+RVA::InGameR::DeleteAllBloons))((void*)ig,nullptr); }
static void W_DestroyAllProjectiles() { uintptr_t br=GetBridge(); if(br) ((void(*)(void*,void*))(Base+RVA::BridgeR::DestroyAllProjectiles))((void*)br,nullptr); }
static void W_ClearBloons() { uintptr_t br=GetBridge(); if(br) ((void(*)(void*,void*))(Base+RVA::BridgeR::ClearBloons))((void*)br,nullptr); }
static void W_StartRound() { uintptr_t sim=GetSim(); if(sim) ((void(*)(void*,void*))(Base+RVA::Sim::StartRound))((void*)sim,nullptr); }
static void W_StartRoundBridge() { uintptr_t br=GetBridge(); if(br) ((void(*)(void*,void*))(Base+RVA::BridgeR::StartRound))((void*)br,nullptr); }
static void W_SetRound(int r) { uintptr_t br=GetBridge(); if(br) ((void(*)(void*,int,void*))(Base+RVA::BridgeR::SetRound))((void*)br,r,nullptr); }
static void W_SetEndRound(int r) { uintptr_t br=GetBridge(); if(br) ((void(*)(void*,int,void*))(Base+RVA::BridgeR::SetEndRound))((void*)br,r,nullptr); }
static void W_ResetRound() { uintptr_t br=GetBridge(); if(br) ((void(*)(void*,void*))(Base+RVA::BridgeR::ResetRound))((void*)br,nullptr); }
static void W_AutoPlay() { auto& s=S(); s.autoPlay=!s.autoPlay; uintptr_t sim=GetSim(); if(sim) Write<bool>(sim+Simulation::AutoPlay,s.autoPlay); }
static void W_BlockAutoPlay() { auto& s=S(); s.blockAutoPlay=!s.blockAutoPlay; uintptr_t br=GetBridge(); if(br) ((void(*)(void*,bool,void*))(Base+RVA::BridgeR::SetBlockAutoPlay))((void*)br,s.blockAutoPlay,nullptr); }
static void W_PauseAutoPlay() { uintptr_t sim=GetSim(); if(sim) Write<bool>(sim+Simulation::PauseAutoPlay,!Read<bool>(sim+Simulation::PauseAutoPlay)); }
static void W_Sandbox() { auto& s=S(); s.sandbox=!s.sandbox; uintptr_t sim=GetSim(); if(sim) ((void(*)(void*,bool,void*))(Base+RVA::Sim::SetSandbox))((void*)sim,s.sandbox,nullptr); }
static void W_FreezeSpawner() { auto& s=S(); s.freezeSpawner=!s.freezeSpawner; uintptr_t sp=GetSpawner(); if(sp) Write<bool>(sp+SpawnerObj::IsPaused,s.freezeSpawner); }
static void W_SetSpawnerRound(int r) { uintptr_t sp=GetSpawner(); if(sp) Write<int>(sp+SpawnerObj::CurrentRound,r); }
static void W_SetBloonHP(float h) { ForBloon([h](uintptr_t b) { Write<float>(b+Bloon::Health,h); }); }
static void W_DamageAllBloons(float d) { ForBloon([d](uintptr_t b) { float h=Read<float>(b+Bloon::Health); Write<float>(b+Bloon::Health,h>d?h-d:0); }); }
static void W_KillAllBloons() { ForBloon([](uintptr_t b) { Write<float>(b+Bloon::Health,0); }); }
static void W_SetBloonDistance(float d) { ForBloon([d](uintptr_t b) { Write<float>(b+Bloon::DistanceTraveled,d); }); }
static void W_BloonMaxDistance() { ForBloon([](uintptr_t b) { float tot=Read<float>(b+Bloon::TotalDistanceToExit); Write<float>(b+Bloon::DistanceTraveled,tot); }); }
static void W_SetRoundMutators(int r) { uintptr_t sim=GetSim(); if(sim) ((void(*)(void*,int,void*))(Base+RVA::Sim::SetRoundMutators))((void*)sim,r,nullptr); }
static void W_Victory() { uintptr_t sim=GetSim(); if(sim) ((bool(*)(void*,void*))(Base+RVA::Sim::SetVictory))((void*)sim,nullptr); }
static void W_Defeat() { uintptr_t sim=GetSim(); if(sim) ((bool(*)(void*,void*))(Base+RVA::Sim::SetDefeat))((void*)sim,nullptr); }
static void W_Win() { uintptr_t br=GetBridge(); if(br) ((void(*)(void*,void*))(Base+RVA::BridgeR::Win))((void*)br,nullptr); }
static void W_Restart() { uintptr_t ig=GetInGame(); if(ig) ((void(*)(void*,void*))(Base+RVA::InGameR::Restart))((void*)ig,nullptr); }
static void W_QuickRestart() { uintptr_t ig=GetInGame(); if(ig) ((void(*)(void*,void*))(Base+RVA::InGameR::QuickRestart))((void*)ig,nullptr); }
static void W_StartFreePlay() { uintptr_t ig=GetInGame(); if(ig) ((void(*)(void*,void*))(Base+RVA::InGameR::StartFreePlay))((void*)ig,nullptr); }
static void W_ForceDefeat() { uintptr_t ig=GetInGame(); if(ig) ((void(*)(void*,void*))(Base+RVA::InGameR::ForceDefeat))((void*)ig,nullptr); }
static void W_Continue() { uintptr_t ig=GetInGame(); if(ig) ((void(*)(void*,void*))(Base+RVA::InGameR::Continue))((void*)ig,nullptr); }
static void W_StopClock() { uintptr_t ig=GetInGame(); if(ig) ((void(*)(void*,void*))(Base+RVA::InGameR::StopClock))((void*)ig,nullptr); }
static void W_ResumeClock() { uintptr_t ig=GetInGame(); if(ig) ((void(*)(void*,void*))(Base+RVA::InGameR::ResumeClock))((void*)ig,nullptr); }
static void W_FastForward() { auto& s=S(); s.fastForward=!s.fastForward; uintptr_t ig=GetInGame(); if(ig) ((void(*)(void*,void*))(Base+RVA::InGameR::PlayFastForward))((void*)ig,nullptr); }
static void W_CheatSetRound(int r) { uintptr_t ig=GetInGame(); if(ig) ((void(*)(void*,int,void*))(Base+RVA::InGameR::CheatSetRound))((void*)ig,r,nullptr); }
static void W_SetMatchWon(bool v) { uintptr_t ig=GetInGame(); if(ig) Write<bool>(ig+InGame::MatchWon,v); }
static void W_SetMatchLost(bool v) { uintptr_t ig=GetInGame(); if(ig) Write<bool>(ig+InGame::MatchLost,v); }
static void W_SetGameStarted(bool v) { uintptr_t sim=GetSim(); if(sim) Write<bool>(sim+Simulation::GameStarted,v); }
static void W_SetGameLost(bool v) { uintptr_t sim=GetSim(); if(sim) Write<bool>(sim+Simulation::GameLost,v); }
static void W_SetGameWon(bool v) { uintptr_t sim=GetSim(); if(sim) Write<bool>(sim+Simulation::GameWon,v); }
static void W_Quit() { uintptr_t ig=GetInGame(); if(ig) ((void(*)(void*,void*))(Base+RVA::InGameR::Quit))((void*)ig,nullptr); }
static void W_SpawnerStart() { uintptr_t sp=GetSpawner(); if(sp) ((void(*)(void*,void*))(Base+RVA::SpawnerR::StartRound))((void*)sp,nullptr); }
static void W_SpawnerSetRound(int r) { uintptr_t sp=GetSpawner(); if(sp) ((void(*)(void*,int,void*))(Base+RVA::SpawnerR::SetRound))((void*)sp,r,nullptr); }
static void W_OnVictory() { uintptr_t ig=GetInGame(); if(ig) ((void(*)(void*,void*))(Base+RVA::InGameR::OnVictory))((void*)ig,nullptr); }
static void W_BloonMgrDamage(int i) { uintptr_t bm=GetBloonMgr(); uintptr_t b=BloonAt(i); if(bm&&b) ((void(*)(void*,void*,void*))(Base+RVA::BloonMgrR::BloonDamage))((void*)bm,(void*)b,nullptr); }
static void W_BloonMgrDestroy(int i) { uintptr_t bm=GetBloonMgr(); uintptr_t b=BloonAt(i); if(bm&&b) ((void(*)(void*,void*,void*))(Base+RVA::BloonMgrR::BloonDestroy))((void*)bm,(void*)b,nullptr); }
static void W_SetMatchJustWon(bool v) { uintptr_t sim=GetSim(); if(sim) Write<bool>(sim+Simulation::MatchJustWon,v); }
static void X_InfinitePierce() { auto& s=S(); s.infinitePierce=!s.infinitePierce; }
static void X_MaxDamage() { auto& s=S(); s.maxDamage=!s.maxDamage; }
static void X_SetShowCancel(int v) { uintptr_t sim=GetSim(); if(sim) Write<int>(sim+Simulation::ShowCancel,v); }
static void X_SetEndRoundTime(float v) { uintptr_t sim=GetSim(); if(sim) Write<float>(sim+Simulation::EndOnRoundAdditionalTime,v); }
static void X_CanEarnXP(bool v) { uintptr_t sim=GetSim(); if(sim) Write<bool>(sim+Simulation::CanEarnXP,v); }
static void X_SetPlayBtnDisabled(bool v) { uintptr_t ig=GetInGame(); if(ig) Write<bool>(ig+InGame::PlayBtnDisabled,v); }
static void X_SetMapEditorSandbox(bool v) { uintptr_t ig=GetInGame(); if(ig) Write<bool>(ig+InGame::MapEditorSandbox,v); }
static void X_FlushProcess() { }
static void X_GetAvailableCash() {}
static void X_SetTakenLeakDmg(bool v) { uintptr_t sim=GetSim(); if(sim) Write<bool>(sim+Simulation::HasTakenLeakDamage,v); }
static void X_BloonAt_SetHP(int i, float h) { uintptr_t b=BloonAt(i); if(b) Write<float>(b+Bloon::Health,h); }
static void X_BloonAt_Destroy(int i) { uintptr_t b=BloonAt(i); if(b) ((void(*)(void*,void*))(Base+RVA::BloonR::Destroy))((void*)b,nullptr); }
static void X_BloonAt_SetDist(int i, float d) { uintptr_t b=BloonAt(i); if(b) Write<float>(b+Bloon::DistanceTraveled,d); }
static void X_BloonDamage(int i, float d) { uintptr_t b=BloonAt(i); if(b) ((void(*)(void*,float,void*))(Base+RVA::BloonR::Damage))((void*)b,d,nullptr); }
static void X_BloonLeak(int i) { uintptr_t b=BloonAt(i); if(b) ((void(*)(void*,void*))(Base+RVA::BloonR::Leaked))((void*)b,nullptr); }
static void X_BloonResetHP(int i) { uintptr_t b=BloonAt(i); if(b) ((void(*)(void*,void*))(Base+RVA::BloonR::ResetHealth))((void*)b,nullptr); }
static void X_BloonSetHPRVA(int i, float h) { uintptr_t b=BloonAt(i); if(b) ((void(*)(void*,float,void*))(Base+RVA::BloonR::SetHealth))((void*)b,h,nullptr); }
static void X_WeaponSpawnDart(int ti) { uintptr_t t=TowerAt(ti); if(t) ForWeapon(t,[](uintptr_t w) { ((void(*)(void*,void*))(Base+RVA::WeaponR::SpawnDart))((void*)w,nullptr); }); }
static void X_WeaponSetEnabled(int ti, bool v) { uintptr_t t=TowerAt(ti); if(t) ForWeapon(t,[v](uintptr_t w) { ((void(*)(void*,bool,void*))(Base+RVA::WeaponR::SetEnabled))((void*)w,v,nullptr); }); }
static void X_AbilitySetCD(int ti, float cd) { uintptr_t t=TowerAt(ti); if(t) ForAbility(t,[cd](uintptr_t a) { ((void(*)(void*,float,void*))(Base+RVA::AbilityR::SetCooldown))((void*)a,cd,nullptr); }); }
static void X_AbilityClearCD(int ti) { uintptr_t t=TowerAt(ti); if(t) ForAbility(t,[](uintptr_t a) { ((void(*)(void*,void*))(Base+RVA::AbilityR::ClearCooldown))((void*)a,nullptr); }); }
static void X_SetMaxHealthIncrease(float v) { uintptr_t sim=GetSim(); if(sim) Write<float>(sim+Simulation::MaxHealthIncrease,v); }
static void X_SetStartingRevives(int v) { uintptr_t sim=GetSim(); if(sim) Write<int>(sim+Simulation::StartingRevives,v); }
static std::string FmtTower(int idx) {
    uintptr_t t=TowerAt(idx); if(!t) return "Not found";
    std::ostringstream ss;
    ss<<"[T"<<idx<<"]"
      <<" Worth:"<<std::fixed<<std::setprecision(0)<<Read<double>(t+Tower::Worth)
      <<" DmgDealt:"<<Read<double>(t+Tower::DamageDealt)
      <<" Cash$:"<<Read<double>(t+Tower::CashEarned)
      <<" Owner:"<<Read<int>(t+Tower::Owner)
      <<(Read<bool>(t+Tower::IsSelectable)?" [SEL]":"");
    uintptr_t wl=Read<uintptr_t>(t+Tower::Weapons);
    int wc=ListCount(wl);
    uintptr_t al=Read<uintptr_t>(t+Tower::Abilities);
    int ac=ListCount(al);
    ss<<" W:"<<wc<<" A:"<<ac;
    return ss.str();
}
static std::string FmtBloon(int idx) {
    uintptr_t b=BloonAt(idx); if(!b) return "Not found";
    std::ostringstream ss;
    ss<<"[B"<<idx<<"]"
      <<" HP:"<<std::fixed<<std::setprecision(1)<<Read<float>(b+Bloon::Health)
      <<" Dist:"<<Read<float>(b+Bloon::DistanceTraveled)
      <<"/"<<Read<float>(b+Bloon::TotalDistanceToExit)
      <<(Read<bool>(b+Bloon::IsBoss)?" [BOSS]":"");
    return ss.str();
}
static std::string FmtTowers(int page) {
    int total=TowerCount(); std::ostringstream ss;
    ss<<"\n  === Towers ("<<total<<") ===\n";
    int start=page*10, end=start+10; if(end>total) end=total;
    for(int i=start;i<end;i++) ss<<"  "<<FmtTower(i)<<"\n";
    if(total>end) ss<<"  ..."<<(total-end)<<" more (page "<<(page+1)<<")\n";
    return ss.str();
}
static std::string FmtBloons(int page) {
    int total=BloonCount(); std::ostringstream ss;
    ss<<"\n  === Bloons ("<<total<<") ===\n";
    int start=page*10, end=start+10; if(end>total) end=total;
    for(int i=start;i<end;i++) ss<<"  "<<FmtBloon(i)<<"\n";
    if(total>end) ss<<"  ..."<<(total-end)<<" more (page "<<(page+1)<<")\n";
    return ss.str();
}
static std::string FmtSim() {
    uintptr_t sim=GetSim();
    if(!sim) return "No simulation";
    std::ostringstream ss;
    ss<<"\n  === Simulation ===\n"
      <<"  Towers:"<<TowerCount()<<" Bloons:"<<BloonCount()<<"\n"
      <<"  DebugOpt:"<<(int)Read<bool>(sim+Simulation::DebugOptions)
      <<" InstantCD:"<<(int)Read<bool>(sim+Simulation::InstantCooldowns)
      <<" PauseWpn:"<<(int)Read<bool>(sim+Simulation::PauseWeapons)<<"\n"
      <<"  Started:"<<(int)Read<bool>(sim+Simulation::GameStarted)
      <<" Won:"<<(int)Read<bool>(sim+Simulation::GameWon)
      <<" Lost:"<<(int)Read<bool>(sim+Simulation::GameLost)
      <<" AutoPlay:"<<(int)Read<bool>(sim+Simulation::AutoPlay)<<"\n"
      <<"  Sandbox:"<<(int)Read<bool>(sim+Simulation::Sandbox)
      <<" NoLivesLost:"<<(int)Read<bool>(sim+Simulation::NoLivesLost)
      <<" LeakDmg:"<<(int)Read<bool>(sim+Simulation::HasTakenLeakDamage)<<"\n"
      <<"  LockHero:"<<(int)Read<bool>(sim+Simulation::LockHeroPurchases)
      <<" LockTower:"<<(int)Read<bool>(sim+Simulation::LockTowerPurchases)<<"\n"
      <<"  TiersCached:"<<Read<int>(sim+Simulation::CachedTierCount)<<"\n";
    return ss.str();
}
static std::string FmtIG() {
    uintptr_t ig=GetInGame();
    if(!ig) return "No InGame";
    std::ostringstream ss;
    ss<<"\n  === InGame ===\n"
      <<"  MatchStarted:"<<(int)Read<bool>(ig+InGame::MatchStarted)
      <<" Won:"<<(int)Read<bool>(ig+InGame::MatchWon)
      <<" Lost:"<<(int)Read<bool>(ig+InGame::MatchLost)<<"\n"
      <<"  PlayDisabled:"<<(int)Read<bool>(ig+InGame::PlayBtnDisabled)
      <<" MapEditorSandbox:"<<(int)Read<bool>(ig+InGame::MapEditorSandbox)<<"\n";
    return ss.str();
}
static std::string FmtOverview() {
    std::ostringstream ss;
    ss<<"\n  === Overview ===\n"
      <<"  Towers:"<<TowerCount()<<" Bloons:"<<BloonCount()<<"\n";
    uintptr_t sim=GetSim();
    if(sim) {
        ss<<"  Started:"<<(int)Read<bool>(sim+Simulation::GameStarted)
          <<" Won:"<<(int)Read<bool>(sim+Simulation::GameWon)
          <<" Lost:"<<(int)Read<bool>(sim+Simulation::GameLost)<<"\n"
          <<"  Sandbox:"<<(int)Read<bool>(sim+Simulation::Sandbox)
          <<" AutoPlay:"<<(int)Read<bool>(sim+Simulation::AutoPlay)<<"\n";
    }
    uintptr_t sp=GetSpawner();
    if(sp) ss<<"  SpawnerRound:"<<Read<int>(sp+SpawnerObj::CurrentRound)
             <<" Paused:"<<(int)Read<bool>(sp+SpawnerObj::IsPaused)<<"\n";
    return ss.str();
}
static ToggleState BuildState() {
    auto& s=S(); ToggleState t={};
    t.infiniteCash=s.infiniteCash; t.infiniteHealth=s.infiniteHealth;
    t.instantCooldowns=s.instantCooldowns; t.pauseWeapons=s.pauseWeapons;
    t.debugOptions=s.debugOptions; t.sandbox=s.sandbox;
    t.autoPlay=s.autoPlay; t.blockAutoPlay=s.blockAutoPlay;
    t.freeTowers=s.freeTowers; t.lockHeroPurchases=s.lockHeroPurchases;
    t.lockTowerPurchases=s.lockTowerPurchases; t.autoAbility=s.autoAbility;
    t.freezeSpawner=s.freezeSpawner; t.infiniteRange=s.infiniteRange;
    t.godMode=s.godMode; t.doubleCash=s.doubleCash;
    t.noLivesLost=s.noLivesLost; t.infinitePierce=s.infinitePierce;
    t.maxDamage=s.maxDamage; t.fastForward=s.fastForward;
    return t;
}
static ResponsePacket Ok(const char* m="") {
    ResponsePacket r={}; r.success=1; r.state=BuildState();
    strncpy_s(r.message,m,RESPONSE_MSG_SIZE-1); return r;
}
static ResponsePacket HandleMonkey(uint32_t fn, int ip, float fp) {
    switch(fn) {
    case 1: Mk_SellAll(); return Ok("Sold all towers");
    case 2: Mk_SellAllProps(); return Ok("Sold all props");
    case 3: Mk_ResetAbilityCDs(); return Ok("Ability CDs reset");
    case 4: Mk_ClearAllAbilityCDs(); return Ok("All ability CDs cleared");
    case 5: Mk_ActivateAllAbilities(); return Ok("All abilities activated");
    case 6: Mk_DeactivateAllAbilities(); return Ok("All abilities deactivated");
    case 7: Mk_PauseWeapons(); return Ok("Pause weapons toggled");
    case 8: Mk_InstantCooldowns(); return Ok("Instant cooldowns toggled");
    case 9: Mk_ToggleWeapons(); return Ok("Weapons toggled");
    case 10: Mk_SetAllWeaponRate(fp); return Ok("Weapon rate set");
    case 11: Mk_SetAllWeaponCD(fp); return Ok("Weapon CD set");
    case 12: Mk_EnableAllWeapons(); return Ok("All weapons enabled");
    case 13: Mk_DisableAllWeapons(); return Ok("All weapons disabled");
    case 14: Mk_SetAllWorth((double)fp); return Ok("All tower worth set");
    case 15: Mk_SetAllDmgDealt((double)fp); return Ok("All tower dmg set");
    case 16: Mk_ResetDamages(); return Ok("Damages reset");
    case 17: Mk_MaxGeraldo(); return Ok("Geraldo maxed");
    case 18: Mk_SetAllAbilityCD(fp); return Ok("All ability CD set");
    case 19: Mk_SetAllAbilityMaxCD(fp); return Ok("All ability max CD set");
    case 20: Mk_AllAbilitiesReady(); return Ok("All abilities ready");
    case 21: Mk_SetAllCashEarned((double)fp); return Ok("Cash earned set");
    case 22: Mk_SetAllOwner(ip); return Ok("Owner set");
    case 23: Mk_SetAllSelectable(ip!=0); return Ok("Selectable set");
    case 24: Mk_DeleteAll(); return Ok("All towers deleted");
    case 25: Mk_LockHeroPurchases(); return Ok("Lock hero toggled");
    case 26: Mk_LockTowerPurchases(); return Ok("Lock tower toggled");
    case 27: Mk_AutoAbility(); return Ok("Auto ability toggled");
    case 28: Mk_FreeTowers(); return Ok("Free towers toggled");
    case 29: Mk_SetAllWeaponCDZero(); return Ok("All weapon CDs zeroed");
    case 30: Mk_AbilitiesChanged(); return Ok("Abilities changed notified");
    case 40: Mk_TowerSell(ip); return Ok("Tower sold");
    case 41: Mk_TowerSetWorth(ip,(double)fp); return Ok("Tower worth set");
    case 42: Mk_TowerSetDmg(ip,(double)fp); return Ok("Tower dmg set");
    case 43: Mk_TowerClearCD(ip); return Ok("Tower CD cleared");
    case 44: Mk_TowerActivateAbility(ip); return Ok("Tower ability activated");
    case 45: Mk_TowerDisableWeapons(ip); return Ok("Tower weapons disabled");
    case 46: Mk_TowerEnableWeapons(ip); return Ok("Tower weapons enabled");
    case 47: Mk_TowerSetWeaponRate(ip,fp); return Ok("Tower weapon rate set");
    case 48: Mk_TowerSetSelectable(ip,(int)fp!=0); return Ok("Tower selectable set");
    case 49: Mk_TowerDestroy(ip); return Ok("Tower destroyed");
    case 50: return Ok(FmtTower(ip).c_str());
    case 51: return Ok(FmtTowers(ip).c_str());
    case 31: Mk_TowerUpgrade(ip); return Ok("Tower upgraded");
    case 32: Mk_TowerAddCash(ip,(double)fp); return Ok("Tower cash added");
    case 33: Mk_TowerSetTarget(ip,(int)fp); return Ok("Tower target set");
    case 34: Mk_SpawnAllDarts(); return Ok("All darts spawned");
    case 35: Mk_UpgradeParagon(ip); return Ok("Paragon upgraded");
    case 36: Mk_TowerMgrSell(ip); return Ok("Tower mgr sold");
    case 37: Mk_TowerMgrDestroy(ip); return Ok("Tower mgr destroyed");
    case 38: Mk_TowerSellDirect(ip); return Ok("Tower sold direct");
    default: return Ok("Unknown monkey cmd");
    }
}
static ResponsePacket HandleValue(uint32_t fn, int ip, float fp) {
    switch(fn) {
    case 1: V_SetCash((double)ip); return Ok("Cash set");
    case 2: V_AddCash((double)ip); return Ok("Cash added");
    case 3: V_InfiniteCash(); return Ok("Infinite cash toggled");
    case 4: V_SetHealth(fp); return Ok("Health set");
    case 5: V_SetHealthDirect(fp); return Ok("Health direct set");
    case 6: V_InfiniteHealth(); return Ok("Infinite health toggled");
    case 7: V_ResetHealth(); return Ok("Health reset");
    case 8: V_InfiniteHealthMode(); return Ok("Infinite health mode");
    case 9: V_SetMaxHealth(fp); return Ok("Max health set");
    case 10: V_SetShield(fp); return Ok("Shield set");
    case 11: V_SetRevives(ip); return Ok("Revives set");
    case 12: V_TakeDmg(fp); return Ok("Damage taken");
    case 13: V_DoubleCash(); return Ok("Double cash toggled");
    case 14: V_NoLivesLost(); return Ok("No lives lost toggled");
    case 15: V_SetCashSpent((double)ip); return Ok("Cash spent set");
    case 16: V_HasCashChanged(ip!=0); return Ok("Cash changed flag set");
    case 17: V_HasHealthChanged(ip!=0); return Ok("Health changed flag set");
    case 18: V_SetBloonOverride(ip); return Ok("Bloon override set");
    case 19: V_GodMode(); return Ok("God mode toggled");
    case 20: V_DebugOptions(); return Ok("Debug options toggled");
    case 21: V_SetTierCount(ip); return Ok("Tier count set");
    case 22: V_RemoveCash((double)ip); return Ok("Cash removed");
    case 23: V_AddCashBridge((double)ip); return Ok("Cash added (bridge)");
    case 24: V_SetCashBridge((double)ip); return Ok("Cash set (bridge)");
    case 25: V_SendCash(ip); return Ok("Cash sent");
    case 26: V_AddSharedCash((double)ip); return Ok("Shared cash added");
    case 27: V_CheatSetHealth(fp); return Ok("Health cheat set");
    case 28: V_SetStartingHealth(fp); return Ok("Starting health set");
    case 29: V_SetSoftcapMod(fp); return Ok("Softcap mod set");
    case 30: return Ok(FmtSim().c_str());
    case 31: return Ok(FmtIG().c_str());
    case 32: V_SetMaxShield(fp); return Ok("Max shield set");
    default: return Ok("Unknown value cmd");
    }
}
static ResponsePacket HandleWorld(uint32_t fn, int ip, float fp) {
    switch(fn) {
    case 1: W_DestroyAllBloons(); return Ok("All bloons destroyed");
    case 2: W_DestroyAllBloonsIG(); return Ok("Bloons deleted (IG)");
    case 3: W_DestroyAllProjectiles(); return Ok("Projectiles destroyed");
    case 4: W_ClearBloons(); return Ok("Bloons cleared");
    case 5: W_StartRound(); return Ok("Round started (sim)");
    case 6: W_StartRoundBridge(); return Ok("Round started (bridge)");
    case 7: W_SetRound(ip); return Ok("Round set");
    case 8: W_SetEndRound(ip); return Ok("End round set");
    case 9: W_ResetRound(); return Ok("Round reset");
    case 10: W_AutoPlay(); return Ok("Auto play toggled");
    case 11: W_BlockAutoPlay(); return Ok("Block auto play toggled");
    case 12: W_PauseAutoPlay(); return Ok("Pause auto play toggled");
    case 13: W_Sandbox(); return Ok("Sandbox toggled");
    case 14: W_FreezeSpawner(); return Ok("Spawner freeze toggled");
    case 15: W_SetSpawnerRound(ip); return Ok("Spawner round set");
    case 16: W_SetBloonHP(fp); return Ok("Bloon HP set");
    case 17: W_DamageAllBloons(fp); return Ok("Bloons damaged");
    case 18: W_KillAllBloons(); return Ok("All bloons killed");
    case 19: W_SetBloonDistance(fp); return Ok("Bloon distance set");
    case 20: W_BloonMaxDistance(); return Ok("Bloons at exit");
    case 21: W_SetRoundMutators(ip); return Ok("Round mutators set");
    case 22: W_Victory(); return Ok("Victory set");
    case 23: W_Defeat(); return Ok("Defeat set");
    case 24: W_Win(); return Ok("Win (bridge)");
    case 25: W_Restart(); return Ok("Restarted");
    case 26: W_QuickRestart(); return Ok("Quick restarted");
    case 27: W_StartFreePlay(); return Ok("Free play started");
    case 28: W_ForceDefeat(); return Ok("Forced defeat");
    case 29: W_Continue(); return Ok("Continued");
    case 30: W_StopClock(); return Ok("Clock stopped");
    case 31: W_ResumeClock(); return Ok("Clock resumed");
    case 32: W_FastForward(); return Ok("Fast forward toggled");
    case 33: W_CheatSetRound(ip); return Ok("Round cheat set");
    case 34: W_SetMatchWon(ip!=0); return Ok("Match won set");
    case 35: W_SetMatchLost(ip!=0); return Ok("Match lost set");
    case 36: W_SetGameStarted(ip!=0); return Ok("Game started set");
    case 37: W_SetGameLost(ip!=0); return Ok("Game lost set");
    case 38: W_SetGameWon(ip!=0); return Ok("Game won set");
    case 39: W_Quit(); return Ok("Quit called");
    case 40: W_SpawnerStart(); return Ok("Spawner started");
    case 41: W_SpawnerSetRound(ip); return Ok("Spawner round set");
    case 42: W_OnVictory(); return Ok("On victory called");
    case 43: W_BloonMgrDamage(ip); return Ok("Bloon mgr damage");
    case 44: W_BloonMgrDestroy(ip); return Ok("Bloon mgr destroy");
    case 45: W_SetMatchJustWon(ip!=0); return Ok("Match just won set");
    case 50: return Ok(FmtBloons(ip).c_str());
    case 51: return Ok(FmtOverview().c_str());
    default: return Ok("Unknown world cmd");
    }
}
static ResponsePacket HandleMisc(uint32_t fn, int ip, float fp) {
    switch(fn) {
    case 1: X_InfinitePierce(); return Ok("Infinite pierce toggled");
    case 2: X_MaxDamage(); return Ok("Max damage toggled");
    case 3: X_SetShowCancel(ip); return Ok("Show cancel set");
    case 4: X_SetEndRoundTime(fp); return Ok("End round time set");
    case 5: X_CanEarnXP(ip!=0); return Ok("Can earn XP set");
    case 6: X_SetPlayBtnDisabled(ip!=0); return Ok("Play btn disabled set");
    case 7: X_SetMapEditorSandbox(ip!=0); return Ok("Map editor sandbox set");
    case 8: X_FlushProcess(); return Ok("Process flushed");
    case 9: X_SetTakenLeakDmg(ip!=0); return Ok("Leak damage flag set");
    case 10: X_BloonAt_SetHP(ip,fp); return Ok("Bloon HP set");
    case 11: X_BloonAt_Destroy(ip); return Ok("Bloon destroyed");
    case 12: X_BloonAt_SetDist(ip,fp); return Ok("Bloon distance set");
    case 13: X_BloonDamage(ip,fp); return Ok("Bloon damaged (RVA)");
    case 14: X_BloonLeak(ip); return Ok("Bloon leaked");
    case 15: X_BloonResetHP(ip); return Ok("Bloon HP reset");
    case 16: X_BloonSetHPRVA(ip,fp); return Ok("Bloon HP set (RVA)");
    case 17: X_WeaponSpawnDart(ip); return Ok("Darts spawned");
    case 18: X_WeaponSetEnabled(ip,(int)fp!=0); return Ok("Weapon enabled set");
    case 19: X_AbilitySetCD(ip,fp); return Ok("Ability CD set (RVA)");
    case 20: return Ok(FmtBloon(ip).c_str());
    case 21: return Ok(FmtTower(ip).c_str());
    case 22: X_AbilityClearCD(ip); return Ok("Ability CD cleared (RVA)");
    case 23: X_SetMaxHealthIncrease(fp); return Ok("Max health increase set");
    case 24: X_SetStartingRevives(ip); return Ok("Starting revives set");
    default: return Ok("Unknown misc cmd");
    }
}
static ResponsePacket Execute(const CommandPacket& cmd) {
    switch((CommandTab)cmd.tab) {
    case TAB_MONKEY: return HandleMonkey(cmd.function,cmd.intParam,cmd.floatParam);
    case TAB_VALUE: return HandleValue(cmd.function,cmd.intParam,cmd.floatParam);
    case TAB_WORLD: return HandleWorld(cmd.function,cmd.intParam,cmd.floatParam);
    case TAB_MISC: return HandleMisc(cmd.function,cmd.intParam,cmd.floatParam);
    case TAB_SYSTEM:
        switch((SystemCmd)cmd.function) {
        case SYS_GET_STATE: return Ok("");
        case SYS_PING: return Ok("pong");
        case SYS_EXIT: return Ok("bye");
        default: return Ok("Unknown sys");
        }
    default: return Ok("Unknown tab");
    }
}
static std::atomic<bool> g_run{true};
static void PipeServer() {
    while(g_run) {
        HANDLE hp = CreateNamedPipeW(PIPE_NAME,PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_MESSAGE|PIPE_READMODE_MESSAGE|PIPE_WAIT,1,
            sizeof(ResponsePacket),sizeof(CommandPacket),0,nullptr);
        if(hp==INVALID_HANDLE_VALUE) { Sleep(1000); continue; }
        BOOL conn = ConnectNamedPipe(hp,nullptr) ? TRUE : (GetLastError()==ERROR_PIPE_CONNECTED);
        if(conn) {
            while(g_run) {
                CommandPacket cmd={}; DWORD br=0;
                if(!ReadFile(hp,&cmd,sizeof(cmd),&br,nullptr)||br!=sizeof(cmd)) break;
                if(cmd.tab==TAB_SYSTEM&&cmd.function==SYS_EXIT) {
                    auto r=Execute(cmd); DWORD bw=0;
                    WriteFile(hp,&r,sizeof(r),&bw,nullptr); FlushFileBuffers(hp); break;
                }
                auto r=Execute(cmd); DWORD bw=0;
                if(!WriteFile(hp,&r,sizeof(r),&bw,nullptr)) break;
                FlushFileBuffers(hp);
            }
        }
        DisconnectNamedPipe(hp); CloseHandle(hp);
    }
}
static void UpdateLoop() {
    while(true) {
        Sleep(100);
        auto& s=S();
        uintptr_t sim=GetSim();
        if(!sim) continue;
        if(s.infiniteCash) ((void(*)(void*,double,int,void*))(Base+RVA::Sim::SetCash))((void*)sim,9999999.0,-1,nullptr);
        if(s.infiniteHealth) ((void(*)(void*,float,bool,void*))(Base+RVA::Sim::SetHealth))((void*)sim,99999.f,true,nullptr);
        if(s.godMode) { ((void(*)(void*,float,bool,void*))(Base+RVA::Sim::SetHealth))((void*)sim,99999.f,true,nullptr); ((void(*)(void*,float,void*))(Base+RVA::Sim::set_Shield))((void*)sim,99999.f,nullptr); }
        if(s.instantCooldowns) Write<bool>(sim+Simulation::InstantCooldowns,true);
        if(s.pauseWeapons) Write<bool>(sim+Simulation::PauseWeapons,true);
        if(s.debugOptions) Write<bool>(sim+Simulation::DebugOptions,true);
        if(s.autoPlay) Write<bool>(sim+Simulation::AutoPlay,true);
        if(s.freeTowers) ((void(*)(void*,double,int,void*))(Base+RVA::Sim::SetCash))((void*)sim,9999999.0,-1,nullptr);
        if(s.autoAbility) ForAllAbilities([](uintptr_t a) {
            if(Read<float>(a+Ability::Cooldown)<=0 && Read<bool>(a+Ability::CanActivate))
                ((void(*)(void*,void*))(Base+RVA::AbilityR::Activate))((void*)a,nullptr);
        });
        if(s.freezeSpawner) { uintptr_t sp=GetSpawner(); if(sp) Write<bool>(sp+SpawnerObj::IsPaused,true); }
        if(s.noLivesLost) Write<bool>(sim+Simulation::NoLivesLost,true);
        if(s.infinitePierce) ForAllWeapons([](uintptr_t) {});
        if(s.maxDamage) ForAllWeapons([](uintptr_t w) { Write<float>(w+Weapon::Rate,0.01f); });
    }
}
static DWORD WINAPI MainThread(LPVOID hMod) {
    int w=0;
    while(!GetModuleHandleA("GameAssembly.dll")) { Sleep(500); if(++w>60) { FreeLibraryAndExitThread((HMODULE)hMod,0); return 0; } }
    Sleep(2000);
    if(!GameInit()) { FreeLibraryAndExitThread((HMODULE)hMod,0); return 0; }
    std::thread(UpdateLoop).detach();
    PipeServer();
    FreeLibraryAndExitThread((HMODULE)hMod,0);
    return 0;
}
BOOL APIENTRY DllMain(HMODULE hMod, DWORD reason, LPVOID) {
    if(reason==DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hMod);
        HANDLE h=CreateThread(nullptr,0,MainThread,hMod,0,nullptr);
        if(h) CloseHandle(h);
    }
    return TRUE;
}
