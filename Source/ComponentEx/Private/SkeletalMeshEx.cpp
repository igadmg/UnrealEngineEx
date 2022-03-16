#include "SkeletalMeshEx.h"

#include "Engine/SkeletalMeshSocket.h"

#include "ComponentEx.final.h"



TArray<USkeletalMeshSocket*> FSkeletalMeshEx::SearchSocketsStartWith(FString InSocketName) const
{
	TArray<USkeletalMeshSocket*> Result;

	for (int32 i = 0; i < This->NumSockets(); i++)
	{
		USkeletalMeshSocket* Socket = This->GetSocketByIndex(i);
		if (Socket && Socket->SocketName.ToString().StartsWith(InSocketName))
		{
			Result.Add(Socket);
		}
	}

	return Result;
}
