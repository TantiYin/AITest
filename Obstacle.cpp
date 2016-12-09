#include "Obstacle.h"
#include <d2d1.h>
#include <sstream>
#include <string>

extern ID2D1HwndRenderTarget* gpRenderTarget;
extern ID2D1SolidColorBrush* gpBrush;
extern IDWriteTextFormat* gpTextFormat;

void Obstacle::Render()
{
	gpBrush->SetColor(D2D1::ColorF(0.0, 0.0, 0.0));
	D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2(Pos().x, Pos().y), BRadius(), BRadius());

	gpRenderTarget->DrawEllipse(ellipse, gpBrush, 1);

	std::wstringstream ss;
	std::wstring LineText;
	ss << L"ID:" << GetID();
	ss >> LineText;
	gpRenderTarget->DrawText(LineText.c_str(), LineText.size(), gpTextFormat, D2D1::Rect(Pos().x, Pos().y, Pos().x + BRadius(), Pos().y + BRadius()), gpBrush);
	//gpRenderTarget->DrawRectangle(D2D1::Rect(Pos().x, Pos().y, Pos().x + BRadius(), Pos().y + BRadius()), gpBrush);
}
