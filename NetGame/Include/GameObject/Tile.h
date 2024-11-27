#pragma once
#include "GameObject.h"
#include "Flag.h"

class CTile : public CGameObject
{
public:
	CTile() {}
	virtual ~CTile() {}
	ETile_Num m_objectNum = ETile_Num::Tile_1;

public:
	virtual bool Init() override;
	virtual void Update(float elapsedTime) override;
	virtual void PostUpdate(float elapsedTime) override;
	virtual void Render(HDC hDC, float elapsedTime) override;
	virtual void SetTexture(const std::string& name, const std::wstring& fileName, ETile_Num num, ETexture_Type type = ETexture_Type::Sprite, const std::string& pathName = TEXTURE_PATH);
	void SettingTile(int num);

	void SetTileNum(int num);

};

