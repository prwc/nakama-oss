// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineFactoryNakama.h"
#include "OnlineSubsystemNakama.h"

IOnlineSubsystemPtr FOnlineFactoryNakama::CreateSubsystem(FName InstanceName)
{
	{
		FOnlineSubsystemNakamaPtr OnlineSub = MakeShared<FOnlineSubsystemNakama, ESPMode::ThreadSafe>(InstanceName);
		if (OnlineSub->IsEnabled())
		{
			if(!OnlineSub->Init())
			{
				UE_LOG_ONLINE(Warning, TEXT("Nakama API failed to initialize!"));
				OnlineSub->Shutdown();
				OnlineSub.Reset();
			}
		}
		else
		{
			UE_LOG_ONLINE(Warning, TEXT("Nakama API disabled!"));
			OnlineSub->Shutdown();
			OnlineSub.Reset();
		}

		return OnlineSub;
	}
}
