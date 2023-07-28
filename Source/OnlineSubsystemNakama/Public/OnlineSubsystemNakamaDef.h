// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#ifndef NAKAMA_SUBSYSTEM
#define NAKAMA_SUBSYSTEM FName(TEXT("NAKAMA"))
#endif

class ONLINESUBSYSTEMNAKAMA_API FNakamaLoginCredentialType
{
public:
	FNakamaLoginCredentialType() = delete;
	FNakamaLoginCredentialType(const FString& InTypeStr)
		: TypeStr(InTypeStr)
	{
		
	}
	const FString& ToString() const { return TypeStr; }
	
	static FNakamaLoginCredentialType Custom;

private:
	FString TypeStr;
};