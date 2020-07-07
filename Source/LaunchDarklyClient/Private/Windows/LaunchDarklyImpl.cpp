#include "LaunchDarklyImpl.h"

#include "Async/Async.h"
#include "Dom/JsonObject.h"
#include "Engine/Engine.h"

#include "LaunchDarklyClient.h"
#include "LaunchDarklySettings.h"
#include "LaunchDarklyHelpers.h"
#include "LdBoolFlagListener.h"
#include "LdConfigObject.h"
#include "LdFloatFlagListener.h"
#include "LdIntFlagListener.h"
#include "LdJsonFlagListener.h"
#include "LdNodeObject.h"
#include "LdStringFlagListener.h"
#include "LdUserObject.h"

LDClient_i* FLaunchDarklyImpl::LdClient = NULL;

void FLaunchDarklyImpl::BoolFlagListener(const char* const FlagName, const int Status)
{
	TMap<FString, TArray<ULdBoolFlagListener*>>& BoolFlagListeners = FLaunchDarklyClientModule::Get()->GetBoolFlagListeners();
	if(BoolFlagListeners.Contains(FlagName))
	{
		TArray<ULdBoolFlagListener*>& ListenerArray = BoolFlagListeners[FlagName];
		AsyncTask(ENamedThreads::GameThread, [&ListenerArray]()
			{
				for(ULdBoolFlagListener* Listener : ListenerArray)
				{
					Listener->ValueChanged();
				}
			}
		);
	}
}

void FLaunchDarklyImpl::FloatFlagListener(const char* const FlagName, const int Status)
{
	TMap<FString, TArray<ULdFloatFlagListener*>>& FloatFlagListeners = FLaunchDarklyClientModule::Get()->GetFloatFlagListeners();
	if(FloatFlagListeners.Contains(FlagName))
	{
		TArray<ULdFloatFlagListener*>& ListenerArray = FloatFlagListeners[FlagName];
		AsyncTask(ENamedThreads::GameThread, [&ListenerArray]()
			{
				for(ULdFloatFlagListener* Listener : ListenerArray)
				{
					Listener->ValueChanged();
				}
			}
		);
	}
}

void FLaunchDarklyImpl::IntFlagListener(const char* const FlagName, const int Status)
{
	TMap<FString, TArray<ULdIntFlagListener*>>& IntFlagListeners = FLaunchDarklyClientModule::Get()->GetIntFlagListeners();
	if(IntFlagListeners.Contains(FlagName))
	{
		TArray<ULdIntFlagListener*>& ListenerArray = IntFlagListeners[FlagName];
		AsyncTask(ENamedThreads::GameThread, [&ListenerArray]()
			{
				for(ULdIntFlagListener* Listener : ListenerArray)
				{
					Listener->ValueChanged();
				}
			}
		);
	}
}

void FLaunchDarklyImpl::JsonFlagListener(const char* const FlagName, const int Status)
{
	TMap<FString, TArray<ULdJsonFlagListener*>>& JsonFlagListeners = FLaunchDarklyClientModule::Get()->GetJsonFlagListeners();
	if(JsonFlagListeners.Contains(FlagName))
	{
		TArray<ULdJsonFlagListener*>& ListenerArray = JsonFlagListeners[FlagName];
		AsyncTask(ENamedThreads::GameThread, [&ListenerArray]()
			{
				for(ULdJsonFlagListener* Listener : ListenerArray)
				{
					Listener->ValueChanged();
				}
			}
		);
	}
}

void FLaunchDarklyImpl::StringFlagListener(const char* const FlagName, const int Status)
{
	TMap<FString, TArray<ULdStringFlagListener*>>& StringFlagListeners = FLaunchDarklyClientModule::Get()->GetStringFlagListeners();
	if(StringFlagListeners.Contains(FlagName))
	{
		TArray<ULdStringFlagListener*>& ListenerArray = StringFlagListeners[FlagName];
		AsyncTask(ENamedThreads::GameThread, [&ListenerArray]()
			{
				for(ULdStringFlagListener* Listener : ListenerArray)
				{
					Listener->ValueChanged();
				}
			}
		);
	}
}

