#pragma once

#include "Components/ActorComponent.h"
#include "LdBoolFlagListenerComponent.generated.h"

UCLASS( ClassGroup=(LaunchDarkly), Blueprintable, meta=(BlueprintSpawnableComponent) )
class LAUNCHDARKLYCLIENT_API ULdBoolFlagListenerComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLaunchDarkly_OnBoolFlagChangedSignature, bool, FlagValue);
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "On Boolean Feature Flag Changed"))
	FLaunchDarkly_OnBoolFlagChangedSignature OnFlagChanged;

	void ValueChanged();

	void ValueChanged(bool FlagValue);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "LaunchDarkly")
	FString FlagName;

	UPROPERTY(EditDefaultsOnly, Category = "LaunchDarkly")
	bool CheckValueOnBeginPlay = true;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "LaunchDarkly")
	bool DefaultValue;
};
