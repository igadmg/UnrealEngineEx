#pragma once



template <typename T>
class TConstCoordinateFrame
{
public:
	TConstCoordinateFrame(const T& Object)
		: Object(Object)
	{
	}



public:
	//FORCEINLINE FVector GetRelativeLocation() const;
	//FORCEINLINE FRotator GetRelativeRotation() const;
	//FORCEINLINE FVector GetRelativeScale() const;
	//FORCEINLINE FTransform GetRelativeTransform() const;
	//FORCEINLINE FVector GetWorldLocation() const;
	//FORCEINLINE FRotator GetWorldRotation() const;
	//FORCEINLINE FVector GetWorldScale() const;
	//FORCEINLINE FTransform GetWorldTransform() const;

	//FORCEINLINE FVector GetForwardVector() const;
	//FORCEINLINE FVector GetRightVector() const;
	//FORCEINLINE FVector GetUpVector() const;



protected:
	const T& Object;
};

template <typename T>
class TCoordinateFrame : public TConstCoordinateFrame<T>
{
public:
	TCoordinateFrame(T& Object)
		: TConstCoordinateFrame<T>(Object)
		, Object(Object)
	{
	}



public:
	//FORCEINLINE TCoordinateFrame<T>& SetRelativeLocation(FVector NewLocation);
	//FORCEINLINE TCoordinateFrame<T>& SetRelativeRotation(FRotator NewRotation);
	//FORCEINLINE TCoordinateFrame<T>& SetRelativeRotation(FQuat NewRotation);
	//FORCEINLINE TCoordinateFrame<T>& SetRelativeScale(FVector NewScale);
	//FORCEINLINE TCoordinateFrame<T>& SetRelativeTransform(FTransform NewTransform);
	//FORCEINLINE TCoordinateFrame<T>& SetWorldLocation(FVector NewLocation);
	//FORCEINLINE TCoordinateFrame<T>& SetWorldRotation(FRotator NewRotation);
	//FORCEINLINE TCoordinateFrame<T>& SetWorldRotation(FQuat NewRotation);
	//FORCEINLINE TCoordinateFrame<T>& SetWorldScale(FVector NewScale);
	//FORCEINLINE TCoordinateFrame<T>& SetWorldTransform(FTransform NewTransform);

	//FORCEINLINE TCoordinateFrame<T>& SetForwardVector(FVector ForwardVector);
	//FORCEINLINE TCoordinateFrame<T>& SetRightVector(FVector RightVector);
	//FORCEINLINE TCoordinateFrame<T>& SetUpVector(FVector UpVector);



protected:
	T& Object;
};



template <typename T, typename Enable = void>
struct FCoordinateFrame
{
	typedef TConstCoordinateFrame<T> ConstReturnType;
	typedef TCoordinateFrame<T> ReturnType;

	static TConstCoordinateFrame<T> MakeConst(const T& Object) { return TConstCoordinateFrame<T>(Object); }
	//static TConstCoordinateFrame<T> MakeConst(const TObjectPtr<const T>& Object) { return TConstCoordinateFrame<T>(*Object); }
	static TCoordinateFrame<T> Make(T& Object) { return TCoordinateFrame<T>(Object); }
	//static TCoordinateFrame<T> Make(TObjectPtr<T>& Object) { return TCoordinateFrame<T>(*Object); }

	template <typename P> static TConstCoordinateFrame<T> MakeConst(const T& Object, const P& Parameter);
	template <typename P> static TCoordinateFrame<T> Make(T& Object, const P& Parameter);
};

template <typename T, typename P, typename Enable = void>
struct FCoordinateFrameAux
{
	typedef TConstCoordinateFrame<T> ConstReturnType;
	typedef TCoordinateFrame<T> ReturnType;

	static TConstCoordinateFrame<T> MakeConst(const T& Object, const P& Parameter);
	static TCoordinateFrame<T> Make(T& Object, const P& Parameter);
};


template <typename T>
static typename FCoordinateFrame<T>::ConstReturnType cf(const T& Object)
{
	return FCoordinateFrame<T>::MakeConst(Object);
}

template <typename T>
static typename FCoordinateFrame<T>::ConstReturnType cf(const T* Object)
{
	return FCoordinateFrame<T>::MakeConst(*Object);
}

template <typename T>
static typename FCoordinateFrame<T>::ConstReturnType cf(const TObjectPtr<const T>& Object)
{
	return FCoordinateFrame<T>::MakeConst(*Object);
}

template <typename T>
static typename FCoordinateFrame<T>::ConstReturnType cf(TObjectPtr<const T>& Object)
{
	return FCoordinateFrame<T>::MakeConst(*Object);
}

template <typename T>
static typename FCoordinateFrame<T>::ReturnType cf(T& Object)
{
	return FCoordinateFrame<T>::Make(Object);
}

template <typename T>
static typename FCoordinateFrame<T>::ReturnType cf(T* Object)
{
	return FCoordinateFrame<T>::Make(*Object);
}

template <typename T>
static typename FCoordinateFrame<T>::ReturnType cf(const TObjectPtr<T>& Object)
{
	return FCoordinateFrame<T>::Make(*Object);
}

template <typename T>
static typename FCoordinateFrame<T>::ReturnType cf(TObjectPtr<T>& Object)
{
	return FCoordinateFrame<T>::Make(*Object);
}


template <typename T, typename P>
static typename FCoordinateFrameAux<T, P>::ConstReturnType cf(const T& Object, const P& Parameter)
{
	return FCoordinateFrameAux<T, P>::MakeConst(Object, Parameter);
}

template <typename T, typename P>
static typename FCoordinateFrameAux<T, P>::ConstReturnType cf(const T* Object, const P& Parameter)
{
	return FCoordinateFrameAux<T, P>::MakeConst(*Object, Parameter);
}

template <typename T, typename P>
static typename FCoordinateFrameAux<T, P>::ReturnType cf(T& Object, const P& Parameter)
{
	return FCoordinateFrameAux<T, P>::Make(Object, Parameter);
}

template <typename T, typename P>
static typename FCoordinateFrameAux<T, P>::ReturnType cf(T* Object, const P& Parameter)
{
	return FCoordinateFrameAux<T, P>::Make(*Object, Parameter);
}
