// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "nakama-cpp/NClientInterface.h"

/**
 * User Online Account Nakama
 */
class FUserOnlineAccountNakama : public FUserOnlineAccount
{
public:
	FUserOnlineAccountNakama() = delete;
	explicit FUserOnlineAccountNakama(const FUniqueNetIdRef& InUserId, const Nakama::NSessionPtr& InSessionPtr);
	virtual ~FUserOnlineAccountNakama() override {}

	virtual FUniqueNetIdRef GetUserId() const override;
	virtual FString GetRealName() const override;
	virtual FString GetAccessToken() const override;
	virtual FString GetDisplayName(const FString& Platform) const override;
	
	virtual bool HasAccessTokenExpired(const FDateTime& Time) const override { return false; }
	virtual bool GetAuthAttribute(const FString& AttrName, FString& OutAttrValue) const override { return false; }
	virtual bool GetUserAttribute(const FString& AttrName, FString& OutAttrValue) const override { return false; }
	virtual bool SetUserAttribute(const FString& AttrName, const FString& AttrValue) override { return false; }

private:
	Nakama::NSessionPtr SessionPtr;
	FUniqueNetIdRef UserId;
};

/**
 * Online Identity Nakama
 */
class FOnlineIdentityNakama : public IOnlineIdentity
{
public:
	FOnlineIdentityNakama() = delete;
	explicit FOnlineIdentityNakama(Nakama::NClientPtr InClient);
	virtual ~FOnlineIdentityNakama() override;

	// IOnlineIdentity
	virtual bool Login(int32 LocalUserNum, const FOnlineAccountCredentials& AccountCredentials) override;
	virtual bool Logout(int32 LocalUserNum) override;
	virtual bool AutoLogin(int32 LocalUserNum) override;
	virtual TSharedPtr<FUserOnlineAccount> GetUserAccount(const FUniqueNetId& UserId) const override;
	virtual TArray<TSharedPtr<FUserOnlineAccount> > GetAllUserAccounts() const override;
	virtual FUniqueNetIdPtr GetUniquePlayerId(int32 LocalUserNum) const override;
	virtual FUniqueNetIdPtr CreateUniquePlayerId(uint8* Bytes, int32 Size) override;
	virtual FUniqueNetIdPtr CreateUniquePlayerId(const FString& Str) override;
	virtual ELoginStatus::Type GetLoginStatus(int32 LocalUserNum) const override;
	virtual ELoginStatus::Type GetLoginStatus(const FUniqueNetId& UserId) const override;
	virtual FString GetPlayerNickname(int32 LocalUserNum) const override;
	virtual FString GetPlayerNickname(const FUniqueNetId& UserId) const override;
	virtual FString GetAuthToken(int32 LocalUserNum) const override;
	virtual void RevokeAuthToken(const FUniqueNetId& UserId, const FOnRevokeAuthTokenCompleteDelegate& Delegate) override;
	virtual void GetUserPrivilege(const FUniqueNetId& UserId, EUserPrivileges::Type Privilege, const FOnGetUserPrivilegeCompleteDelegate& Delegate) override;
	virtual FPlatformUserId GetPlatformUserIdFromUniqueNetId(const FUniqueNetId& UniqueNetId) const override;
	virtual FString GetAuthType() const override;

private:
	Nakama::NClientPtr Client;
	TMap<int32, TSharedPtr<FUserOnlineAccount>> UserAccounts;
};
