#pragma once

#include "AttachmentDescription.generated.h"



USTRUCT(Atomic, BlueprintType)
struct FAttachmentDescription
{
	GENERATED_BODY()


	UPROPERTY(Category = "Attachment", EditAnywhere, BlueprintReadWrite)
	class USceneComponent* Component = nullptr;

	UPROPERTY(Category = "Attachment", EditAnywhere, BlueprintReadWrite)
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
