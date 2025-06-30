#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <MinHook.h>
#include "D3DMenu.h"
#include "ConsoleAnimation.h"
#include "Logging.h"

#pragma comment(lib, "libMinHook.x86.lib")

typedef HRESULT(WINAPI* EndScene_t)(LPDIRECT3DDEVICE9);
extern EndScene_t oEndScene;

HRESULT WINAPI hkEndScene(LPDIRECT3DDEVICE9 pDevice);
bool HookD3D();
DWORD WINAPI InitThread(LPVOID);