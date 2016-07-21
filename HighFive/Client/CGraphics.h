#pragma once
class CGraphics
{
	static CGraphics * singleInstance;
	CGraphics() {}
public:
	static CGraphics * Get();

	bool WorldToScreen(float worldX, float worldY, float worldZ, float& screenX, float &screenY);
	void Draw3DText(std::string text, float size, float x, float y, float z, color_t color);
	void Draw3DProgressBar(color_t bgColor, color_t frontColor, float width, float height, float worldX, float worldY, float worldZ, float value);
	~CGraphics();
};

