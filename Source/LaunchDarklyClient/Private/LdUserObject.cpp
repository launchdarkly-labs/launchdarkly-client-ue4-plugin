#include "LdUserObject.h"

#include "LdNodeObject.h"
#include "LaunchDarklyHelpers.h"

#if PLATFORM_WINDOWS
#include <launchdarkly/api.h>
#endif

const FString ULdUserObject::KeyAnonymousAttribute = "anonymous";
const FString ULdUserObject::KeyAvatarAttribute = "avatar";
const FString ULdUserObject::KeyCustomAttributes = "custom";
const FString ULdUserObject::KeyEmailAttribute = "email";
const FString ULdUserObject::KeyFirstNameAttribute = "first-name";
const FString ULdUserObject::KeyIpAttribute = "ip";
const FString ULdUserObject::KeyLastNameAttribute = "last-name";
const FString ULdUserObject::KeyNameAttribute = "name";
const FString ULdUserObject::KeyPrivateAttributeNames = "private-attribute-names";
const FString ULdUserObject::KeySecondaryKeyAttribute = "secondary-key";
const FString ULdUserObject::KeyUserKeyAttribute = "user-key";

ULdUserObject::ULdUserObject()
{
	UserAttributeData = MakeShared<FJsonObject>();
}

ULdUserObject* ULdUserObject::CreateLdUserObject(FString UserKey)
{
	ULdUserObject* UserObject = NewObject<ULdUserObject>();
	UserObject->SetUserKeyAttribute(UserKey);

	return UserObject;
}

#if PLATFORM_WINDOWS
LDUser* const ULdUserObject::ToLdUser()
{
	FString UserKey = "UE4_DefaultUserKey";
	if(UserAttributeData->HasField(KeyUserKeyAttribute))
	{
		UserKey = UserAttributeData->GetStringField(KeyUserKeyAttribute);
	}
	LDUser* const LdUser = LDUserNew(TCHAR_TO_ANSI(*UserKey));

	if(UserAttributeData->HasField(KeyAnonymousAttribute))
	{
		LDUserSetAnonymous(LdUser, UserAttributeData->GetBoolField(KeyAnonymousAttribute));
	}
	if(UserAttributeData->HasField(KeyAvatarAttribute))
	{
		FString Avatar = UserAttributeData->GetStringField(KeyAvatarAttribute);
		LDUserSetAvatar(LdUser, TCHAR_TO_ANSI(*Avatar));
	}
	if(UserAttributeData->HasField(KeyEmailAttribute))
	{
		FString Email = UserAttributeData->GetStringField(KeyEmailAttribute);
		LDUserSetEmail(LdUser, TCHAR_TO_ANSI(*Email));
	}
	if(UserAttributeData->HasField(KeyFirstNameAttribute))
	{
		FString FirstName = UserAttributeData->GetStringField(KeyFirstNameAttribute);
		LDUserSetFirstName(LdUser, TCHAR_TO_ANSI(*FirstName));
	}
	if(UserAttributeData->HasField(KeyIpAttribute))
	{
		FString IpAddy = UserAttributeData->GetStringField(KeyIpAttribute);
		LDUserSetIP(LdUser, TCHAR_TO_ANSI(*IpAddy));
	}
	if(UserAttributeData->HasField(KeyLastNameAttribute))
	{
		FString LastName = UserAttributeData->GetStringField(KeyLastNameAttribute);
		LDUserSetLastName(LdUser, TCHAR_TO_ANSI(*LastName));
	}
	if(UserAttributeData->HasField(KeyNameAttribute))
	{
		FString FullName = UserAttributeData->GetStringField(KeyNameAttribute);
		LDUserSetName(LdUser, TCHAR_TO_ANSI(*FullName));
	}
	if(UserAttributeData->HasField(KeySecondaryKeyAttribute))
	{
		FString SecondaryKey = UserAttributeData->GetStringField(KeySecondaryKeyAttribute);
		LDUserSetSecondary(LdUser, TCHAR_TO_ANSI(*SecondaryKey));
	}
	if(UserAttributeData->HasField(KeyCustomAttributes))
	{
		TSharedPtr<FJsonObject> JsonObj = UserAttributeData->GetObjectField(KeyCustomAttributes);

		FString JsonStr = ULaunchDarklyHelpers::JsonObjectToString(JsonObj);
		LDJSON* LdNodeAttribute = LDJSONDeserialize(TCHAR_TO_ANSI(*JsonStr));
		LDUserSetCustomAttributesJSON(LdUser, LdNodeAttribute);
	}

	struct LDJSON* PrivateAttributeText;
	struct LDJSON* PrivateAttributesJson = LDNewArray();
	for(FString PrivateAttributeName : PrivateAttributeNames)
	{
		PrivateAttributeText = LDNewText(TCHAR_TO_ANSI(*PrivateAttributeName));
		LDArrayPush(PrivateAttributesJson, PrivateAttributeText);
	}
	LDUserSetPrivateAttributes(LdUser, PrivateAttributesJson);

	return LdUser;
}
#endif

