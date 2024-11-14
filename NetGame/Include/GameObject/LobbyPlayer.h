#pragma once
#include "GameObject.h"

class CLobbyPlayer : public CGameObject
{
public:
	CLobbyPlayer() {}
	virtual ~CLobbyPlayer() {}

public:
	void SetJob(EPlayer_Job job) { m_job = job; }

public:
	virtual bool Init() override;
	virtual void Update(float elapsedTime) override;
	virtual void PostUpdate(float elapsedTime) override;
	virtual void Render(HDC hDC, float elapsedTime) override;
	virtual bool SetColorKey(unsigned char r, unsigned char g, unsigned char b, int idx);
	virtual void SetTexture(const std::string& name, const std::wstring& fileName, int num, ETexture_Type type = ETexture_Type::Sprite, const std::string& pathName = TEXTURE_PATH);

private:
	EPlayer_Job m_job = EPlayer_Job::Sword;
};

