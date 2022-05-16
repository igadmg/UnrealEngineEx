#pragma once

#include "ExPrologue.h"
#include "Engine/SkeletalMesh.h"



DECLARE_CONST_EXTENSION(USkeletalMesh)
	COMPONENTEX_API TArray<USkeletalMeshSocket*> SearchSocketsStartWith(const FString& InSocketName) const;
};

DECLARE_MUTABLE_EXTENSION(USkeletalMesh)
};

DECLARE_EXTENSION(USkeletalMesh);