void FLaunchDarklyImpl::ClientStatusCallback(int status)
{
	switch(status)
	{
		case 0: // LDStatusInitializing
			break;
		case 1: // LDStatusInitialized
			AsyncTask(ENamedThreads::GameThread, []()
				{
					RestoreFlagListeners();
					UpdateAllFlagListeners();
				}
			);
			break;
		case 2: // LDStatusFailed
			FLaunchDarklyImpl::LdLogger("Client Failed.");
			break;
		case 3: // LDStatusShuttingdown
			break;
		case 4: // LDStatusShutdown
			break;
		default:
			break;
	}
}

void FLaunchDarklyImpl::LdLogger(const char* s)
{
	FString LogMessage = FString(s);
	FString DebugMessage = FString::Printf(TEXT(">>> FLaunchDarklyImpl (Windows) Logger - %s."), *LogMessage);
	UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (Windows) Logger - %s."), *LogMessage);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, DebugMessage);
}

bool FLaunchDarklyImpl::InitializeClient(ULdConfigObject* LdConfigObject, ULdUserObject* LdUserObject, int ConnectionTimeoutMillis)
{
	const ULaunchDarklySettings* LaunchDarklySettings = GetDefault<ULaunchDarklySettings>();
	if(LaunchDarklySettings && LaunchDarklySettings->IsLoggingEnabled)
	{
		LDSetLogFunction(LD_LOG_TRACE, FLaunchDarklyImpl::LdLogger);
	}

	if(LdClient)
	{
		UE_LOG(LaunchDarklyClient, Warning, TEXT("Initialization called twice on LaunchDarkly Plugin; Intialization aborted. Perhaps Shutdown not called?"));
		return false;
	}

	ResetListeners();
	LDSetClientStatusCallback(FLaunchDarklyImpl::ClientStatusCallback);

	LDConfig* const LdConfig = LdConfigObject->ToLdConfig();
	LDUser* const LdUser = LdUserObject->ToLdUser();
	LdClient = LDClientInit(LdConfig, LdUser, ConnectionTimeoutMillis);

	bool IsInitSuccessful = LDClientIsInitialized(LdClient);
	if(IsInitSuccessful)
	{
		UE_LOG(LaunchDarklyClient, Log, TEXT("Client initialization was successful."));
	}
	else
	{
		UE_LOG(LaunchDarklyClient, Error, TEXT("Client initialization has failed."));
	}

	return IsInitSuccessful;
}

void FLaunchDarklyImpl::ShutdownClient()
{
	UE_LOG(LaunchDarklyClient, Log, TEXT("Shutting down LaunchDarkly."));

	if(IsInitialized())
	{
		ResetListeners();
		LDClientClose(LdClient);
	}
	else
	{
		UE_LOG(LaunchDarklyClient, Warning, TEXT("Shutdown called on an uninitialized client."));
	}

	LdClient = nullptr;
}

bool FLaunchDarklyImpl::IsInitialized()
{
	return LdClient && LDClientIsInitialized(LdClient);
}

bool FLaunchDarklyImpl::IsOffline()
{
	if(!IsInitialized())
	{
		UE_LOG(LaunchDarklyClient, Warning, TEXT("FLaunchDarklyImpl::IsOffline() called while uninitialized; returning false."));
		return false;
	}

	return LDClientIsOffline(LdClient);
}

void FLaunchDarklyImpl::SetOffline()
{
	if(!IsInitialized())
	{
		UE_LOG(LaunchDarklyClient, Warning, TEXT("FLaunchDarklyImpl::SetOffline() called while uninitialized."));
		return;
	}

	LDClientSetOffline(LdClient);
}

void FLaunchDarklyImpl::SetOnline()
{
	if(!IsInitialized())
	{
		UE_LOG(LaunchDarklyClient, Warning, TEXT("FLaunchDarklyImpl::SetOnline() called while uninitialized."));
		return;
	}

	LDClientSetOnline(LdClient);
}

void FLaunchDarklyImpl::RefreshUserAttributes(ULdUserObject* LdUserObject)
{
	LDUser* LdUser = LdUserObject->ToLdUser();
	LDClientIdentify(LdClient, LdUser);
}

bool FLaunchDarklyImpl::GetBoolVariation(FString FlagName, bool DefaultValue)
{
	if(!IsInitialized())
	{
		UE_LOG(LaunchDarklyClient, Warning, TEXT("FLaunchDarklyImpl::GetBoolVariation(%s) called while uninitialized; returning default."), *FlagName);
		return DefaultValue;
	}

	bool FlagValue = LDBoolVariation(LdClient, TCHAR_TO_ANSI(*FlagName), false);

	UE_LOG(LaunchDarklyClient, Log, TEXT("Flag '%s' queried (with default %s) returned %s."),
		*FlagName,
		DefaultValue ? TEXT("True") : TEXT("False"),
		FlagValue ? TEXT("True") : TEXT("False"));

	return FlagValue;
}

