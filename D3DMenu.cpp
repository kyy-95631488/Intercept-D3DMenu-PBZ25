#include "D3DMenu.h"
#include "Logging.h"
#include <string>
#include <ctime>

Color predefinedColors[] = {
    {"White", D3DCOLOR_ARGB(255, 240, 240, 240)},
    {"Red", D3DCOLOR_ARGB(255, 255, 85, 85)},
    {"Green", D3DCOLOR_ARGB(255, 85, 255, 85)},
    {"Blue", D3DCOLOR_ARGB(255, 85, 155, 255)},
    {"Yellow", D3DCOLOR_ARGB(255, 255, 255, 85)},
    {"Cyan", D3DCOLOR_ARGB(255, 85, 255, 255)},
    {"Magenta", D3DCOLOR_ARGB(255, 255, 85, 255)},
    {"Orange", D3DCOLOR_ARGB(255, 255, 165, 85)},
    {"Purple", D3DCOLOR_ARGB(255, 155, 85, 255)},
    {"Pink", D3DCOLOR_ARGB(255, 255, 192, 203)},
    {"RGB", D3DCOLOR_ARGB(255, 240, 240, 240)}
};
const int colorCount = sizeof(predefinedColors) / sizeof(predefinedColors[0]);

D3DMenu g_Menu;
int espNameTag = 0;
int antiKick = 0;
int menuColorIndex = 3;
int borderColorIndex = 3;
int crosshairType = 0;
int crosshairColorIndex = 1;
int crosshairSize = 5;
int boxStyle = 1;
int noRecoil = 0;

D3DMenu::D3DMenu() : pFont(nullptr), pSprite(nullptr), pLine(nullptr), itemCount(0), selector(0),
x(0), y(0), w(0), h(0), baseW(280), baseH(320), scaleFactor(1.0f),
titleColor(D3DCOLOR_ARGB(255, 220, 220, 220)),
backColor(D3DCOLOR_ARGB(240, 20, 20, 30)),
borderColor(D3DCOLOR_ARGB(255, 85, 155, 255)),
shadowColor(D3DCOLOR_ARGB(180, 10, 10, 20)),
show(true), animProgress(0.0f), rgbAnimProgress(0.0f),
menuColorIndex(3), borderColorIndex(3), crosshairColorIndex(1),
boxStyle(1), crosshairType(0), crosshairSize(5), espNameTag(0),
lastKeyTime(0), keyDelay(150) {
}

DWORD D3DMenu::GetRGBColor(float progress) {
    float r = sinf(progress * 2.0f * D3DX_PI) * 127.5f + 127.5f;
    float g = sinf(progress * 2.0f * D3DX_PI + 2.0f * 3.14159f / 3.0f) * 127.5f + 127.5f;
    float b = sinf(progress * 2.0f * D3DX_PI + 4.0f * 3.14159f / 3.0f) * 127.5f + 127.5f;
    return D3DCOLOR_ARGB(255, (BYTE)r, (BYTE)g, (BYTE)b);
}

void D3DMenu::CalculateDimensions(LPDIRECT3DDEVICE9 device) {
    D3DVIEWPORT9 vp;
    device->GetViewport(&vp);
    float screenW = (float)vp.Width;
    float screenH = (float)vp.Height;

    scaleFactor = min(screenW / 1920.0f, screenH / 1080.0f);
    w = (int)(baseW * scaleFactor);
    h = (int)((60 + itemCount * 24 + 40) * scaleFactor);
    x = (int)(50 * scaleFactor);
    y = (int)(50 * scaleFactor);
}

void D3DMenu::CreateItem(const char* title, int* hack, int maxval, int type, DWORD color) {
    if (itemCount < 50) {
        items[itemCount++] = { title, hack, maxval, type, color };
        Log("Menu item created: %s", title);
    }
}

void D3DMenu::InitFont(LPDIRECT3DDEVICE9 device) {
    if (!pFont && device) {
        int fontSize = (int)(17 * scaleFactor);
        if (SUCCEEDED(D3DXCreateFontA(device, fontSize, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
            "Inter", &pFont))) {
            Log("Font initialized (Inter).");
        }
        else {
            Log("Font initialization failed, falling back to Arial.");
            D3DXCreateFontA(device, fontSize, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                "Arial", &pFont);
        }
    }
    if (!pSprite && device) {
        if (SUCCEEDED(D3DXCreateSprite(device, &pSprite))) {
            Log("Sprite initialized.");
        }
        else {
            Log("Sprite initialization failed.");
        }
    }
    if (!pLine && device) {
        if (SUCCEEDED(D3DXCreateLine(device, &pLine))) {
            Log("Line initialized.");
            pLine->SetWidth(1.5f * scaleFactor);
            pLine->SetAntialias(TRUE);
        }
        else {
            Log("Line initialization failed.");
        }
    }
}

