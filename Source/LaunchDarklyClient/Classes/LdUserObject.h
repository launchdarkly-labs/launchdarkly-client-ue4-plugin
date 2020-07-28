#pragma once

#include "Dom/JsonObject.h"

#include "LdUserObject.generated.h"

#if PLATFORM_WINDOWS
struct LDUser;
#endif

class ULdNodeObject;

UCLASS(BlueprintType)
class LAUNCHDARKLYCLIENT_API ULdUserObject : public UObject
{
	GENERATED_BODY()
public:
	ULdUserObject();

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly User Attributes")
	FString ToJsonString();

#if PLATFORM_WINDOWS
	LDUser* const ToLdUser();
#endif

protected:
	static const FString KeyAnonymousAttribute;
	static const FString KeyAvatarAttribute;
	static const FString KeyCustomAttributes;
	static const FString KeyEmailAttribute;
	static const FString KeyFirstNameAttribute;
	static const FString KeyIpAttribute;
	static const FString KeyLastNameAttribute;
	static const FString KeyNameAttribute;
	static const FString KeyPrivateAttributeNames;
	static const FString KeySecondaryKeyAttribute;
	static const FString KeyUserKeyAttribute;

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly User Attributes")
		static ULdUserObject* CreateLdUserObject(FString UserKey);

	TSharedPtr<FJsonObject> UserAttributeData;
	TArray<FString> PrivateAttributeNames;

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly User Attributes")
		ULdUserObject* SetCustomBoolAttribute(FString KeyName, bool KeyValue, bool isPrivate);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly User Attributes")
		ULdUserObject* SetCustomFloatAttribute(FString KeyName, float KeyValue, bool isPrivate);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly User Attributes")
		ULdUserObject* SetCustomIntAttribute(FString KeyName, int KeyValue, bool isPrivate);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly User Attributes")
		ULdUserObject* SetCustomStringAttribute(FString KeyName, FString KeyValue, bool isPrivate);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly User Attributes")
		ULdUserObject* SetCustomFloatArrayAttribute(FString KeyName, TArray<float> KeyValues, bool isPrivate);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly User Attributes")
		ULdUserObject* SetCustomIntArrayAttribute(FString KeyName, TArray<int> KeyValues, bool isPrivate);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly User Attributes")
		ULdUserObject* SetCustomStringArrayAttribute(FString KeyName, TArray<FString> KeyValues, bool isPrivate);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly User Attributes")
		ULdUserObject* SetAnonymousAttribute(bool IsAnonymous);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly User Attributes")
		ULdUserObject* SetAvatarAttribute(FString Avatar, bool isPrivate);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly User Attributes")
		ULdUserObject* SetEmailAttribute(FString Email, bool isPrivate);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly User Attributes")
		ULdUserObject* SetFirstNameAttribute(FString FirstName, bool isPrivate);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly User Attributes")
		ULdUserObject* SetIpAttribute(FString Ip, bool isPrivate);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly User Attributes")
		ULdUserObject* SetLastNameAttribute(FString LastName, bool isPrivate);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly User Attributes")
		ULdUserObject* SetNameAttribute(FString Name, bool isPrivate);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly User Attributes")
		ULdUserObject* SetSecondaryKeyAttribute(FString SecondaryKey, bool isPrivate);

	UFUNCTION(BlueprintCallable, Category = "LaunchDarkly User Attributes")
		ULdUserObject* SetUserKeyAttribute(FString UserKey);

	void AddPrivateAttribute(FString AttributeName);
	void RemovePrivateAttribute(FString AttributeName);
	TSharedPtr<FJsonObject> GetOrInitCustomAttributesObject();
};
