// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineIdentityNakama.h"
#include "NakamaUtils.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemNakamaDef.h"

FUserOnlineAccountNakama::FUserOnlineAccountNakama(const FUniqueNetIdRef& InUserId, const Nakama::NSessionPtr& InSessionPtr)
	: SessionPtr(InSessionPtr)
	, UserId(InUserId)
{
	
}

FUniqueNetIdRef FUserOnlineAccountNakama::GetUserId() const
{
	return UserId;
}

FString FUserOnlineAccountNakama::GetRealName() const
{
	return FNakamaUtils::StdStringToUEString(SessionPtr->getUsername());
}

FString FUserOnlineAccountNakama::GetAccessToken() const
{
	return FNakamaUtils::StdStringToUEString(SessionPtr->getAuthToken());
}

FString FUserOnlineAccountNakama::GetDisplayName(const FString& Platform) const
{
	return GetRealName();
}

FOnlineIdentityNakama::FOnlineIdentityNakama(Nakama::NClientPtr InClient)
{
	Client = InClient;	
}

FOnlineIdentityNakama::~FOnlineIdentityNakama()
{
	Client.reset();
}

bool FOnlineIdentityNakama::Login(int32 LocalUserNum, const FOnlineAccountCredentials& AccountCredentials)
{
	if(AccountCredentials.Type.Equals(FNakamaLoginCredentialType::Custom.ToString(), ESearchCase::IgnoreCase))
	{
		const std::function<void(NSessionPtr)> SuccessCallback = [this, LocalUserNum](const NSessionPtr& Session)
		{
			const ELoginStatus::Type OldLoginStatus = GetLoginStatus(LocalUserNum);
			
			TSharedPtr<FUserOnlineAccount>& UserAccount = UserAccounts.FindOrAdd(LocalUserNum);
			const FUniqueNetIdRef UserIdRef = CreateUniquePlayerId(FNakamaUtils::StdStringToUEString(Session->getUserId())).ToSharedRef();
			UserAccount = MakeShared<FUserOnlineAccountNakama>(UserIdRef, Session);
			TriggerOnLoginCompleteDelegates(LocalUserNum, true, *UserIdRef, TEXT(""));

			const ELoginStatus::Type NewLoginStatus = GetLoginStatus(LocalUserNum);
			if(NewLoginStatus != OldLoginStatus)
			{
				TriggerOnLoginStatusChangedDelegates(LocalUserNum, ELoginStatus::NotLoggedIn, ELoginStatus::LoggedIn, *UserIdRef);
			}
		};

		const ErrorCallback ErrorCallback = [this, LocalUserNum](const NError& Error)
		{
			const FString ErrorMessage = *FNakamaUtils::StdStringToUEString(Error.message);
			TriggerOnLoginCompleteDelegates(LocalUserNum, false, *FUniqueNetIdString::EmptyId(), ErrorMessage);
		};
		
		Client->authenticateCustom(
			FNakamaUtils::UEStringToStdString(AccountCredentials.Token), 
			FNakamaUtils::UEStringToStdString(AccountCredentials.Id),
			true,
			NStringMap(),
			SuccessCallback,
			ErrorCallback);
		
		return true;
	}
	return false;
}

bool FOnlineIdentityNakama::Logout(int32 LocalUserNum)
{
	if(UserAccounts.Contains(LocalUserNum))
	{
		TriggerOnLoginStatusChangedDelegates(
			LocalUserNum,
			ELoginStatus::LoggedIn,
			ELoginStatus::NotLoggedIn,
			*UserAccounts[LocalUserNum]->GetUserId());
		
		UserAccounts.Remove(LocalUserNum);
	}

	TriggerOnLogoutCompleteDelegates(LocalUserNum, true);
	
	return true;
}

bool FOnlineIdentityNakama::AutoLogin(int32 LocalUserNum)
{
	return false;
}

TSharedPtr<FUserOnlineAccount> FOnlineIdentityNakama::GetUserAccount(const FUniqueNetId& UserId) const
{
	for(const TTuple<int, TSharedPtr<FUserOnlineAccount, ESPMode::ThreadSafe>>& UserAccount : UserAccounts)
	{
		if(UserAccount.Value.IsValid() == false) continue;
		
		const FUniqueNetIdRef UserIdRef =  UserAccount.Value->GetUserId();
		if(UserAccount.Value->GetUserId().Get() == UserId)
		{
			return UserAccount.Value;
		}
	}
	return nullptr;
}