void D3DMenu::DrawText(LPDIRECT3DDEVICE9 device, const char* text, int x, int y, DWORD color, bool shadow, float textAnimProgress) {
    if (!pFont) InitFont(device);
    if (pFont && pSprite) {
        BYTE alpha = (BYTE)(255 * (0.8f + 0.2f * sin(textAnimProgress * 2.0f * D3DX_PI)));
        DWORD animColor = D3DCOLOR_ARGB(alpha, (BYTE)GetRValue(color), (BYTE)GetGValue(color), (BYTE)GetBValue(color));

        RECT rect = { x, y, x + w, y + (int)(22 * scaleFactor) };
        if (shadow) {
            RECT shadowRect = { x + (int)(1 * scaleFactor), y + (int)(1 * scaleFactor), x + w + (int)(1 * scaleFactor), y + (int)(23 * scaleFactor) };
            pFont->DrawTextA(pSprite, text, -1, &shadowRect, DT_LEFT | DT_NOCLIP, shadowColor);
        }
        pFont->DrawTextA(pSprite, text, -1, &rect, DT_LEFT | DT_NOCLIP, animColor);
    }
}

void D3DMenu::DrawBox(LPDIRECT3DDEVICE9 device, int x, int y, int w, int h, DWORD fill, DWORD border) {
    // Subtle gradient background
    if (boxStyle == 1) {
        D3DRECT mainRect = { x, y, x + w, y + h };
        device->Clear(1, &mainRect, D3DCLEAR_TARGET, fill, 0, 0);

        D3DRECT gradientRect = { x, y, x + w, y + (int)(h * 0.3f) };
        device->Clear(1, &gradientRect, D3DCLEAR_TARGET, D3DCOLOR_ARGB(240, 30, 30, 40), 0, 0);
    }

    // Thinner, modern border
    int borderWidth = (int)(1.5f * scaleFactor);
    DWORD animatedBorderColor = (borderColorIndex == colorCount - 1) ? GetRGBColor(rgbAnimProgress) : predefinedColors[borderColorIndex].value;
    D3DRECT borders[] = {
        { x, y, x + w, y + borderWidth },                     // Top
        { x, y + h - borderWidth, x + w, y + h },            // Bottom
        { x, y + borderWidth, x + borderWidth, y + h - borderWidth }, // Left
        { x + w - borderWidth, y + borderWidth, x + w, y + h - borderWidth } // Right
    };
    for (auto& r : borders)
        device->Clear(1, &r, D3DCLEAR_TARGET, animatedBorderColor, 0, 0);

    // Subtle shadow
    D3DRECT shadow[] = {
        { x + (int)(1 * scaleFactor), y + h, x + w + (int)(1 * scaleFactor), y + h + (int)(2 * scaleFactor) },
        { x + w, y + (int)(1 * scaleFactor), x + w + (int)(2 * scaleFactor), y + h + (int)(1 * scaleFactor) }
    };
    for (auto& r : shadow)
        device->Clear(1, &r, D3DCLEAR_TARGET, shadowColor, 0, 0);
}

void D3DMenu::DrawSelectionBox(LPDIRECT3DDEVICE9 device, int x, int y, int w, int h, float progress) {
    DWORD selColor = predefinedColors[menuColorIndex].value;
    DWORD selFade = D3DCOLOR_ARGB((int)(180 * (1.0f - progress)),
        (BYTE)(GetRValue(selColor) * 0.7f),
        (BYTE)(GetGValue(selColor) * 0.7f),
        (BYTE)(GetBValue(selColor) * 0.7f));
    D3DRECT selRect = { x, y, x + w, y + h };
    device->Clear(1, &selRect, D3DCLEAR_TARGET, selFade, 0, 0);
    // Minimalist selection bar
    D3DRECT selBorder = { x, y + h - (int)(1.5f * scaleFactor), x + w, y + h };
    device->Clear(1, &selBorder, D3DCLEAR_TARGET, selColor, 0, 0);
}

