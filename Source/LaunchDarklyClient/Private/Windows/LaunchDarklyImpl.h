#pragma once

#include "LaunchDarklyImplBase.h"

#include <launchdarkly/api.h>

struct LDClient;

////////////////////////////////////////////////////////////////////////////////////////////////////
// WINDOWS /////////////////////////////////////////////////////////////////////////////////////////
class LAUNCHDARKLYCLIENT_API FLaunchDarklyImpl : public FLaunchDarklyImplBase
{
public:
	~FLaunchDarklyImpl() override {}

	////////////////////////////////////////////////////////////////////////////////////////////////
	// CLIENT STATE ////////////////////////////////////////////////////////////////////////////////
	bool InitializeClient(ULdConfigObject* LdConfig, ULdUserObject* LdUserObject, int ConnectionTimeoutMillis) override;

	void ShutdownClient() override;

	bool IsInitialized() override;

	bool IsOffline() override;

	void SetOffline() override;

	void SetOnline() override;

	////////////////////////////////////////////////////////////////////////////////////////////////
	// USER ATTRIBUTES /////////////////////////////////////////////////////////////////////////////
	void RefreshUserAttributes(ULdUserObject* LdUserObject) override;

	////////////////////////////////////////////////////////////////////////////////////////////////
	// VARIATIONS //////////////////////////////////////////////////////////////////////////////////
	bool GetBoolVariation(FString FlagName, bool DefaultValue) override;

	float GetFloatVariation(FString FlagName, float DefaultValue) override;

	int GetIntVariation(FString FlagName, int DefaultValue) override;

	TSharedPtr<FJsonObject> GetJsonVariation(FString FlagName, TSharedPtr<FJsonObject> DefaultValue) override;

	FString GetStringVariation(FString FlagName, FString DefaultValue) override;

	void RegisterBoolFlagListener(ULdBoolFlagListener* FlagListener, FString FlagName) override;

	void UnregisterBoolFlagListener(ULdBoolFlagListener* FlagListener, FString FlagName) override;

	void RegisterIntFlagListener(ULdIntFlagListener* FlagListener, FString FlagName) override;

	void UnregisterIntFlagListener(ULdIntFlagListener* FlagListener, FString FlagName) override;

	void RegisterFloatFlagListener(ULdFloatFlagListener* FlagListener, FString FlagName) override;

	void UnregisterFloatFlagListener(ULdFloatFlagListener* FlagListener, FString FlagName) override;

	void RegisterJsonFlagListener(ULdJsonFlagListener* FlagListener, FString FlagName) override;

	void UnregisterJsonFlagListener(ULdJsonFlagListener* FlagListener, FString FlagName) override;

	void RegisterStringFlagListener(ULdStringFlagListener* FlagListener, FString FlagName) override;

	void UnregisterStringFlagListener(ULdStringFlagListener* FlagListener, FString FlagName) override;

	void RegisterBoolFlagListenerComponent(ULdBoolFlagListenerComponent* FlagListener, FString FlagName) override;

	void UnregisterBoolFlagListenerComponent(ULdBoolFlagListenerComponent* FlagListener, FString FlagName) override;

	void RegisterFloatFlagListenerComponent(ULdFloatFlagListenerComponent* FlagListener, FString FlagName) override;

	void UnregisterFloatFlagListenerComponent(ULdFloatFlagListenerComponent* FlagListener, FString FlagName) override;

	void RegisterIntFlagListenerComponent(ULdIntFlagListenerComponent* FlagListener, FString FlagName) override;

	void UnregisterIntFlagListenerComponent(ULdIntFlagListenerComponent* FlagListener, FString FlagName) override;

	void RegisterJsonFlagListenerComponent(ULdJsonFlagListenerComponent* FlagListener, FString FlagName) override;

	void UnregisterJsonFlagListenerComponent(ULdJsonFlagListenerComponent* FlagListener, FString FlagName) override;

	void RegisterStringFlagListenerComponent(ULdStringFlagListenerComponent* FlagListener, FString FlagName) override;

	void UnregisterStringFlagListenerComponent(ULdStringFlagListenerComponent* FlagListener, FString FlagName) override;

	////////////////////////////////////////////////////////////////////////////////////////////////
	// METRIC TRACKING /////////////////////////////////////////////////////////////////////////////
	void Track(FString MetricName) override;

	void Track(FString MetricName, TSharedPtr<FJsonObject> Data) override;

protected:
	// Static all the things that need to live in the two worlds of LD and UE4 Game threads.
	static LDClient* LdClient;

	static void BoolFlagListener(const char* const FlagName, const int FlagStatus);
	static void FloatFlagListener(const char* const FlagName, const int FlagStatus);
	static void IntFlagListener(const char* const FlagName, const int FlagStatus);
	static void JsonFlagListener(const char* const FlagName, const int FlagStatus);
	static void StringFlagListener(const char* const FlagName, const int FlagStatus);
	static void BoolFlagListenerEvent(const char* const FlagName, const int FlagStatus);
	static void FloatFlagListenerEvent(const char* const FlagName, const int FlagStatus);
	static void IntFlagListenerEvent(const char* const FlagName, const int FlagStatus);
	static void JsonFlagListenerEvent(const char* const FlagName, const int FlagStatus);
	static void StringFlagListenerEvent(const char* const FlagName, const int FlagStatus);

	static void LdLogger(const LDLogLevel LogLvl, const char* const Message);
	static void ClientStatusCallback(int s);

	static void CleanupFlagListeners();
	static void RestoreFlagListeners();
	static void UpdateAllFlagListeners();
};