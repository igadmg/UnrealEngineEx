#pragma once



class TSplineComponentConstCoordinateFrameForInputKey
{
public:
	TSplineComponentConstCoordinateFrameForInputKey(const USplineComponent& Object, float InputKey)
		: Object(Object)
		, InputKey(InputKey)
	{
	}



public:
	FORCEINLINE FVector GetRelativeLocation() const {
		return Object.GetLocationAtSplineInputKey(InputKey, ESplineCoordinateSpace::Local);
	}
	FORCEINLINE FRotator GetRelativeRotation() const {
		return Object.GetRotationAtSplineInputKey(InputKey, ESplineCoordinateSpace::Local);
	}
	FORCEINLINE FVector GetRelativeScale() const {
		return Object.GetScaleAtSplineInputKey(InputKey);
	}
	FORCEINLINE FTransform GetRelativeTransform() const {
		return Object.GetTransformAtSplineInputKey(InputKey, ESplineCoordinateSpace::Local);
	}


	FORCEINLINE FVector GetWorldLocation() const {
		return Object.GetLocationAtSplineInputKey(InputKey, ESplineCoordinateSpace::World);
	}
	FORCEINLINE FRotator GetWorldRotation() const {
		return Object.GetRotationAtSplineInputKey(InputKey, ESplineCoordinateSpace::World);
	}
	FORCEINLINE FVector GetWorldScale() const {
		return Object.GetScaleAtSplineInputKey(InputKey);
	}
	FORCEINLINE FTransform GetWorldTransform() const {
		return Object.GetTransformAtSplineInputKey(InputKey, ESplineCoordinateSpace::World);
	}


	DEFINE_CF_COMMON_FUNCTIONS()



protected:
	const USplineComponent& Object;
	float InputKey;
};

class TSplineComponentCoordinateFrameForInputKey : public TSplineComponentConstCoordinateFrameForInputKey
{
public:
	TSplineComponentCoordinateFrameForInputKey(USplineComponent& Object, float InputKey)
		: TSplineComponentConstCoordinateFrameForInputKey(Object, InputKey)
		, Object(Object)
	{
	}



public:
	FORCEINLINE TSplineComponentCoordinateFrameForInputKey& SetRelativeLocation(FVector NewLocation) { return *this; }
	FORCEINLINE TSplineComponentCoordinateFrameForInputKey& SetRelativeRotation(FRotator NewRotation) { return *this; }
	FORCEINLINE TSplineComponentCoordinateFrameForInputKey& SetRelativeRotation(FQuat NewRotation) { return *this; }
	FORCEINLINE TSplineComponentCoordinateFrameForInputKey& SetRelativeScale(FVector NewScale) { return *this; }
	FORCEINLINE TSplineComponentCoordinateFrameForInputKey& SetRelativeTransform(FTransform NewTransform) { return *this; }
	FORCEINLINE TSplineComponentCoordinateFrameForInputKey& SetWorldLocation(FVector NewLocation) { return *this; }
	FORCEINLINE TSplineComponentCoordinateFrameForInputKey& SetWorldRotation(FRotator NewRotation) { return *this; }
	FORCEINLINE TSplineComponentCoordinateFrameForInputKey& SetWorldRotation(FQuat NewRotation) { return *this; }
	FORCEINLINE TSplineComponentCoordinateFrameForInputKey& SetWorldScale(FVector NewScale) { return *this; }
	FORCEINLINE TSplineComponentCoordinateFrameForInputKey& SetWorldTransform(FTransform NewTransform) { return *this; }



protected:
	USplineComponent& Object;
};



template <typename T>
struct FCoordinateFrameAux<T, float, typename std::enable_if<std::is_base_of<USplineComponent, T>::value>::type>
{
	typedef TSplineComponentConstCoordinateFrameForInputKey ConstReturnType;
	typedef TSplineComponentCoordinateFrameForInputKey ReturnType;

	static TSplineComponentConstCoordinateFrameForInputKey MakeConst(const T& Object, float InputKey) { return TSplineComponentConstCoordinateFrameForInputKey(Object, InputKey); }
	static TSplineComponentCoordinateFrameForInputKey Make(T& Object, float InputKey) { return TSplineComponentCoordinateFrameForInputKey(Object, InputKey); }
};



