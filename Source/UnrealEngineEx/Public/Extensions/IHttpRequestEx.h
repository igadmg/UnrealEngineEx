#pragma once

#include "ExPrologue.h"
#include "Extensions/ArrayEx.h"
#include "Interfaces/IHttpRequest.h"
#include "JsonObjectConverter.h"
#include "ValidEx.h"

#include "Extensions/IHttpResponseEx.h"


DECLARE_CONST_EXTENSION(IHttpRequest)
	FString GetContentAsString()
	{
		auto Content = This()->GetContent();
		// Content is NOT null-terminated; we need to specify lengths here
		FUTF8ToTCHAR TCHARData(reinterpret_cast<const ANSICHAR*>(Content.GetData()), Content.Num());
		return FString(TCHARData.Length(), TCHARData.Get());
	}
};

DECLARE_MUTABLE_EXTENSION(IHttpRequest)
	template <typename TUStruct>
	bool SetContentAsUStruct(const TUStruct& InStruct, int64 CheckFlags = 0, int64 SkipFlags = 0, int32 Indent = 0, const FJsonObjectConverter::CustomExportCallback* ExportCb = nullptr, bool bPrettyPrint = true)
	{
		FString JsonString;
		if (FJsonObjectConverter::UStructToJsonObjectString(InStruct, JsonString, CheckFlags, SkipFlags, Indent, ExportCb, bPrettyPrint))
		{
			This()->SetContentAsString(JsonString);

			return true;
		}

		return false;
	}

	template <typename TUStruct>
	bool SetContentAsUStruct(const TArray<TUStruct>& InStructArray, int64 CheckFlags = 0, int64 SkipFlags = 0, int32 Indent = 0, const FJsonObjectConverter::CustomExportCallback* ExportCb = nullptr, bool bPrettyPrint = true)
	{
		auto SerializedStructArray = ex(InStructArray).Select<FString>([CheckFlags, SkipFlags, Indent, ExportCb, bPrettyPrint](const auto& Struct) {
			FString JsonString;
			FJsonObjectConverter::UStructToJsonObjectString(Struct, JsonString, CheckFlags, SkipFlags, Indent, ExportCb, bPrettyPrint);
			return JsonString;
		});

		This()->SetContentAsString(FString::Format(TEXT("[{0}]"), { FString::Join(SerializedStructArray, TEXT(", ")) } ));

		return true;
	}

	template <typename TFailureData>
	bool ProcessRequest(UObject* Owner, TFunction<void()> OnSuccess, TFunction<void(FHttpResponsePtr, const TFailureData&)> OnFailure)
	{
		This()->OnProcessRequestComplete().BindWeakLambda(Owner
			, [OnSuccess, OnFailure](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
				if (ex(Response).IsSuccessful())
				{
					OnSuccess();
				}
				else
				{
					TFailureData Data;
					if (ex(Response).GetContentAsUStruct(Data))
						OnFailure(Response, Data);
					else
						OnFailure(Response, TFailureData());
				}
			});

		return This()->ProcessRequest();
	}

	template <typename TSuccessData, typename TFailureData>
	bool ProcessRequest(UObject* Owner, TFunction<void(const TSuccessData&)> OnSuccess, TFunction<void(FHttpResponsePtr, const TFailureData&)> OnFailure)
	{
		This()->OnProcessRequestComplete().BindWeakLambda(Owner
			, [OnSuccess, OnFailure](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
				if (ex(Response).IsSuccessful())
				{
					TSuccessData Data;
					if (ex(Response).GetContentAsUStruct(Data))
						OnSuccess(Data);
					else
						OnFailure(Response, TFailureData());
				}
				else
				{
					TFailureData Data;
					if (ex(Response).GetContentAsUStruct(Data))
						OnFailure(Response, Data);
					else
						OnFailure(Response, TFailureData());
				}
			});

		return This()->ProcessRequest();
	}
};

DECLARE_EXTENSION(IHttpRequest);
