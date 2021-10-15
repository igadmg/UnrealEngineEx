#pragma once

#include "AttachmentDescription.generated.h"



USTRUCT(Atomic, BlueprintType, Immutable)
struct FAttachmentDescription
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* Component = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName = NAME_None;


	FAttachmentDescription()
	{
	}

	FAttachmentDescription(class USceneComponent* Component, FName SocketName)
		: Component(Component)
		, SocketName(SocketName)
	{
	}
};

