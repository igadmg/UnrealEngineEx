#pragma once

#include "Engine/SkeletalMesh.h"



struct COMPONENTEX_API FSkeletalMeshEx
{
	FSkeletalMeshEx(USkeletalMesh* This)
		: This(This)
	{
	}


public:
	TArray<USkeletalMeshSocket*> SearchSocketsStartWith(FString InSocketName) const;


protected:
	USkeletalMesh* This;
};
