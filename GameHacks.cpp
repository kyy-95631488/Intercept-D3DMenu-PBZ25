#include "GameHacks.h"
#include "Logging.h"

DWORD_PTR recoilJumpBack = 0;
DWORD_PTR recoilJumpCondition = 0;

void __declspec(naked) noRecoilNewmem() {
    __asm {
        mov al, 0
        test al, al
        je continueRecoilPatch
        jmp recoilJumpBack

        continueRecoilPatch :
        jmp recoilJumpCondition
    }
}

void ToggleESPNameTag(bool enable) {
    static BYTE originalBytes[6];
    static bool isPatched = false;
    static LPVOID targetAddress = nullptr;

    if (!targetAddress) {
        HMODULE hModule = GetModuleHandleA("PointBlank.exe");
        if (hModule) {
            targetAddress = (LPVOID)((DWORD)hModule + 0x62CB10);
            Log("Target address for ESP Name Tag: %p", targetAddress);
        }
        else {
            Log("Failed to get PointBlank.exe module handle.");
            return;
        }
    }

    if (enable && !isPatched) {
        ReadProcessMemory(GetCurrentProcess(), targetAddress, originalBytes, 6, nullptr);
        BYTE patchOn[] = { 0xC6, 0x01, 0x01, 0x8D, 0x49, 0x0C };
        WriteProcessMemory(GetCurrentProcess(), targetAddress, patchOn, sizeof(patchOn), nullptr);
        isPatched = true;
        Log("ESP Name Tag enabled.");
    }
    else if (!enable && isPatched) {
        WriteProcessMemory(GetCurrentProcess(), targetAddress, originalBytes, 6, nullptr);
        isPatched = false;
        Log("ESP Name Tag disabled.");
    }
}

void ToggleAntiKick(bool enable) {
    static BYTE originalBytes[2];
    static bool isPatched = false;
    static LPVOID targetAddress = nullptr;

    if (!targetAddress) {
        HMODULE hModule = GetModuleHandleA("PointBlank.exe");
        if (hModule) {
            targetAddress = (LPVOID)((DWORD)hModule + 0x78C63C);
            Log("Target address for Anti-Kick: %p", targetAddress);
        }
        else {
            Log("Failed to get PointBlank.exe module handle for Anti-Kick.");
            return;
        }
    }

    if (enable && !isPatched) {
        ReadProcessMemory(GetCurrentProcess(), targetAddress, originalBytes, 2, nullptr);
        BYTE patchOn[] = { 0xEB, 0x15 };
        WriteProcessMemory(GetCurrentProcess(), targetAddress, patchOn, sizeof(patchOn), nullptr);
        isPatched = true;
        Log("Anti-Kick enabled.");
    }
    else if (!enable && isPatched) {
        BYTE patchOff[] = { 0x78, 0x15 };
        WriteProcessMemory(GetCurrentProcess(), targetAddress, patchOff, sizeof(patchOff), nullptr);
        isPatched = false;
        Log("Anti-Kick disabled.");
    }
}

void ToggleNoRecoil(bool enable) {
    static BYTE originalBytes[8] = {};
    static bool isPatched = false;
    static LPVOID targetAddress = nullptr;

    if (!targetAddress) {
        HMODULE hModule = GetModuleHandleA("PointBlank.exe");
        if (hModule) {
            targetAddress = (LPVOID)((DWORD_PTR)hModule + 0x627776);
            Log("Target address for No Recoil: %p", targetAddress);
        }
        else {
            Log("Failed to get PointBlank.exe module handle for No Recoil.");
            return;
        }
    }

    if (!recoilJumpBack || !recoilJumpCondition) {
        HMODULE hModule = GetModuleHandleA("PointBlank.exe");
        if (hModule) {
            recoilJumpBack = (DWORD_PTR)hModule + 0x62777B;
            recoilJumpCondition = (DWORD_PTR)hModule + 0x6279AC;
        }
    }

    if (enable && !isPatched) {
        ReadProcessMemory(GetCurrentProcess(), targetAddress, originalBytes, 8, nullptr);

        DWORD oldProtect;
        VirtualProtect(targetAddress, 8, PAGE_EXECUTE_READWRITE, &oldProtect);

        BYTE patch[8] = { 0xE9, 0x00, 0x00, 0x00, 0x00, 0x90, 0x90, 0x90 };
        DWORD relativeAddress = ((DWORD_PTR)&noRecoilNewmem - (DWORD_PTR)targetAddress) - 5;
        memcpy(&patch[1], &relativeAddress, 4);

        memcpy(targetAddress, patch, 8);

        VirtualProtect(targetAddress, 8, oldProtect, &oldProtect);

        isPatched = true;
        Log("No Recoil enabled.");
    }
    else if (!enable && isPatched) {
        DWORD oldProtect;
        VirtualProtect(targetAddress, 8, PAGE_EXECUTE_READWRITE, &oldProtect);
        memcpy(targetAddress, originalBytes, 8);
        VirtualProtect(targetAddress, 8, oldProtect, &oldProtect);

        isPatched = false;
        Log("No Recoil disabled.");
    }
}