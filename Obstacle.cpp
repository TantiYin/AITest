#include "Obstacle.h"
#include <d2d1.h>

extern ID2D1HwndRenderTarget* gpRenderTarget;
extern ID2D1SolidColorBrush* gpBrush;


void Obstacle::Render()
{
	gpBrush->SetColor(D2D1::ColorF(0.0, 0.0, 0.0));
	D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2(Pos().x, Pos().y), BRadius(), BRadius());

	gpRenderTarget->DrawEllipse(ellipse, gpBrush, 1);
}
