#pragma once

#include "Dom/JsonObject.h"

#include "LdConfigObject.generated.h"

#if PLATFORM_WINDOWS
struct LDConfig;
#endif

UCLASS(BlueprintType)
class LAUNCHDARKLYCLIENT_API ULdConfigObject : public UObject
{
	GENERATED_BODY()
public:
	ULdConfigObject ();

	FString ToJsonString();

#if PLATFORM_WINDOWS
	LDConfig* const ToLdConfig(); 
#endif

protected:
	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly Config")
	static ULdConfigObject* CreateLdConfigObject(FString MobileEnvironmentKey);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly Config")
	ULdConfigObject* SetAllAttributesPrivate();

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly Config")
	ULdConfigObject* SetBackgroundPollingIntervalMillis(int PollingIntervalMillis);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly Config")
	ULdConfigObject* SetBaseUri(FString BaseUri);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly Config")
	ULdConfigObject* SetConnectionTimeoutMillis(int ConnectionTimeoutMillis);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly Config")
	ULdConfigObject* SetDisableBackgroundUpdating(bool DisableBackgroundUpdating);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly Config")
	ULdConfigObject* SetEventsCapacity(int EventsCapacity);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly Config")
	ULdConfigObject* SetEventsFlushIntervalMillis(int FlushIntervalMillis);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly Config")
	ULdConfigObject* SetEventsUri(FString EventsUri);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly Config")
	ULdConfigObject* SetOffline(bool SetOffline);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly Config")
	ULdConfigObject* SetPollingIntervalInMillis(int PollingIntervalInMillis);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly Config")
	ULdConfigObject* AddPrivateAttributeName(FString PrivateAttributeName);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly Config")
	ULdConfigObject* SetStream(bool IsStreaming);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly Config")
	ULdConfigObject* SetStreamUri(FString StreamUri);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly Config")
	ULdConfigObject* SetUseReport(bool UseReport);

	TSharedPtr<FJsonObject> ConfigData;
	TArray<FString> PrivateAttributesArray;

	static const FString KeyAllAttributesPrivate;
	static const FString KeyBackgroundPollingIntervalMillis;
	static const FString KeyBaseUri;
	static const FString KeyConnectionTimeoutMillis;
	static const FString KeyDisableBackgroundUpdating;
	static const FString KeyEventsCapacity;
	static const FString KeyEventsFlushIntervalMillis;
	static const FString KeyEventsUri;
	static const FString KeyMobileKey;
	static const FString KeyOffline;
	static const FString KeyPollingIntervalInMillis;
	static const FString KeyPrivateAttributeNames;
	static const FString KeyStream;
	static const FString KeyStreamUri;
	static const FString KeyUseReport;

private:
	ULdConfigObject* SetMobileKey(FString MobileKey);
};
