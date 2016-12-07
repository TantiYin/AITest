#include "GameWorld.h"
#include "vehicle.h"
#include <d2d1.h>


extern ID2D1HwndRenderTarget* gpRenderTarget;
extern ID2D1SolidColorBrush* gpBrush;
extern IDWriteTextFormat* gpTextFormat;

GameWorld::GameWorld(int x, int y): m_cxClient(x), m_cyClient(y), m_vCrosshair(Vector2(x / 2.0, y / 2.0))
{
	mVehicle = new Vehicle(this, Vector2(0, 0), 2, Vector2(0, 10), Vector2(0, 1), 2, 50, 100);
}

GameWorld::~GameWorld()
{
	delete mVehicle;
}

void GameWorld::Update(double t)
{
	mVehicle->Update(t);

}

void GameWorld::Render()
{
	//and finally the crosshair
	gpBrush->SetColor(D2D1::ColorF(1.0, 0.0, 0.0));
	D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(m_vCrosshair.x, m_vCrosshair.y), 4, 4);
	gpRenderTarget->DrawEllipse(ellipse, gpBrush, 1);
	gpRenderTarget->DrawLine(D2D1::Point2F(m_vCrosshair.x - 8, m_vCrosshair.y), D2D1::Point2F(m_vCrosshair.x + 8, m_vCrosshair.y), gpBrush, 1);
	gpRenderTarget->DrawLine(D2D1::Point2F(m_vCrosshair.x, m_vCrosshair.y - 8), D2D1::Point2F(m_vCrosshair.x, m_vCrosshair.y + 8), gpBrush, 1);
	D2D1_SIZE_F size = gpRenderTarget->GetSize();
	WCHAR LineText[30] = L"Click to move crosshair";
	gpRenderTarget->DrawText(LineText, ARRAYSIZE(LineText) - 1, gpTextFormat, D2D1::RectF(5, size.height - 20, size.width, size.height), gpBrush);

	mVehicle->Render();
}

void GameWorld::SetCrosshair(POINTS p)
{
	m_vCrosshair.x = (double)p.x;
	m_vCrosshair.y = (double)p.y;
}
