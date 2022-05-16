#pragma once

#include "Engine/DeveloperSettings.h"
#include "DebugExSettings.generated.h"


UCLASS(config = Game, defaultconfig)
class DEBUGEX_API UDebugExSettings : public UDeveloperSettings
{
	GENERATED_BODY()


public:
	UPROPERTY(Category = "DebugEx", EditAnywhere, BlueprintReadOnly, Config, meta = (RowType = DebugMenuRenameConfig))
	TSoftObjectPtr<class UDataTable> PropertyRenameTable;


public:
	UDebugExSettings(const FObjectInitializer& ObjectInitializer);
};
