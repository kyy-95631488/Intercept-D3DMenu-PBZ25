#include "Hooking.h"
#include "GameHacks.h"

EndScene_t oEndScene = nullptr;

HRESULT WINAPI hkEndScene(LPDIRECT3DDEVICE9 pDevice) {
    Log("hkEndScene called");
    static bool init = false;
    if (!init) {
        g_Menu.CreateItem("ESP Name Tag", &espNameTag, 1, 0, D3DCOLOR_ARGB(255, 255, 100, 100));
        g_Menu.CreateItem("Anti-Kick", &antiKick, 1, 0, D3DCOLOR_ARGB(255, 255, 100, 100));
        g_Menu.CreateItem("Crosshair", &crosshairType, 3, 1, D3DCOLOR_ARGB(255, 255, 100, 100));
        g_Menu.CreateItem("Crosshair Color", &crosshairColorIndex, colorCount - 1, 1, D3DCOLOR_ARGB(255, 255, 100, 100));
        g_Menu.CreateItem("Crosshair Size", &crosshairSize, 20, 1, D3DCOLOR_ARGB(255, 200, 200, 200));
        g_Menu.CreateItem("Menu Color", &menuColorIndex, colorCount - 1, 1, D3DCOLOR_ARGB(255, 100, 255, 100));
        g_Menu.CreateItem("Border Color", &borderColorIndex, colorCount - 1, 1, D3DCOLOR_ARGB(255, 100, 100, 255));
        g_Menu.CreateItem("Box Style", &boxStyle, 1, 0, D3DCOLOR_ARGB(255, 100, 255, 255));
        g_Menu.CreateItem("No-Recoil", &noRecoil, 1, 0, D3DCOLOR_ARGB(255, 100, 255, 100));
        init = true;
        g_Menu.menuColorIndex = menuColorIndex;
        g_Menu.borderColorIndex = borderColorIndex;
        g_Menu.crosshairType = crosshairType;
        g_Menu.crosshairColorIndex = crosshairColorIndex;
        g_Menu.crosshairSize = crosshairSize;
        g_Menu.boxStyle = boxStyle;
        Log("Menu items initialized.");
    }
    g_Menu.menuColorIndex = menuColorIndex;
    g_Menu.borderColorIndex = borderColorIndex;
    g_Menu.crosshairType = crosshairType;
    g_Menu.crosshairColorIndex = crosshairColorIndex;
    g_Menu.crosshairSize = crosshairSize;
    g_Menu.boxStyle = boxStyle;

    ToggleESPNameTag(espNameTag != 0);
    ToggleAntiKick(antiKick != 0);
    ToggleNoRecoil(noRecoil != 0);

    g_Menu.Render(pDevice, "Intercept || Simple Menu V1");
    g_Menu.UpdateInput();
    return oEndScene(pDevice);
}

bool HookD3D() {
    HWND hwnd = nullptr;

    for (int i = 0; i < 300; i++) {
        hwnd = FindWindowA(nullptr, "Point Blank");
        if (hwnd) break;
        Log("Window not found, retrying...");
        Sleep(100);
    }

    if (!hwnd) {
        Log("Failed to find window with title \"Point Blank\" after waiting.");
        return false;
    }
    Log("Window found.");

    LPDIRECT3D9 pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (!pD3D) {
        Log("Direct3DCreate9 failed.");
        return false;
    }

    D3DPRESENT_PARAMETERS d3dpp = {};
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hwnd;

    LPDIRECT3DDEVICE9 pDevice = nullptr;
    if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDevice))) {
        Log("CreateDevice failed.");
        pD3D->Release();
        return false;
    }

    void** vtable = *reinterpret_cast<void***>(pDevice);
    EndScene_t pEndScene = (EndScene_t)vtable[42];

    if (MH_Initialize() != MH_OK) {
        Log("MinHook initialization failed.");
        pDevice->Release();
        pD3D->Release();
        return false;
    }

    if (MH_CreateHook(pEndScene, &hkEndScene, (LPVOID*)&oEndScene) != MH_OK) {
        Log("Failed to create hook.");
        pDevice->Release();
        pD3D->Release();
        return false;
    }

    if (MH_EnableHook(pEndScene) != MH_OK) {
        Log("Failed to enable hook.");
        pDevice->Release();
        pD3D->Release();
        return false;
    }

    pDevice->Release();
    pD3D->Release();
    Log("Hook applied to EndScene using MinHook.");
    MessageBoxA(nullptr, "Sukses inject ke PointBlank, Happy Cheating", "Success", MB_OK | MB_ICONINFORMATION);
    return true;
}

DWORD WINAPI InitThread(LPVOID) {
    AllocConsole();
    FILE* fDummy;
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
    freopen_s(&fDummy, "CONIN$", "r", stdin);

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD consoleMode;
    GetConsoleMode(hConsole, &consoleMode);
    consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hConsole, consoleMode);

    Log("Console allocated for logging.");
    ShowConsoleAnimation();

    CreateThread(nullptr, 0, AnimatedTitleThread, nullptr, 0, nullptr);

    while (!HookD3D()) {
        Log("Retrying hook...");
        Sleep(1000);
    }
    Log("Hooking done.");
    g_Menu.show = false;
    return 0;
}