#pragma once

#include "CoreMinimal.h"

class FJsonObject;
class ULdConfigObject;
class ULdBoolFlagListener;
class ULdFloatFlagListener;
class ULdIntFlagListener;
class ULdJsonFlagListener;
class ULdStringFlagListener;
class ULdBoolFlagListenerComponent;
class ULdFloatFlagListenerComponent;
class ULdIntFlagListenerComponent;
class ULdJsonFlagListenerComponent;
class ULdStringFlagListenerComponent;
class ULdUserObject;

typedef void (*FlagListenerFP)(const char* const, const int);

DECLARE_LOG_CATEGORY_EXTERN(LaunchDarklyClient, Log, All);

class LAUNCHDARKLYCLIENT_API FLaunchDarklyImplBase
{
public:

	virtual ~FLaunchDarklyImplBase() {}

	////////////////////////////////////////////////////////////////////////////////////////////////
	// CLIENT STATE ////////////////////////////////////////////////////////////////////////////////
	virtual bool InitializeClient(ULdConfigObject* LdConfig, ULdUserObject* LdUser, int ConnectionTimeoutMillis);

	virtual void ShutdownClient();

	virtual bool IsInitialized();

	virtual bool IsOffline();

	virtual void SetOffline();

	virtual void SetOnline();

	////////////////////////////////////////////////////////////////////////////////////////////////
	// USER ATTRIBUTES /////////////////////////////////////////////////////////////////////////////
	virtual void RefreshUserAttributes(ULdUserObject* LdUserObject);

	////////////////////////////////////////////////////////////////////////////////////////////////
	// VARIATIONS //////////////////////////////////////////////////////////////////////////////////
	virtual bool GetBoolVariation(FString FlagName, bool DefaultValue);

	virtual float GetFloatVariation(FString FlagName, float DefaultValue);

	virtual int GetIntVariation(FString FlagName, int DefaultValue);

	virtual TSharedPtr<FJsonObject> GetJsonVariation(FString FlagName, TSharedPtr<FJsonObject> DefaultValue);

	virtual FString GetStringVariation(FString FlagName, FString DefaultValue);

	TMap<FString, TArray<ULdBoolFlagListener*>>& GetBoolFlagListeners();

	virtual void RegisterBoolFlagListener(ULdBoolFlagListener* FlagListener, FString FlagName);

	virtual void UnregisterBoolFlagListener(ULdBoolFlagListener* FlagListener, FString FlagName);

	TMap<FString, TArray<ULdFloatFlagListener*>>& GetFloatFlagListeners();

	virtual void RegisterFloatFlagListener(ULdFloatFlagListener* FlagListener, FString FlagName);

	virtual void UnregisterFloatFlagListener(ULdFloatFlagListener* FlagListener, FString FlagName);

	TMap<FString, TArray<ULdIntFlagListener*>>& GetIntFlagListeners();

	virtual void RegisterIntFlagListener(ULdIntFlagListener* FlagListener, FString FlagName);

	virtual void UnregisterIntFlagListener(ULdIntFlagListener* FlagListener, FString FlagName);

	TMap<FString, TArray<ULdJsonFlagListener*>>& GetJsonFlagListeners();

	virtual void RegisterJsonFlagListener(ULdJsonFlagListener* FlagListener, FString FlagName);

	virtual void UnregisterJsonFlagListener(ULdJsonFlagListener* FlagListener, FString FlagName);

	TMap<FString, TArray<ULdStringFlagListener*>>& GetStringFlagListeners();

	virtual void RegisterStringFlagListener(ULdStringFlagListener* FlagListener, FString FlagName);

	virtual void UnregisterStringFlagListener(ULdStringFlagListener* FlagListener, FString FlagName);

	TMap<FString, TArray<ULdBoolFlagListenerComponent*>>& GetBoolFlagListenerComponents();

	virtual void RegisterBoolFlagListenerComponent(ULdBoolFlagListenerComponent* FlagListenerComponent, FString FlagName);
	
	virtual void UnregisterBoolFlagListenerComponent(ULdBoolFlagListenerComponent* FlagListener, FString FlagName);

	TMap<FString, TArray<ULdFloatFlagListenerComponent*>>& GetFloatFlagListenerComponents();

	virtual void RegisterFloatFlagListenerComponent(ULdFloatFlagListenerComponent* FlagListenerComponent, FString FlagName);
	
	virtual void UnregisterFloatFlagListenerComponent(ULdFloatFlagListenerComponent* FlagListener, FString FlagName);

	TMap<FString, TArray<ULdIntFlagListenerComponent*>>& GetIntFlagListenerComponents();

	virtual void RegisterIntFlagListenerComponent(ULdIntFlagListenerComponent* FlagListenerComponent, FString FlagName);
	
	virtual void UnregisterIntFlagListenerComponent(ULdIntFlagListenerComponent* FlagListener, FString FlagName);

	TMap<FString, TArray<ULdJsonFlagListenerComponent*>>& GetJsonFlagListenerComponents();

	virtual void RegisterJsonFlagListenerComponent(ULdJsonFlagListenerComponent* FlagListenerComponent, FString FlagName);
	
	virtual void UnregisterJsonFlagListenerComponent(ULdJsonFlagListenerComponent* FlagListener, FString FlagName);

	TMap<FString, TArray<ULdStringFlagListenerComponent*>>& GetStringFlagListenerComponents();

	virtual void RegisterStringFlagListenerComponent(ULdStringFlagListenerComponent* FlagListenerComponent, FString FlagName);
	
	virtual void UnregisterStringFlagListenerComponent(ULdStringFlagListenerComponent* FlagListener, FString FlagName);

	TMap<FString, FlagListenerFP>& GetUnregisteredFlagListeners();


	////////////////////////////////////////////////////////////////////////////////////////////////
	// METRIC TRACKING /////////////////////////////////////////////////////////////////////////////
	virtual void Track(FString MetricName);

	virtual void Track(FString MetricName, TSharedPtr<FJsonObject> Data);

	////////////////////////////////////////////////////////////////////////////////////////////////
	// ANDROID SPECIFIC ////////////////////////////////////////////////////////////////////////////
	virtual void InitJavaFunctions();

protected:
	void ResetListeners();

	UPROPERTY()
	TMap<FString, FlagListenerFP> UnregisteredFlagListeners;
	UPROPERTY()
	TMap<FString, TArray<ULdBoolFlagListener*>> BoolFlagListeners;
	UPROPERTY()
	TMap<FString, TArray<ULdFloatFlagListener*>> FloatFlagListeners;
	UPROPERTY()
	TMap<FString, TArray<ULdIntFlagListener*>> IntFlagListeners;
	UPROPERTY()
	TMap<FString, TArray<ULdJsonFlagListener*>> JsonFlagListeners;
	UPROPERTY()
	TMap<FString, TArray<ULdStringFlagListener*>> StringFlagListeners;
	UPROPERTY()
	TMap<FString, TArray<ULdBoolFlagListenerComponent*>> BoolFlagListenerComponents;
	UPROPERTY()
	TMap<FString, TArray<ULdFloatFlagListenerComponent*>> FloatFlagListenerComponents;
	UPROPERTY()
	TMap<FString, TArray<ULdIntFlagListenerComponent*>> IntFlagListenerComponents;
	UPROPERTY()
	TMap<FString, TArray<ULdJsonFlagListenerComponent*>> JsonFlagListenerComponents;
	UPROPERTY()
	TMap<FString, TArray<ULdStringFlagListenerComponent*>> StringFlagListenerComponents;
};