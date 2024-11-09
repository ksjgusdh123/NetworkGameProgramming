#pragma once
class CCollision
{
public:
	CCollision();
	~CCollision();

public:
	void UpdateCollision(Vector2& pos, Vector2& size);
	void Render(HDC hDC, float elapsedTime);

	bool CheckCollision(CCollision* box);
private:
	BoxInfo m_info;

};

