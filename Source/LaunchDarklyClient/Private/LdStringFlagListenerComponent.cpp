#include "LdStringFlagListenerComponent.h"

#include "LaunchDarklyClient.h"

void ULdStringFlagListenerComponent::BeginPlay()
{
	Super::BeginPlay();

	FLaunchDarklyClientModule::Get()->RegisterStringFlagListenerComponent(this, FlagName);

	if(CheckValueOnBeginPlay)
	{
		FString FlagValue = FLaunchDarklyClientModule::Get()->GetStringVariation(FlagName, DefaultValue);
		ValueChanged(FlagValue);
	}
}

void ULdStringFlagListenerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	OnFlagChanged.Clear();
	FLaunchDarklyClientModule::Get()->UnregisterStringFlagListenerComponent(this, FlagName);
}

void ULdStringFlagListenerComponent::ValueChanged()
{
	FString FlagValue = FLaunchDarklyClientModule::Get()->GetStringVariation(FlagName, DefaultValue);
	UE_LOG(LaunchDarklyClient, Warning, TEXT("ULdStringFlagListenerComponent::ValueChanged() called for flag '%s'."), *(this->FlagName));
	OnFlagChanged.Broadcast(FlagValue);
}

void ULdStringFlagListenerComponent::ValueChanged(FString FlagValue)
{
	UE_LOG(LaunchDarklyClient, Warning, TEXT("ULdStringFlagListenerComponent::ValueChanged(...) called for flag '%s'."), *(this->FlagName));
	OnFlagChanged.Broadcast(FlagValue);
}
