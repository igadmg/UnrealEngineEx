#pragma once



template <>
class TConstCoordinateFrame<UStaticMeshSocket>
{
public:
	TConstCoordinateFrame(const UStaticMeshSocket& Object)
		: Object(Object)
	{
	}



public:
	FORCEINLINE FVector GetRelativeLocation() const { return Object.RelativeLocation; }
	FORCEINLINE FRotator GetRelativeRotation() const { return Object.RelativeRotation; }
	FORCEINLINE FVector GetRelativeScale() const { return Object.RelativeScale; }
	FORCEINLINE FTransform GetRelativeTransform() const { return FTransform(Object.RelativeRotation, Object.RelativeLocation, Object.RelativeScale); }
	FORCEINLINE FVector GetWorldLocation() const;
	FORCEINLINE FRotator GetWorldRotation() const;
	FORCEINLINE FVector GetWorldScale() const;
	FORCEINLINE FTransform GetWorldTransform() const;



protected:
	const UStaticMeshSocket& Object;
};