FString ULdUserObject::ToJsonString()
{
	return ULaunchDarklyHelpers::JsonObjectToString(UserAttributeData);
}

void ULdUserObject::AddPrivateAttribute(FString AttributeName)
{
	if(PrivateAttributeNames.Contains(AttributeName) == false)
	{
		PrivateAttributeNames.Add(AttributeName);
		TArray<TSharedPtr<FJsonValue>> PrivateAttributesJson;
		if(UserAttributeData->HasField(KeyPrivateAttributeNames))
		{
			PrivateAttributesJson = UserAttributeData->GetArrayField(KeyPrivateAttributeNames);
		}
		PrivateAttributesJson.Add(MakeShared<FJsonValueString>(AttributeName));
		UserAttributeData->SetArrayField(KeyPrivateAttributeNames, PrivateAttributesJson);
	}
}

void ULdUserObject::RemovePrivateAttribute(FString RemovedAttributeName)
{
	if(PrivateAttributeNames.Contains(RemovedAttributeName))
	{
		PrivateAttributeNames.Remove(RemovedAttributeName);
		TArray<TSharedPtr<FJsonValue>> PrivateAttributesJson;
		for(FString AttributeName : PrivateAttributeNames)
		{
			PrivateAttributesJson.Add(MakeShared<FJsonValueString>(AttributeName));
		}
		UserAttributeData->SetArrayField(KeyPrivateAttributeNames, PrivateAttributesJson);
	}
}

ULdUserObject* ULdUserObject::SetCustomBoolAttribute(FString KeyName, bool KeyValue, bool IsPrivate)
{
	TSharedPtr<FJsonObject> CustomAttributes = GetOrInitCustomAttributesObject();
	CustomAttributes->SetBoolField(KeyName, KeyValue);
	UserAttributeData->SetObjectField(KeyCustomAttributes, CustomAttributes);

	if(IsPrivate)
	{
		AddPrivateAttribute(KeyName);
	}
	else
	{
		RemovePrivateAttribute(KeyName);
	}

	return this;
}

ULdUserObject* ULdUserObject::SetCustomFloatAttribute(FString KeyName, float KeyValue, bool IsPrivate)
{
	TSharedPtr<FJsonObject> CustomAttributes = GetOrInitCustomAttributesObject();
	CustomAttributes->SetNumberField(KeyName, KeyValue);
	UserAttributeData->SetObjectField(KeyCustomAttributes, CustomAttributes);

	if(IsPrivate)
	{
		AddPrivateAttribute(KeyName);
	}
	else
	{
		RemovePrivateAttribute(KeyName);
	}

	return this;
}

ULdUserObject* ULdUserObject::SetCustomIntAttribute(FString KeyName, int KeyValue, bool IsPrivate)
{
	TSharedPtr<FJsonObject> CustomAttributes = GetOrInitCustomAttributesObject();
	if(UserAttributeData->HasField(KeyCustomAttributes))
	{
		CustomAttributes = UserAttributeData->GetObjectField(KeyCustomAttributes);
	}
	CustomAttributes->SetNumberField(KeyName, KeyValue);
	UserAttributeData->SetObjectField(KeyCustomAttributes, CustomAttributes);

	if(IsPrivate)
	{
		AddPrivateAttribute(KeyName);
	}
	else
	{
		RemovePrivateAttribute(KeyName);
	}

	return this;
}


ULdUserObject* ULdUserObject::SetCustomStringAttribute(FString KeyName, FString KeyValue, bool IsPrivate)
{
	TSharedPtr<FJsonObject> CustomAttributes = GetOrInitCustomAttributesObject();
	CustomAttributes->SetStringField(KeyName, KeyValue);
	UserAttributeData->SetObjectField(KeyCustomAttributes, CustomAttributes);

	if(IsPrivate)
	{
		AddPrivateAttribute(KeyName);
	}
	else
	{
		RemovePrivateAttribute(KeyName);
	}

	return this;
}

ULdUserObject* ULdUserObject::SetCustomFloatArrayAttribute(FString KeyName, TArray<float> KeyValues, bool IsPrivate)
{
	TSharedPtr<FJsonObject> CustomAttributes = GetOrInitCustomAttributesObject();
	TArray<TSharedPtr<FJsonValue>> JsonValues;
	for(float KeyValue : KeyValues)
	{
		JsonValues.Add(MakeShared<FJsonValueNumber>(KeyValue));
	}
	CustomAttributes->SetArrayField(KeyName, JsonValues);
	UserAttributeData->SetObjectField(KeyCustomAttributes, CustomAttributes);

	if(IsPrivate)
	{
		AddPrivateAttribute(KeyName);
	}
	else
	{
		RemovePrivateAttribute(KeyName);
	}

	return this;
}

