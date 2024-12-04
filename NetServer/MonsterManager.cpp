#include "MonsterManager.h"
#include "TileManager.h"
#define min(a,b)            (((a) < (b)) ? (a) : (b))

void MonsterManager::Init()
{
	inGameData = GameManager::GetInst().GetInGameData();
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
	info.size = vector2(107.f, 139.f);
	inGameData->monster[1] = info;

	for (int i = 0; i < MONSTER_ATTACK_NUM; ++i) {
		MonsterAttackInfo attckInfo;
		attckInfo.pos = vector2(-10000.f, -10000.f);
		attckInfo.size = vector2(50.f, 50.f);
		attckInfo.type = '0';
		attckInfo.state = EObject_State::Attack;
		attckInfo.direct = EObject_Dir::Right;
		attckInfo.velocity = 5.0f;
		attckInfo.is_alive = false;
		attckInfo.timer = 0.f;
		attckInfo.target = vector2(-1000.f, -1000.f);
		inGameData->monsterAttack[i] = attckInfo;
	}
}

void MonsterManager::CreateBossMonster()
{
	MonsterInfo info;
	info.type = '2';
	info.pos = vector2(0.f, 370.f);
	info.hp = 100;
	info.direct = EObject_Dir::Right;
	info.state = EObject_State::Basic;
	info.is_alive = true;
	info.size = vector2(130.f, 160.f);
	info.timer = 0.f;
	inGameData->monster[0] = info;

	info.type = '3';
	inGameData->monster[1] = info;

	for (int i = 0; i < MONSTER_ATTACK_NUM; ++i) {
		MonsterAttackInfo attckInfo;
		attckInfo.pos = vector2(-1000.f, -1000.f);
		attckInfo.size = vector2(50.f, 50.f);
		attckInfo.type = '1';
		attckInfo.state = EObject_State::Attack;
		attckInfo.direct = EObject_Dir::Right;
		attckInfo.velocity = 5.0f;
		attckInfo.is_alive = false;
		attckInfo.timer = 0.f;
		attckInfo.target = vector2(-1000.f, -1000.f);
		inGameData->monsterAttack[i] = attckInfo;
	}
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
			if (m.state == EObject_State::Die_L || m.state == EObject_State::Die)
			{
				m.timer += 0.05f;
				if (m.timer > 1.0f) m.is_alive = false;
				break;
			}

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

					for (MonsterAttackInfo& monsterAttack : inGameData->monsterAttack) {
						if (monsterAttack.is_alive) continue;
						monsterAttack.pos = m.pos;
						monsterAttack.target = vector2(m.target.x, m.target.y - 40.f);
						monsterAttack.is_alive = true;
						monsterAttack.timer = 0.f;
						break;
					}
				}
			}
		}
		break;
		case '2':
			if (m.state == EObject_State::Die_L || m.state == EObject_State::Die)
			{
				m.timer += 0.05f;
				if (m.timer > 0.8f) m.is_alive = false;
				break;
			}

			m.timer += 0.05f;

			if (m.timer > 2.0f) {
				if (m.is_alive) {
					int targetNum = bossAttack();
					if (targetNum != -1) {
						GamePlayerInfo p = inGameData->players[targetNum];
						m.target = p.pos;
						m.timer = 0.f;

						float distance = abs(m.target.x - m.pos.x);
						int bossAttack = 0;
						if (distance > 400.f)
							bossAttack = 2;
						else if (distance <= 400.f && distance > 100.f)
							bossAttack = 1;
						else
							bossAttack = 0;

						switch (bossAttack) {
						case 0:
							if (m.target.x >= m.pos.x) {
								m.state = EObject_State::Attack;
								m.direct = EObject_Dir::Right;
							}
							else {
								m.state = EObject_State::Attack_L;
								m.direct = EObject_Dir::Left;
							}
							break;
						case 1:
							if (m.target.x >= m.pos.x) {
								m.state = EObject_State::Attack2;
								m.direct = EObject_Dir::Right;
							}
							else {
								m.state = EObject_State::Attack_L2;
								m.direct = EObject_Dir::Left;
							}
							break;
						case 2:
							if (m.target.x >= m.pos.x) {
								m.state = EObject_State::Telpo;
								m.direct = EObject_Dir::Up;
							}
							else {
								m.state = EObject_State::Telpo_L;
								m.direct = EObject_Dir::Up;
							}
							break;
						}
						


					}
				}
				m.timer = 0.f;
			}


			if (m.timer >= 1.f) {
				if (m.state == EObject_State::Attack)
					m.state = EObject_State::Basic;
				if (m.state == EObject_State::Attack_L)
					m.state = EObject_State::Basic_L;
				if (m.state == EObject_State::Attack_L2) {
					m.state = EObject_State::Basic_L;
					int Attack_Num = 0;
					for (MonsterAttackInfo& monsterAttack : inGameData->monsterAttack) {
						if (monsterAttack.is_alive) continue;
						if (Attack_Num < 1) {
							monsterAttack.pos = vector2(m.pos.x - 60.f, m.pos.y + 60.f);
							monsterAttack.is_alive = true;
							monsterAttack.timer = 0.f;
							monsterAttack.direct = EObject_Dir::Left;
						}
						else {
							monsterAttack.pos = vector2(m.pos.x - 60.f, m.pos.y - 60.f);
							monsterAttack.is_alive = true;
							monsterAttack.timer = 0.f;
							monsterAttack.direct = EObject_Dir::Left;
						}
						Attack_Num += 1;
						if (Attack_Num >= 2)
							break;
					}

				}
				if (m.state == EObject_State::Attack2) {
					m.state = EObject_State::Basic;
					int Attack_Num = 0;
					for (MonsterAttackInfo& monsterAttack : inGameData->monsterAttack) {
						if (monsterAttack.is_alive) continue;
						if (Attack_Num < 1) {
							monsterAttack.pos = vector2(m.pos.x + 60.f, m.pos.y + 60.f);
							monsterAttack.is_alive = true;
							monsterAttack.timer = 0.f;
							monsterAttack.direct = EObject_Dir::Right;
						}
						else {
							monsterAttack.pos = vector2(m.pos.x + 60.f, m.pos.y - 60.f);
							monsterAttack.is_alive = true;
							monsterAttack.timer = 0.f;
							monsterAttack.direct = EObject_Dir::Right;
						}
						Attack_Num += 1;
						if (Attack_Num >= 2)
							break;
					}
				}
				if (m.state == EObject_State::Telpo) {
					m.state = EObject_State::Telpo;
					m.pos.x = m.target.x;
				}
				if (m.state == EObject_State::Telpo_L) {
					m.state = EObject_State::Telpo_L;
					m.pos.x = m.target.x;
				}

			}
			break;
		default:

			break;
		}
	}

	for (MonsterAttackInfo& m : inGameData->monsterAttack){
		if (!m.is_alive)
			continue;
		switch (m.type) {
		case '0':				// Riche 공격
		{
			float dx = m.target.x - m.pos.x;
			float dy = m.target.y - m.pos.y;
			float distance = sqrt(dx * dx + dy * dy);

			if (distance > 0.0f) { // 거리 계산
				float velocityFactor = min(distance, m.velocity); // distance와 x 방향 속도 중 작은 값 선택
				m.pos.x += (dx / distance) * velocityFactor; // x축 이동
				m.pos.y += (dy / distance) * velocityFactor; // y축 이동
			}

			m.timer += 0.05;
			if (m.timer >= 3.0f)
				m.is_alive = false;

			if (distance < 10)
				m.is_alive = false;
		}
			break;
		case '1':
		{
			if (m.direct == EObject_Dir::Right)
				m.pos.x += m.velocity * 2.5f;
			else if (m.direct == EObject_Dir::Left)
				m.pos.x -= m.velocity * 2.5f;

			m.timer += 0.05f;
			if (m.timer >= 3.0f)
				m.is_alive = false;
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

int MonsterManager::bossAttack()
{
	EObject_State riche_state = inGameData->monster[0].state;
	if (riche_state == EObject_State::Attack_L || riche_state == EObject_State::Attack || riche_state == EObject_State::Attack2  || riche_state == EObject_State::Attack_L2) return -1;

	for (int i = 0; i < PLAYER_NUM; ++i) {
		if (inGameData->players[i].hp > 0) return i;
	}

	return -1;
}
