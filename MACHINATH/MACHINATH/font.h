#pragma once

HRESULT InitFont();

void UninitFont();

void DrawTextTo(RECT rect, const char* text, int textLength);
