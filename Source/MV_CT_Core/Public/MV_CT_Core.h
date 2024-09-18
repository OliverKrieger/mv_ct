#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FMV_CT_Core : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
