#pragma once

#include "EngineInfo.h"
#include "Ref.h"

class CGameObject :
	public CRef
{
public:
	EObject_Type GetObjectType() const { return m_objectType; }
	class CScene* GetScene() const { return m_scene; }
	std::string GetName() const { return m_name; }
	Vector2 GetSize() const { return m_size; }
	Vector2 GetPos() const { return m_pos; }
	Vector2 GetVelocity() const { return m_velocity; }
	float GetAngle() const { return m_angle; }

	void SetObjectType(EObject_Type type) { m_objectType = type; }
	void SetScene(class CScene* scene) { m_scene = scene; }
	void SetName(const std::string& name) { m_name = name; }
	void SetSize(const Vector2& size) { m_size = size; }
	void SetSize(float x, float y) { m_size = Vector2(x, y); }
	void SetPos(const Vector2& pos) { m_pos = pos; }
	void SetPos(float x, float y) { m_pos = Vector2(x, y); }
	void SetVelocity(const Vector2& velocity) { m_velocity = velocity; }
	void SetVelocity(float x, float y) { m_velocity = Vector2(x, y); }
	void SetAngle(float angle) { m_angle = angle; }

	void AddPos(const Vector2& pos) { m_pos.x += pos.x; m_pos.y += pos.y; }
	void AddAngle(float angle) { m_angle += angle; }

public:
	virtual bool Init();
	virtual void Update(float elapsedTime);
	virtual void PostUpdate(float elapsedTime);
	virtual void Render(HDC hDC, float elapsedTime);

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
};

