#include "stdafx.h"

bool Utils::WorldToScreenRel(CVector3 worldCoords, Vector2& screenCoords)
{
	float num1, num2;

	if (GRAPHICS::_WORLD3D_TO_SCREEN2D, worldCoords.fX, worldCoords.fY, worldCoords.fZ, &num1, &num2)
	{
		screenCoords = Vector2();
		return false;
	}
	screenCoords = Vector2((num1 - 0.5f) * 2, (num2 - 0.5f) * 2);
	return true;
}

CVector3 Utils::ScreenRelToWorld(CVector3 camPos, CVector3 camRot, Vector2 coord)
{
	auto camForward = RotationToDirection(camRot);
	auto rotUp = camRot + CVector3(10000, 0, 0);
	auto rotDown = camRot + CVector3(-10000, 0, 0);
	auto rotLeft = camRot + CVector3(0, 0, -10000);
	auto rotRight = camRot + CVector3(0, 0, 10000);

	auto camRight = RotationToDirection(rotRight) - RotationToDirection(rotLeft);
	auto camUp = RotationToDirection(rotUp) - RotationToDirection(rotDown);

	auto rollRad = -DegToRad(camRot.fY);

	auto camRightRoll = camRight * (float)std::cos(rollRad) - camUp * (float)std::sin(rollRad);
	auto camUpRoll = camRight * (float)std::sin(rollRad) + camUp * (float)std::cos(rollRad);

	auto point3D = camPos + camForward * 10000.0f + camRightRoll + camUpRoll;
	Vector2 point2D;
	if (!WorldToScreenRel(point3D, point2D)) return camPos + camForward * 10000.0f;
	auto point3DZero = camPos + camForward * 10000.0f;
	Vector2 point2DZero;
	if (!WorldToScreenRel(point3DZero, point2DZero)) return camPos + camForward * 10000.0f;

	const double eps = 0.0000001;
	if (std::abs(point2D.fX - point2DZero.fX) < eps || std::abs(point2D.fY - point2DZero.fY) < eps) return camPos + camForward * 10000.0f;
	auto scaleX = (coord.fX - point2DZero.fX) / (point2D.fX - point2DZero.fX);
	auto scaleY = (coord.fY - point2DZero.fY) / (point2D.fY - point2DZero.fY);
	auto point3Dret = camPos + camForward * 10000.0f + camRightRoll * scaleX + camUpRoll * scaleY;
	return point3Dret;
}

CVector3 Utils::RotationToDirection(CVector3 rotation)
{
	auto z = DegToRad(rotation.fZ);
	auto x = DegToRad(rotation.fX);
	auto num = std::abs(std::cos(x));
	return CVector3((float)(-std::sin(z) * num), (float)(std::cos(z) * num), (float)std::sin(x));
}

CVector3 Utils::DirectionToRotation(CVector3 direction)
{
	direction.Normalize();

	auto x = std::atan2(direction.fZ, direction.fY);
	auto y = 0;
	auto z = -std::atan2(direction.fX, direction.fY);

	return CVector3((float)RadToDeg(x), (float)RadToDeg(y), (float)RadToDeg(z));
}

double Utils::DegToRad(double deg)
{
	return deg * PI / 180.0;
}

double Utils::RadToDeg(double deg)
{
	return deg * 180.0 / PI;
}

double Utils::BoundRotationDeg(double angleDeg)
{
	auto twoPi = (int)(angleDeg / 360);
	auto res = angleDeg - twoPi * 360;
	if (res < 0) res += 360;
	return res;
}