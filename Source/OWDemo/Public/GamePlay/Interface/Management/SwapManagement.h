// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "ManagementBase.h"
#include "SwapManagement.generated.h"

UINTERFACE(MinimalAPI)
class USwapManagement : public UManagementBase
{
	GENERATED_BODY()
};

class OWDEMO_API ISwapManagement : public IManagementBase
{
	GENERATED_BODY()

public:
	UFUNCTION(Category = Swap)
		virtual void Draw();

	UFUNCTION(Category = Swap)
		virtual void Sheath();
};
