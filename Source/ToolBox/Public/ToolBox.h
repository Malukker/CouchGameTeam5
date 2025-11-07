#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBoxModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:
    
    void AddMenu(FMenuBarBuilder& MenuBarBuilder);
    void FillMenu(FMenuBuilder& MenuBuilder);
};
