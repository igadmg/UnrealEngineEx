#pragma once

#include "UObject/UnrealType.h"
#include "PropertyAccessor.generated.h"



USTRUCT(BlueprintType)
struct UNREALENGINEEX_API FPropertyAccessor
{
	GENERATED_BODY()


public:
	/*
	template <typename T>
	T GetValue() const
	{
		T* ValuePtr = Property->ContainerPtrToValuePtr<T>(Object);
		return *ValuePtr;
	}

	template <typename T>
	void SetValue(const T& value)
	{
		T* ValuePtr = Property->ContainerPtrToValuePtr<T>(Object);
		*ValuePtr = value;
	}
	*/
};


USTRUCT(BlueprintType)
struct UNREALENGINEEX_API FUObjectPropertyAccessor : public FPropertyAccessor
{
	GENERATED_BODY()


protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UObject* Object;

	FProperty* Property;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FName PropertyName;


public:
	FUObjectPropertyAccessor() {}
	FUObjectPropertyAccessor(UObject* Object, FName PropertyName)
		: Object(Object)
		, Property(nullptr)
		, PropertyName(PropertyName)
	{
		if (::IsValid(Object))
		{
			Property = Object->GetClass()->FindPropertyByName(PropertyName);
		}
	}
	FUObjectPropertyAccessor(UObject* Object, FProperty* Property)
		: Object(Object)
		, Property(Property)
		, PropertyName(Property->GetFName())
	{
	}


public:
	bool IsValid() const
	{
		return ::IsValid(Object) && Property != nullptr;
	}

	template <typename T>
	T GetValue() const
	{
		T* ValuePtr = Property->ContainerPtrToValuePtr<T>(Object);
		return *ValuePtr;
	}

	template <typename T>
	void SetValue(const T& value)
	{
		T* ValuePtr = Property->ContainerPtrToValuePtr<T>(Object);
		*ValuePtr = value;
	}
};

USTRUCT(BlueprintType)
struct UNREALENGINEEX_API FUStructPropertyAccessor : public FPropertyAccessor
{
	GENERATED_BODY()


protected:
	void* Object;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UStruct* PropertyType;

	FProperty* Property;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FName PropertyName;


public:
	FUStructPropertyAccessor() {}
	FUStructPropertyAccessor(void* Object, UStruct* PropertyType, FName PropertyName)
		: Object(Object)
		, PropertyType(PropertyType)
		, Property(nullptr)
		, PropertyName(PropertyName)
	{
		if (::IsValid(PropertyType))
		{
			Property = PropertyType->FindPropertyByName(PropertyName);
		}
	}
	FUStructPropertyAccessor(UObject* Object, UStruct* PropertyType, FProperty* Property)
		: Object(Object)
		, PropertyType(PropertyType)
		, Property(Property)
		, PropertyName(Property->GetFName())
	{
	}


public:
	bool IsValid() const
	{
		return Object != nullptr && PropertyType != nullptr && Property != nullptr;
	}

	template <typename T>
	T GetValue() const
	{
		T* ValuePtr = Property->ContainerPtrToValuePtr<T>(Object);
		return *ValuePtr;
	}

	template <typename T>
	void SetValue(const T& value)
	{
		T* ValuePtr = Property->ContainerPtrToValuePtr<T>(Object);
		*ValuePtr = value;
	}
};


UCLASS(BlueprintType)
class UNREALENGINEEX_API UBoolPropertyAccessor : public UObject
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable)
	bool GetValue()
	{
		return false;
	}

	UFUNCTION(BlueprintCallable)
	void SetValue(bool vlaue)
	{

	}
};
