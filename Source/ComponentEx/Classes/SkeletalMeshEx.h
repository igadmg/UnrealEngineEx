#pragma once

#include "Engine/SkeletalMesh.h"



struct FSkeletalMeshEx
{
	FSkeletalMeshEx(USkeletalMesh* This)
		: This(This)
	{
	}



public:
	COMPONENTEX_API TArray<USkeletalMeshSocket*> SearchSocketsStartWith(FString InSocketName) const;



protected:
	USkeletalMesh* This;
};
