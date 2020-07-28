#include "LdConfigObject.h"

#if PLATFORM_WINDOWS
#include <launchdarkly/api.h>
#endif


#include "LaunchDarklyHelpers.h"

const FString ULdConfigObject::KeyAllAttributesPrivate = "all-attributes-private";
const FString ULdConfigObject::KeyBackgroundPollingIntervalMillis = "background-polling-interval-millis";
const FString ULdConfigObject::KeyBaseUri = "base-uri";
const FString ULdConfigObject::KeyConnectionTimeoutMillis = "connection-timeout-millis";
const FString ULdConfigObject::KeyDisableBackgroundUpdating = "disable-background-updating";
const FString ULdConfigObject::KeyEventsCapacity = "events-capacity";
const FString ULdConfigObject::KeyEventsFlushIntervalMillis = "events-flush-interval-millis";
const FString ULdConfigObject::KeyEventsUri = "events-uri";
const FString ULdConfigObject::KeyMobileKey = "mobile-key";
const FString ULdConfigObject::KeyOffline = "offline";
const FString ULdConfigObject::KeyPollingIntervalInMillis = "polling-interval-in-millis";
const FString ULdConfigObject::KeyPrivateAttributeNames = "private-attribute-name";
const FString ULdConfigObject::KeyStream = "stream";
const FString ULdConfigObject::KeyStreamUri = "stream-uri";
const FString ULdConfigObject::KeyUseReport = "use-report";

ULdConfigObject::ULdConfigObject()
{
	ConfigData = MakeShared<FJsonObject>();
}

ULdConfigObject* ULdConfigObject::CreateLdConfigObject(FString MobileKey)
{
	ULdConfigObject* configObject =  NewObject<ULdConfigObject>();
	configObject->SetMobileKey(MobileKey);

	return configObject;
}

#if PLATFORM_WINDOWS
LDConfig* const ULdConfigObject::ToLdConfig()
{
	FString MobileKey = ConfigData->GetStringField(KeyMobileKey);
	struct LDConfig*const LdConfig = LDConfigNew(TCHAR_TO_ANSI(*MobileKey));

	if(ConfigData->HasTypedField<EJson::Boolean>(KeyAllAttributesPrivate))
	{
		LDConfigSetAllAttributesPrivate(LdConfig, ConfigData->GetBoolField(KeyAllAttributesPrivate));
	}

	if(ConfigData->HasTypedField<EJson::Number>(KeyBackgroundPollingIntervalMillis))
	{
		LDConfigSetBackgroundPollingIntervalMillis(LdConfig, ConfigData->GetNumberField(KeyBackgroundPollingIntervalMillis));
	}

	if(ConfigData->HasTypedField<EJson::String>(KeyBaseUri))
	{
		FString BaseUri = ConfigData->GetStringField(KeyBaseUri);
		LDConfigSetAppURI(LdConfig, TCHAR_TO_ANSI(*BaseUri));
	}

	if(ConfigData->HasTypedField<EJson::Number>(KeyConnectionTimeoutMillis))
	{
		LDConfigSetConnectionTimeoutMillies(LdConfig, ConfigData->GetNumberField(KeyConnectionTimeoutMillis));
	}

	if(ConfigData->HasTypedField<EJson::Boolean>(KeyDisableBackgroundUpdating))
	{
		LDConfigSetDisableBackgroundUpdating(LdConfig, ConfigData->GetBoolField(KeyDisableBackgroundUpdating));
	}

	if(ConfigData->HasTypedField<EJson::Number>(KeyEventsCapacity))
	{
		LDConfigSetEventsCapacity(LdConfig, ConfigData->GetNumberField(KeyEventsCapacity));
	}

	if(ConfigData->HasTypedField<EJson::Number>(KeyEventsFlushIntervalMillis))
	{
		LDConfigSetEventsFlushIntervalMillis(LdConfig, ConfigData->GetNumberField(KeyEventsFlushIntervalMillis));
	}

	if(ConfigData->HasTypedField<EJson::String>(KeyEventsUri))
	{
		FString EventsUri = ConfigData->GetStringField(KeyEventsUri);
		LDConfigSetEventsURI(LdConfig, TCHAR_TO_ANSI(*EventsUri));
	}

	if(ConfigData->HasTypedField<EJson::Boolean>(KeyOffline))
	{
		LDConfigSetOffline(LdConfig, ConfigData->GetBoolField(KeyOffline));
	}

	if(ConfigData->HasTypedField<EJson::Number>(KeyPollingIntervalInMillis))
	{
		LDConfigSetPollingIntervalMillis(LdConfig, ConfigData->GetNumberField(KeyPollingIntervalInMillis));
	}

	if(ConfigData->HasTypedField<EJson::Array>(KeyPrivateAttributeNames))
	{
		TArray<TSharedPtr<FJsonValue>> PrivateAttributeArray = ConfigData->GetArrayField(KeyPrivateAttributeNames);
		struct LDJSON* PrivateAttributeText;
		struct LDJSON* PrivateAttributesJson = LDNewArray();
		for(TSharedPtr<FJsonValue> PrivateAttribute : PrivateAttributeArray)
		{
			FString PrivateAttributeString = PrivateAttribute->AsString();
			PrivateAttributeText = LDNewText(TCHAR_TO_ANSI(*PrivateAttributeString));
			LDArrayPush(PrivateAttributesJson, PrivateAttributeText);
		}
		LDConfigSetPrivateAttributes(LdConfig, PrivateAttributesJson);
		LDJSONFree(PrivateAttributesJson);
	}

	if(ConfigData->HasTypedField<EJson::Boolean>(KeyStream))
	{
		LDConfigSetStreaming(LdConfig, ConfigData->GetBoolField(KeyStream));
	}

	if(ConfigData->HasTypedField<EJson::String>(KeyStreamUri))
	{
		FString StreamUri = ConfigData->GetStringField(KeyStreamUri);
		LDConfigSetStreamURI(LdConfig, TCHAR_TO_ANSI(*StreamUri));
	}

	if(ConfigData->HasTypedField<EJson::Boolean>(KeyUseReport))
	{
		LDConfigSetUseReport(LdConfig, ConfigData->GetBoolField(KeyUseReport));
	}
	
	return LdConfig;
}
#endif

