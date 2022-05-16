#pragma once

#include "ExPrologue.h"
#include "Interfaces/IHttpResponse.h"
#include "JsonObjectConverter.h"
#include "ValidEx.h"



DECLARE_CONST_EXTENSION(IHttpResponse)
};

DECLARE_MUTABLE_EXTENSION(IHttpResponse)
	bool IsSuccessful()
	{
		auto ResponseCode = This()->GetResponseCode();
		return ResponseCode >= 200 && ResponseCode < 300;
	}

	template<typename TUStruct>
	bool GetContentAsUStruct(TUStruct& InStruct)
	{
		return FJsonObjectConverter::JsonObjectStringToUStruct(This()->GetContentAsString(), &InStruct);
	}
};

DECLARE_EXTENSION(IHttpResponse);
