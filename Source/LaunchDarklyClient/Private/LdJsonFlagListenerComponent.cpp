#include "LdJsonFlagListenerComponent.h"

#include "LaunchDarklyClient.h"
#include "LdNodeObject.h"

void ULdJsonFlagListenerComponent::BeginPlay()
{
	Super::BeginPlay();

	FLaunchDarklyClientModule::Get()->RegisterJsonFlagListenerComponent(this, FlagName);

	if(CheckValueOnBeginPlay)
	{
		TSharedPtr<FJsonObject> FlagValue = FLaunchDarklyClientModule::Get()->GetJsonVariation(FlagName, nullptr);
		ULdNodeObject* FlagValueObject = NewObject<ULdNodeObject>();
		FlagValueObject->Initialize(FlagValue);
		ValueChanged(FlagValueObject);
	}
}

void ULdJsonFlagListenerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	OnFlagChanged.Clear();
	FLaunchDarklyClientModule::Get()->UnregisterJsonFlagListenerComponent(this, FlagName);
}

void ULdJsonFlagListenerComponent::ValueChanged()
{
	TSharedPtr<FJsonObject> JsonDefault = DefaultValue != NULL ? DefaultValue->GetObjectData() : NULL;
	TSharedPtr<FJsonObject> JsonData = FLaunchDarklyClientModule::Get()->GetJsonVariation(FlagName, JsonDefault);

	ULdNodeObject* FlagValue = NewObject<ULdNodeObject>();
	FlagValue->Initialize(JsonData);
	UE_LOG(LaunchDarklyClient, Warning, TEXT("ULdJsonFlagListenerComponent::ValueChanged() called for flag '%s'."), *(this->FlagName));
	OnFlagChanged.Broadcast(FlagValue);
}

void ULdJsonFlagListenerComponent::ValueChanged(ULdNodeObject* FlagValue)
{
	UE_LOG(LaunchDarklyClient, Warning, TEXT("ULdJsonFlagListenerComponent::ValueChanged(...) called for flag '%s'."), *(this->FlagName));
	OnFlagChanged.Broadcast(FlagValue);
}
