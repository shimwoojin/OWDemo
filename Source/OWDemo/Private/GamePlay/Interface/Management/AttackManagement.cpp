// If you want to use this project, mail to ggoggal@gmail.com

#include "AttackManagement.h"

void IAttackManagement::StartAttack()
{
	LOG(Warning, TEXT("IAttackManagement::StartAttack() Called, Implements Interface"));
}

void IAttackManagement::EndAttack()
{
	LOG(Warning, TEXT("IAttackManagement::EndAttack() Called, Implements Interface"));
}

void IAttackManagement::AttackTick(float DeltaTime, float TotalDuration)
{
}