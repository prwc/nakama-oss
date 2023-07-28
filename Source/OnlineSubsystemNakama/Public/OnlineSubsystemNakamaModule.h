// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class FOnlineFactoryNakama;

class FOnlineSubsystemNakamaModule : public IModuleInterface
{
public:
	FOnlineSubsystemNakamaModule();
	virtual ~FOnlineSubsystemNakamaModule() override;

	// IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual bool SupportsDynamicReloading() override
	{
		return false;
	}

	virtual bool SupportsAutomaticShutdown() override
	{
		return false;
	}
	
private:
	/** Class responsible for creating instance(s) of the subsystem */
	TUniquePtr<FOnlineFactoryNakama> NakamaFactory;
};
