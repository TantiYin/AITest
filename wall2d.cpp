#include "wall2d.h"
#include <d2d1.h>

extern ID2D1HwndRenderTarget* gpRenderTarget;
extern ID2D1SolidColorBrush* gpBrush;


void Wall2d::CalculateNormal()
{
    Vector2 temp = Vec2DNormalize(mB - mA);

    mNormal.x = -temp.y;
    mNormal.y = temp.x;
}

void Wall2d::Render() const
{
    gpBrush->SetColor(D2D1::ColorF(0.0f, 1.0f, 0.0f));
    gpRenderTarget->DrawLine(D2D1::Point2(mA.x, mA.y), D2D1::Point2(mB.x, mB.y), gpBrush, 1);
}
