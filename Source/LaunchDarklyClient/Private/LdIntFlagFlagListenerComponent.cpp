#include "LdIntFlagListenerComponent.h"

#include "LaunchDarklyClient.h"

void ULdIntFlagListenerComponent::BeginPlay()
{
	Super::BeginPlay();

	FLaunchDarklyClientModule::Get()->RegisterIntFlagListenerComponent(this, FlagName);

	if(CheckValueOnBeginPlay)
	{
		int FlagValue = FLaunchDarklyClientModule::Get()->GetIntVariation(FlagName, DefaultValue);
		ValueChanged(FlagValue);
	}
}

void ULdIntFlagListenerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	OnFlagChanged.Clear();
	FLaunchDarklyClientModule::Get()->UnregisterIntFlagListenerComponent(this, FlagName);
}

void ULdIntFlagListenerComponent::ValueChanged()
{
	int FlagValue = FLaunchDarklyClientModule::Get()->GetIntVariation(FlagName, DefaultValue);
	UE_LOG(LaunchDarklyClient, Warning, TEXT("ULdIntFlagListenerComponent::ValueChanged() called for flag '%s'."), *(this->FlagName));
	OnFlagChanged.Broadcast(FlagValue);
}

void ULdIntFlagListenerComponent::ValueChanged(int FlagValue)
{
	UE_LOG(LaunchDarklyClient, Warning, TEXT("ULdIntFlagListenerComponent::ValueChanged(...) called for flag '%s'."), *(this->FlagName));
	OnFlagChanged.Broadcast(FlagValue);
}
