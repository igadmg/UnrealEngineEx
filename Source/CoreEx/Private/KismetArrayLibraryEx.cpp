#include "KismetArrayLibraryEx.h"

#include "CoreEx.final.h"



int32 UKismetArrayLibraryEx::Array_BinarySearchLower(const TArray<int32>& TargetArray, const int32& NewIValuetem)
{
	// We should never hit these!  They're stubs to avoid NoExport on the class.  Call the Generic* equivalent instead
	check(0);
	return 0;
}

int32 UKismetArrayLibraryEx::GenericArray_BinarySearchLower(void* TargetArray, const FArrayProperty* ArrayProp, const void* Value)
{
	int32 NewIndex = INDEX_NONE;
	if (TargetArray)
	{
		FScriptArrayHelper ArrayHelper(ArrayProp, TargetArray);
		FProperty* InnerProp = ArrayProp->Inner;

		// TODO find less function.
		// Use it to compare.

		//NewIndex = ArrayHelper.AddValue();
		//InnerProp->CopySingleValueToScriptVM(ArrayHelper.GetRawPtr(NewIndex), Value);
	}
	return NewIndex;
}