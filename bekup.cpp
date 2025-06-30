//#include "D3DMenu.h"
//#include "Logging.h"
//
//Color predefinedColors[] = {
//    {"White", D3DCOLOR_ARGB(255, 255, 255, 255)},
//    {"Red", D3DCOLOR_ARGB(255, 255, 0, 0)},
//    {"Green", D3DCOLOR_ARGB(255, 0, 255, 0)},
//    {"Blue", D3DCOLOR_ARGB(255, 0, 0, 255)},
//    {"Yellow", D3DCOLOR_ARGB(255, 255, 255, 0)},
//    {"Cyan", D3DCOLOR_ARGB(255, 0, 255, 255)},
//    {"Magenta", D3DCOLOR_ARGB(255, 255, 0, 255)},
//    {"Orange", D3DCOLOR_ARGB(255, 255, 165, 0)},
//    {"Purple", D3DCOLOR_ARGB(255, 128, 0, 128)},
//    {"Pink", D3DCOLOR_ARGB(255, 255, 192, 203)},
//    {"RGB", D3DCOLOR_ARGB(255, 255, 255, 255)}
//};
//const int colorCount = sizeof(predefinedColors) / sizeof(predefinedColors[0]);
//
//D3DMenu g_Menu;
//int espNameTag = 0;
//int antiKick = 0;
//int menuColorIndex = 3;
//int borderColorIndex = 3;
//int crosshairType = 0;
//int crosshairColorIndex = 1;
//int crosshairSize = 5;
//int boxStyle = 1;
//int noRecoil = 0;
//
//D3DMenu::D3DMenu() : pFont(nullptr), pSprite(nullptr), pLine(nullptr), itemCount(0), selector(0),
//x(0), y(0), w(0), h(0), baseW(250), baseH(300), scaleFactor(1.0f),
//titleColor(D3DCOLOR_ARGB(255, 255, 255, 255)), backColor(D3DCOLOR_ARGB(230, 25, 25, 35)),
//borderColor(D3DCOLOR_ARGB(255, 100, 150, 255)), shadowColor(D3DCOLOR_ARGB(180, 0, 0, 0)),
//show(true), animProgress(0.0f), rgbAnimProgress(0.0f), menuColorIndex(3), borderColorIndex(3),
//crosshairColorIndex(1), boxStyle(1), crosshairType(0), crosshairSize(5), espNameTag(0),
//lastKeyTime(0), keyDelay(150) {
//}
//
//DWORD D3DMenu::GetRGBColor(float progress) {
//    float r = sinf(progress * 2.0f * D3DX_PI) * 127.5f + 127.5f;
//    float g = sinf(progress * 2.0f * D3DX_PI + 2.0f * 3.14159f / 3.0f) * 127.5f + 127.5f;
//    float b = sinf(progress * 2.0f * D3DX_PI + 4.0f * 3.14159f / 3.0f) * 127.5f + 127.5f;
//    return D3DCOLOR_ARGB(255, (BYTE)r, (BYTE)g, (BYTE)b);
//}
//
//void D3DMenu::CalculateDimensions(LPDIRECT3DDEVICE9 device) {
//    D3DVIEWPORT9 vp;
//    device->GetViewport(&vp);
//    float screenW = (float)vp.Width;
//    float screenH = (float)vp.Height;
//
//    scaleFactor = min(screenW / 1920.0f, screenH / 1080.0f);
//    w = (int)(baseW * scaleFactor);
//    h = (int)((60 + itemCount * 22 + 30) * scaleFactor);
//    x = (int)(50 * scaleFactor);
//    y = (int)(50 * scaleFactor);
//}
//
//void D3DMenu::CreateItem(const char* title, int* hack, int maxval, int type, DWORD color) {
//    if (itemCount < 50) {
//        items[itemCount++] = { title, hack, maxval, type, color };
//        Log("Menu item created: %s", title);
//    }
//}
//
//void D3DMenu::InitFont(LPDIRECT3DDEVICE9 device) {
//    if (!pFont && device) {
//        int fontSize = (int)(16 * scaleFactor);
//        if (SUCCEEDED(D3DXCreateFontA(device, fontSize, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
//            OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
//            "Verdana", &pFont))) {
//            Log("Font initialized.");
//        }
//        else {
//            Log("Font initialization failed.");
//        }
//    }
//    if (!pSprite && device) {
//        if (SUCCEEDED(D3DXCreateSprite(device, &pSprite))) {
//            Log("Sprite initialized.");
//        }
//        else {
//            Log("Sprite initialization failed.");
//        }
//    }
//    if (!pLine && device) {
//        if (SUCCEEDED(D3DXCreateLine(device, &pLine))) {
//            Log("Line initialized.");
//            pLine->SetWidth(2.0f * scaleFactor);
//            pLine->SetAntialias(TRUE);
//        }
//        else {
//            Log("Line initialization failed.");
//        }
//    }
//}
//
//void D3DMenu::DrawText(LPDIRECT3DDEVICE9 device, const char* text, int x, int y, DWORD color, bool shadow, float textAnimProgress) {
//    if (!pFont) InitFont(device);
//    if (pFont && pSprite) {
//        BYTE alpha = (BYTE)(255 * (0.7f + 0.3f * sin(textAnimProgress * 2.0f * D3DX_PI)));
//        DWORD animColor = D3DCOLOR_ARGB(alpha, (BYTE)GetRValue(color), (BYTE)GetGValue(color), (BYTE)GetBValue(color));
//
//        RECT rect = { x, y, x + w, y + (int)(20 * scaleFactor) };
//        if (shadow) {
//            RECT shadowRect = { x + (int)(2 * scaleFactor), y + (int)(2 * scaleFactor), x + w + (int)(2 * scaleFactor), y + (int)(22 * scaleFactor) };
//            pFont->DrawTextA(pSprite, text, -1, &shadowRect, DT_LEFT | DT_NOCLIP, shadowColor);
//        }
//        pFont->DrawTextA(pSprite, text, -1, &rect, DT_LEFT | DT_NOCLIP, animColor);
//    }
//}
//
//void D3DMenu::DrawBox(LPDIRECT3DDEVICE9 device, int x, int y, int w, int h, DWORD fill, DWORD border) {
//    if (boxStyle == 1) {
//        D3DRECT mainRect = { x + (int)(4 * scaleFactor), y + (int)(4 * scaleFactor), x + w - (int)(4 * scaleFactor), y + h - (int)(4 * scaleFactor) };
//        device->Clear(1, &mainRect, D3DCLEAR_TARGET, fill, 0, 0);
//
//        D3DRECT gradientRect = { x + (int)(4 * scaleFactor), y + (int)(4 * scaleFactor), x + w - (int)(4 * scaleFactor), y + h / 2 };
//        device->Clear(1, &gradientRect, D3DCLEAR_TARGET, D3DCOLOR_ARGB(230, 35, 35, 50), 0, 0);
//    }
//
//    int borderWidth = (int)(2 * scaleFactor);
//    DWORD animatedBorderColor = (borderColorIndex == colorCount - 1) ? GetRGBColor(rgbAnimProgress) : predefinedColors[borderColorIndex].value;
//    D3DRECT borders[] = {
//        { x, y, x + w, y + borderWidth },
//        { x, y + h - borderWidth, x + w, y + h },
//        { x, y + borderWidth, x + borderWidth, y + h - borderWidth },
//        { x + w - borderWidth, y + borderWidth, x + w, y + h - borderWidth }
//    };
//    for (auto& r : borders)
//        device->Clear(1, &r, D3DCLEAR_TARGET, animatedBorderColor, 0, 0);
//
//    D3DRECT shadow[] = {
//        { x + (int)(2 * scaleFactor), y + h, x + w + (int)(2 * scaleFactor), y + h + (int)(4 * scaleFactor) },
//        { x + w, y + (int)(2 * scaleFactor), x + w + (int)(4 * scaleFactor), y + h + (int)(2 * scaleFactor) }
//    };
//    for (auto& r : shadow)
//        device->Clear(1, &r, D3DCLEAR_TARGET, shadowColor, 0, 0);
//}
//
//void D3DMenu::DrawSelectionBox(LPDIRECT3DDEVICE9 device, int x, int y, int w, int h, float progress) {
//    DWORD selColor = predefinedColors[menuColorIndex].value;
//    DWORD selFade = D3DCOLOR_ARGB((int)(255 * (1.0f - progress)),
//        (BYTE)(GetRValue(selColor) / 2),
//        (BYTE)(GetGValue(selColor) / 2),
//        (BYTE)(GetBValue(selColor) / 2));
//    D3DRECT selRect = { x, y, x + w, y + h };
//    device->Clear(1, &selRect, D3DCLEAR_TARGET, selFade, 0, 0);
//    D3DRECT selBorder = { x, y, x + w, y + (int)(1 * scaleFactor) };
//    device->Clear(1, &selBorder, D3DCLEAR_TARGET, selColor, 0, 0);
//}
//
//void D3DMenu::DrawCrosshair(LPDIRECT3DDEVICE9 device) {
//    if (crosshairType == 0) return;
//
//    D3DVIEWPORT9 vp;
//    device->GetViewport(&vp);
//    float centerX = vp.Width / 2.0f;
//    float centerY = vp.Height / 2.0f;
//    DWORD crosshairColor = (crosshairColorIndex == colorCount - 1) ? GetRGBColor(rgbAnimProgress) : predefinedColors[crosshairColorIndex].value;
//
//    if (pLine) {
//        pLine->Begin();
//        if (crosshairType == 1) {
//            D3DXVECTOR2 points[2] = { {centerX, centerY}, {centerX + 1, centerY} };
//            pLine->SetWidth((float)crosshairSize * scaleFactor);
//            pLine->Draw(points, 2, crosshairColor);
//        }
//        else if (crosshairType == 2) {
//            D3DXVECTOR2 pointsH[2] = { {centerX - crosshairSize * scaleFactor, centerY}, {centerX + crosshairSize * scaleFactor, centerY} };
//            D3DXVECTOR2 pointsV[2] = { {centerX, centerY - crosshairSize * scaleFactor}, {centerX, centerY + crosshairSize * scaleFactor} };
//            pLine->SetWidth(2.0f * scaleFactor);
//            pLine->Draw(pointsH, 2, crosshairColor);
//            pLine->Draw(pointsV, 2, crosshairColor);
//        }
//        else if (crosshairType == 3) {
//            const int segments = 32;
//            D3DXVECTOR2 points[33];
//            float radius = (float)crosshairSize * scaleFactor;
//            for (int i = 0; i <= segments; i++) {
//                float angle = (float)i / segments * 2.0f * D3DX_PI;
//                points[i] = D3DXVECTOR2(centerX + radius * cosf(angle), centerY + radius * sinf(angle));
//            }
//            pLine->SetWidth(2.0f * scaleFactor);
//            pLine->Draw(points, segments + 1, crosshairColor);
//        }
//        pLine->End();
//    }
//}
//
//void D3DMenu::DrawBorderAimPov(LPDIRECT3DDEVICE9 device, int espNameTag) {
//    if (!pLine || !pFont || !pSprite) {
//        InitFont(device);
//        if (!pLine || !pFont || !pSprite) {
//            Log("Failed to initialize pLine, pFont, or pSprite for Border Aim/POV.");
//            return;
//        }
//    }
//
//    pLine->Begin();
//    D3DVIEWPORT9 vp;
//    device->GetViewport(&vp);
//    float centerX = vp.Width / 2.0f;
//    float centerY = vp.Height / 2.0f;
//
//    float boxWidth = 40.0f * scaleFactor;
//    float boxHeight = 40.0f * scaleFactor;
//    D3DXVECTOR2 points[5] = {
//        {centerX - boxWidth / 2, centerY - boxHeight / 2},
//        {centerX + boxWidth / 2, centerY - boxHeight / 2},
//        {centerX + boxWidth / 2, centerY + boxHeight / 2},
//        {centerX - boxWidth / 2, centerY + boxHeight / 2},
//        {centerX - boxWidth / 2, centerY - boxHeight / 2}
//    };
//    pLine->SetWidth(2.0f * scaleFactor);
//    pLine->Draw(points, 5, D3DCOLOR_ARGB(255, 255, 0, 0));
//
//    if (espNameTag) {
//        const char* nickname = "Player";
//        DWORD lineColor = D3DCOLOR_ARGB(255, 255, 0, 0);
//
//        int textHeight = (int)(20 * scaleFactor);
//        int textY = (int)(centerY - boxHeight / 2 - textHeight - 5 * scaleFactor);
//        int textX = (int)(centerX - (strlen(nickname) * 8 * scaleFactor) / 2);
//
//        DrawText(device, nickname, textX, textY, lineColor, true, 1.0f);
//
//        D3DXVECTOR2 linePoints[2] = {
//            {centerX, centerY - boxHeight / 2},
//            {centerX, (float)textY + textHeight}
//        };
//        pLine->SetWidth(2.0f * scaleFactor);
//        pLine->Draw(linePoints, 2, lineColor);
//    }
//
//    pLine->End();
//}
//
//std::string D3DMenu::GetCurrentTime() {
//    std::time_t now = std::time(nullptr);
//    struct tm tstruct;
//    char buf[80];
//    localtime_s(&tstruct, &now);
//    strftime(buf, sizeof(buf), "%H:%M:%S", &tstruct);
//    return std::string(buf);
//}
//
//void D3DMenu::Render(LPDIRECT3DDEVICE9 device, const char* title) {
//    CalculateDimensions(device);
//
//    ULONGLONG currentTime = GetTickCount64();
//
//    if (GetAsyncKeyState(VK_INSERT) & 0x8000) {
//        if (currentTime - lastKeyTime >= keyDelay) {
//            show = !show;
//            Log("INSERT pressed, menu %s", show ? "shown" : "hidden");
//            lastKeyTime = currentTime;
//        }
//    }
//
//    if (pSprite) pSprite->Begin(D3DXSPRITE_ALPHABLEND);
//
//    DrawCrosshair(device);
//    DrawBorderAimPov(device, espNameTag);
//
//    if (!show) {
//        if (pSprite) pSprite->End();
//        return;
//    }
//
//    animProgress += 0.05f;
//    if (animProgress > 1.0f) animProgress = 0.0f;
//    rgbAnimProgress += 0.02f;
//    if (rgbAnimProgress > 1.0f) rgbAnimProgress = 0.0f;
//
//    DrawBox(device, x, y, w, h, backColor, borderColor);
//
//    int titleWidth = (int)(strlen(title) * 8 * scaleFactor);
//    DrawText(device, title, x + (w - titleWidth) / 2, y + (int)(10 * scaleFactor), titleColor, true, 1.0f);
//
//    for (int i = 0; i < itemCount; i++) {
//        int itemY = y + (int)(40 * scaleFactor) + i * (int)(22 * scaleFactor);
//        std::string line = (i == selector ? "> " : "  ");
//        line += items[i].title;
//        if (items[i].hack) {
//            if (items[i].type == 0) {
//                line += *items[i].hack ? " [ON]" : " [OFF]";
//            }
//            else if (items[i].title == std::string("Crosshair")) {
//                switch (*items[i].hack) {
//                case 0: line += " [OFF]"; break;
//                case 1: line += " [DOT]"; break;
//                case 2: line += " [CROSS]"; break;
//                case 3: line += " [CIRCLE]"; break;
//                }
//            }
//            else if (items[i].title == std::string("Menu Color") ||
//                items[i].title == std::string("Border Color") ||
//                items[i].title == std::string("Crosshair Color")) {
//                int index = *items[i].hack;
//                line += " [" + std::string(predefinedColors[index].name) + "]";
//            }
//            else if (items[i].title == std::string("Box Style")) {
//                line += *items[i].hack ? " [WITH BG]" : " [BORDER ONLY]";
//            }
//            else {
//                line += " [" + std::to_string(*items[i].hack) + "]";
//            }
//        }
//        DWORD itemColor = (i == selector) ? D3DCOLOR_ARGB(255, 200, 220, 255) : items[i].color;
//        if (i == selector) {
//            DrawSelectionBox(device, x + (int)(5 * scaleFactor), itemY, w - (int)(10 * scaleFactor), (int)(20 * scaleFactor), animProgress);
//            DrawText(device, line.c_str(), x + (int)(15 * scaleFactor), itemY, itemColor, true, animProgress);
//        }
//        else {
//            DrawText(device, line.c_str(), x + (int)(15 * scaleFactor), itemY, itemColor, true, 1.0f);
//        }
//    }
//
//    std::string timeStr = "Time: " + GetCurrentTime();
//    int timeWidth = (int)(timeStr.length() * 8 * scaleFactor);
//    int timeY = y + (int)(40 * scaleFactor) + itemCount * (int)(22 * scaleFactor) + (int)(10 * scaleFactor);
//    DrawText(device, timeStr.c_str(), x + (w - timeWidth) / 2, timeY, D3DCOLOR_ARGB(255, 200, 200, 200), true, 1.0f);
//
//    if (pSprite) pSprite->End();
//}
//
//void D3DMenu::UpdateInput() {
//    ULONGLONG currentTime = GetTickCount64();
//    if (currentTime - lastKeyTime >= keyDelay) {
//        if (GetAsyncKeyState(VK_UP) & 0x8000) {
//            selector = (selector - 1 + itemCount) % itemCount;
//            animProgress = 0.0f;
//            lastKeyTime = currentTime;
//        }
//        if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
//            selector = (selector + 1) % itemCount;
//            animProgress = 0.0f;
//            lastKeyTime = currentTime;
//        }
//        if (items[selector].hack) {
//            if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
//                if (items[selector].type == 0) *items[selector].hack = *items[selector].hack == 0 ? 1 : 0;
//                else if (*items[selector].hack < items[selector].maxval) (*items[selector].hack)++;
//                lastKeyTime = currentTime;
//            }
//            if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
//                if (items[selector].type == 0) *items[selector].hack = *items[selector].hack == 0 ? 1 : 0;
//                else if (*items[selector].hack > 0) (*items[selector].hack)--;
//                lastKeyTime = currentTime;
//            }
//        }
//    }
//}
//
//void D3DMenu::Release() {
//    if (pFont) {
//        pFont->Release();
//        pFont = nullptr;
//    }
//    if (pSprite) {
//        pSprite->Release();
//        pSprite = nullptr;
//    }
//    if (pLine) {
//        pLine->Release();
//        pLine = nullptr;
//    }
//}