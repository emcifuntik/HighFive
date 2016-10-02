#include "stdafx.h"

CGraphics * CGraphics::singleInstance = nullptr;

CGraphics * CGraphics::Get()
{
	if (!singleInstance)
		singleInstance = new CGraphics();
	return singleInstance;
}


bool CGraphics::WorldToScreen(CVector3 pos, CVector3 &out)
{
	auto viewPortGame = GTA::CViewportGame::Get();
	auto matrix = viewPortGame->ViewPort;
	_D3DMATRIX transposed;
	transposed._11 = matrix._11;
	transposed._21 = matrix._12;
	transposed._31 = matrix._13;
	transposed._41 = matrix._14;

	transposed._12 = matrix._21;
	transposed._22 = matrix._22;
	transposed._32 = matrix._23;
	transposed._42 = matrix._24;

	transposed._13 = matrix._31;
	transposed._23 = matrix._32;
	transposed._33 = matrix._33;
	transposed._43 = matrix._34;

	transposed._14 = matrix._41;
	transposed._24 = matrix._42;
	transposed._34 = matrix._43;
	transposed._44 = matrix._44;

	CVector4 vUpward, vRight, vForward;

	vForward = CVector4(transposed._41, transposed._42, transposed._43, transposed._44);
	vRight = CVector4(transposed._21, transposed._22, transposed._23, transposed._24);
	vUpward = CVector4(transposed._31, transposed._32, transposed._33, transposed._34);

	out.fZ = (vForward.fX * pos.fX) + (vForward.fY * pos.fY) + (vForward.fZ * pos.fZ) + vForward.fW;
	out.fX = (vRight.fX * pos.fX) + (vRight.fY * pos.fY) + (vRight.fZ * pos.fZ) + vRight.fW;
	out.fY = (vUpward.fX * pos.fX) + (vUpward.fY * pos.fY) + (vUpward.fZ * pos.fZ) + vUpward.fW;
	if (out.fZ < 0.001f)
		return false;

	float invw = 1.0f / out.fZ;
	out.fX *= invw;
	out.fY *= invw;
	float xTmp = (float)viewPortGame->Width / 2;
	float yTmp = (float)viewPortGame->Height / 2;

	xTmp += (int)(0.5*out.fX * viewPortGame->Width + 0.5);
	yTmp -= (int)(0.5*out.fY * viewPortGame->Height + 0.5);

	out.fX += xTmp;
	out.fY = yTmp;

	out.fX = out.fX * (1.0f / viewPortGame->Width);
	out.fY = out.fY * (1.0f / viewPortGame->Height);
	return true;
}

void CGraphics::Draw3DText(std::string text, float size, float x, float y, float z, color_t color)
{
	CVector3 screenPos;
	WorldToScreen(CVector3(x, y, z), screenPos);
	auto viewPortGame = GTA::CViewportGame::Get();
	CGlobals::Get().DX11Renderer->RenderText(screenPos.fX * viewPortGame->Width, screenPos.fY * viewPortGame->Height, color.red, color.green, color.blue, color.alpha, true, 14.f, FW1_CENTER, text.c_str());
}

void CGraphics::Draw3DProgressBar(color_t bgColor, color_t frontColor, float width, float height, float x, float y, float z, float value)
{
	if (value > 1.f)
		value = 1.f;
	CVector3 screenPos;
	WorldToScreen(CVector3(x, y, z), screenPos);
	screenPos.fY += 0.04f;
	auto viewPortGame = GTA::CViewportGame::Get();
	DWORD colorOut = Utils::RGBAToHex(bgColor.red, bgColor.blue, bgColor.green, bgColor.alpha);
	DWORD colorIn = Utils::RGBAToHex(bgColor.red, bgColor.blue, bgColor.green, bgColor.alpha);
	CGlobals::Get().DX11Renderer->DrawHealthBox(screenPos.fX * viewPortGame->Width, screenPos.fY * viewPortGame->Height, colorOut, colorIn, width, height, value);
}

CGraphics::~CGraphics()
{
}