float FLaunchDarklyImpl::GetFloatVariation(FString FlagName, float DefaultValue)
{
	if(!IsInitialized())
	{
		UE_LOG(LaunchDarklyClient, Warning, TEXT("FLaunchDarklyImpl::GetBoolVariation(%s) called while uninitialized; returning default."), *FlagName);
		return DefaultValue;
	}

	float FlagValue = (float)LDDoubleVariation(LdClient, TCHAR_TO_ANSI(*FlagName), DefaultValue);
	UE_LOG(LaunchDarklyClient, Log, TEXT("Flag '%s' queried (with default %f) returned %f."), *FlagName, DefaultValue, FlagValue);

	return FlagValue;
}

int FLaunchDarklyImpl::GetIntVariation(FString FlagName, int DefaultValue)
{
	if(!IsInitialized())
	{
		UE_LOG(LaunchDarklyClient, Warning, TEXT("FLaunchDarklyImpl::GetBoolVariation(%s) called while uninitialized; returning default."), *FlagName);
		return DefaultValue;
	}

	int FlagValue = LDIntVariation(LdClient, TCHAR_TO_ANSI(*FlagName), DefaultValue);
	UE_LOG(LaunchDarklyClient, Log, TEXT("Flag '%s' queried (with default %d) returned %d."), *FlagName, DefaultValue, FlagValue);

	return FlagValue;
}

TSharedPtr<FJsonObject> FLaunchDarklyImpl::GetJsonVariation(FString FlagName, TSharedPtr<FJsonObject> DefaultValue)
{
	if(!IsInitialized())
	{
		UE_LOG(LaunchDarklyClient, Warning, TEXT("FLaunchDarklyImpl::GetBoolVariation(%s) called while uninitialized; returning default."), *FlagName);
		return DefaultValue;
	}

	FString DefaultValueAsString = DefaultValue == NULL ? ULaunchDarklyHelpers::JsonObjectToString(DefaultValue) : "{}";
	LDNode* DefaultValueAsLdNode = LDNodeFromJSON(TCHAR_TO_ANSI(*DefaultValueAsString));
	LDNode* FlagValueAsLdNode = LDJSONVariation(LdClient, TCHAR_TO_ANSI(*FlagName), DefaultValueAsLdNode);
	FString FlagValueAsString = FString(LDHashToJSON(FlagValueAsLdNode));
	TSharedPtr<FJsonObject> FlagValue = ULaunchDarklyHelpers::StringToJsonObject(FlagValueAsString);
	UE_LOG(LaunchDarklyClient, Log, TEXT("Flag '%s' queried (with default %s) returned %s."), *FlagName, *DefaultValueAsString, *FlagValueAsString);

	return FlagValue;
}

FString FLaunchDarklyImpl::GetStringVariation(FString FlagName, FString DefaultValue)
{
	if(!IsInitialized())
	{
		UE_LOG(LaunchDarklyClient, Warning, TEXT("FLaunchDarklyImpl::GetBoolVariation(%s) called while uninitialized; returning default."), *FlagName);
		return DefaultValue;
	}

	char* const Vsz = LDStringVariationAlloc(LdClient, TCHAR_TO_ANSI(*FlagName), TCHAR_TO_ANSI(*DefaultValue));
	FString FlagValue = FString(Vsz);
	LDFree(Vsz);
	UE_LOG(LaunchDarklyClient, Log, TEXT("Flag '%s' queried (with default %s) returned %s."), *FlagName, *DefaultValue, *FlagValue);

	return FlagValue;
}

void FLaunchDarklyImpl::RegisterBoolFlagListener(ULdBoolFlagListener* FlagListener, FString FlagName)
{
	if(BoolFlagListeners.Contains(FlagName) == false)
	{
		if(IsInitialized())
		{
			LDClientRegisterFeatureFlagListener(LdClient, TCHAR_TO_ANSI(*FlagName), FLaunchDarklyImpl::BoolFlagListener);
		}
		TArray<ULdBoolFlagListener*> FlagListenerArray;
		FlagListenerArray.Add(FlagListener);
		BoolFlagListeners.Add(FlagName, FlagListenerArray);
	}
	else
	{
		BoolFlagListeners[FlagName].Add(FlagListener);
	}
	FlagListener->AddToRoot();
}