FString ULdConfigObject::ToJsonString()
{
	return ULaunchDarklyHelpers::JsonObjectToString(ConfigData);
}

ULdConfigObject* ULdConfigObject::SetAllAttributesPrivate()
{
	ConfigData->SetBoolField(KeyAllAttributesPrivate, true);
	return this;
}

ULdConfigObject* ULdConfigObject::SetBackgroundPollingIntervalMillis(int BackgroundPollingIntervalMillis)
{
	ConfigData->SetNumberField(KeyBackgroundPollingIntervalMillis, BackgroundPollingIntervalMillis);
	return this;
}

ULdConfigObject* ULdConfigObject::SetBaseUri(FString BaseUri)
{
	ConfigData->SetStringField(KeyBaseUri, BaseUri);
	return this;
}

ULdConfigObject* ULdConfigObject::SetConnectionTimeoutMillis(int ConnectionTimeoutMillis)
{
	ConfigData->SetNumberField(KeyConnectionTimeoutMillis, ConnectionTimeoutMillis);
	return this;
}

ULdConfigObject* ULdConfigObject::SetDisableBackgroundUpdating(bool DisableBackgroundUpdating)
{
	ConfigData->SetBoolField(KeyDisableBackgroundUpdating, DisableBackgroundUpdating);
	return this;
}

ULdConfigObject* ULdConfigObject::SetEventsCapacity(int EventsCapacity)
{
	ConfigData->SetNumberField(KeyEventsCapacity, EventsCapacity);
	return this;
}

ULdConfigObject* ULdConfigObject::SetEventsFlushIntervalMillis(int FlushIntervalMillis)
{
	ConfigData->SetNumberField(KeyEventsFlushIntervalMillis, FlushIntervalMillis);
	return this;
}

ULdConfigObject* ULdConfigObject::SetEventsUri(FString EventsUri)
{
	ConfigData->SetStringField(KeyEventsUri, EventsUri);
	return this;
}

ULdConfigObject* ULdConfigObject::SetMobileKey(FString MobileKey)
{
	ConfigData->SetStringField(KeyMobileKey, MobileKey);
	return this;
}

ULdConfigObject* ULdConfigObject::SetOffline(bool Offline)
{
	ConfigData->SetBoolField(KeyOffline, Offline);
	return this;
}

ULdConfigObject* ULdConfigObject::SetPollingIntervalInMillis(int PollingIntervalInMillis)
{
	ConfigData->SetNumberField(KeyPollingIntervalInMillis, PollingIntervalInMillis);
	return this;
}

ULdConfigObject* ULdConfigObject::AddPrivateAttributeName(FString PrivateAttributeName)
{
	if(PrivateAttributesArray.Contains(PrivateAttributeName) == false)
	{
		PrivateAttributesArray.Add(PrivateAttributeName);
		TArray<TSharedPtr<FJsonValue>> JsonValueArray;
		if(ConfigData->HasTypedField<EJson::Array>(KeyPrivateAttributeNames))
		{
			JsonValueArray = ConfigData->GetArrayField(KeyPrivateAttributeNames);
		}
		TSharedPtr<FJsonValue> JsonValueString = MakeShared<FJsonValueString>(PrivateAttributeName);
		JsonValueArray.Add(JsonValueString);
		ConfigData->SetArrayField(KeyPrivateAttributeNames, JsonValueArray);
	}
	return this;
}

ULdConfigObject* ULdConfigObject::SetStream(bool IsStreaming)
{
	ConfigData->SetBoolField(KeyStream,IsStreaming);
	return this;
}

ULdConfigObject* ULdConfigObject::SetStreamUri(FString StreamUri)
{
	ConfigData->SetStringField(KeyStreamUri, StreamUri);
	return this;
}

ULdConfigObject* ULdConfigObject::SetUseReport(bool UseReport)
{
	ConfigData->SetBoolField(KeyUseReport, UseReport);
	return this;
}
