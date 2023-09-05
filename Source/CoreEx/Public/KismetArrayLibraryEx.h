#pragma once

#include "Kismet/KismetArrayLibrary.h"
#include "KismetArrayLibraryEx.generated.h"


UCLASS(meta = (BlueprintThreadSafe))
class COREEX_API UKismetArrayLibraryEx : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


protected:
	UFUNCTION(Category = "Utilities|Array", BlueprintCallable, CustomThunk, meta = (ArrayParm = "TargetArray", ArrayTypeDependentParams = "Value", AutoCreateRefTerm = "Value", CompactNodeTitle = "BSL", DeprecatedFunction, DisplayName = "BinarySearchLower"))
	static int32 Array_BinarySearchLower(const TArray<int32>& TargetArray, const int32& Value);
	static int32 GenericArray_BinarySearchLower(void* TargetArray, const FArrayProperty* ArrayProp, const void* Value);


public:
	DECLARE_FUNCTION(execArray_BinarySearchLower)
	{
		Stack.MostRecentProperty = nullptr;
		Stack.StepCompiledIn<FArrayProperty>(NULL);
		void* ArrayAddr = Stack.MostRecentPropertyAddress;
		FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Stack.MostRecentProperty);
		if (!ArrayProperty)
		{
			Stack.bArrayContextFailed = true;
			return;
		}

		// Since NewItem isn't really an int, step the stack manually
		const FProperty* InnerProp = ArrayProperty->Inner;
		const int32 PropertySize = InnerProp->ElementSize * InnerProp->ArrayDim;
		void* StorageSpace = FMemory_Alloca(PropertySize);
		InnerProp->InitializeValue(StorageSpace);

		Stack.MostRecentPropertyAddress = NULL;
		Stack.StepCompiledIn<FProperty>(StorageSpace);

		P_FINISH;
		P_NATIVE_BEGIN;
		MARK_PROPERTY_DIRTY(Stack.Object, ArrayProperty);
		*(int32*)RESULT_PARAM = GenericArray_BinarySearchLower(ArrayAddr, ArrayProperty, StorageSpace);
		P_NATIVE_END;
		InnerProp->DestroyValue(StorageSpace);
	}
};