void FLaunchDarklyImpl::UnregisterBoolFlagListener(ULdBoolFlagListener* FlagListener, FString FlagName)
{
	if(BoolFlagListeners.Contains(FlagName))
	{
		FlagListener->RemoveFromRoot();
		BoolFlagListeners[FlagName].Remove(FlagListener);
		if(BoolFlagListeners[FlagName].Num() == 0)
		{
			BoolFlagListeners.Remove(FlagName);
			if(IsInitialized())
			{
				LDClientUnregisterFeatureFlagListener(LdClient, TCHAR_TO_ANSI(*FlagName), FLaunchDarklyImpl::BoolFlagListener);
			}
		}
	}
}
void FLaunchDarklyImpl::RegisterFloatFlagListener(ULdFloatFlagListener* FlagListener, FString FlagName)
{
	if(FloatFlagListeners.Contains(FlagName) == false)
	{
		if(IsInitialized())
		{
			LDClientRegisterFeatureFlagListener(LdClient, TCHAR_TO_ANSI(*FlagName), FLaunchDarklyImpl::FloatFlagListener);
		}
		TArray<ULdFloatFlagListener*> FlagListenerArray;
		FlagListenerArray.Add(FlagListener);
		FloatFlagListeners.Add(FlagName, FlagListenerArray);
	}
	else
	{
		FloatFlagListeners[FlagName].Add(FlagListener);
	}
	FlagListener->AddToRoot();
}

void FLaunchDarklyImpl::UnregisterFloatFlagListener(ULdFloatFlagListener* FlagListener, FString FlagName)
{
	if(FloatFlagListeners.Contains(FlagName))
	{
		FlagListener->RemoveFromRoot();
		FloatFlagListeners[FlagName].Remove(FlagListener);
		if(FloatFlagListeners[FlagName].Num() == 0)
		{
			FloatFlagListeners.Remove(FlagName);
			if(IsInitialized())
			{
				LDClientUnregisterFeatureFlagListener(LdClient, TCHAR_TO_ANSI(*FlagName), FLaunchDarklyImpl::FloatFlagListener);
			}
		}
	}
}
void FLaunchDarklyImpl::RegisterIntFlagListener(ULdIntFlagListener* FlagListener, FString FlagName)
{
	if(IntFlagListeners.Contains(FlagName) == false)
	{
		if(IsInitialized())
		{
			LDClientRegisterFeatureFlagListener(LdClient, TCHAR_TO_ANSI(*FlagName), FLaunchDarklyImpl::IntFlagListener);
		}
		TArray<ULdIntFlagListener*> FlagListenerArray;
		FlagListenerArray.Add(FlagListener);
		IntFlagListeners.Add(FlagName, FlagListenerArray);
	}
	else
	{
		IntFlagListeners[FlagName].Add(FlagListener);
	}
	FlagListener->AddToRoot();
}

void FLaunchDarklyImpl::UnregisterIntFlagListener(ULdIntFlagListener* FlagListener, FString FlagName)
{
	if(IntFlagListeners.Contains(FlagName))
	{
		FlagListener->RemoveFromRoot();
		IntFlagListeners[FlagName].Remove(FlagListener);
		if(IntFlagListeners[FlagName].Num() == 0)
		{
			IntFlagListeners.Remove(FlagName);
			if(IsInitialized())
			{
				LDClientUnregisterFeatureFlagListener(LdClient, TCHAR_TO_ANSI(*FlagName), FLaunchDarklyImpl::IntFlagListener);
			}
		}
	}
}
void FLaunchDarklyImpl::RegisterJsonFlagListener(ULdJsonFlagListener* FlagListener, FString FlagName)
{
	if(JsonFlagListeners.Contains(FlagName) == false)
	{
		if(IsInitialized())
		{
			LDClientRegisterFeatureFlagListener(LdClient, TCHAR_TO_ANSI(*FlagName), FLaunchDarklyImpl::JsonFlagListener);
		}
		TArray<ULdJsonFlagListener*> FlagListenerArray;
		FlagListenerArray.Add(FlagListener);
		JsonFlagListeners.Add(FlagName, FlagListenerArray);
	}
	else
	{
		JsonFlagListeners[FlagName].Add(FlagListener);
	}
	FlagListener->AddToRoot();
}