void D3DMenu::DrawCrosshair(LPDIRECT3DDEVICE9 device) {
    if (crosshairType == 0) return;

    D3DVIEWPORT9 vp;
    device->GetViewport(&vp);
    float centerX = vp.Width / 2.0f;
    float centerY = vp.Height / 2.0f;
    DWORD crosshairColor = (crosshairColorIndex == colorCount - 1) ? GetRGBColor(rgbAnimProgress) : predefinedColors[crosshairColorIndex].value;

    if (pLine) {
        pLine->Begin();
        if (crosshairType == 1) {
            D3DXVECTOR2 points[2] = { {centerX, centerY}, {centerX + 1, centerY} };
            pLine->SetWidth((float)crosshairSize * scaleFactor);
            pLine->Draw(points, 2, crosshairColor);
        }
        else if (crosshairType == 2) {
            D3DXVECTOR2 pointsH[2] = { {centerX - crosshairSize * scaleFactor, centerY}, {centerX + crosshairSize * scaleFactor, centerY} };
            D3DXVECTOR2 pointsV[2] = { {centerX, centerY - crosshairSize * scaleFactor}, {centerX, centerY + crosshairSize * scaleFactor} };
            pLine->SetWidth(1.5f * scaleFactor);
            pLine->Draw(pointsH, 2, crosshairColor);
            pLine->Draw(pointsV, 2, crosshairColor);
        }
        else if (crosshairType == 3) {
            const int segments = 32;
            D3DXVECTOR2 points[33];
            float radius = (float)crosshairSize * scaleFactor;
            for (int i = 0; i <= segments; i++) {
                float angle = (float)i / segments * 2.0f * D3DX_PI;
                points[i] = D3DXVECTOR2(centerX + radius * cosf(angle), centerY + radius * sinf(angle));
            }
            pLine->SetWidth(1.5f * scaleFactor);
            pLine->Draw(points, segments + 1, crosshairColor);
        }
        pLine->End();
    }
}

void D3DMenu::DrawBorderAimPov(LPDIRECT3DDEVICE9 device, int espNameTag) {
    if (!pLine || !pFont || !pSprite) {
        InitFont(device);
        if (!pLine || !pFont || !pSprite) {
            Log("Failed to initialize pLine, pFont, or pSprite for Border Aim/POV.");
            return;
        }
    }

    pLine->Begin();
    D3DVIEWPORT9 vp;
    device->GetViewport(&vp);
    float centerX = vp.Width / 2.0f;
    float centerY = vp.Height / 2.0f;

    float boxWidth = 35.0f * scaleFactor;
    float boxHeight = 35.0f * scaleFactor;
    D3DXVECTOR2 points[5] = {
        {centerX - boxWidth / 2, centerY - boxHeight / 2},
        {centerX + boxWidth / 2, centerY - boxHeight / 2},
        {centerX + boxWidth / 2, centerY + boxHeight / 2},
        {centerX - boxWidth / 2, centerY + boxHeight / 2},
        {centerX - boxWidth / 2, centerY - boxHeight / 2}
    };
    pLine->SetWidth(1.5f * scaleFactor);
    pLine->Draw(points, 5, D3DCOLOR_ARGB(255, 255, 85, 85));

    if (espNameTag) {
        const char* nickname = "Player";
        DWORD lineColor = D3DCOLOR_ARGB(255, 255, 85, 85);

        int textHeight = (int)(18 * scaleFactor);
        int textY = (int)(centerY - boxHeight / 2 - textHeight - 4 * scaleFactor);
        int textX = (int)(centerX - (strlen(nickname) * 7 * scaleFactor) / 2);

        DrawText(device, nickname, textX, textY, lineColor, true, 1.0f);

        D3DXVECTOR2 linePoints[2] = {
            {centerX, centerY - boxHeight / 2},
            {centerX, (float)textY + textHeight}
        };
        pLine->SetWidth(1.5f * scaleFactor);
        pLine->Draw(linePoints, 2, lineColor);
    }

    pLine->End();
}

std::string D3DMenu::GetCurrentTime() {
    std::time_t now = std::time(nullptr);
    struct tm tstruct;
    char buf[80];
    localtime_s(&tstruct, &now);
    strftime(buf, sizeof(buf), "%H:%M:%S", &tstruct);
    return std::string(buf);
}

