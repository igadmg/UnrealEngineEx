#pragma once



template <>
class TConstCoordinateFrame<AActor>
{
public:
	TConstCoordinateFrame(const AActor& Object)
		: Object(Object)
	{
	}



public:
	//FORCEINLINE FVector GetRelativeLocation() const;
	//FORCEINLINE FRotator GetRelativeRotation() const;
	//FORCEINLINE FVector GetRelativeScale() const;
	//FORCEINLINE FTransform GetRelativeTransform() const;
	FORCEINLINE FVector GetWorldLocation() const { return Object.GetActorLocation(); }
	FORCEINLINE FRotator GetWorldRotation() const { return Object.GetActorRotation(); }
	FORCEINLINE FVector GetWorldScale() const { return Object.GetActorScale3D(); }
	FORCEINLINE FTransform GetWorldTransform() const { return Object.GetActorTransform(); }

	DEFINE_CF_COMMON_FUNCTIONS()


protected:
	const AActor& Object;
};

template <>
class TCoordinateFrame<AActor> : public TConstCoordinateFrame<AActor>
{
public:
	TCoordinateFrame(AActor& Object)
		: TConstCoordinateFrame<AActor>(Object)
		, Object(Object)
	{
	}



public:
	//FORCEINLINE TCoordinateFrame<AActor>& SetRelativeLocation(FVector NewLocation);
	//FORCEINLINE TCoordinateFrame<AActor>& SetRelativeRotation(FRotator NewRotation);
	//FORCEINLINE TCoordinateFrame<AActor>& SetRelativeRotation(FQuat NewRotation);
	//FORCEINLINE TCoordinateFrame<AActor>& SetRelativeScale(FVector NewScale);
	//FORCEINLINE TCoordinateFrame<AActor>& SetRelativeTransform(FTransform NewTransform);
	FORCEINLINE TCoordinateFrame<AActor>& SetWorldLocation(FVector NewLocation) { Object.SetActorLocation(NewLocation); return *this; }
	FORCEINLINE TCoordinateFrame<AActor>& SetWorldRotation(FRotator NewRotation) { Object.SetActorRotation(NewRotation); return *this; }
	FORCEINLINE TCoordinateFrame<AActor>& SetWorldRotation(FQuat NewRotation) { Object.SetActorRotation(NewRotation); return *this; }
	FORCEINLINE TCoordinateFrame<AActor>& SetWorldScale(FVector NewScale) { Object.SetActorScale3D(NewScale); return *this; }
	FORCEINLINE TCoordinateFrame<AActor>& SetWorldTransform(FTransform NewTransform) { Object.SetActorTransform(NewTransform); return *this; }



protected:
	AActor& Object;
};




template <typename T>
struct FCoordinateFrame<T, typename std::enable_if<std::is_base_of<AActor, T>::value>::type>
{
	typedef TConstCoordinateFrame<AActor> ConstReturnType;
	typedef TCoordinateFrame<AActor> ReturnType;

	static TConstCoordinateFrame<AActor> MakeConst(const T& Object) { return TConstCoordinateFrame<AActor>(Object); }
	static TCoordinateFrame<AActor> Make(T& Object) { return TCoordinateFrame<AActor>(Object); }
};