void FLaunchDarklyImpl::UnregisterJsonFlagListener(ULdJsonFlagListener* FlagListener, FString FlagName)
{
	if(JsonFlagListeners.Contains(FlagName))
	{
		FlagListener->RemoveFromRoot();
		JsonFlagListeners[FlagName].Remove(FlagListener);
		if(JsonFlagListeners[FlagName].Num() == 0)
		{
			JsonFlagListeners.Remove(FlagName);
			if(IsInitialized())
			{
				LDClientUnregisterFeatureFlagListener(LdClient, TCHAR_TO_ANSI(*FlagName), FLaunchDarklyImpl::JsonFlagListener);
			}
		}
	}
}
void FLaunchDarklyImpl::RegisterStringFlagListener(ULdStringFlagListener* FlagListener, FString FlagName)
{
	if(StringFlagListeners.Contains(FlagName) == false)
	{
		if(IsInitialized())
		{
			LDClientRegisterFeatureFlagListener(LdClient, TCHAR_TO_ANSI(*FlagName), FLaunchDarklyImpl::StringFlagListener);
		}
		TArray<ULdStringFlagListener*> FlagListenerArray;
		FlagListenerArray.Add(FlagListener);
		StringFlagListeners.Add(FlagName, FlagListenerArray);
	}
	else
	{
		StringFlagListeners[FlagName].Add(FlagListener);
	}
	FlagListener->AddToRoot();
}

void FLaunchDarklyImpl::UnregisterStringFlagListener(ULdStringFlagListener* FlagListener, FString FlagName)
{
	if(StringFlagListeners.Contains(FlagName))
	{
		FlagListener->RemoveFromRoot();
		StringFlagListeners[FlagName].Remove(FlagListener);
		if(StringFlagListeners[FlagName].Num() == 0)
		{
			StringFlagListeners.Remove(FlagName);
			if(IsInitialized())
			{
				LDClientUnregisterFeatureFlagListener(LdClient, TCHAR_TO_ANSI(*FlagName), FLaunchDarklyImpl::StringFlagListener);
			}
		}
	}
}

void FLaunchDarklyImpl::Track(FString MetricName)
{
	if(!IsInitialized())
	{
		UE_LOG(LaunchDarklyClient, Warning, TEXT("FLaunchDarklyImpl::Track(%s) called while uninitialized."), *MetricName);
		return;
	}

	LDClientTrack(LdClient, TCHAR_TO_ANSI(*MetricName));
}

void FLaunchDarklyImpl::Track(FString MetricName, TSharedPtr<FJsonObject> const Data)
{
	if(!IsInitialized())
	{
		UE_LOG(LaunchDarklyClient, Warning, TEXT("FLaunchDarklyImpl::Track(%s, ...) called while uninitialized."), *MetricName);
		return;
	}

	FString DataAsString = ULaunchDarklyHelpers::JsonObjectToString(Data);
	LDNode* DataAsLdNode = LDNodeFromJSON(TCHAR_TO_ANSI(*DataAsString));

	LDClientTrackData(LdClient, TCHAR_TO_ANSI(*MetricName), DataAsLdNode);
}

