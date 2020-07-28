#pragma once

#include "Components/ActorComponent.h"
#include "LdJsonFlagListenerComponent.generated.h"

class ULdNodeObject;

UCLASS( ClassGroup=(LaunchDarkly), Blueprintable, meta=(BlueprintSpawnableComponent) )
class LAUNCHDARKLYCLIENT_API ULdJsonFlagListenerComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLaunchDarkly_OnJsonFlagChangedSignature, ULdNodeObject*, FlagValue);
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "On JSON Feature Flag Changed"))
	FLaunchDarkly_OnJsonFlagChangedSignature OnFlagChanged;

	void ValueChanged();

	void ValueChanged(ULdNodeObject* FlagValue);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "LaunchDarkly")
	FString FlagName;

	UPROPERTY(EditDefaultsOnly, Category = "LaunchDarkly")
	bool CheckValueOnBeginPlay = true;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "LaunchDarkly")
	ULdNodeObject* DefaultValue;
};
