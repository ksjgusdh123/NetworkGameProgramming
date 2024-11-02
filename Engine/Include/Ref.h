#pragma once

#include "EngineInfo.h"

class CRef
{
public:
	int GetRefCount() const					{ return m_refCount; }
	std::string GetName() const				{ return m_name; }
	std::string GetTypeName() const			{ return m_typeName; }
	size_t GetTypeID() const				{ return m_typeID; }
	bool GetEnable() const					{ return m_enable; }
	bool GetActive() const					{ return m_active; }

	void SetName(const std::string& name)	{ m_name = name; }
	void SetEnable(bool enable)				{ m_enable = enable; }
	void SetActive(bool enable)				{ m_active = enable; }

	template <typename T>
	void SetTypeID()
	{
		m_typeName = typeid(T).name();
		m_typeID = typeid(T).hash_code();
	}

public:
	void AddRef();
	int Release();

	void Destroy();

private:
	int			m_refCount	= 0;
	std::string	m_name;
	std::string	m_typeName;
	size_t		m_typeID;
	bool		m_enable	= true;
	bool		m_active	= true;
};

