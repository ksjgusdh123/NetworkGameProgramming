#include "Input.h"
#include "Scene/Scene.h"

DEFINITION_SINGLE(CInput)

CInput::CInput()
{
}

CInput::~CInput()
{
}

bool CInput::Init()
{
	//AddBindKey("SceneChange", '1');
	//SetKeyCtrl("Attack");

	AddBindKey("Jump", VK_SPACE);
	AddBindKey("Attack", 'A');
	AddBindKey("MoveLeft", VK_LEFT);
	AddBindKey("MoveRight", VK_RIGHT);

	return true;
}

void CInput::Update()
{
	UpdateKeyState();
	UpdateBindKey();
}

bool CInput::AddBindKey(const std::string& name, unsigned char key)
{
	// 같은 이름의 BindKey가 이미 등록되어 있을 경우
	if (FindBindKey(name))
		return false;

	std::shared_ptr<KeyState> keyState;

	// 해당 키로 이미 KeyState가 등록되어 있는지를 찾는다.
	KeyState* p = FindKeyState(key);
	keyState.reset(p);

	// 등록이 안되어 있을 경우 새로운 KeyState를 만들어서 등록한다.
	if (!keyState)
	{
		keyState = std::make_shared<KeyState>();

		keyState->key = key;

		m_mapKeyState.insert(std::make_pair(key, keyState));
	}

	std::shared_ptr<BindKey> bindKey = std::make_shared<BindKey>();
	bindKey->key = keyState;
	bindKey->name = name;

	m_mapBindKey.insert(std::make_pair(name, bindKey));

	return true;
}

void CInput::ClearCallback()
{
	auto	iter = m_mapBindKey.begin();
	auto	iterEnd = m_mapBindKey.end();
	for (; iter != iterEnd; ++iter)
	{
		for (int i = 0; i < (int)EInput_Type::END; ++i)
		{
			size_t	size = iter->second->vecFunc[i].size();
			for (size_t j = 0; j < size; ++j)
			{
				iter->second->vecFunc[i][j].reset();
			}

			iter->second->vecFunc[i].clear();
		}
	}
}

void CInput::ClearCallback(const CScene* scene)
{
	auto	iter = m_mapBindKey.begin();
	auto	iterEnd = m_mapBindKey.end();
	for (; iter != iterEnd; ++iter)
	{
		for (int i = 0; i < (int)EInput_Type::END; ++i)
		{
			auto	iter1 = iter->second->vecFunc[i].begin();
			auto	iter1End = iter->second->vecFunc[i].end();
			for (; iter1 != iter1End;)
			{
				if ((*iter1)->scene == scene)
				{
					(*iter1).reset();
					iter1 = iter->second->vecFunc[i].erase(iter1);
					iter1End = iter->second->vecFunc[i].end();
					continue;
				}

				++iter1;
			}
		}
	}
}

void CInput::UpdateKeyState()
{
	GetAsyncKeyState(VK_CONTROL) & 0x8000 ? m_ctrl = true : m_ctrl = false;
	GetAsyncKeyState(VK_MENU) & 0x8000 ? m_alt = true : m_alt = false;
	GetAsyncKeyState(VK_SHIFT) & 0x8000 ? m_shift = true : m_shift = false;

	for (const auto& [key, keyState] : m_mapKeyState)
	{
		bool	push = false;

		if (GetAsyncKeyState(keyState->key) & 0x8000)
		{
			push = true;
		}

		// 키를 눌렀을 경우
		if (push)
		{
			// down과 push 모두 false라면 이 키를 지금 누른 것이다.
			// 그러므로 둘다 true로 변경한다.
			if (!keyState->down && !keyState->push)
			{
				keyState->down = true;
				keyState->push = true;
			}

			// down과 push 둘중 하나라도 true라면 down은 false가 되어야 한다.
			// push는 이미 위에서 true로 변경되었다.
			else
				keyState->down = false;
		}
		// 키가 안눌러졌을 경우 push가 true라면
		// 이전 프레임에 키를 누르고 있다가 지금 떨어졌다는 것이다.
		else if (keyState->push)
		{
			keyState->up = true;
			keyState->push = false;
			keyState->down = false;
		}
		else if (keyState->up)
			keyState->up = false;
	}
}

void CInput::UpdateBindKey()
{
	for (const auto& [name, bindKey] : m_mapBindKey)
	{
		if (bindKey->key->down &&
			bindKey->ctrl == m_ctrl &&
			bindKey->alt == m_alt &&
			bindKey->shift == m_shift)
		{
			size_t size = bindKey->vecFunc[(int)EInput_Type::DOWN].size();
			for (int i = 0; i < size; ++i)
			{
				bindKey->vecFunc[(int)EInput_Type::DOWN][i]->func();
			}
		}

		if (bindKey->key->push &&
			bindKey->ctrl == m_ctrl &&
			bindKey->alt == m_alt &&
			bindKey->shift == m_shift)
		{
			size_t size = bindKey->vecFunc[(int)EInput_Type::PUSH].size();
			for (int i = 0; i < size; ++i)
			{
				bindKey->vecFunc[(int)EInput_Type::PUSH][i]->func();
			}
		}

		if (bindKey->key->up &&
			bindKey->ctrl == m_ctrl &&
			bindKey->alt == m_alt &&
			bindKey->shift == m_shift)
		{
			size_t size = bindKey->vecFunc[(int)EInput_Type::UP].size();
			for (int i = 0; i < size; ++i)
			{
				bindKey->vecFunc[(int)EInput_Type::UP][i]->func();
			}
		}
	}
}

void CInput::SetKeyCtrl(std::string_view name, bool ctrl)
{
	BindKey* key = FindBindKey(name);
	if (!key)
		return;

	key->ctrl = ctrl;
}

void CInput::SetKeyAlt(std::string_view name, bool alt)
{
	BindKey* key = FindBindKey(name);
	if (!key)
		return;

	key->alt = alt;
}

void CInput::SetKeyShift(std::string_view name, bool shift)
{
	BindKey* key = FindBindKey(name);
	if (!key)
		return;

	key->shift = shift;
}

KeyState* CInput::FindKeyState(unsigned char key)
{
	auto iter = m_mapKeyState.find(key);
	if (iter == m_mapKeyState.end())
		return nullptr;

	return iter->second.get();
}

BindKey* CInput::FindBindKey(std::string_view name)
{
	auto iter = m_mapBindKey.find(name.data());
	if (iter == m_mapBindKey.end())
		return nullptr;

	return iter->second.get();
}
