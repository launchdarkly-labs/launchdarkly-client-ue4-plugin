#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "LaunchDarklyBPLibrary.generated.h"

class ULdBoolFlagListener;
class ULdConfigObject;
class ULdFloatFlagListener;
class ULdIntFlagListener;
class ULdJsonFlagListener;
class ULdNodeObject;
class ULdStringFlagListener;
class ULdUserObject;

UCLASS()
class LAUNCHDARKLYCLIENT_API ULaunchDarklyBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	////////////////////////////////////////////////////////////////////////////////////////////////
	// CLIENT STATE ////////////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly Client")
		static bool InitializeLaunchDarkly(ULdConfigObject* LdConfig, ULdUserObject* LdUser, int ConnectionTimeoutMillis);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly Client")
		static void ShutdownLaunchDarkly();

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly Client")
		static bool IsLaunchDarklyClientInitialized();

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly Client")
		static bool IsLaunchDarklyClientOffline();

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly Client")
		static void SetLaunchDarklyClientOffline();

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly Client")
		static void SetLaunchDarklyClientOnline();

	////////////////////////////////////////////////////////////////////////////////////////////////
	// USER ATTRIBUTES /////////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly User Attributes")
		static void RefreshUserAttributes(ULdUserObject* LdUser);

	////////////////////////////////////////////////////////////////////////////////////////////////
	// VARIATIONS //////////////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly Variation Acquisition")
		static bool GetBoolVariation(FString FlagName, bool DefaultValue);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly Variation Acquisition")
		static float GetFloatVariation(FString FlagName, float DefaultValue);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly Variation Acquisition")
		static int GetIntVariation(FString FlagName, int DefaultValue);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly Variation Acquisition")
		static ULdNodeObject* GetJsonVariation(FString FlagName, ULdNodeObject* DefaultValue);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly Variation Acquisition")
		static FString GetStringVariation(FString FlagName, FString DefaultValue);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly Variation Acquisition")
		static ULdBoolFlagListener* CreateBoolVariationListener(FString FlagName, bool DefaultValue);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly Variation Acquisition")
		static ULdFloatFlagListener* CreateFloatVariationListener(FString FlagName, float DefaultValue);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly Variation Acquisition")
		static ULdIntFlagListener* CreateIntVariationListener(FString FlagName, int DefaultValue);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly Variation Acquisition")
		static ULdJsonFlagListener* CreateJsonVariationListener(FString FlagName, ULdNodeObject* DefaultValue);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly Variation Acquisition")
		static ULdStringFlagListener* CreateStringVariationListener(FString FlagName, FString DefaultValue);

	////////////////////////////////////////////////////////////////////////////////////////////////
	// METRIC CONTROLS /////////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly Metrics")
		static void TrackMetric(FString MetricName);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly Metrics")
		static void TrackMetricWithData(FString MetricName, ULdNodeObject* Data);
};