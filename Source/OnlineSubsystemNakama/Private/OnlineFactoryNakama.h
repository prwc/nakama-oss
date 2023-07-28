// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"

/**
 * Class responsible for creating instance(s) of the subsystem
 */
class FOnlineFactoryNakama : public IOnlineFactory
{
public:

	FOnlineFactoryNakama() {}
	virtual ~FOnlineFactoryNakama() override {}

	virtual IOnlineSubsystemPtr CreateSubsystem(FName InstanceName) override;
};