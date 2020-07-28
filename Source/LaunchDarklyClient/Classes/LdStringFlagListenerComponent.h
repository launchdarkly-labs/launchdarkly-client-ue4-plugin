#pragma once

#include "Components/ActorComponent.h"
#include "LdStringFlagListenerComponent.generated.h"

UCLASS( ClassGroup=(LaunchDarkly), Blueprintable, meta=(BlueprintSpawnableComponent) )
class LAUNCHDARKLYCLIENT_API ULdStringFlagListenerComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLaunchDarkly_OnStringFlagChangedSignature, FString, FlagValue);
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "On String Feature Flag Changed"))
	FLaunchDarkly_OnStringFlagChangedSignature OnFlagChanged;

	void ValueChanged();

	void ValueChanged(FString FlagValue);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "LaunchDarkly")
	FString FlagName;

	UPROPERTY(EditDefaultsOnly, Category = "LaunchDarkly")
	bool CheckValueOnBeginPlay = true;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "LaunchDarkly")
	FString DefaultValue;
};