TArray<TSharedPtr<FUserOnlineAccount>> FOnlineIdentityNakama::GetAllUserAccounts() const
{
	TArray<TSharedPtr<FUserOnlineAccount>> UserAccountArray;
	UserAccounts.GenerateValueArray(UserAccountArray);
	return UserAccountArray;
}

FUniqueNetIdPtr FOnlineIdentityNakama::GetUniquePlayerId(int32 LocalUserNum) const
{
	return UserAccounts.Contains(LocalUserNum) ? UserAccounts.Find(LocalUserNum)->Get()->GetUserId() : FUniqueNetIdPtr();
}

FUniqueNetIdPtr FOnlineIdentityNakama::CreateUniquePlayerId(uint8* Bytes, int32 Size)
{
	return nullptr;
}

FUniqueNetIdPtr FOnlineIdentityNakama::CreateUniquePlayerId(const FString& Str)
{
	return FUniqueNetIdString::Create(Str, TEXT("Nakama"));
}

ELoginStatus::Type FOnlineIdentityNakama::GetLoginStatus(int32 LocalUserNum) const
{
	return UserAccounts.Contains(LocalUserNum) ? ELoginStatus::LoggedIn : ELoginStatus::NotLoggedIn;
}

ELoginStatus::Type FOnlineIdentityNakama::GetLoginStatus(const FUniqueNetId& UserId) const
{
	for(const TTuple<int, TSharedPtr<FUserOnlineAccount, ESPMode::ThreadSafe>>& UserAccount : UserAccounts)
	{
		if(UserAccount.Value.IsValid() == false) continue;
		
		const FUniqueNetIdRef UserIdRef =  UserAccount.Value->GetUserId();
		if(UserAccount.Value->GetUserId().Get() == UserId)
		{
			return ELoginStatus::LoggedIn;
		}
	}
	return ELoginStatus::NotLoggedIn;
}

FString FOnlineIdentityNakama::GetPlayerNickname(int32 LocalUserNum) const
{
	if(UserAccounts.Contains(LocalUserNum))
	{
		const TSharedPtr<FUserOnlineAccount>* UserOnlineAccount = UserAccounts.Find(LocalUserNum);
		if(UserOnlineAccount && UserOnlineAccount->IsValid())
		{
			return UserOnlineAccount->Get()->GetDisplayName();
		}
	}
	return TEXT("");
}

FString FOnlineIdentityNakama::GetPlayerNickname(const FUniqueNetId& UserId) const
{
	const TSharedPtr<FUserOnlineAccount> UserAccountPtr = GetUserAccount(UserId);
	if(UserAccountPtr.IsValid())
	{
		return UserAccountPtr->GetDisplayName();
	}
	return TEXT("");
}

FString FOnlineIdentityNakama::GetAuthToken(int32 LocalUserNum) const
{
	if(UserAccounts.Contains(LocalUserNum))
    {
    	const TSharedPtr<FUserOnlineAccount>* UserOnlineAccount = UserAccounts.Find(LocalUserNum);
    	if(UserOnlineAccount && UserOnlineAccount->IsValid())
    	{
    		return UserOnlineAccount->Get()->GetAccessToken();
    	}
    }
    return TEXT("");
}

void FOnlineIdentityNakama::RevokeAuthToken(const FUniqueNetId& UserId, const FOnRevokeAuthTokenCompleteDelegate& Delegate)
{
	// ToDo: refresh auth token
}

void FOnlineIdentityNakama::GetUserPrivilege(const FUniqueNetId& UserId, EUserPrivileges::Type Privilege, const FOnGetUserPrivilegeCompleteDelegate& Delegate)
{
}

FPlatformUserId FOnlineIdentityNakama::GetPlatformUserIdFromUniqueNetId(const FUniqueNetId& UniqueNetId) const
{
	// ToDo: Retrieve platform user ID from native online subsystem
	return FPlatformUserId();
}

FString FOnlineIdentityNakama::GetAuthType() const
{
	return TEXT("Nakama");
}
