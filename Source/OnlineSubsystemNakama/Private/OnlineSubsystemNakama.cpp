// Copyright Epic Games, Inc. All Rights Reserved.

#include "OnlineSubsystemNakama.h"
#include "NakamaCoreClientFactory.h"
#include "NakamaUtils.h"
#include "OnlineIdentityNakama.h"
#include "OnlineSubsystemNakamaDef.h"

FOnlineSubsystemNakama::FOnlineSubsystemNakama(FName InInstanceName) 
	: FOnlineSubsystemImpl(NAKAMA_SUBSYSTEM, InInstanceName)
	, IdentityInterface(nullptr)
	, TickTimer(0.0f)
{
}

FOnlineSubsystemNakama::~FOnlineSubsystemNakama()
{
	
}

IOnlineIdentityPtr FOnlineSubsystemNakama::GetIdentityInterface() const
{
	return IdentityInterface;
}

bool FOnlineSubsystemNakama::Tick(float DeltaTime)
{
	TickTimer += DeltaTime;
    if(TickTimer >= 50.0f/1000.0f) // ToDo: Use tick period from INI
    {
    	TickTimer = 0.0f;
    	
    	if(Client)
    	{
    		Client->tick();
    	}
    }
	
	return FOnlineSubsystemImpl::Tick(DeltaTime);
}

bool FOnlineSubsystemNakama::Init()
{
	UE_LOG_ONLINE(VeryVerbose, TEXT("FOnlineSubsystemNakama::Init()"));
	
	// Default Client Parameters
	// ToDo: Use config from INI
	const FString ServerKey = FString(TEXT("defaultkey"));
	const FString Host = FString(TEXT("localhost"));
	constexpr int32 Port = 7350;
	constexpr bool bUserSSL = false;

	NClientParameters Parameters;
	Parameters.serverKey = FNakamaUtils::UEStringToStdString(ServerKey);
	Parameters.host = FNakamaUtils::UEStringToStdString(Host);
	Parameters.ssl = bUserSSL;
	Parameters.port = Port;
	
	Client = NakamaCoreClientFactory::createNakamaClient(Parameters, NLogLevel::Info);
	IdentityInterface = MakeShared<FOnlineIdentityNakama, ESPMode::ThreadSafe>(Client);
	
	return true;
}

bool FOnlineSubsystemNakama::Shutdown()
{
	UE_LOG_ONLINE(VeryVerbose, TEXT("FOnlineSubsystemNakama::Shutdown()"));
	
	IdentityInterface.Reset();
	
	return FOnlineSubsystemImpl::Shutdown();
}

FString FOnlineSubsystemNakama::GetAppId() const
{
	return TEXT("Nakama");
}

bool FOnlineSubsystemNakama::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) 
{
	if (FOnlineSubsystemImpl::Exec(InWorld, Cmd, Ar))
	{
		return true;
	}

	// TODO: Add Exec for debugging here
	
	return false;
}

FText FOnlineSubsystemNakama::GetOnlineServiceName() const
{
	return NSLOCTEXT("OnlineSubsystemNakama", "OnlineServiceName", "Nakama");
}

bool FOnlineSubsystemNakama::IsEnabled() const
{
	// Check the ini for disabling Nakama
	bool bEnableNakama = FOnlineSubsystemImpl::IsEnabled();
	if (bEnableNakama)
	{
#if UE_EDITOR
		bEnableNakama = IsRunningDedicatedServer() || IsRunningGame();
#endif
	}
	return bEnableNakama;
}