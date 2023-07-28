// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineSubsystemNakamaModule.h"
#include "OnlineFactoryNakama.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemNakama.h"
#include "OnlineSubsystemNakamaDef.h"

IMPLEMENT_MODULE(FOnlineSubsystemNakamaModule, OnlineSubsystemNakama)

FOnlineSubsystemNakamaModule::FOnlineSubsystemNakamaModule()
	: NakamaFactory(nullptr)
{
}

FOnlineSubsystemNakamaModule::~FOnlineSubsystemNakamaModule()
{
}

void FOnlineSubsystemNakamaModule::StartupModule()
{
	UE_LOG_ONLINE(Log, TEXT("Nakama Startup!"));
	
	NakamaFactory = MakeUnique<FOnlineFactoryNakama>();

	// Create and register our singleton factory with the main online subsystem for easy access
	FOnlineSubsystemModule& OnlineSubsystemModule = FModuleManager::GetModuleChecked<FOnlineSubsystemModule>("OnlineSubsystem");
	OnlineSubsystemModule.RegisterPlatformService(NAKAMA_SUBSYSTEM, NakamaFactory.Get());
}

void FOnlineSubsystemNakamaModule::ShutdownModule()
{
	UE_LOG_ONLINE(Log, TEXT("Nakama Shutdown!"));

	FOnlineSubsystemModule& OnlineSubsystemModule = FModuleManager::GetModuleChecked<FOnlineSubsystemModule>("OnlineSubsystem");
	OnlineSubsystemModule.UnregisterPlatformService(NAKAMA_SUBSYSTEM);
	
	NakamaFactory = nullptr;
}

