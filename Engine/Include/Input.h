#pragma once

#include "EngineInfo.h"

enum class EInput_Type : unsigned char
{
	DOWN,
	PUSH,
	UP,
	END
};

struct KeyState
{
	unsigned char	key		= 0;
	bool			down	= false;
	bool			push	= false;
	bool			up		= false;
};

struct BindFunction
{
	void* obj = nullptr;
	std::function<void()> func;
	class CScene* scene;
};

struct BindKey
{
	std::string	name;
	bool		ctrl	= false;
	bool		alt		= false;
	bool		shift	= false;
	std::shared_ptr<KeyState> key;
	std::vector<std::shared_ptr<BindFunction>>	vecFunc[(int)EInput_Type::END];
};

class CInput
{
private:
	std::unordered_map<unsigned char, std::shared_ptr<KeyState>>	m_mapKeyState;
	std::unordered_map<std::string, std::shared_ptr<BindKey>>		m_mapBindKey;
	bool	m_ctrl	= false;
	bool	m_alt	= false;
	bool	m_shift	= false;

public:
	bool Init();
	void Update();

public:
	bool AddBindKey(const std::string& name, unsigned char key);
	void ClearCallback();
	void ClearCallback(const class CScene* scene);

private:
	void UpdateKeyState();
	void UpdateBindKey();

	void SetKeyCtrl(std::string_view name, bool ctrl = true);
	void SetKeyAlt(std::string_view name, bool alt = true);
	void SetKeyShift(std::string_view name, bool shift = true);

	KeyState* FindKeyState(unsigned char key);
	BindKey* FindBindKey(std::string_view name);

public:
	template <typename T>
	void AddBindFunction(std::string_view keyName, EInput_Type type, T* obj,
		void(T::* func)(), class CScene* scene)
	{
		BindKey* key = FindBindKey(keyName);

		if (!key)
			return;

		std::shared_ptr<BindFunction> bindFunc = std::make_shared<BindFunction>();
		bindFunc->obj = obj;
		bindFunc->func = std::bind(func, obj);
		bindFunc->scene = scene;

		key->vecFunc[(int)type].push_back(bindFunc);
	}

	template <typename T>
	bool DeleteBindFunction(std::string_view name, EInput_Type type, T* obj)
	{
		BindKey* key = FindBindKey(name);

		if (!key)
			return false;

		auto iter = key->vecFunc[(int)type].begin();
		auto iterEnd = key->vecFunc[(int)type].end();
		for (; iter != iterEnd;)
		{
			if ((*iter)->obj == obj)
			{
				iter->reset();
				iter = key->vecFunc[(int)type].erase(iter);
				iterEnd = key->vecFunc[(int)type].end();
				continue;
			}

			++iter;
		}

		return true;
	}

	DECLARE_SINGLE(CInput)
};

