#pragma once

#include "Components/ActorComponent.h"
#include "LdIntFlagListenerComponent.generated.h"

UCLASS( ClassGroup=(LaunchDarkly), Blueprintable, meta=(BlueprintSpawnableComponent) )
class LAUNCHDARKLYCLIENT_API ULdIntFlagListenerComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLaunchDarkly_OnIntFlagChangedSignature, int, FlagValue);
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "On Integer Numeric Feature Flag Changed"))
	FLaunchDarkly_OnIntFlagChangedSignature OnFlagChanged;

	void ValueChanged();

	void ValueChanged(int FlagValue);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "LaunchDarkly")
	FString FlagName;

	UPROPERTY(EditDefaultsOnly, Category = "LaunchDarkly")
	bool CheckValueOnBeginPlay = true;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "LaunchDarkly")
	int DefaultValue;
};
