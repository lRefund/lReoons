#pragma once
#include <cstdint>
namespace Offsets {
namespace IL2CPP {
constexpr uintptr_t List_Items = 0x10;
constexpr uintptr_t List_Size = 0x18;
constexpr uintptr_t Array_Start = 0x20;
}
namespace Simulation {
constexpr uintptr_t Entity = 0x10;
constexpr uintptr_t Model = 0x20;
constexpr uintptr_t Time = 0x30;
constexpr uintptr_t RoundTime = 0x38;
constexpr uintptr_t CanEarnXP = 0x40;
constexpr uintptr_t DebugOptions = 0x41;
constexpr uintptr_t InstantCooldowns = 0x42;
constexpr uintptr_t PauseWeapons = 0x43;
constexpr uintptr_t BloonManager = 0x68;
constexpr uintptr_t TowerManager = 0x70;
constexpr uintptr_t PowerManager = 0x78;
constexpr uintptr_t ArtifactManager = 0x80;
constexpr uintptr_t GeraldoMgr = 0x88;
constexpr uintptr_t PropManager = 0x90;
constexpr uintptr_t InputManagers = 0xB0;
constexpr uintptr_t TowerInventories = 0xB8;
constexpr uintptr_t CashInventories = 0xD8;
constexpr uintptr_t Process = 0x130;
constexpr uintptr_t EndOnRoundAdditionalTime = 0x140;
constexpr uintptr_t HasCashChanged = 0x150;
constexpr uintptr_t HasHealthChanged = 0x151;
constexpr uintptr_t ShowCancel = 0x154;
constexpr uintptr_t MaxHealthIncrease = 0x160;
constexpr uintptr_t StartingHealth = 0x168;
constexpr uintptr_t SoftcapPctMod = 0x170;
constexpr uintptr_t MaxSoftcapHealth = 0x178;
constexpr uintptr_t MaxHealth = 0x180;
constexpr uintptr_t MaxShield = 0x188;
constexpr uintptr_t StartingRevives = 0x190;
constexpr uintptr_t GameStarted = 0x1C0;
constexpr uintptr_t GameWon = 0x1C1;
constexpr uintptr_t MatchJustWon = 0x1C2;
constexpr uintptr_t GameLost = 0x1C3;
constexpr uintptr_t NoLivesLost = 0x1C4;
constexpr uintptr_t AutoPlay = 0x1C5;
constexpr uintptr_t BlockAutoPlay = 0x1C6;
constexpr uintptr_t PauseAutoPlay = 0x1C7;
constexpr uintptr_t LockHeroPurchases = 0x1C8;
constexpr uintptr_t LockTowerPurchases = 0x1C9;
constexpr uintptr_t CashSpent = 0x1F0;
constexpr uintptr_t BloonLivesLostOverride = 0x200;
constexpr uintptr_t CashManagers = 0x218;
constexpr uintptr_t HasTakenLeakDamage = 0x220;
constexpr uintptr_t Shield = 0x228;
constexpr uintptr_t Revives = 0x230;
constexpr uintptr_t Health = 0x238;
constexpr uintptr_t MapPtr = 0x240;
constexpr uintptr_t Sandbox = 0x270;
constexpr uintptr_t LastSetHealth = 0x274;
constexpr uintptr_t CachedTierCount = 0x29C;
}
namespace CashMgr {
constexpr uintptr_t Cash = 0x10;
constexpr uintptr_t DoubleCash = 0x18;
constexpr uintptr_t IsActivePlayer = 0x19;
}
namespace Tower {
constexpr uintptr_t Weapons = 0x108;
constexpr uintptr_t TowerBehaviors = 0x120;
constexpr uintptr_t Abilities = 0x150;
constexpr uintptr_t TowerModel = 0x1B0;
constexpr uintptr_t TargetType = 0x1B8;
constexpr uintptr_t Worth = 0x1C8;
constexpr uintptr_t DamageDealt = 0x1CC;
constexpr uintptr_t CashEarned = 0x1D0;
constexpr uintptr_t Owner = 0x1D8;
constexpr uintptr_t IsSelectable = 0x1FD;
}
namespace TowerMgr {
constexpr uintptr_t SimPtr = 0x28;
constexpr uintptr_t Towers = 0x30;
constexpr uintptr_t TowerHistory = 0x38;
constexpr uintptr_t CashSpent = 0x78;
}
namespace Weapon {
constexpr uintptr_t TowerPtr = 0x28;
constexpr uintptr_t WeaponModel = 0x90;
constexpr uintptr_t Cooldown = 0xB4;
constexpr uintptr_t Rate = 0xBC;
constexpr uintptr_t Enabled = 0xC5;
}
namespace Ability {
constexpr uintptr_t TowerPtr = 0x28;
constexpr uintptr_t AbilityModel = 0x90;
constexpr uintptr_t Cooldown = 0xB0;
constexpr uintptr_t CooldownMax = 0xB4;
constexpr uintptr_t IsActive = 0xBC;
constexpr uintptr_t CanActivate = 0xBD;
}
namespace Projectile {
constexpr uintptr_t TowerPtr = 0x28;
constexpr uintptr_t EmittedBy = 0x90;
constexpr uintptr_t ProjectileModel = 0xB0;
constexpr uintptr_t Pierce = 0xDC;
constexpr uintptr_t Damage = 0xF0;
}
namespace Bloon {
constexpr uintptr_t DistanceTraveled = 0x120;
constexpr uintptr_t TotalDistanceToExit = 0x128;
constexpr uintptr_t Health = 0x158;
constexpr uintptr_t PathPtr = 0x160;
constexpr uintptr_t BloonModel = 0x168;
constexpr uintptr_t IsBoss = 0x19C;
}
namespace BloonMgr {
constexpr uintptr_t Bloons = 0x30;
constexpr uintptr_t BloonDegradeStepper = 0x58;
constexpr uintptr_t ActiveBloonCount = 0x64;
}
namespace MapObj {
constexpr uintptr_t SimPtr = 0x28;
constexpr uintptr_t MapModel = 0x38;
constexpr uintptr_t Paths = 0x50;
constexpr uintptr_t Areas = 0x58;
constexpr uintptr_t Spawner = 0x60;
}
namespace SpawnerObj {
constexpr uintptr_t SimPtr = 0x28;
constexpr uintptr_t Emissions = 0x38;
constexpr uintptr_t CurrentRound = 0x50;
constexpr uintptr_t IsPaused = 0x78;
}
namespace InGame {
constexpr uintptr_t InputMgr = 0x70;
constexpr uintptr_t Bridge = 0xC0;
constexpr uintptr_t MatchStarted = 0xC9;
constexpr uintptr_t MatchWon = 0xCA;
constexpr uintptr_t MatchLost = 0xCB;
constexpr uintptr_t LastSetHealth = 0xD0;
constexpr uintptr_t LastSetCash = 0xD4;
constexpr uintptr_t LastSetRound = 0xD8;
constexpr uintptr_t MapEditorSandbox = 0xF0;
constexpr uintptr_t PlayBtnDisabled = 0x170;
}
namespace UnityToSim {
constexpr uintptr_t SimPtr = 0x28;
}
namespace PowerMgr {
constexpr uintptr_t Powers = 0x28;
constexpr uintptr_t SimPtr = 0x30;
constexpr uintptr_t UsedPowers = 0x38;
}
namespace RVA {
namespace Sim {
constexpr uintptr_t SetCash = 0x4912E0;
constexpr uintptr_t AddCash = 0x487870;
constexpr uintptr_t GetCash = 0x48B0D0;
constexpr uintptr_t RemoveCash = 0x48FA80;
constexpr uintptr_t TotalCash = 0x4944F0;
constexpr uintptr_t SetHealth = 0x491440;
constexpr uintptr_t set_Health = 0x496670;
constexpr uintptr_t get_Health = 0x496140;
constexpr uintptr_t TakeDamage = 0x494240;
constexpr uintptr_t ResetHealth = 0x4901C0;
constexpr uintptr_t ResetToInfinitHealth = 0x490230;
constexpr uintptr_t set_Shield = 0x496B40;
constexpr uintptr_t get_Shield = 0x4965B0;
constexpr uintptr_t set_Revives = 0x496A70;
constexpr uintptr_t get_Revives = 0x496560;
constexpr uintptr_t set_MaxHealth = 0x496A10;
constexpr uintptr_t get_MaxHealth = 0x4964E0;
constexpr uintptr_t StartRound = 0x493580;
constexpr uintptr_t GetCurrentRound = 0x48B3F0;
constexpr uintptr_t GetEndRound = 0x48B3F0;
constexpr uintptr_t AreRoundsActive = 0x488080;
constexpr uintptr_t CanSendNextRound = 0x4883F0;
constexpr uintptr_t SellTower = 0x490F50;
constexpr uintptr_t DestroyAllBloons = 0x488F80;
constexpr uintptr_t SetSandbox = 0x491700;
constexpr uintptr_t SetVictory = 0x491F20;
constexpr uintptr_t SetDefeat = 0x4913A0;
constexpr uintptr_t SetRoundMutators = 0x491580;
constexpr uintptr_t LockHeroPurchases = 0x48D100;
constexpr uintptr_t LockTowerPurchases = 0x48D160;
constexpr uintptr_t SetHelperMessage = 0x491520;
constexpr uintptr_t AbilitiesChanged = 0x487820;
constexpr uintptr_t GetTiersUsed = 0x48C340;
constexpr uintptr_t TiersRemaining = 0x494460;
constexpr uintptr_t SendCash = 0x491190;
constexpr uintptr_t AddSharedCash = 0x487CC0;
}
namespace TowerMgrR {
constexpr uintptr_t CreateTower = 0x5A3A10;
constexpr uintptr_t DestroyTower = 0x5A5410;
constexpr uintptr_t SellTower = 0x5A92B0;
constexpr uintptr_t UpgradeTower = 0x5AA8B0;
constexpr uintptr_t UpgradeTowerParagon = 0x5AABC0;
constexpr uintptr_t GetMaxTowerCount = 0x5A6660;
}
namespace TowerR {
constexpr uintptr_t Sell = 0x5B2550;
constexpr uintptr_t Upgrade = 0x5B36A0;
constexpr uintptr_t Destroy = 0x5AF370;
constexpr uintptr_t AddCash = 0x5ACED0;
constexpr uintptr_t SetTargetType = 0x5B2D90;
}
namespace WeaponR {
constexpr uintptr_t SpawnDart = 0x7F4040;
constexpr uintptr_t SetEnabled = 0x7F4DB0;
}
namespace AbilityR {
constexpr uintptr_t Activate = 0x90C4A0;
constexpr uintptr_t ClearCooldown = 0x90D8B0;
constexpr uintptr_t SetCooldown = 0x90F550;
constexpr uintptr_t Deactivate = 0x90DD10;
}
namespace BloonR {
constexpr uintptr_t SetHealth = 0xA95950;
constexpr uintptr_t Damage = 0xA90AA0;
constexpr uintptr_t Leaked = 0xA93080;
constexpr uintptr_t ResetHealth = 0xA950F0;
constexpr uintptr_t Destroy = 0xA91370;
}
namespace BloonMgrR {
constexpr uintptr_t BloonDamage = 0xAB4720;
constexpr uintptr_t BloonDestroy = 0xAB4780;
constexpr uintptr_t GetAllBloons = 0xAB5680;
}
namespace SpawnerR {
constexpr uintptr_t StartRound = 0xA82000;
constexpr uintptr_t SetRound = 0xA82080;
}
namespace InGameR {
constexpr uintptr_t CheatSetCash = 0x4D4CA0;
constexpr uintptr_t CheatSetHealth = 0x4D4E30;
constexpr uintptr_t CheatSetRound = 0x4D4FC0;
constexpr uintptr_t DeleteAllTowers = 0x4DBC60;
constexpr uintptr_t DeleteAllBloons = 0x4DBC30;
constexpr uintptr_t Restart = 0x4E9090;
constexpr uintptr_t QuickRestart = 0x4E7160;
constexpr uintptr_t StartFreePlay = 0x4ED050;
constexpr uintptr_t ForceDefeat = 0x4DDC40;
constexpr uintptr_t Quit = 0x4E75E0;
constexpr uintptr_t PlayFastForward = 0x4E6EA0;
constexpr uintptr_t StopClock = 0x4EDE90;
constexpr uintptr_t ResumeClock = 0x4E9BF0;
constexpr uintptr_t OnVictory = 0x4E5CE0;
constexpr uintptr_t Continue = 0x4DB020;
}
namespace BridgeR {
constexpr uintptr_t AddCash = 0xF27C60;
constexpr uintptr_t SetCash = 0xF38690;
constexpr uintptr_t GetCash = 0xF2DEE0;
constexpr uintptr_t GetHealth = 0xF2E7E0;
constexpr uintptr_t SellAllTowers = 0xF36B00;
constexpr uintptr_t SellAllProps = 0xF368F0;
constexpr uintptr_t StartRound = 0xF38F90;
constexpr uintptr_t ResetAbilityCooldowns = 0xF35A10;
constexpr uintptr_t DestroyAllBloons = 0xF2BA90;
constexpr uintptr_t DestroyAllProjectiles = 0xF2BAB0;
constexpr uintptr_t ClearBloons = 0xF29000;
constexpr uintptr_t GetCurrentRound = 0xF2E3B0;
constexpr uintptr_t SetRound = 0xF38C40;
constexpr uintptr_t SetEndRound = 0xF38890;
constexpr uintptr_t ResetRound = 0xF362E0;
constexpr uintptr_t ToggleWeapons = 0xF39020;
constexpr uintptr_t Win = 0xF39DA0;
constexpr uintptr_t MaxGeraldoStock = 0xF32420;
constexpr uintptr_t ResetDamages = 0xF360D0;
constexpr uintptr_t SetBlockAutoPlay = 0xF37BE0;
constexpr uintptr_t GetAvailableCash = 0xF2D490;
}
}
}
#define PIPE_NAME L"\\\\.\\pipe\\lReoons"
#define RESPONSE_MSG_SIZE 4096
enum CommandTab : uint32_t { TAB_MONKEY = 0, TAB_VALUE = 1, TAB_WORLD = 2, TAB_SYSTEM = 3, TAB_MISC = 4 };
enum SystemCmd : uint32_t { SYS_GET_STATE = 0, SYS_PING = 1, SYS_EXIT = 2 };
struct CommandPacket {
    uint32_t tab;
    uint32_t function;
    int32_t intParam;
    float floatParam;
};
struct ToggleState {
    bool infiniteCash, infiniteHealth, instantCooldowns, pauseWeapons;
    bool debugOptions, sandbox, autoPlay, blockAutoPlay;
    bool freeTowers, lockHeroPurchases, lockTowerPurchases, autoAbility;
    bool freezeSpawner, infiniteRange, godMode, doubleCash;
    bool noLivesLost, infinitePierce, maxDamage, fastForward;
};
struct ResponsePacket {
    uint32_t success;
    ToggleState state;
    char message[RESPONSE_MSG_SIZE];
};
