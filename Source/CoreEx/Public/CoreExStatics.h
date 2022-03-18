#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "CoreExStatics.generated.h"


DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(bool, FObjectArrayPredicate, UObject*, Item);

UENUM(BlueprintType)
enum class EValid : uint8
{
	IsValid,
	IsNotValid
};

UCLASS()
class COREEX_API UCoreExStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:
	/** Return self class to blueprint. */
	UFUNCTION(Category = "CoreEx", BlueprintPure, meta = (HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static UClass* GetSelfClass(const UObject* WorldContextObject);
};
