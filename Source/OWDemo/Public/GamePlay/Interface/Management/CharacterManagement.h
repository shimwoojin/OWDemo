// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "ManagementBase.h"
#include "CharacterManagement.generated.h"

UINTERFACE(MinimalAPI)
class UCharacterManagement : public UManagementBase
{
	GENERATED_BODY()
};

class OWDEMO_API ICharacterManagement : public IManagementBase
{
	GENERATED_BODY()

public:

	UFUNCTION(category = CharacterDefault)
		virtual void Swap(int32 Number);

	UFUNCTION(category = CharacterDefault)
		virtual void Sprint(bool bPressed);

	UFUNCTION(category = CharacterDefault)
		virtual void Run();

	UFUNCTION(category = CharacterDefault)
		virtual void Walk();
};
