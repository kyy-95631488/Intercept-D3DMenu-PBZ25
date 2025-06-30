#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <ctime>
#include <cmath>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

struct Color {
    const char* name;
    DWORD value;
};

extern Color predefinedColors[];
extern const int colorCount;

struct MenuItem {
    const char* title;
    int* hack;
    int maxval;
    int type;
    DWORD color;
};

class D3DMenu {
public:
    LPD3DXFONT pFont;
    LPD3DXSPRITE pSprite;
    LPD3DXLINE pLine;
    int itemCount;
    int selector;
    int x, y, w, h;
    int baseW, baseH;
    float scaleFactor;
    DWORD titleColor;
    DWORD backColor;
    DWORD borderColor;
    DWORD shadowColor;
    MenuItem items[50];
    bool show;
    float animProgress;
    float rgbAnimProgress;
    int menuColorIndex;
    int borderColorIndex;
    int crosshairColorIndex;
    int boxStyle;
    int crosshairType;
    int crosshairSize;
    int espNameTag;
    ULONGLONG lastKeyTime;
    const ULONGLONG keyDelay;

    D3DMenu();
    DWORD GetRGBColor(float progress);
    void CalculateDimensions(LPDIRECT3DDEVICE9 device);
    void CreateItem(const char* title, int* hack, int maxval, int type, DWORD color);
    void InitFont(LPDIRECT3DDEVICE9 device);
    void DrawText(LPDIRECT3DDEVICE9 device, const char* text, int x, int y, DWORD color, bool shadow, float textAnimProgress);
    void DrawBox(LPDIRECT3DDEVICE9 device, int x, int y, int w, int h, DWORD fill, DWORD border);
    void DrawSelectionBox(LPDIRECT3DDEVICE9 device, int x, int y, int w, int h, float progress);
    void DrawCrosshair(LPDIRECT3DDEVICE9 device);
    void DrawBorderAimPov(LPDIRECT3DDEVICE9 device, int espNameTag);
    std::string GetCurrentTime();
    void Render(LPDIRECT3DDEVICE9 device, const char* title);
    void UpdateInput();
    void Release();
};

extern D3DMenu g_Menu;
extern int espNameTag;
extern int antiKick;
extern int menuColorIndex;
extern int borderColorIndex;
extern int crosshairType;
extern int crosshairColorIndex;
extern int crosshairSize;
extern int boxStyle;
extern int noRecoil;