void FLaunchDarklyImpl::RestoreFlagListeners()
{
	TMap<FString, TArray<ULdBoolFlagListener*>>& BoolFlagListeners = FLaunchDarklyClientModule::Get()->GetBoolFlagListeners();
	for(auto Entry : BoolFlagListeners)
	{
		LDClientRegisterFeatureFlagListener(LdClient, TCHAR_TO_ANSI(*(Entry.Key)), FLaunchDarklyImpl::BoolFlagListener);
	}

	TMap<FString, TArray<ULdFloatFlagListener*>>& FloatFlagListeners = FLaunchDarklyClientModule::Get()->GetFloatFlagListeners();
	for(auto Entry : FloatFlagListeners)
	{
		LDClientRegisterFeatureFlagListener(LdClient, TCHAR_TO_ANSI(*(Entry.Key)), FLaunchDarklyImpl::FloatFlagListener);
	}

	TMap<FString, TArray<ULdIntFlagListener*>>& IntFlagListeners = FLaunchDarklyClientModule::Get()->GetIntFlagListeners();
	for(auto Entry : IntFlagListeners)
	{
		LDClientRegisterFeatureFlagListener(LdClient, TCHAR_TO_ANSI(*(Entry.Key)), FLaunchDarklyImpl::IntFlagListener);
	}

	TMap<FString, TArray<ULdJsonFlagListener*>>& JsonFlagListeners = FLaunchDarklyClientModule::Get()->GetJsonFlagListeners();
	for(auto Entry : JsonFlagListeners)
	{
		LDClientRegisterFeatureFlagListener(LdClient, TCHAR_TO_ANSI(*(Entry.Key)), FLaunchDarklyImpl::JsonFlagListener);
	}

	TMap<FString, TArray<ULdStringFlagListener*>>& StringFlagListeners = FLaunchDarklyClientModule::Get()->GetStringFlagListeners();
	for(auto Entry : StringFlagListeners)
	{
		LDClientRegisterFeatureFlagListener(LdClient, TCHAR_TO_ANSI(*(Entry.Key)), FLaunchDarklyImpl::StringFlagListener);
	}
}

void FLaunchDarklyImpl::UpdateAllFlagListeners()
{
	TMap<FString, TArray<ULdBoolFlagListener*>>& BoolFlagListeners = FLaunchDarklyClientModule::Get()->GetBoolFlagListeners();
	for(auto Entry : BoolFlagListeners)
	{
		bool FVal = LDBoolVariation(LdClient, TCHAR_TO_ANSI(*(Entry.Key)), false);
		TArray<ULdBoolFlagListener*>& Listeners = Entry.Value;
		for(ULdBoolFlagListener* Listener : Listeners)
		{
			Listener->ValueChanged(FVal);
		}
	}

	TMap<FString, TArray<ULdFloatFlagListener*>>& FloatFlagListeners = FLaunchDarklyClientModule::Get()->GetFloatFlagListeners();
	for(auto Entry : FloatFlagListeners)
	{
		float FVal = (float)LDDoubleVariation(LdClient, TCHAR_TO_ANSI(*(Entry.Key)), 0.0);
		TArray<ULdFloatFlagListener*>& Listeners = Entry.Value;
		for(ULdFloatFlagListener* Listener : Listeners)
		{
			Listener->ValueChanged(FVal);
		}
	}

	TMap<FString, TArray<ULdIntFlagListener*>>& IntFlagListeners = FLaunchDarklyClientModule::Get()->GetIntFlagListeners();
	for(auto Entry : IntFlagListeners)
	{
		int FVal = LDIntVariation(LdClient, TCHAR_TO_ANSI(*(Entry.Key)), 0);
		TArray<ULdIntFlagListener*>& Listeners = Entry.Value;
		for(ULdIntFlagListener* Listener : Listeners)
		{
			Listener->ValueChanged(FVal);
		}
	}

	TMap<FString, TArray<ULdJsonFlagListener*>>& JsonFlagListeners = FLaunchDarklyClientModule::Get()->GetJsonFlagListeners();
	for(auto Entry : JsonFlagListeners)
	{
		LDNode* DefaultValueAsLdNode = LDNodeFromJSON("{}");
		LDNode* FlagValueAsLdNode = LDJSONVariation(LdClient, TCHAR_TO_ANSI(*(Entry.Key)), DefaultValueAsLdNode);
		FString FlagValueAsString = FString(LDHashToJSON(FlagValueAsLdNode));

		ULdNodeObject* FVal = NewObject<ULdNodeObject>();
		FVal->Initialize(ULaunchDarklyHelpers::StringToJsonObject(FlagValueAsString));
		TArray<ULdJsonFlagListener*>& Listeners = Entry.Value;
		for(ULdJsonFlagListener* Listener : Listeners)
		{
			Listener->ValueChanged(FVal);
		}
	}

	TMap<FString, TArray<ULdStringFlagListener*>>& StringFlagListeners = FLaunchDarklyClientModule::Get()->GetStringFlagListeners();
	for(auto Entry : StringFlagListeners)
	{
		FString FVal = LDStringVariationAlloc(LdClient, TCHAR_TO_ANSI(*(Entry.Key)), "");
		TArray<ULdStringFlagListener*>& Listeners = Entry.Value;
		for(ULdStringFlagListener* Listener : Listeners)
		{
			Listener->ValueChanged(FVal);
		}
	}
}
