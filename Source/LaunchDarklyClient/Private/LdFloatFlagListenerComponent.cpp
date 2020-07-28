#include "LdFloatFlagListenerComponent.h"

#include "LaunchDarklyClient.h"

void ULdFloatFlagListenerComponent::BeginPlay()
{
	Super::BeginPlay();

	FLaunchDarklyClientModule::Get()->RegisterFloatFlagListenerComponent(this, FlagName);

	if(CheckValueOnBeginPlay)
	{
		float FlagValue = FLaunchDarklyClientModule::Get()->GetFloatVariation(FlagName, DefaultValue);
		ValueChanged(FlagValue);
	}
}

void ULdFloatFlagListenerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	OnFlagChanged.Clear();
	FLaunchDarklyClientModule::Get()->UnregisterFloatFlagListenerComponent(this, FlagName);
}

void ULdFloatFlagListenerComponent::ValueChanged()
{
	float FlagValue = FLaunchDarklyClientModule::Get()->GetFloatVariation(FlagName, DefaultValue);
	UE_LOG(LaunchDarklyClient, Warning, TEXT("ULdFloatFlagListenerComponent::ValueChanged() called for flag '%s'."), *(this->FlagName));
	OnFlagChanged.Broadcast(FlagValue);
}

void ULdFloatFlagListenerComponent::ValueChanged(float FlagValue)
{
	UE_LOG(LaunchDarklyClient, Warning, TEXT("ULdFloatFlagListenerComponent::ValueChanged(...) called for flag '%s'."), *(this->FlagName));
	OnFlagChanged.Broadcast(FlagValue);
}
