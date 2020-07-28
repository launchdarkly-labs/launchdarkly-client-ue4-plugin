#include "LdBoolFlagListenerComponent.h"

#include "LaunchDarklyClient.h"

void ULdBoolFlagListenerComponent::BeginPlay()
{
	Super::BeginPlay();

	FLaunchDarklyClientModule::Get()->RegisterBoolFlagListenerComponent(this, FlagName);

	if(CheckValueOnBeginPlay)
	{
		bool FlagValue = FLaunchDarklyClientModule::Get()->GetBoolVariation(FlagName, DefaultValue);
		ValueChanged(FlagValue);
	}
}

void ULdBoolFlagListenerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	OnFlagChanged.Clear();
	FLaunchDarklyClientModule::Get()->UnregisterBoolFlagListenerComponent(this, FlagName);
}

void ULdBoolFlagListenerComponent::ValueChanged()
{
	bool FlagValue = FLaunchDarklyClientModule::Get()->GetBoolVariation(FlagName, DefaultValue);
	UE_LOG(LaunchDarklyClient, Warning, TEXT("ULdBoolFlagListenerComponent::ValueChanged() called for flag '%s'."), *(this->FlagName));
	OnFlagChanged.Broadcast(FlagValue);
}

void ULdBoolFlagListenerComponent::ValueChanged(bool FlagValue)
{
	UE_LOG(LaunchDarklyClient, Warning, TEXT("ULdBoolFlagListenerComponent::ValueChanged(...) called for flag '%s'."), *(this->FlagName));
	OnFlagChanged.Broadcast(FlagValue);
}