ULdUserObject* ULdUserObject::SetCustomIntArrayAttribute(FString KeyName, TArray<int> KeyValues, bool IsPrivate)
{
	TSharedPtr<FJsonObject> CustomAttributes = GetOrInitCustomAttributesObject();
	TArray<TSharedPtr<FJsonValue>> JsonValues;
	for(int KeyValue : KeyValues)
	{
		JsonValues.Add(MakeShared<FJsonValueNumber>(KeyValue));
	}
	CustomAttributes->SetArrayField(KeyName, JsonValues);
	UserAttributeData->SetObjectField(KeyCustomAttributes, CustomAttributes);

	if(IsPrivate)
	{
		AddPrivateAttribute(KeyName);
	}
	else
	{
		RemovePrivateAttribute(KeyName);
	}

	return this;
}

ULdUserObject* ULdUserObject::SetCustomStringArrayAttribute(FString KeyName, TArray<FString> KeyValues, bool IsPrivate)
{
	TSharedPtr<FJsonObject> CustomAttributes = GetOrInitCustomAttributesObject();
	TArray<TSharedPtr<FJsonValue>> JsonValues;
	for(FString KeyValue : KeyValues)
	{
		JsonValues.Add(MakeShared<FJsonValueString>(KeyValue));
	}
	CustomAttributes->SetArrayField(KeyName, JsonValues);
	UserAttributeData->SetObjectField(KeyCustomAttributes, CustomAttributes);

	if(IsPrivate)
	{
		AddPrivateAttribute(KeyName);
	}
	else
	{
		RemovePrivateAttribute(KeyName);
	}

	return this;
}

ULdUserObject* ULdUserObject::SetAnonymousAttribute(bool IsAnonymous)
{
	UserAttributeData->SetBoolField(KeyAnonymousAttribute, IsAnonymous);
	return this;
}

ULdUserObject* ULdUserObject::SetAvatarAttribute(FString Avatar, bool IsPrivate)
{
	UserAttributeData->SetStringField(KeyAvatarAttribute, Avatar);

	if(IsPrivate)
	{
		AddPrivateAttribute(KeyAvatarAttribute);
	}
	else
	{
		RemovePrivateAttribute(KeyAvatarAttribute);
	}

	return this;
}

ULdUserObject* ULdUserObject::SetEmailAttribute(FString Email, bool IsPrivate)
{
	UserAttributeData->SetStringField(KeyEmailAttribute, Email);

	if(IsPrivate)
	{
		AddPrivateAttribute(KeyEmailAttribute);
	}
	else
	{
		RemovePrivateAttribute(KeyEmailAttribute);
	}

	return this;
}

ULdUserObject* ULdUserObject::SetFirstNameAttribute(FString FirstName, bool IsPrivate)
{
	UserAttributeData->SetStringField(KeyFirstNameAttribute, FirstName);

	if(IsPrivate)
	{
		AddPrivateAttribute(KeyFirstNameAttribute);
	}
	else
	{
		RemovePrivateAttribute(KeyFirstNameAttribute);
	}

	return this;
}

ULdUserObject* ULdUserObject::SetIpAttribute(FString Ip, bool IsPrivate)
{
	UserAttributeData->SetStringField(KeyIpAttribute, Ip);

	if(IsPrivate)
	{
		AddPrivateAttribute(KeyIpAttribute);
	}
	else
	{
		RemovePrivateAttribute(KeyIpAttribute);
	}

	return this;
}

ULdUserObject* ULdUserObject::SetLastNameAttribute(FString LastName, bool IsPrivate)
{
	UserAttributeData->SetStringField(KeyLastNameAttribute, LastName);

	if(IsPrivate)
	{
		AddPrivateAttribute(KeyLastNameAttribute);
	}
	else
	{
		RemovePrivateAttribute(KeyLastNameAttribute);
	}

	return this;
}

ULdUserObject* ULdUserObject::SetNameAttribute(FString Name, bool IsPrivate)
{
	UserAttributeData->SetStringField(KeyNameAttribute, Name);

	if(IsPrivate)
	{
		AddPrivateAttribute(KeyNameAttribute);
	}
	else
	{
		RemovePrivateAttribute(KeyNameAttribute);
	}

	return this;
}

ULdUserObject* ULdUserObject::SetSecondaryKeyAttribute(FString SecondaryKey, bool IsPrivate)
{
	UserAttributeData->SetStringField(KeySecondaryKeyAttribute, SecondaryKey);

	if(IsPrivate)
	{
		AddPrivateAttribute(KeySecondaryKeyAttribute);
	}
	else
	{
		RemovePrivateAttribute(KeySecondaryKeyAttribute);
	}

	return this;
}

ULdUserObject* ULdUserObject::SetUserKeyAttribute(FString UserKey)
{
	UserAttributeData->SetStringField(KeyUserKeyAttribute, UserKey);
	return this;
}

TSharedPtr<FJsonObject> ULdUserObject::GetOrInitCustomAttributesObject()
{
	if(UserAttributeData->HasField(KeyCustomAttributes))
	{
		return UserAttributeData->GetObjectField(KeyCustomAttributes);
	}
	
	return MakeShared<FJsonObject>();
}