class TSplineComponentConstCoordinateFrameForPoint
{
public:
	TSplineComponentConstCoordinateFrameForPoint(const USplineComponent& Object, int32 PointIndex)
		: Object(Object)
		, PointIndex(PointIndex)
	{
	}



public:
	FORCEINLINE FVector GetRelativeLocation() const {
		return Object.GetLocationAtSplinePoint(PointIndex, ESplineCoordinateSpace::Local);
	}
	FORCEINLINE FRotator GetRelativeRotation() const {
		return Object.GetRotationAtSplinePoint(PointIndex, ESplineCoordinateSpace::Local);
	}
	FORCEINLINE FVector GetRelativeScale() const {
		return Object.GetScaleAtSplinePoint(PointIndex);
	}
	FORCEINLINE FTransform GetRelativeTransform() const {
		return Object.GetTransformAtSplinePoint(PointIndex, ESplineCoordinateSpace::Local);
	}

	FORCEINLINE FVector GetWorldLocation() const {
		return Object.GetLocationAtSplinePoint(PointIndex, ESplineCoordinateSpace::World);
	}
	FORCEINLINE FRotator GetWorldRotation() const {
		return Object.GetRotationAtSplinePoint(PointIndex, ESplineCoordinateSpace::World);
	}
	FORCEINLINE FVector GetWorldScale() const {
		return Object.GetScaleAtSplinePoint(PointIndex);
	}
	FORCEINLINE FTransform GetWorldTransform() const {
		return Object.GetTransformAtSplinePoint(PointIndex, ESplineCoordinateSpace::World);
	}



protected:
	const USplineComponent& Object;
	int32 PointIndex;
};

class TSplineComponentCoordinateFrameForPoint : public TSplineComponentConstCoordinateFrameForPoint
{
public:
	TSplineComponentCoordinateFrameForPoint(USplineComponent& Object, int32 PointIndex)
		: TSplineComponentConstCoordinateFrameForPoint(Object, PointIndex)
		, Object(Object)
	{
	}



public:
	FORCEINLINE TSplineComponentCoordinateFrameForPoint& SetRelativeLocation(FVector NewLocation) { return *this; }
	FORCEINLINE TSplineComponentCoordinateFrameForPoint& SetRelativeRotation(FRotator NewRotation) { return *this; }
	FORCEINLINE TSplineComponentCoordinateFrameForPoint& SetRelativeRotation(FQuat NewRotation) { return *this; }
	FORCEINLINE TSplineComponentCoordinateFrameForPoint& SetRelativeScale(FVector NewScale) { return *this; }
	FORCEINLINE TSplineComponentCoordinateFrameForPoint& SetRelativeTransform(FTransform NewTransform) { return *this; }
	FORCEINLINE TSplineComponentCoordinateFrameForPoint& SetWorldLocation(FVector NewLocation) { return *this; }
	FORCEINLINE TSplineComponentCoordinateFrameForPoint& SetWorldRotation(FRotator NewRotation) { return *this; }
	FORCEINLINE TSplineComponentCoordinateFrameForPoint& SetWorldRotation(FQuat NewRotation) { return *this; }
	FORCEINLINE TSplineComponentCoordinateFrameForPoint& SetWorldScale(FVector NewScale) { return *this; }
	FORCEINLINE TSplineComponentCoordinateFrameForPoint& SetWorldTransform(FTransform NewTransform) { return *this; }

	FORCEINLINE TSplineComponentCoordinateFrameForPoint& SetForwardVector(FVector ForwardVector) {
		Object.SetTangentAtSplinePoint(PointIndex, ForwardVector, ESplineCoordinateSpace::World, false);
		return *this;
	}
	FORCEINLINE TSplineComponentCoordinateFrameForPoint& SetRightVector(FVector RightVector) {
		return *this;
	}
	FORCEINLINE TSplineComponentCoordinateFrameForPoint& SetUpVector(FVector UpVector) {
		Object.SetUpVectorAtSplinePoint(PointIndex, UpVector, ESplineCoordinateSpace::World, false);
		return *this;
	}



protected:
	USplineComponent& Object;
};



template <typename T>
struct FCoordinateFrameAux<T, int32, typename std::enable_if<std::is_base_of<USplineComponent, T>::value>::type>
{
	typedef TSplineComponentConstCoordinateFrameForPoint ConstReturnType;
	typedef TSplineComponentCoordinateFrameForPoint ReturnType;

	static TSplineComponentConstCoordinateFrameForPoint MakeConst(const T& Object, int32 PointIndex) { return TSplineComponentConstCoordinateFrameForPoint(Object, PointIndex); }
	static TSplineComponentCoordinateFrameForPoint Make(T& Object, int32 PointIndex) { return TSplineComponentCoordinateFrameForPoint(Object, PointIndex); }
};
