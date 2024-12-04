#pragma once

#include "EngineInfo.h"
#include "Ref.h"

class CGameObject :
	public CRef
{
public:
	CGameObject() {}
	virtual ~CGameObject();

public:
	EObject_Type GetObjectType() const { return m_objectType; }
	class CScene* GetScene() const { return m_scene; }
	std::string GetName() const { return m_name; }
	Vector2 GetSize() const { return m_size; }
	Vector2 GetPos() const { return m_pos; }
	Vector2 GetPivot() const { return m_pivot; }
	Vector2 GetVelocity() const { return m_velocity; }
	float GetAngle() const { return m_angle; }
	EObject_Dir GetDir() const { return m_objectDir; }
	EObject_State GetState() const { return m_objectState; }
	class CCollision* GetCollision() const { return m_collisionBox.get(); }

	void SetObjectType(EObject_Type type) { m_objectType = type; }
	void SetScene(class CScene* scene) { m_scene = scene; }
	void SetName(const std::string& name) { m_name = name; }
	void SetSize(const Vector2& size) { m_size = size; }
	void SetSize(float x, float y) { m_size = Vector2(x, y); }
	void SetPos(const Vector2& pos) { m_pos = pos; }
	void SetPos(float x, float y) { m_pos = Vector2(x, y); }
	void SetPivot(const Vector2& pivot) { m_pivot = pivot; }
	void SetPivot(float x, float y) { m_pivot = Vector2(x, y); }
	void SetVelocity(const Vector2& velocity) { m_velocity = velocity; }
	void SetVelocity(float x, float y) { m_velocity = Vector2(x, y); }
	void SetAngle(float angle) { m_angle = angle; }
	void SetDir(EObject_Dir dir) { m_objectDir = dir; }
	void SetState(EObject_State state) { m_objectState = state; }
	void SetIdx(int num) { m_idx = num; }
	void SetTime(float num) { m_time = num; }
	void AddPos(const Vector2& pos) { m_pos.x += pos.x; m_pos.y += pos.y; }
	void AddAngle(float angle) { m_angle += angle; }

	virtual void SetTexture(const std::string& name, const std::wstring& fileName, EObject_Dir dir, ETexture_Type type = ETexture_Type::Sprite, const std::string& pathName = TEXTURE_PATH);
	void SetAnimation(RECT* rects, int num, EObject_State state = EObject_State::Basic, ETexture_Type type = ETexture_Type::CIMAGE);
	virtual bool SetColorKey(unsigned char r, unsigned char g, unsigned char b, int idx = 0);

public:
	virtual bool Init();
	virtual void Update(float elapsedTime);
	virtual void PostUpdate(float elapsedTime);
	virtual void Render(HDC hDC, float elapsedTime);

	void CreateTexture(int num);

	int GetItemId() { return item_id; };
	void SetItemId(int id) { item_id = id; };
private:
	int item_id;
public:
	int m_hp = 100;

protected:
	EObject_Type   m_objectType = EObject_Type::Monster;
	ERender_Type	m_renderType = ERender_Type::Elipse;
	class CScene*	m_scene;
	std::string		m_name;
	Vector2			m_size;
	Vector2			m_pos;
	Vector2			m_pivot = { 0.5f, 0.5f };
	Vector2			m_velocity;
	float			m_angle = 0.f;
	class CTexture** m_texture = nullptr;
	int m_textureNum = 0;
	EObject_Dir m_objectDir = EObject_Dir::Right;


	float m_time = 0;	
	EObject_State m_objectState = EObject_State::Basic;
	EObject_State m_prevObjectState = EObject_State::Basic;
	std::array<std::vector<RECT>, (int)EObject_State::Max> m_animationBox;
	int m_idx = 0;

	std::shared_ptr<class CCollision> m_collisionBox;

};

