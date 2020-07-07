#include "LaunchDarklyBPLibrary.h"

#include "LaunchDarklyClient.h"
#include "LDConfigObject.h"
#include "LdBoolFlagListener.h"
#include "LdFloatFlagListener.h"
#include "LdIntFlagListener.h"
#include "LdJsonFlagListener.h"
#include "LdStringFlagListener.h"
#include "LdNodeObject.h"
#include "LdUserObject.h"

bool ULaunchDarklyBPLibrary::InitializeLaunchDarkly(ULdConfigObject* LdConfig, ULdUserObject* LdUser, int ConnectionTimeoutMillis)
{
	return FLaunchDarklyClientModule::Get()->InitializeClient(LdConfig, LdUser, ConnectionTimeoutMillis);
}

void ULaunchDarklyBPLibrary::ShutdownLaunchDarkly()
{
	FLaunchDarklyClientModule::Get()->ShutdownClient();
}

bool ULaunchDarklyBPLibrary::IsLaunchDarklyClientInitialized()
{
	return FLaunchDarklyClientModule::Get()->IsInitialized();
}

bool ULaunchDarklyBPLibrary::IsLaunchDarklyClientOffline()
{
	return FLaunchDarklyClientModule::Get()->IsOffline();
}

void ULaunchDarklyBPLibrary::SetLaunchDarklyClientOffline()
{
	return FLaunchDarklyClientModule::Get()->SetOffline();
}

void ULaunchDarklyBPLibrary::SetLaunchDarklyClientOnline()
{
	return FLaunchDarklyClientModule::Get()->SetOnline();
}

void ULaunchDarklyBPLibrary::RefreshUserAttributes(ULdUserObject* LdUser)
{
	FLaunchDarklyClientModule::Get()->RefreshUserAttributes(LdUser);
}

bool ULaunchDarklyBPLibrary::GetBoolVariation(FString FlagName, bool DefaultValue)
{
	return FLaunchDarklyClientModule::Get()->GetBoolVariation(FlagName, DefaultValue);
}

float ULaunchDarklyBPLibrary::GetFloatVariation(FString FlagName, float DefaultValue)
{
	return FLaunchDarklyClientModule::Get()->GetFloatVariation(FlagName, DefaultValue);
}

int ULaunchDarklyBPLibrary::GetIntVariation(FString FlagName, int DefaultValue)
{
	return FLaunchDarklyClientModule::Get()->GetIntVariation(FlagName, DefaultValue);
}

ULdNodeObject* ULaunchDarklyBPLibrary::GetJsonVariation(FString FlagName, ULdNodeObject* DefaultValue)
{
	ULdNodeObject* NodeObject = NewObject<ULdNodeObject>();
	NodeObject->Initialize(
		FLaunchDarklyClientModule::Get()->GetJsonVariation(
			FlagName, 
			DefaultValue != nullptr ? DefaultValue->GetObjectData() : nullptr
		)
	);
	return NodeObject;
}

FString ULaunchDarklyBPLibrary::GetStringVariation(FString FlagName, FString DefaultValue)
{
	return FLaunchDarklyClientModule::Get()->GetStringVariation(FlagName, DefaultValue);
}

ULdBoolFlagListener* ULaunchDarklyBPLibrary::CreateBoolVariationListener(FString FlagName, bool DefaultValue)
{
	ULdBoolFlagListener* FlagListener = NewObject<ULdBoolFlagListener>();
	FlagListener->RegisterWithFlag(FlagName, DefaultValue);
	return FlagListener;
}

ULdFloatFlagListener* ULaunchDarklyBPLibrary::CreateFloatVariationListener(FString FlagName, float DefaultValue)
{
	ULdFloatFlagListener* FlagListener = NewObject<ULdFloatFlagListener>();
	FlagListener->RegisterWithFlag(FlagName, DefaultValue);
	return FlagListener;
}

ULdIntFlagListener* ULaunchDarklyBPLibrary::CreateIntVariationListener(FString FlagName, int DefaultValue)
{
	ULdIntFlagListener* FlagListener = NewObject<ULdIntFlagListener>();
	FlagListener->RegisterWithFlag(FlagName, DefaultValue);
	return FlagListener;
}

ULdJsonFlagListener* ULaunchDarklyBPLibrary::CreateJsonVariationListener(FString FlagName, ULdNodeObject* DefaultValue)
{
	ULdJsonFlagListener* FlagListener = NewObject<ULdJsonFlagListener>();
	FlagListener->RegisterWithFlag(FlagName, DefaultValue);
	return FlagListener;
}

ULdStringFlagListener* ULaunchDarklyBPLibrary::CreateStringVariationListener(FString FlagName, FString DefaultValue)
{
	ULdStringFlagListener* FlagListener = NewObject<ULdStringFlagListener>();
	FlagListener->RegisterWithFlag(FlagName, DefaultValue);
	return FlagListener;
}

void ULaunchDarklyBPLibrary::TrackMetric(FString MetricName)
{
	return FLaunchDarklyClientModule::Get()->Track(MetricName);
}

void ULaunchDarklyBPLibrary::TrackMetricWithData(FString MetricName, ULdNodeObject* Data)
{
	return FLaunchDarklyClientModule::Get()->Track(MetricName, Data->GetObjectData());
}
