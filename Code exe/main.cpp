#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dwmapi.h>
#include <tlhelp32.h>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <string>
#include "offsets.h"
#pragma comment(lib, "dwmapi.lib")
using namespace Offsets;
#define RST  "\033[0m"
#define R1   "\033[38;2;80;0;0m"
#define R2   "\033[38;2;140;15;15m"
#define R3   "\033[38;2;200;30;30m" 
#define R4   "\033[38;2;255;50;50m"  
#define R5   "\033[38;2;255;100;100m" 
#define R6   "\033[38;2;255;160;160m"
#define DIM  "\033[38;2;100;40;40m" 
#define GRY  "\033[38;2;120;80;80m" 
#define WHT  "\033[38;2;220;200;200m"
#define GRN  "\033[38;2;60;220;60m"   
#define OFF  "\033[38;2;90;40;40m"   
#define SUC  "\033[38;2;255;80;80m"  
#define ERR  "\033[38;2;180;0;0m"   
#define BG   "\033[48;2;15;0;0m"    
static const wchar_t* GAME_PROCESS = L"BloonsTD6.exe";
static const wchar_t* DLL_NAME = L"lReoons.dll";
static void EnableVT() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(h, &mode);
    SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}
static void SetupConsoleWindow() {
    HWND hw = GetConsoleWindow();
    if (!hw) return;
    LONG style = GetWindowLongW(hw, GWL_STYLE);
    style &= ~(WS_MAXIMIZEBOX | WS_THICKFRAME);
    SetWindowLongW(hw, GWL_STYLE, style);
    SetWindowLongW(hw, GWL_EXSTYLE, GetWindowLongW(hw, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(hw, 0, 230, LWA_ALPHA);
    SetWindowPos(hw, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
}
static DWORD FindProcess(const wchar_t* name) {
    HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (h == INVALID_HANDLE_VALUE) return 0;
    PROCESSENTRY32W pe = {}; pe.dwSize = sizeof(pe);
    DWORD pid = 0;
    if (Process32FirstW(h, &pe)) do {
        if (_wcsicmp(pe.szExeFile, name) == 0) { pid = pe.th32ProcessID; break; }
    } while (Process32NextW(h, &pe));
    CloseHandle(h); return pid;
}
static bool InjectDLL(DWORD pid, const wchar_t* dll) {
    wchar_t full[MAX_PATH] = {};
    GetFullPathNameW(dll, MAX_PATH, full, nullptr);
    HANDLE hp = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hp) return false;
    size_t sz = (wcslen(full) + 1) * sizeof(wchar_t);
    void* mem = VirtualAllocEx(hp, nullptr, sz, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!mem) { CloseHandle(hp); return false; }
    WriteProcessMemory(hp, mem, full, sz, nullptr);
    HMODULE k32 = GetModuleHandleA("kernel32.dll");
    HANDLE ht = CreateRemoteThread(hp, nullptr, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(k32, "LoadLibraryW"), mem, 0, nullptr);
    if (!ht) { VirtualFreeEx(hp, mem, 0, MEM_RELEASE); CloseHandle(hp); return false; }
    WaitForSingleObject(ht, 5000);
    VirtualFreeEx(hp, mem, 0, MEM_RELEASE);
    CloseHandle(ht); CloseHandle(hp);
    return true;
}
class PipeClient {
    HANDLE hp = INVALID_HANDLE_VALUE;
public:
    bool Connect() {
        for (int i = 0; i < 20; i++) {
            hp = CreateFileW(PIPE_NAME, GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
            if (hp != INVALID_HANDLE_VALUE) {
                DWORD mode = PIPE_READMODE_MESSAGE;
                SetNamedPipeHandleState(hp, &mode, nullptr, nullptr);
                return true;
            }
            Sleep(500);
        }
        return false;
    }
    ResponsePacket Send(uint32_t tab, uint32_t fn, int32_t ip = 0, float fp = 0.f) {
        CommandPacket cmd = { tab,fn,ip,fp }; ResponsePacket r = {};
        DWORD bw = 0, br = 0;
        if (hp == INVALID_HANDLE_VALUE) return r;
        if (!WriteFile(hp, &cmd, sizeof(cmd), &bw, nullptr)) return r;
        FlushFileBuffers(hp);
        if (!ReadFile(hp, &r, sizeof(r), &br, nullptr)) return r;
        return r;
    }
    ToggleState GetState() { return Send(TAB_SYSTEM, SYS_GET_STATE).state; }
    bool Ping() { return Send(TAB_SYSTEM, SYS_PING).success != 0; }
    void Exit() { Send(TAB_SYSTEM, SYS_EXIT); }
    void Close() { if (hp != INVALID_HANDLE_VALUE) { CloseHandle(hp); hp = INVALID_HANDLE_VALUE; } }
};
static PipeClient pipe;
static int currentTab = 0;
static int page = 0;

static void Cls() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(h, &csbi);
    DWORD cells = csbi.dwSize.X * csbi.dwSize.Y;
    DWORD written;
    COORD origin = { 0, 0 };
    FillConsoleOutputCharacterW(h, L' ', cells, origin, &written);
    FillConsoleOutputAttribute(h, csbi.wAttributes, cells, origin, &written);
    SetConsoleCursorPosition(h, origin);
}
static void Pt(const char* label, bool state) {
    printf("  %s%-18s %s%s" RST "\n", DIM, label, state ? GRN : OFF, state ? "ON" : "OFF");
}
static void Ph(const char* h) {
    printf("\n " R3 "--- " R4 "%s" R3 " ---" RST "\n", h);
}
static int ReadInt(const char* prompt) {
    printf("  " R5 "%s: " WHT, prompt);
    int v = 0; scanf_s("%d", &v); return v;
}
static float ReadFloat(const char* prompt) {
    printf("  " R5 "%s: " WHT, prompt);
    float v = 0; scanf_s("%f", &v); return v;
}
static void PrintGradientBanner() {
    printf("\n");
    printf("  " R1 "l" R2 "R" R3 "e" R4 "o" R5 "o" R4 "l" R3 "n" R2 "s" RST "\n");
    printf("  " DIM "~~~~~~~~~" RST "\n");
}
static void DrawHeader() {
    PrintGradientBanner();
    const char* tabs[] = { "[M]onkey","[V]alue","[W]orld","mi[X]c" };
    const char* cols[] = { R4, R3, R2, R1 };
    for (int i = 0; i < 4; i++) {
        printf("  %s%s" RST, i == currentTab ? R4 : DIM, tabs[i]);
    }
    printf("\n");
    printf("  " GRY "[Q]uit  [R]efresh  Page:%d" RST "\n", page);
    printf("  " R1 "--------------------------------------" RST "\n");
}
static void DrawMonkey() {
    auto st = pipe.GetState();
    Ph("MONKEY  Tower Control");
    printf(R5 "  1)  Sell all towers        2)  Sell all props\n");
    printf("  3)  Reset ability CDs      4)  Clear ALL ability CDs\n");
    printf("  5)  Activate all abilities 6)  Deactivate all abilities\n");
    printf("  7)  Pause weapons          8)  Instant cooldowns\n");
    printf("  9)  Toggle weapons         10) Set weapon rate\n");
    printf("  11) Set weapon CD          12) Enable all weapons\n");
    printf("  13) Disable all weapons    14) Set all tower worth\n");
    printf("  15) Set all tower dmg      16) Reset damages\n");
    printf("  17) Max Geraldo stock      18) Set ability CD\n");
    printf("  19) Set ability max CD     20) All abilities ready\n");
    printf("  21) Set cash earned        22) Set owner\n");
    printf("  23) Set selectable         24) Delete ALL towers\n");
    printf("  25) Lock hero purchases    26) Lock tower purchases\n");
    printf("  27) Auto ability           28) Free towers\n");
    printf("  29) Zero weapon CDs        30) Abilities changed\n" RST);
    Ph("Individual Tower [i]");
    printf(R6 "  31) Upgrade tower[i]       32) Add cash to tower[i]\n");
    printf("  33) Set target type[i]     34) Spawn all darts\n");
    printf("  35) Upgrade paragon[i]     36) Mgr sell tower[i]\n");
    printf("  37) Mgr destroy tower[i]   38) Sell tower direct[i]\n");
    printf("  40) Sell tower[i]          41) Set tower worth[i]\n");
    printf("  42) Set tower dmg[i]       43) Clear tower CD[i]\n");
    printf("  44) Activate tower ab[i]   45) Disable tower wpn[i]\n");
    printf("  46) Enable tower wpn[i]    47) Set tower wpn rate[i]\n");
    printf("  48) Set tower select[i]    49) Destroy tower[i]\n");
    printf("  50) Info tower[i]          51) List towers (page)\n" RST);
    Ph("Toggles");
    Pt("Instant CDs", st.instantCooldowns);
    Pt("Pause Weapons", st.pauseWeapons);
    Pt("Lock Hero", st.lockHeroPurchases);
    Pt("Lock Tower", st.lockTowerPurchases);
    Pt("Auto Ability", st.autoAbility);
    Pt("Free Towers", st.freeTowers);
}
static void DrawValue() {
    auto st = pipe.GetState();
    Ph("VALUE  Economy & Health");
    printf(R5 "  1)  Set cash               2)  Add cash\n");
    printf("  3)  Infinite cash          4)  Set health\n");
    printf("  5)  Set health (direct)    6)  Infinite health\n");
    printf("  7)  Reset health           8)  Infinite health mode\n");
    printf("  9)  Set max health         10) Set shield\n");
    printf("  11) Set revives            12) Take damage\n");
    printf("  13) Double cash            14) No lives lost\n");
    printf("  15) Set cash spent         16) Cash changed flag\n");
    printf("  17) Health changed flag    18) Bloon override\n");
    printf("  19) God mode               20) Debug options\n");
    printf("  21) Set tier count\n" RST);
    Ph("Extended");
    printf(R6 "  22) Remove cash            23) Add cash (bridge)\n");
    printf("  24) Set cash (bridge)      25) Send cash to player\n");
    printf("  26) Add shared cash        27) Cheat set health\n");
    printf("  28) Set starting health    29) Set softcap mod\n");
    printf("  30) Simulation info        31) InGame info\n");
    printf("  32) Set max shield\n" RST);
    Ph("Toggles");
    Pt("Infinite Cash", st.infiniteCash);
    Pt("Infinite Health", st.infiniteHealth);
    Pt("God Mode", st.godMode);
    Pt("Double Cash", st.doubleCash);
    Pt("No Lives Lost", st.noLivesLost);
    Pt("Debug Options", st.debugOptions);
}
static void DrawWorld() {
    auto st = pipe.GetState();
    Ph("WORLD  Bloons & Rounds");
    printf(R5 "  1)  Destroy all bloons     2)  Delete bloons (IG)\n");
    printf("  3)  Destroy projectiles    4)  Clear bloons\n");
    printf("  5)  Start round (sim)      6)  Start round (bridge)\n");
    printf("  7)  Set round              8)  Set end round\n");
    printf("  9)  Reset round            10) Auto play\n");
    printf("  11) Block auto play        12) Pause auto play\n");
    printf("  13) Sandbox                14) Freeze spawner\n");
    printf("  15) Set spawner round      16) Set all bloon HP\n");
    printf("  17) Damage all bloons      18) Kill all bloons\n");
    printf("  19) Set bloon distance     20) Bloons to exit\n");
    printf("  21) Set round mutators     22) Set victory\n");
    printf("  23) Set defeat             24) Win (bridge)\n");
    printf("  25) Restart                26) Quick restart\n");
    printf("  27) Start free play        28) Force defeat\n");
    printf("  29) Continue               30) Stop clock\n");
    printf("  31) Resume clock           32) Fast forward\n");
    printf("  33) Cheat set round        34) Set match won\n");
    printf("  35) Set match lost         36) Set game started\n");
    printf("  37) Set game lost          38) Set game won\n");
    printf("  39) Quit game\n" RST);
    Ph("Extended");
    printf(R6 "  40) Spawner start round    41) Spawner set round\n");
    printf("  42) On victory             43) BloonMgr damage[i]\n");
    printf("  44) BloonMgr destroy[i]    45) Match just won flag\n");
    printf("  50) List bloons (page)     51) Overview\n" RST);
    Ph("Toggles");
    Pt("Auto Play", st.autoPlay);
    Pt("Block Auto Play", st.blockAutoPlay);
    Pt("Sandbox", st.sandbox);
    Pt("Freeze Spawner", st.freezeSpawner);
    Pt("Fast Forward", st.fastForward);
}
static void DrawMisc() {
    auto st = pipe.GetState();
    Ph("MISC  Extras");
    printf(R5 "  1)  Infinite pierce        2)  Max damage\n");
    printf("  3)  Set show cancel        4)  Set end round time\n");
    printf("  5)  Set can earn XP        6)  Play btn disabled\n");
    printf("  7)  Map editor sandbox     8)  Flush process\n");
    printf("  9)  Leak damage flag\n");
    printf("  10) Set bloon[i] HP        11) Destroy bloon[i]\n");
    printf("  12) Set bloon[i] dist\n" RST);
    Ph("Extended (RVA)");
    printf(R6 "  13) Bloon damage[i]        14) Bloon leak[i]\n");
    printf("  15) Bloon reset HP[i]      16) Bloon set HP (RVA)[i]\n");
    printf("  17) Weapon spawn dart[i]   18) Weapon set enabled[i]\n");
    printf("  19) Ability set CD[i]      20) Info bloon[i]\n");
    printf("  21) Info tower[i]          22) Ability clear CD[i]\n");
    printf("  23) Max health increase    24) Starting revives\n" RST);
    Ph("Toggles");
    Pt("Infinite Pierce", st.infinitePierce);
    Pt("Max Damage", st.maxDamage);
}
static void DrawTab() {
    Cls(); DrawHeader();
    switch (currentTab) {
    case 0: DrawMonkey(); break; case 1: DrawValue(); break;
    case 2: DrawWorld(); break;  case 3: DrawMisc(); break;
    }
}
static bool NeedsInt(int tab, int fn) {
    if (tab == 0) return (fn >= 31 && fn <= 38) || (fn >= 40 && fn <= 50);
    if (tab == 1) return fn == 1 || fn == 2 || fn == 11 || fn == 15 || fn == 16 || fn == 17 || fn == 18 || fn == 21 || fn == 22 || fn == 23 || fn == 24 || fn == 25 || fn == 26;
    if (tab == 2) return fn == 7 || fn == 8 || fn == 15 || fn == 21 || fn == 33 || fn == 34 || fn == 35 || fn == 36 || fn == 37 || fn == 38 || fn == 41 || fn == 43 || fn == 44 || fn == 45;
    if (tab == 3) return fn == 3 || fn == 5 || fn == 6 || fn == 7 || fn == 9 || fn == 10 || fn == 11 || fn == 12 || fn == 13 || fn == 14 || fn == 15 || fn == 16 || fn == 17 || fn == 18 || fn == 19 || fn == 22 || fn == 24;
    return false;
}
static bool NeedsFloat(int tab, int fn) {
    if (tab == 0) return fn == 10 || fn == 11 || fn == 14 || fn == 15 || fn == 18 || fn == 19 || fn == 21 || fn == 32 || fn == 33 || fn == 41 || fn == 42 || fn == 47 || fn == 48;
    if (tab == 1) return fn == 4 || fn == 5 || fn == 9 || fn == 10 || fn == 12 || fn == 27 || fn == 28 || fn == 29 || fn == 32;
    if (tab == 2) return fn == 16 || fn == 17 || fn == 19;
    if (tab == 3) return fn == 4 || fn == 10 || fn == 12 || fn == 13 || fn == 16 || fn == 18 || fn == 19 || fn == 23;
    return false;
}
static uint32_t TabToCmd(int t) {
    switch (t) {
    case 0: return TAB_MONKEY; case 1: return TAB_VALUE;
    case 2: return TAB_WORLD;  case 3: return TAB_MISC;
    default: return TAB_MONKEY;
    }
}
int main() {
    EnableVT();
    SetConsoleTitleW(L"lReoolns");
    SetupConsoleWindow();
    printf(BG);
    printf("\n" R3 "  Waiting for " R4 "BloonsTD6.exe" R3 "..." RST "\n");
    DWORD pid = 0;
    while (!pid) { pid = FindProcess(GAME_PROCESS); if (!pid) Sleep(1000); }
    printf(R5 "  Found PID: %lu" RST "\n", pid);
    printf(R3 "  Injecting..." RST "\n");
    if (!InjectDLL(pid, DLL_NAME)) {
        printf(ERR "  Injection failed!" RST "\n");
        system("pause"); return 1;
    }
    printf(R4 "  Injected! Connecting..." RST "\n");
    if (!pipe.Connect()) {
        printf(ERR "  Pipe connection failed!" RST "\n");
        system("pause"); return 1;
    }
    printf(SUC "  Connected!" RST "\n"); Sleep(400);
    DrawTab();
    while (true) {
        printf("\n" R4 " > " WHT);
        char buf[64] = {}; fgets(buf, sizeof(buf), stdin);
        char c = buf[0];
        if (c == 'q' || c == 'Q') { pipe.Exit(); break; }
        if (c == 'r' || c == 'R') { DrawTab(); continue; }
        if (c == 'm' || c == 'M') { currentTab = 0; page = 0; DrawTab(); continue; }
        if (c == 'v' || c == 'V') { currentTab = 1; page = 0; DrawTab(); continue; }
        if (c == 'w' || c == 'W') { currentTab = 2; page = 0; DrawTab(); continue; }
        if (c == 'x' || c == 'X') { currentTab = 3; page = 0; DrawTab(); continue; }
        if (c == '+' || c == '=') { page++; DrawTab(); continue; }
        if (c == '-' || c == '_') { if (page > 0) page--; DrawTab(); continue; }
        int fn = atoi(buf);
        if (fn <= 0) { DrawTab(); continue; }
        int ip = 0; float fp = 0;
        if (NeedsInt(currentTab, fn)) ip = ReadInt("int");
        if (NeedsFloat(currentTab, fn)) fp = ReadFloat("float");
        auto r = pipe.Send(TabToCmd(currentTab), fn, ip, fp);
        printf("  %s%s" RST "\n", r.success ? SUC : ERR, r.message);
    }
    pipe.Close();
    printf(RST);
    return 0;
}
