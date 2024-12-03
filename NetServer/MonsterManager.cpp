#include "MonsterManager.h"
#include "TileManager.h"

void MonsterManager::Init()
{
	inGameData = GameManager::GetInst().GetInGameData();
}

void MonsterManager::InitBossData()
{
	MonsterManager::GetInst().Init();
	TileManager::GetInst().CreateBossTile();
	CreateBossMonster();
}

void MonsterManager::CreateMonster()
{
	MonsterInfo info;
	info.type = '0';
	info.pos = vector2(-100.f, 410.f);
	info.original_pos = info.pos;
	info.hp = 100;
	info.direct = EObject_Dir::Right;
	info.state = EObject_State::Walk;
	info.is_alive = true;
	info.velocity = 50.f;
	info.size = vector2(43.f, 61.f);
	inGameData->monster[0] = info;

	info.type = '1';
	info.pos = vector2(120.f, 80.f);
	info.original_pos = info.pos;
	info.hp = 100;
	info.direct = EObject_Dir::Right;
	info.state = EObject_State::Basic;
	info.is_alive = true;
	info.timer = 0.f;
	inGameData->monster[1] = info;

}

void MonsterManager::CreateBossMonster()
{
	MonsterInfo info;
	info.type = '2';
	info.pos = vector2(-100.f, 410.f);
	info.original_pos = info.pos;
	info.hp = 100;
	info.direct = EObject_Dir::Right;
	info.state = EObject_State::Walk;
	info.is_alive = true;
	info.velocity = 50.f;
	inGameData->monster[0] = info;
}

void MonsterManager::UpdateMonster()
{
	for (MonsterInfo& m : inGameData->monster)
	{
		if (!m.is_alive)
			continue;

		switch (m.type) {
		case '0':
		{
			if (m.state == EObject_State::Die_L || m.state == EObject_State::Die)
			{
				m.timer += 0.05f;
				if (m.timer > 1.0f) m.is_alive = false;
				break;
			}
			float range = 100.f;

			if (m.direct == EObject_Dir::Right && m.pos.x >= m.original_pos.x + range)
			{
				m.direct = EObject_Dir::Left;
				m.state = EObject_State::Walk_L;
				m.velocity = -50.f;
			}
			else if (m.direct == EObject_Dir::Left && m.pos.x <= m.original_pos.x - range)
			{
				m.direct = EObject_Dir::Right;
				m.state = EObject_State::Walk;
				m.velocity = 50.f;
			}

			m.pos.x += m.velocity * 0.1f;
		}
		break;
		case '1':
		{
			int targetNum = IsPlayerInRicheAttackArea();
			if (targetNum != -1) {
				GamePlayerInfo p = inGameData->players[targetNum];

				if (p.pos.x >= m.pos.x) {
					m.state = EObject_State::Attack;
					m.direct = EObject_Dir::Right;
				}
				else {
					m.state = EObject_State::Attack_L;
					m.direct = EObject_Dir::Left;
				}
				m.target = p.pos;
				m.timer = 0.f;
			}

			if (m.state == EObject_State::Attack || m.state == EObject_State::Attack_L) {
				m.timer += 0.05f;

				if (m.timer >= 1.f) {
					if (m.state == EObject_State::Attack)
						m.state = EObject_State::Basic;
					if (m.state == EObject_State::Attack_L)
						m.state = EObject_State::Basic_L;

					//CRicheAttack* ra = m_scene->CreateObject<CRicheAttack>("riche_attack");
					//ra->SetPos(m_pos);
					//ra->SetTarget(m_target);
				}
			}
		}
		break;
		default:

			break;
		}
	}
}

int MonsterManager::IsPlayerInRicheAttackArea()
{
	EObject_State riche_state = inGameData->monster[1].state;
	if (riche_state == EObject_State::Attack_L || riche_state == EObject_State::Attack) return -1;

	for (int i = 0; i < PLAYER_NUM; ++i) {
		float dx = inGameData->players[i].pos.x - inGameData->monster[1].pos.x;
		float dy = inGameData->players[i].pos.y - inGameData->monster[1].pos.y;
		float distance = sqrt(dx * dx + dy * dy);
		if (distance < 400) return i;
	}

	return -1;
}