void D3DMenu::Render(LPDIRECT3DDEVICE9 device, const char* title) {
    CalculateDimensions(device);

    ULONGLONG currentTime = GetTickCount64();

    if (GetAsyncKeyState(VK_INSERT) & 0x8000) {
        if (currentTime - lastKeyTime >= keyDelay) {
            show = !show;
            Log("INSERT pressed, menu %s", show ? "shown" : "hidden");
            lastKeyTime = currentTime;
        }
    }

    if (pSprite) pSprite->Begin(D3DXSPRITE_ALPHABLEND);

    DrawCrosshair(device);
    DrawBorderAimPov(device, espNameTag);

    if (!show) {
        if (pSprite) pSprite->End();
        return;
    }

    animProgress += 0.03f;
    if (animProgress > 1.0f) animProgress = 0.0f;
    rgbAnimProgress += 0.015f;
    if (rgbAnimProgress > 1.0f) rgbAnimProgress = 0.0f;

    DrawBox(device, x, y, w, h, backColor, borderColor);

    int titleWidth = (int)(strlen(title) * 9 * scaleFactor);
    DrawText(device, title, x + (w - titleWidth) / 2, y + (int)(12 * scaleFactor), titleColor, true, 1.0f);

    for (int i = 0; i < itemCount; i++) {
        int itemY = y + (int)(44 * scaleFactor) + i * (int)(24 * scaleFactor);
        std::string line = (i == selector ? "> " : "  ");
        line += items[i].title;
        if (items[i].hack) {
            if (items[i].type == 0) {
                line += *items[i].hack ? " [ON]" : " [OFF]";
            }
            else if (items[i].title == std::string("Crosshair")) {
                switch (*items[i].hack) {
                case 0: line += " [OFF]"; break;
                case 1: line += " [DOT]"; break;
                case 2: line += " [CROSS]"; break;
                case 3: line += " [CIRCLE]"; break;
                }
            }
            else if (items[i].title == std::string("Menu Color") ||
                items[i].title == std::string("Border Color") ||
                items[i].title == std::string("Crosshair Color")) {
                int index = *items[i].hack;
                line += " [" + std::string(predefinedColors[index].name) + "]";
            }
            else if (items[i].title == std::string("Box Style")) {
                line += *items[i].hack ? " [SOLID]" : " [OUTLINE]";
            }
            else {
                line += " [" + std::to_string(*items[i].hack) + "]";
            }
        }
        DWORD itemColor = (i == selector) ? D3DCOLOR_ARGB(255, 220, 220, 220) : D3DCOLOR_ARGB(255, 180, 180, 180);
        if (i == selector) {
            DrawSelectionBox(device, x + (int)(4 * scaleFactor), itemY, w - (int)(8 * scaleFactor), (int)(22 * scaleFactor), animProgress);
            DrawText(device, line.c_str(), x + (int)(12 * scaleFactor), itemY, itemColor, true, animProgress);
        }
        else {
            DrawText(device, line.c_str(), x + (int)(12 * scaleFactor), itemY, itemColor, true, 1.0f);
        }
    }

    std::string timeStr = "Time: " + GetCurrentTime();
    int timeWidth = (int)(timeStr.length() * 7 * scaleFactor);
    int timeY = y + (int)(44 * scaleFactor) + itemCount * (int)(24 * scaleFactor) + (int)(12 * scaleFactor);
    DrawText(device, timeStr.c_str(), x + (w - timeWidth) / 2, timeY, D3DCOLOR_ARGB(255, 150, 150, 150), true, 1.0f);

    if (pSprite) pSprite->End();
}

void D3DMenu::UpdateInput() {
    ULONGLONG currentTime = GetTickCount64();
    if (currentTime - lastKeyTime >= keyDelay) {
        if (GetAsyncKeyState(VK_UP) & 0x8000) {
            selector = (selector - 1 + itemCount) % itemCount;
            animProgress = 0.0f;
            lastKeyTime = currentTime;
        }
        if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
            selector = (selector + 1) % itemCount;
            animProgress = 0.0f;
            lastKeyTime = currentTime;
        }
        if (items[selector].hack) {
            if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
                if (items[selector].type == 0) *items[selector].hack = *items[selector].hack == 0 ? 1 : 0;
                else if (*items[selector].hack < items[selector].maxval) (*items[selector].hack)++;
                lastKeyTime = currentTime;
            }
            if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
                if (items[selector].type == 0) *items[selector].hack = *items[selector].hack == 0 ? 1 : 0;
                else if (*items[selector].hack > 0) (*items[selector].hack)--;
                lastKeyTime = currentTime;
            }
        }
    }
}

void D3DMenu::Release() {
    if (pFont) {
        pFont->Release();
        pFont = nullptr;
    }
    if (pSprite) {
        pSprite->Release();
        pSprite = nullptr;
    }
    if (pLine) {
        pLine->Release();
        pLine = nullptr;
    }
}