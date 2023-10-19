// If you want to use this project, mail to ggoggal@gmail.com


#include "StackSkill.h"

AStackSkill::AStackSkill()
{
	MaxStack = 100.f;
	Stack = 0.f;
}

bool AStackSkill::Activate()
{
	if (CanUseSkill())
	{
		//@Todo

		return true;
	}

	return false;
}

bool AStackSkill::CanUseSkill()
{
	return Super::CanUseSkill() && IsStackFull();
}

bool AStackSkill::IsStackFull()
{
	return Stack >= MaxStack ? true : false;
}
