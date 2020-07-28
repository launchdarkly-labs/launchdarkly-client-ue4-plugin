#include "LaunchDarklyImpl.h"
#include "LaunchDarklyClient.h"

#include "Async/Async.h"
#include "Dom/JsonObject.h"
#include "Engine/Engine.h"

#include "LdConfigObject.h"
#include "LdBoolFlagListener.h"
#include "LdBoolFlagListenerComponent.h"
#include "LdFloatFlagListener.h"
#include "LdFloatFlagListenerComponent.h"
#include "LdIntFlagListener.h"
#include "LdIntFlagListenerComponent.h"
#include "LdJsonFlagListener.h"
#include "LdJsonFlagListenerComponent.h"
#include "LdNodeObject.h"
#include "LdStringFlagListener.h"
#include "LdStringFlagListenerComponent.h"
#include "LdUserObject.h"
#include "LaunchDarklyHelpers.h"

#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"

static jmethodID AndroidLdSdkInterface_InitializeClient = 0;
static jmethodID AndroidLdSdkInterface_ShutdownClient = 0;
static jmethodID AndroidLdSdkInterface_IsInitialized = 0;
static jmethodID AndroidLdSdkInterface_IsOffline = 0;
static jmethodID AndroidLdSdkInterface_SetOffline = 0;
static jmethodID AndroidLdSdkInterface_SetOnline = 0;

static jmethodID AndroidLdSdkInterface_RefreshUserAttributes = 0;

static jmethodID AndroidLdSdkInterface_GetBoolVariation = 0;
static jmethodID AndroidLdSdkInterface_GetFloatVariation = 0;
static jmethodID AndroidLdSdkInterface_GetIntVariation = 0;
static jmethodID AndroidLdSdkInterface_GetJsonVariation = 0;
static jmethodID AndroidLdSdkInterface_GetStringVariation = 0;

static jmethodID AndroidLdSdkInterface_RegisterBoolFeatureFlagListener = 0;
static jmethodID AndroidLdSdkInterface_UnregisterBoolFeatureFlagListener = 0;
static jmethodID AndroidLdSdkInterface_RegisterFloatFeatureFlagListener = 0;
static jmethodID AndroidLdSdkInterface_UnregisterFloatFeatureFlagListener = 0;
static jmethodID AndroidLdSdkInterface_RegisterIntFeatureFlagListener = 0;
static jmethodID AndroidLdSdkInterface_UnregisterIntFeatureFlagListener = 0;
static jmethodID AndroidLdSdkInterface_RegisterJsonFeatureFlagListener = 0;
static jmethodID AndroidLdSdkInterface_UnregisterJsonFeatureFlagListener = 0;
static jmethodID AndroidLdSdkInterface_RegisterStringFeatureFlagListener = 0;
static jmethodID AndroidLdSdkInterface_UnregisterStringFeatureFlagListener = 0;

static jmethodID AndroidLdSdkInterface_RegisterBoolFeatureFlagListenerComponent = 0;
static jmethodID AndroidLdSdkInterface_UnregisterBoolFeatureFlagListenerComponent = 0;
static jmethodID AndroidLdSdkInterface_RegisterFloatFeatureFlagListenerComponent = 0;
static jmethodID AndroidLdSdkInterface_UnregisterFloatFeatureFlagListenerComponent = 0;
static jmethodID AndroidLdSdkInterface_RegisterIntFeatureFlagListenerComponent = 0;
static jmethodID AndroidLdSdkInterface_UnregisterIntFeatureFlagListenerComponent = 0;
static jmethodID AndroidLdSdkInterface_RegisterJsonFeatureFlagListenerComponent = 0;
static jmethodID AndroidLdSdkInterface_UnregisterJsonFeatureFlagListenerComponent = 0;
static jmethodID AndroidLdSdkInterface_RegisterStringFeatureFlagListenerComponent = 0;
static jmethodID AndroidLdSdkInterface_UnregisterStringFeatureFlagListenerComponent = 0;

static jmethodID AndroidLdSdkInterface_Track = 0;
static jmethodID AndroidLdSdkInterface_TrackWithData = 0;
#endif

void FLaunchDarklyImpl::InitJavaFunctions()
{
#if PLATFORM_ANDROID
	if(JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		AndroidLdSdkInterface_InitializeClient = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_InitializeClient", "(Ljava/lang/String;Ljava/lang/String;I)Z", false);
		AndroidLdSdkInterface_ShutdownClient = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_ShutdownClient", "()V", false);
		AndroidLdSdkInterface_IsInitialized = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_IsInitialized", "()Z", false);
		AndroidLdSdkInterface_IsOffline = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_IsOffline", "()Z", false);
		AndroidLdSdkInterface_SetOffline = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_SetOffline", "()V", false);
		AndroidLdSdkInterface_SetOnline = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_SetOnline", "()V", false);

		AndroidLdSdkInterface_RefreshUserAttributes = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_RefreshUserAttributes", "(Ljava/lang/String;)V", false);

		AndroidLdSdkInterface_GetBoolVariation = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_GetBoolVariation", "(Ljava/lang/String;Z)Z", false);
		AndroidLdSdkInterface_GetFloatVariation = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_GetFloatVariation", "(Ljava/lang/String;F)F", false);
		AndroidLdSdkInterface_GetIntVariation = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_GetIntVariation", "(Ljava/lang/String;I)I", false);
		AndroidLdSdkInterface_GetJsonVariation = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_GetJsonVariation", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;", false);
		AndroidLdSdkInterface_GetStringVariation = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_GetStringVariation", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;", false);

		AndroidLdSdkInterface_RegisterBoolFeatureFlagListener = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_RegisterBoolFeatureFlagListener", "(Ljava/lang/String;)V", false);
		AndroidLdSdkInterface_UnregisterBoolFeatureFlagListener = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_UnregisterBoolFeatureFlagListener", "(Ljava/lang/String;)V", false);
		AndroidLdSdkInterface_RegisterFloatFeatureFlagListener = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_RegisterFloatFeatureFlagListener", "(Ljava/lang/String;)V", false);
		AndroidLdSdkInterface_UnregisterFloatFeatureFlagListener = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_UnregisterFloatFeatureFlagListener", "(Ljava/lang/String;)V", false);
		AndroidLdSdkInterface_RegisterIntFeatureFlagListener = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_RegisterIntFeatureFlagListener", "(Ljava/lang/String;)V", false);
		AndroidLdSdkInterface_UnregisterIntFeatureFlagListener = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_UnregisterIntFeatureFlagListener", "(Ljava/lang/String;)V", false);
		AndroidLdSdkInterface_RegisterJsonFeatureFlagListener = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_RegisterJsonFeatureFlagListener", "(Ljava/lang/String;)V", false);
		AndroidLdSdkInterface_UnregisterJsonFeatureFlagListener = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_UnregisterJsonFeatureFlagListener", "(Ljava/lang/String;)V", false);
		AndroidLdSdkInterface_RegisterStringFeatureFlagListener = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_RegisterStringFeatureFlagListener", "(Ljava/lang/String;)V", false);
		AndroidLdSdkInterface_UnregisterStringFeatureFlagListener = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_UnregisterStringFeatureFlagListener", "(Ljava/lang/String;)V", false);

		AndroidLdSdkInterface_RegisterBoolFeatureFlagListenerComponent = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_RegisterBoolFeatureFlagListenerComponent", "(Ljava/lang/String;)V", false);
		AndroidLdSdkInterface_UnregisterBoolFeatureFlagListenerComponent = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_UnregisterBoolFeatureFlagListenerComponent", "(Ljava/lang/String;)V", false);
		AndroidLdSdkInterface_RegisterFloatFeatureFlagListenerComponent = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_RegisterFloatFeatureFlagListenerComponent", "(Ljava/lang/String;)V", false);
		AndroidLdSdkInterface_UnregisterFloatFeatureFlagListenerComponent = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_UnregisterFloatFeatureFlagListenerComponent", "(Ljava/lang/String;)V", false);
		AndroidLdSdkInterface_RegisterIntFeatureFlagListenerComponent = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_RegisterIntFeatureFlagListenerComponent", "(Ljava/lang/String;)V", false);
		AndroidLdSdkInterface_UnregisterIntFeatureFlagListenerComponent = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_UnregisterIntFeatureFlagListenerComponent", "(Ljava/lang/String;)V", false);
		AndroidLdSdkInterface_RegisterJsonFeatureFlagListenerComponent = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_RegisterJsonFeatureFlagListenerComponent", "(Ljava/lang/String;)V", false);
		AndroidLdSdkInterface_UnregisterJsonFeatureFlagListenerComponent = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_UnregisterJsonFeatureFlagListenerComponent", "(Ljava/lang/String;)V", false);
		AndroidLdSdkInterface_RegisterStringFeatureFlagListenerComponent = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_RegisterStringFeatureFlagListenerComponent", "(Ljava/lang/String;)V", false);
		AndroidLdSdkInterface_UnregisterStringFeatureFlagListenerComponent = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_UnregisterStringFeatureFlagListenerComponent", "(Ljava/lang/String;)V", false);

		AndroidLdSdkInterface_RegisterBoolFeatureFlagListenerComponent = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_RegisterBoolFeatureFlagListenerComponent", "(Ljava/lang/String;)V", false);
		AndroidLdSdkInterface_UnregisterBoolFeatureFlagListenerComponent = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_UnregisterBoolFeatureFlagListenerComponent", "(Ljava/lang/String;)V", false);
		AndroidLdSdkInterface_RegisterFloatFeatureFlagListenerComponent = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_RegisterFloatFeatureFlagListenerComponent", "(Ljava/lang/String;)V", false);
		AndroidLdSdkInterface_UnregisterFloatFeatureFlagListenerComponent = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_UnregisterFloatFeatureFlagListenerComponent", "(Ljava/lang/String;)V", false);
		AndroidLdSdkInterface_RegisterIntFeatureFlagListenerComponent = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_RegisterIntFeatureFlagListenerComponent", "(Ljava/lang/String;)V", false);
		AndroidLdSdkInterface_UnregisterIntFeatureFlagListenerComponent = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_UnregisterIntFeatureFlagListenerComponent", "(Ljava/lang/String;)V", false);
		AndroidLdSdkInterface_RegisterJsonFeatureFlagListenerComponent = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_RegisterJsonFeatureFlagListenerComponent", "(Ljava/lang/String;)V", false);
		AndroidLdSdkInterface_UnregisterJsonFeatureFlagListenerComponent = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_UnregisterJsonFeatureFlagListenerComponent", "(Ljava/lang/String;)V", false);
		AndroidLdSdkInterface_RegisterStringFeatureFlagListenerComponent = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_RegisterStringFeatureFlagListenerComponent", "(Ljava/lang/String;)V", false);
		AndroidLdSdkInterface_UnregisterStringFeatureFlagListenerComponent = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_UnregisterStringFeatureFlagListenerComponent", "(Ljava/lang/String;)V", false);

		AndroidLdSdkInterface_Track = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_Track", "(Ljava/lang/String;)V", false);
		AndroidLdSdkInterface_TrackWithData = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidLdSdkInterface_TrackWithData", "(Ljava/lang/String;Ljava/lang/String;)V", false);
	}
	else
	{
		check(0);
	}
#else
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("CALLED BUT UNSUPPORTED: LaunchDarkly plugin initialized while PLATFORM_ANDROID not defined."));
#endif
}

bool FLaunchDarklyImpl::InitializeClient(ULdConfigObject* LdConfig, ULdUserObject* LdUser, int ConnectionTimeoutMillis)
{
	bool isInitialized = false;
#if PLATFORM_ANDROID
	if(JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: InitializeClient."));
		FString ConfigAsString = LdConfig->ToJsonString();
		FString UserAsString = LdUser->ToJsonString();
		jstring JavaConfigAsString = Env->NewStringUTF(TCHAR_TO_UTF8(*ConfigAsString));
		jstring JavaUserAsString = Env->NewStringUTF(TCHAR_TO_UTF8(*UserAsString));
		jint JavaConnectionTimeout = (jint)ConnectionTimeoutMillis;
		isInitialized = (bool)(FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, AndroidLdSdkInterface_InitializeClient, JavaConfigAsString, JavaUserAsString, JavaConnectionTimeout) != JNI_FALSE);
	}
#endif
	return isInitialized;
}

void FLaunchDarklyImpl::ShutdownClient()
{
#if PLATFORM_ANDROID
	if(JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: ."));
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidLdSdkInterface_ShutdownClient);
	}
#endif
}


bool FLaunchDarklyImpl::IsInitialized()
{
#if PLATFORM_ANDROID
	if(JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		return (bool)(FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, AndroidLdSdkInterface_IsInitialized) != JNI_FALSE);
	}
#endif
	return false;
}

bool FLaunchDarklyImpl::IsOffline()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("LD Plugin - Shutting down LaunchDarkly."));
#if PLATFORM_ANDROID
	if(JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		return FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, AndroidLdSdkInterface_IsOffline);
	}
#endif

	return true;
}

void FLaunchDarklyImpl::SetOffline()
{
#if PLATFORM_ANDROID
	if(JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidLdSdkInterface_SetOffline);
	}
#endif
}

void FLaunchDarklyImpl::SetOnline()
{
#if PLATFORM_ANDROID
	if(JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidLdSdkInterface_SetOnline);
	}
#endif
}

void FLaunchDarklyImpl::RefreshUserAttributes(ULdUserObject* LdUserObject)
{
#if PLATFORM_ANDROID
	if(JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: RefreshUserAttributes."));
		FString UserAsString = LdUserObject->ToJsonString();
		jstring JavaUserAsString = Env->NewStringUTF(TCHAR_TO_UTF8(*UserAsString));
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidLdSdkInterface_RefreshUserAttributes, JavaUserAsString);
	}
#endif
}

bool FLaunchDarklyImpl::GetBoolVariation(FString FlagName, bool DefaultValue)
{
	bool FlagValue = DefaultValue;
#if PLATFORM_ANDROID
	if(JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: GetBoolVariation >>%s<<."), *FlagName);
		jstring JavaFlagName = Env->NewStringUTF(TCHAR_TO_UTF8(*FlagName));
		jboolean JavaDefaultValue = DefaultValue ? JNI_TRUE : JNI_FALSE;
		FlagValue = FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, AndroidLdSdkInterface_GetBoolVariation, JavaFlagName, JavaDefaultValue) != JNI_FALSE;
	}
#endif
	return FlagValue;
}

float FLaunchDarklyImpl::GetFloatVariation(FString FlagName, float DefaultValue)
{
	float FlagValue = DefaultValue;
#if PLATFORM_ANDROID
	if(JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: GetFloatVariation >>%s<<."), *FlagName);
		jstring JavaFlagName = Env->NewStringUTF(TCHAR_TO_UTF8(*FlagName));
		jfloat JavaDefaultValue = DefaultValue;
		FlagValue = FJavaWrapper::CallFloatMethod(Env, FJavaWrapper::GameActivityThis, AndroidLdSdkInterface_GetFloatVariation, JavaFlagName, JavaDefaultValue);
	}
#endif
	return FlagValue;
}

int FLaunchDarklyImpl::GetIntVariation(FString FlagName, int DefaultValue)
{
	int FlagValue = DefaultValue;
#if PLATFORM_ANDROID
	if(JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: GetIntVariation >>%s<<."), *FlagName);
		jstring JavaFlagName = Env->NewStringUTF(TCHAR_TO_UTF8(*FlagName));
		jint JavaDefaultValue = DefaultValue;
		FlagValue = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, AndroidLdSdkInterface_GetIntVariation, JavaFlagName, JavaDefaultValue);
	}
#endif
	return FlagValue;
}

TSharedPtr<FJsonObject> FLaunchDarklyImpl::GetJsonVariation(FString FlagName, TSharedPtr<FJsonObject> DefaultValue)
{
	TSharedPtr<FJsonObject> FlagValue = DefaultValue;
#if PLATFORM_ANDROID
	if(JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: GetJsonVariation >>%s<<."), *FlagName);
		jstring JavaFlagName = Env->NewStringUTF(TCHAR_TO_UTF8(*FlagName));
		FString DefaultValueAsFString = ULaunchDarklyHelpers::JsonObjectToString(DefaultValue);
		jstring JavaDefaultValue = Env->NewStringUTF(TCHAR_TO_UTF8(*DefaultValueAsFString));
		jstring VariationAsJString = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, AndroidLdSdkInterface_GetJsonVariation, JavaFlagName, JavaDefaultValue);
		FString VariationAsString = FString(Env->GetStringUTFChars(VariationAsJString, 0));
		FlagValue = ULaunchDarklyHelpers::StringToJsonObject(VariationAsString);
	}
#endif
	return FlagValue;
}

FString FLaunchDarklyImpl::GetStringVariation(FString FlagName, FString DefaultValue)
{
	FString FlagValue = DefaultValue;
#if PLATFORM_ANDROID
	if(JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: GetStringVariation >>%s<<."), *FlagName);
		jstring JavaFlagName = Env->NewStringUTF(TCHAR_TO_UTF8(*FlagName));
		jstring JavaDefaultValue = Env->NewStringUTF(TCHAR_TO_UTF8(*DefaultValue));
		jstring VariationValue = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, AndroidLdSdkInterface_GetStringVariation, JavaFlagName, JavaDefaultValue);
		FlagValue = FString(Env->GetStringUTFChars(VariationValue, 0));
	}
#endif
	return FlagValue;
}

void FLaunchDarklyImpl::RegisterBoolFlagListener(ULdBoolFlagListener* FlagListener, FString FlagName)
{
	TMap<FString, TArray<ULdBoolFlagListener*>>& BoolFlagListeners = FLaunchDarklyClientModule::Get()->GetBoolFlagListeners();
	if(BoolFlagListeners.Contains(FlagName) == false)
	{
		UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: RegisterBoolFlagListener STARTED >>%s<<."), *FlagName);
#if PLATFORM_ANDROID
		if(JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
		{
			jstring JFlagName = Env->NewStringUTF(TCHAR_TO_UTF8(*FlagName));
			UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: RegisterBoolFlagListener COMPLETED."));
			FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidLdSdkInterface_RegisterBoolFeatureFlagListener, JFlagName);
		}
#endif
		TArray<ULdBoolFlagListener*> FlagListenerArray;
		FlagListenerArray.Add(FlagListener);
		BoolFlagListeners.Add(FlagName, FlagListenerArray);
	}
	else
	{
		TArray<ULdBoolFlagListener*>& FlagListenerArray = BoolFlagListeners[FlagName];
		FlagListenerArray.Add(FlagListener);
		UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: RegisterBoolFlagListener COMPLETED >>%s<<. Total listeners: %d"), *FlagName, FlagListenerArray.Num());
	}
}

void FLaunchDarklyImpl::UnregisterBoolFlagListener(ULdBoolFlagListener* FlagListener, FString FlagName)
{
	TMap<FString, TArray<ULdBoolFlagListener*>>& BoolFlagListeners = FLaunchDarklyClientModule::Get()->GetBoolFlagListeners();
	if(BoolFlagListeners.Contains(FlagName))
	{
		UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: UnregisterBoolFlagListener STARTED >>%s<<."), *FlagName);
		TArray<ULdBoolFlagListener*>& FlagListenerArray = BoolFlagListeners[FlagName];
		FlagListenerArray.Remove(FlagListener);
		if(FlagListenerArray.Num() == 0)
		{
			BoolFlagListeners.Remove(FlagName);
#if PLATFORM_ANDROID
			if(JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
			{
				jstring JFlagName = Env->NewStringUTF(TCHAR_TO_UTF8(*FlagName));
				UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: UnregisterBoolFlagListener COMPLETED."));
				FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidLdSdkInterface_UnregisterBoolFeatureFlagListener, JFlagName);
			}
#endif
		}
		else
		{
			UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: UnregisterBoolFlagListener COMPLETED. %d listeners remain."), FlagListenerArray.Num());
		}
	}
}

void FLaunchDarklyImpl::RegisterFloatFlagListener(ULdFloatFlagListener* FlagListener, FString FlagName)
{
	TMap<FString, TArray<ULdFloatFlagListener*>>& FloatFlagListeners = FLaunchDarklyClientModule::Get()->GetFloatFlagListeners();
	if(FloatFlagListeners.Contains(FlagName) == false)
	{
		UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: RegisterFloatFlagListener STARTED >>%s<<."), *FlagName);
#if PLATFORM_ANDROID
		if(JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
		{
			jstring JFlagName = Env->NewStringUTF(TCHAR_TO_UTF8(*FlagName));
			UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: RegisterFloatFlagListener COMPLETED."));
			FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidLdSdkInterface_RegisterFloatFeatureFlagListener, JFlagName);
		}
#endif
		TArray<ULdFloatFlagListener*> FlagListenerArray;
		FlagListenerArray.Add(FlagListener);
		FloatFlagListeners.Add(FlagName, FlagListenerArray);
	}
	else
	{
		TArray<ULdFloatFlagListener*>& FlagListenerArray = FloatFlagListeners[FlagName];
		FlagListenerArray.Add(FlagListener);
		UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: RegisterFloatFlagListener COMPLETED >>%s<<. Total listeners: %d"), *FlagName, FlagListenerArray.Num());
	}
}

void FLaunchDarklyImpl::UnregisterFloatFlagListener(ULdFloatFlagListener* FlagListener, FString FlagName)
{
	TMap<FString, TArray<ULdFloatFlagListener*>>& FloatFlagListeners = FLaunchDarklyClientModule::Get()->GetFloatFlagListeners();
	if(FloatFlagListeners.Contains(FlagName))
	{
		UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: UnregisterFloatFlagListener STARTED >>%s<<."), *FlagName);
		TArray<ULdFloatFlagListener*>& FlagListenerArray = FloatFlagListeners[FlagName];
		FlagListenerArray.Remove(FlagListener);
		if(FlagListenerArray.Num() == 0)
		{
			FloatFlagListeners.Remove(FlagName);
#if PLATFORM_ANDROID
			if(JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
			{
				jstring JFlagName = Env->NewStringUTF(TCHAR_TO_UTF8(*FlagName));
				UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: UnregisterFloatFlagListener  COMPLETED."));
				FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidLdSdkInterface_UnregisterFloatFeatureFlagListener, JFlagName);
			}
#endif
		}
		else
		{
			UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: UnregisterFloatFlagListener COMPLETED. %d listeners remain."), FlagListenerArray.Num());
		}
	}
}

void FLaunchDarklyImpl::RegisterIntFlagListener(ULdIntFlagListener* FlagListener, FString FlagName)
{
	TMap<FString, TArray<ULdIntFlagListener*>>& IntFlagListeners = FLaunchDarklyClientModule::Get()->GetIntFlagListeners();
	if(IntFlagListeners.Contains(FlagName) == false)
	{
		UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: RegisterIntFlagListener STARTED >>%s<<."), *FlagName);
#if PLATFORM_ANDROID
		if(JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
		{
			jstring JFlagName = Env->NewStringUTF(TCHAR_TO_UTF8(*FlagName));
			UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: RegisterIntFlagListener COMPLETED."));
			FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidLdSdkInterface_RegisterIntFeatureFlagListener, JFlagName);
		}
#endif
		TArray<ULdIntFlagListener*> FlagListenerArray;
		FlagListenerArray.Add(FlagListener);
		IntFlagListeners.Add(FlagName, FlagListenerArray);
	}
	else
	{
		TArray<ULdIntFlagListener*>& FlagListenerArray = IntFlagListeners[FlagName];
		FlagListenerArray.Add(FlagListener);
		UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: RegisterIntFlagListener COMPLETED >>%s<<. Total listeners: %d"), *FlagName, FlagListenerArray.Num());
	}
}

void FLaunchDarklyImpl::UnregisterIntFlagListener(ULdIntFlagListener* FlagListener, FString FlagName)
{
	TMap<FString, TArray<ULdIntFlagListener*>>& IntFlagListeners = FLaunchDarklyClientModule::Get()->GetIntFlagListeners();
	if(IntFlagListeners.Contains(FlagName))
	{
		UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: UnregisterIntFlagListener STARTED >>%s<<."), *FlagName);
		TArray<ULdIntFlagListener*>& FlagListenerArray = IntFlagListeners[FlagName];
		FlagListenerArray.Remove(FlagListener);
		if(FlagListenerArray.Num() == 0)
		{
			IntFlagListeners.Remove(FlagName);
#if PLATFORM_ANDROID
			if(JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
			{
				jstring JFlagName = Env->NewStringUTF(TCHAR_TO_UTF8(*FlagName));
				UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: UnregisterIntFlagListener COMPLETED."));
				FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidLdSdkInterface_UnregisterIntFeatureFlagListener, JFlagName);
			}
#endif
		}
		else
		{
			UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: UnregisterIntFlagListener COMPLETED. %d listeners remain."), FlagListenerArray.Num());
		}
	}
}

void FLaunchDarklyImpl::RegisterJsonFlagListener(ULdJsonFlagListener* FlagListener, FString FlagName)
{
	TMap<FString, TArray<ULdJsonFlagListener*>>& JsonFlagListeners = FLaunchDarklyClientModule::Get()->GetJsonFlagListeners();
	if(JsonFlagListeners.Contains(FlagName) == false)
	{
#if PLATFORM_ANDROID
		if(JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
		{
			jstring JFlagName = Env->NewStringUTF(TCHAR_TO_UTF8(*FlagName));
			FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidLdSdkInterface_RegisterJsonFeatureFlagListener, JFlagName);
		}
#endif
		TArray<ULdJsonFlagListener*> FlagListenerArray;
		FlagListenerArray.Add(FlagListener);
		JsonFlagListeners.Add(FlagName, FlagListenerArray);
	}
	else
	{
		TArray<ULdJsonFlagListener*>& FlagListenerArray = JsonFlagListeners[FlagName];
		FlagListenerArray.Add(FlagListener);
	}
}

void FLaunchDarklyImpl::UnregisterJsonFlagListener(ULdJsonFlagListener* FlagListener, FString FlagName)
{
	TMap<FString, TArray<ULdJsonFlagListener*>>& JsonFlagListeners = FLaunchDarklyClientModule::Get()->GetJsonFlagListeners();
	if(JsonFlagListeners.Contains(FlagName))
	{
		TArray<ULdJsonFlagListener*>& FlagListenerArray = JsonFlagListeners[FlagName];
		FlagListenerArray.Remove(FlagListener);
		if(FlagListenerArray.Num() == 0)
		{
			JsonFlagListeners.Remove(FlagName);
#if PLATFORM_ANDROID
			if(JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
			{
				jstring JFlagName = Env->NewStringUTF(TCHAR_TO_UTF8(*FlagName));
				FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidLdSdkInterface_UnregisterJsonFeatureFlagListener, JFlagName);
			}
#endif
		}
	}
}

void FLaunchDarklyImpl::RegisterStringFlagListener(ULdStringFlagListener* FlagListener, FString FlagName)
{
	TMap<FString, TArray<ULdStringFlagListener*>>& StringFlagListeners = FLaunchDarklyClientModule::Get()->GetStringFlagListeners();
	if(StringFlagListeners.Contains(FlagName) == false)
	{
		UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: RegisterStringFlagListener STARTED >>%s<<."), *FlagName);
#if PLATFORM_ANDROID
		if(JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
		{
			jstring JFlagName = Env->NewStringUTF(TCHAR_TO_UTF8(*FlagName));
			UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: RegisterStringFlagListener COMPLETED."));
			FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidLdSdkInterface_RegisterStringFeatureFlagListener, JFlagName);
		}
#endif
		TArray<ULdStringFlagListener*> FlagListenerArray;
		FlagListenerArray.Add(FlagListener);
		StringFlagListeners.Add(FlagName, FlagListenerArray);
	}
	else
	{
		TArray<ULdStringFlagListener*>& FlagListenerArray = StringFlagListeners[FlagName];
		FlagListenerArray.Add(FlagListener);
		UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: RegisterStringFlagListener COMPLETED >>%s<<. Total listeners: %d"), *FlagName, FlagListenerArray.Num());
	}
}

void FLaunchDarklyImpl::UnregisterStringFlagListener(ULdStringFlagListener* FlagListener, FString FlagName)
{
	TMap<FString, TArray<ULdStringFlagListener*>>& StringFlagListeners = FLaunchDarklyClientModule::Get()->GetStringFlagListeners();
	if(StringFlagListeners.Contains(FlagName))
	{
		UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: UnregisterStringFlagListener STARTED >>%s<<."), *FlagName);
		TArray<ULdStringFlagListener*>& FlagListenerArray = StringFlagListeners[FlagName];
		FlagListenerArray.Remove(FlagListener);
		if(FlagListenerArray.Num() == 0)
		{
			StringFlagListeners.Remove(FlagName);
#if PLATFORM_ANDROID
			if(JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
			{
				jstring JFlagName = Env->NewStringUTF(TCHAR_TO_UTF8(*FlagName));
				FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidLdSdkInterface_UnregisterStringFeatureFlagListener, JFlagName);
				UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: UnregisterStringFlagListener COMPLETED."));
			}
#endif
		}
		else
		{
			UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: UnregisterStringFlagListener COMPLETED. %d listeners remain."), FlagListenerArray.Num());
		}
	}
}

void FLaunchDarklyImpl::RegisterBoolFlagListenerComponent(ULdBoolFlagListenerComponent* FlagListener, FString FlagName)
{
	TMap<FString, TArray<ULdBoolFlagListenerComponent*>>& BoolFlagListeners = FLaunchDarklyClientModule::Get()->GetBoolFlagListenerComponents();
	if(BoolFlagListeners.Contains(FlagName) == false)
	{
		UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: RegisterBoolFlagListenerComponent STARTED >>%s<<."), *FlagName);
#if PLATFORM_ANDROID
		if(JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
		{
			jstring JFlagName = Env->NewStringUTF(TCHAR_TO_UTF8(*FlagName));
			UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: RegisterBoolFlagListenerComponent COMPLETED."));
			FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidLdSdkInterface_RegisterBoolFeatureFlagListenerComponent, JFlagName);
		}
#endif
		TArray<ULdBoolFlagListenerComponent*> FlagListenerArray;
		FlagListenerArray.Add(FlagListener);
		BoolFlagListeners.Add(FlagName, FlagListenerArray);
	}
	else
	{
		TArray<ULdBoolFlagListenerComponent*>& FlagListenerArray = BoolFlagListeners[FlagName];
		FlagListenerArray.Add(FlagListener);
		UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: RegisterBoolFlagListenerComponent COMPLETED >>%s<<. Total listeners: %d"), *FlagName, FlagListenerArray.Num());
	}
}

void FLaunchDarklyImpl::UnregisterBoolFlagListenerComponent(ULdBoolFlagListenerComponent* FlagListener, FString FlagName)
{
	TMap<FString, TArray<ULdBoolFlagListenerComponent*>>& BoolFlagListeners = FLaunchDarklyClientModule::Get()->GetBoolFlagListenerComponents();
	if(BoolFlagListeners.Contains(FlagName))
	{
		UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: UnregisterBoolFlagListenerComponent STARTED >>%s<<."), *FlagName);
		TArray<ULdBoolFlagListenerComponent*>& FlagListenerArray = BoolFlagListeners[FlagName];
		FlagListenerArray.Remove(FlagListener);
		if(FlagListenerArray.Num() == 0)
		{
			BoolFlagListeners.Remove(FlagName);
#if PLATFORM_ANDROID
			if(JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
			{
				jstring JFlagName = Env->NewStringUTF(TCHAR_TO_UTF8(*FlagName));
				UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: UnregisterBoolFlagListenerComponent COMPLETED."));
				FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidLdSdkInterface_UnregisterBoolFeatureFlagListenerComponent, JFlagName);
			}
#endif
		}
		else
		{
			UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: UnregisterBoolFlagListenerComponent COMPLETED. %d listeners remain."), FlagListenerArray.Num());
		}
	}
}

void FLaunchDarklyImpl::RegisterFloatFlagListenerComponent(ULdFloatFlagListenerComponent* FlagListener, FString FlagName)
{
	TMap<FString, TArray<ULdFloatFlagListenerComponent*>>& FloatFlagListeners = FLaunchDarklyClientModule::Get()->GetFloatFlagListenerComponents();
	if(FloatFlagListeners.Contains(FlagName) == false)
	{
		UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: RegisterFloatFlagListenerComponent STARTED >>%s<<."), *FlagName);
#if PLATFORM_ANDROID
		if(JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
		{
			jstring JFlagName = Env->NewStringUTF(TCHAR_TO_UTF8(*FlagName));
			UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: RegisterFloatFlagListenerComponent COMPLETED."));
			FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidLdSdkInterface_RegisterFloatFeatureFlagListenerComponent, JFlagName);
		}
#endif
		TArray<ULdFloatFlagListenerComponent*> FlagListenerArray;
		FlagListenerArray.Add(FlagListener);
		FloatFlagListeners.Add(FlagName, FlagListenerArray);
	}
	else
	{
		TArray<ULdFloatFlagListenerComponent*>& FlagListenerArray = FloatFlagListeners[FlagName];
		FlagListenerArray.Add(FlagListener);
		UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: RegisterFloatFlagListenerComponent COMPLETED >>%s<<. Total listeners: %d"), *FlagName, FlagListenerArray.Num());
	}
}

void FLaunchDarklyImpl::UnregisterFloatFlagListenerComponent(ULdFloatFlagListenerComponent* FlagListener, FString FlagName)
{
	TMap<FString, TArray<ULdFloatFlagListenerComponent*>>& FloatFlagListeners = FLaunchDarklyClientModule::Get()->GetFloatFlagListenerComponents();
	if(FloatFlagListeners.Contains(FlagName))
	{
		UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: UnregisterFloatFlagListenerComponent STARTED >>%s<<."), *FlagName);
		TArray<ULdFloatFlagListenerComponent*>& FlagListenerArray = FloatFlagListeners[FlagName];
		FlagListenerArray.Remove(FlagListener);
		if(FlagListenerArray.Num() == 0)
		{
			FloatFlagListeners.Remove(FlagName);
#if PLATFORM_ANDROID
			if(JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
			{
				jstring JFlagName = Env->NewStringUTF(TCHAR_TO_UTF8(*FlagName));
				UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: UnregisterFloatFlagListenerComponent  COMPLETED."));
				FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidLdSdkInterface_UnregisterFloatFeatureFlagListenerComponent, JFlagName);
			}
#endif
		}
		else
		{
			UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: UnregisterFloatFlagListenerComponent COMPLETED. %d listeners remain."), FlagListenerArray.Num());
		}
	}
}

void FLaunchDarklyImpl::RegisterIntFlagListenerComponent(ULdIntFlagListenerComponent* FlagListener, FString FlagName)
{
	TMap<FString, TArray<ULdIntFlagListenerComponent*>>& IntFlagListeners = FLaunchDarklyClientModule::Get()->GetIntFlagListenerComponents();
	if(IntFlagListeners.Contains(FlagName) == false)
	{
		UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: RegisterIntFlagListenerComponent STARTED >>%s<<."), *FlagName);
#if PLATFORM_ANDROID
		if(JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
		{
			jstring JFlagName = Env->NewStringUTF(TCHAR_TO_UTF8(*FlagName));
			UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: RegisterIntFlagListenerComponent COMPLETED."));
			FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidLdSdkInterface_RegisterIntFeatureFlagListenerComponent, JFlagName);
		}
#endif
		TArray<ULdIntFlagListenerComponent*> FlagListenerArray;
		FlagListenerArray.Add(FlagListener);
		IntFlagListeners.Add(FlagName, FlagListenerArray);
	}
	else
	{
		TArray<ULdIntFlagListenerComponent*>& FlagListenerArray = IntFlagListeners[FlagName];
		FlagListenerArray.Add(FlagListener);
		UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: RegisterIntFlagListener COMPLETED >>%s<<. Total listeners: %d"), *FlagName, FlagListenerArray.Num());
	}
}

void FLaunchDarklyImpl::UnregisterIntFlagListenerComponent(ULdIntFlagListenerComponent* FlagListener, FString FlagName)
{
	TMap<FString, TArray<ULdIntFlagListenerComponent*>>& IntFlagListeners = FLaunchDarklyClientModule::Get()->GetIntFlagListenerComponents();
	if(IntFlagListeners.Contains(FlagName))
	{
		UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: UnregisterIntFlagListener STARTED >>%s<<."), *FlagName);
		TArray<ULdIntFlagListenerComponent*>& FlagListenerArray = IntFlagListeners[FlagName];
		FlagListenerArray.Remove(FlagListener);
		if(FlagListenerArray.Num() == 0)
		{
			IntFlagListeners.Remove(FlagName);
#if PLATFORM_ANDROID
			if(JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
			{
				jstring JFlagName = Env->NewStringUTF(TCHAR_TO_UTF8(*FlagName));
				UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: UnregisterIntFlagListener COMPLETED."));
				FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidLdSdkInterface_UnregisterIntFeatureFlagListener, JFlagName);
			}
#endif
		}
		else
		{
			UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: UnregisterIntFlagListener COMPLETED. %d listeners remain."), FlagListenerArray.Num());
		}
	}
}

void FLaunchDarklyImpl::RegisterJsonFlagListenerComponent(ULdJsonFlagListenerComponent* FlagListener, FString FlagName)
{
	TMap<FString, TArray<ULdJsonFlagListenerComponent*>>& JsonFlagListeners = FLaunchDarklyClientModule::Get()->GetJsonFlagListenerComponents();
	if(JsonFlagListeners.Contains(FlagName) == false)
	{
#if PLATFORM_ANDROID
		if(JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
		{
			jstring JFlagName = Env->NewStringUTF(TCHAR_TO_UTF8(*FlagName));
			FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidLdSdkInterface_RegisterJsonFeatureFlagListenerComponent, JFlagName);
		}
#endif
		TArray<ULdJsonFlagListenerComponent*> FlagListenerArray;
		FlagListenerArray.Add(FlagListener);
		JsonFlagListeners.Add(FlagName, FlagListenerArray);
	}
	else
	{
		TArray<ULdJsonFlagListenerComponent*>& FlagListenerArray = JsonFlagListeners[FlagName];
		FlagListenerArray.Add(FlagListener);
	}
}

void FLaunchDarklyImpl::UnregisterJsonFlagListenerComponent(ULdJsonFlagListenerComponent* FlagListener, FString FlagName)
{
	TMap<FString, TArray<ULdJsonFlagListenerComponent*>>& JsonFlagListeners = FLaunchDarklyClientModule::Get()->GetJsonFlagListenerComponents();
	if(JsonFlagListeners.Contains(FlagName))
	{
		TArray<ULdJsonFlagListenerComponent*>& FlagListenerArray = JsonFlagListeners[FlagName];
		FlagListenerArray.Remove(FlagListener);
		if(FlagListenerArray.Num() == 0)
		{
			JsonFlagListeners.Remove(FlagName);
#if PLATFORM_ANDROID
			if(JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
			{
				jstring JFlagName = Env->NewStringUTF(TCHAR_TO_UTF8(*FlagName));
				FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidLdSdkInterface_UnregisterJsonFeatureFlagListenerComponent, JFlagName);
			}
#endif
		}
	}
}

void FLaunchDarklyImpl::RegisterStringFlagListenerComponent(ULdStringFlagListenerComponent* FlagListener, FString FlagName)
{
	TMap<FString, TArray<ULdStringFlagListenerComponent*>>& StringFlagListeners = FLaunchDarklyClientModule::Get()->GetStringFlagListenerComponents();
	if(StringFlagListeners.Contains(FlagName) == false)
	{
		UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: RegisterStringFlagListenerComponent STARTED >>%s<<."), *FlagName);
#if PLATFORM_ANDROID
		if(JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
		{
			jstring JFlagName = Env->NewStringUTF(TCHAR_TO_UTF8(*FlagName));
			UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: RegisterStringFlagListenerComponent COMPLETED."));
			FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidLdSdkInterface_RegisterStringFeatureFlagListenerComponent, JFlagName);
		}
#endif
		TArray<ULdStringFlagListenerComponent*> FlagListenerArray;
		FlagListenerArray.Add(FlagListener);
		StringFlagListeners.Add(FlagName, FlagListenerArray);
	}
	else
	{
		TArray<ULdStringFlagListenerComponent*>& FlagListenerArray = StringFlagListeners[FlagName];
		FlagListenerArray.Add(FlagListener);
		UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: RegisterStringFlagListenerComponent COMPLETED >>%s<<. Total listeners: %d"), *FlagName, FlagListenerArray.Num());
	}
}

void FLaunchDarklyImpl::UnregisterStringFlagListenerComponent(ULdStringFlagListenerComponent* FlagListener, FString FlagName)
{
	TMap<FString, TArray<ULdStringFlagListenerComponent*>>& StringFlagListeners = FLaunchDarklyClientModule::Get()->GetStringFlagListenerComponents();
	if(StringFlagListeners.Contains(FlagName))
	{
		UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: UnregisterStringFlagListenerComponent STARTED >>%s<<."), *FlagName);
		TArray<ULdStringFlagListenerComponent*>& FlagListenerArray = StringFlagListeners[FlagName];
		FlagListenerArray.Remove(FlagListener);
		if(FlagListenerArray.Num() == 0)
		{
			StringFlagListeners.Remove(FlagName);
#if PLATFORM_ANDROID
			if(JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
			{
				jstring JFlagName = Env->NewStringUTF(TCHAR_TO_UTF8(*FlagName));
				FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidLdSdkInterface_UnregisterStringFeatureFlagListenerComponent, JFlagName);
				UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: UnregisterStringFlagListenerComponent COMPLETED."));
			}
#endif
		}
		else
		{
			UE_LOG(LaunchDarklyClient, Log, TEXT(">>> FLaunchDarklyImpl (ANDROID) Logger: UnregisterStringFlagListenerComponent COMPLETED. %d listeners remain."), FlagListenerArray.Num());
		}
	}
}

void FLaunchDarklyImpl::Track(FString MetricName)
{
#if PLATFORM_ANDROID
	if(JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		jstring JavaMetricName = Env->NewStringUTF(TCHAR_TO_UTF8(*MetricName));
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidLdSdkInterface_Track, JavaMetricName);
	}
#endif
}

void FLaunchDarklyImpl::Track(FString MetricName, TSharedPtr<FJsonObject> const Data)
{
#if PLATFORM_ANDROID
	if(JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		jstring JavaMetricName = Env->NewStringUTF(TCHAR_TO_UTF8(*MetricName));
		FString DataAsFString = ULaunchDarklyHelpers::JsonObjectToString(Data);
		jstring DataAsJString = Env->NewStringUTF(TCHAR_TO_UTF8(*DataAsFString));
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidLdSdkInterface_Track, JavaMetricName, DataAsJString);
	}
#endif
}

#if PLATFORM_ANDROID
extern "C" void Java_com_epicgames_ue4_GameActivity_nativeBoolVariationCallback(JNIEnv * Env, jobject Thiz, jstring JFlagName, jboolean JFlagValue)
{
	FString FlagName = FString(Env->GetStringUTFChars(JFlagName, 0));
	TMap<FString, TArray<ULdBoolFlagListener*>>& BoolFlagListeners = FLaunchDarklyClientModule::Get()->GetBoolFlagListeners();
	if(BoolFlagListeners.Contains(FlagName))
	{
		bool FlagValue = JFlagValue == JNI_TRUE;
		TArray<ULdBoolFlagListener*>& Listeners = BoolFlagListeners[FlagName];
		AsyncTask(ENamedThreads::GameThread, [&Listeners, FlagValue]()
			{
				for(ULdBoolFlagListener* Listener : Listeners)
				{
					Listener->ValueChanged(FlagValue);
				}
			}
		);
	}
}

extern "C" void Java_com_epicgames_ue4_GameActivity_nativeFloatVariationCallback(JNIEnv * Env, jobject Thiz, jstring JFlagName, jfloat JFlagValue)
{
	FString FlagName = FString(Env->GetStringUTFChars(JFlagName, 0));
	TMap<FString, TArray<ULdFloatFlagListener*>>& FloatFlagListeners = FLaunchDarklyClientModule::Get()->GetFloatFlagListeners();
	if(FloatFlagListeners.Contains(FlagName))
	{
		float FlagValue = (float)JFlagValue;
		TArray<ULdFloatFlagListener*>& Listeners = FloatFlagListeners[FlagName];
		AsyncTask(ENamedThreads::GameThread, [&Listeners, FlagValue]()
			{
				for(ULdFloatFlagListener* Listener : Listeners)
				{
					Listener->ValueChanged(FlagValue);
				}
			}
		);
	}
}

extern "C" void Java_com_epicgames_ue4_GameActivity_nativeIntVariationCallback(JNIEnv * Env, jobject Thiz, jstring JFlagName, jint JFlagValue)
{
	FString FlagName = FString(Env->GetStringUTFChars(JFlagName, 0));
	TMap<FString, TArray<ULdIntFlagListener*>>& IntFlagListeners = FLaunchDarklyClientModule::Get()->GetIntFlagListeners();
	if(IntFlagListeners.Contains(FlagName))
	{
		int FlagValue = (int)JFlagValue;
		TArray<ULdIntFlagListener*>& Listeners = IntFlagListeners[FlagName];
		AsyncTask(ENamedThreads::GameThread, [&Listeners, FlagValue]()
			{
				for(ULdIntFlagListener* Listener : Listeners)
				{
					Listener->ValueChanged(FlagValue);
				}
			}
		);
	}
}

extern "C" void Java_com_epicgames_ue4_GameActivity_nativeJsonVariationCallback(JNIEnv * Env, jobject Thiz, jstring JFlagName, jstring JFlagValue)
{
	FString FlagName = FString(Env->GetStringUTFChars(JFlagName, 0));
	TMap<FString, TArray<ULdJsonFlagListener*>>& JsonFlagListeners = FLaunchDarklyClientModule::Get()->GetJsonFlagListeners();
	if(JsonFlagListeners.Contains(FlagName))
	{
		FString FlagValueAsString = FString(Env->GetStringUTFChars(JFlagValue, 0));
		TArray<ULdJsonFlagListener*>& Listeners = JsonFlagListeners[FlagName];
		AsyncTask(ENamedThreads::GameThread, [&Listeners, FlagValueAsString]()
			{
				TSharedPtr<FJsonObject> FlagJsonData = ULaunchDarklyHelpers::StringToJsonObject(FlagValueAsString);
				ULdNodeObject* FlagValue = NewObject<ULdNodeObject>();
				FlagValue->Initialize(FlagJsonData);
				for(ULdJsonFlagListener* Listener : Listeners)
				{
					Listener->ValueChanged(FlagValue);
				}
			}
		);
	}
}

extern "C" void Java_com_epicgames_ue4_GameActivity_nativeStringVariationCallback(JNIEnv * Env, jobject Thiz, jstring JFlagName, jstring JFlagValue)
{
	FString FlagName = FString(Env->GetStringUTFChars(JFlagName, 0));
	TMap<FString, TArray<ULdStringFlagListener*>>& StringFlagListeners = FLaunchDarklyClientModule::Get()->GetStringFlagListeners();
	if(StringFlagListeners.Contains(FlagName))
	{
		FString FlagValue = FString(Env->GetStringUTFChars(JFlagValue, 0));
		TArray<ULdStringFlagListener*>& Listeners = StringFlagListeners[FlagName];
		AsyncTask(ENamedThreads::GameThread, [&Listeners, FlagValue]()
			{
				for(ULdStringFlagListener* Listener : Listeners)
				{
					Listener->ValueChanged(FlagValue);
				}
			}
		);
	}
}


extern "C" void Java_com_epicgames_ue4_GameActivity_nativeBoolVariationComponentCallback(JNIEnv * Env, jobject Thiz, jstring JFlagName, jboolean JFlagValue)
{
	FString FlagName = FString(Env->GetStringUTFChars(JFlagName, 0));
	TMap<FString, TArray<ULdBoolFlagListenerComponent*>>& BoolFlagListeners = FLaunchDarklyClientModule::Get()->GetBoolFlagListenerComponents();
	if(BoolFlagListeners.Contains(FlagName))
	{
		bool FlagValue = JFlagValue == JNI_TRUE;
		TArray<ULdBoolFlagListenerComponent*>& Listeners = BoolFlagListeners[FlagName];
		AsyncTask(ENamedThreads::GameThread, [&Listeners, FlagValue]()
			{
				for(ULdBoolFlagListenerComponent* Listener : Listeners)
				{
					Listener->ValueChanged(FlagValue);
				}
			}
		);
	}
}

extern "C" void Java_com_epicgames_ue4_GameActivity_nativeFloatVariationComponentCallback(JNIEnv * Env, jobject Thiz, jstring JFlagName, jfloat JFlagValue)
{
	FString FlagName = FString(Env->GetStringUTFChars(JFlagName, 0));
	TMap<FString, TArray<ULdFloatFlagListenerComponent*>>& FloatFlagListeners = FLaunchDarklyClientModule::Get()->GetFloatFlagListenerComponents();
	if(FloatFlagListeners.Contains(FlagName))
	{
		float FlagValue = (float)JFlagValue;
		TArray<ULdFloatFlagListenerComponent*>& Listeners = FloatFlagListeners[FlagName];
		AsyncTask(ENamedThreads::GameThread, [&Listeners, FlagValue]()
			{
				for(ULdFloatFlagListenerComponent* Listener : Listeners)
				{
					Listener->ValueChanged(FlagValue);
				}
			}
		);
	}
}

extern "C" void Java_com_epicgames_ue4_GameActivity_nativeIntVariationComponentCallback(JNIEnv * Env, jobject Thiz, jstring JFlagName, jint JFlagValue)
{
	FString FlagName = FString(Env->GetStringUTFChars(JFlagName, 0));
	TMap<FString, TArray<ULdIntFlagListenerComponent*>>& IntFlagListeners = FLaunchDarklyClientModule::Get()->GetIntFlagListenerComponents();
	if(IntFlagListeners.Contains(FlagName))
	{
		int FlagValue = (int)JFlagValue;
		TArray<ULdIntFlagListenerComponent*>& Listeners = IntFlagListeners[FlagName];
		AsyncTask(ENamedThreads::GameThread, [&Listeners, FlagValue]()
			{
				for(ULdIntFlagListenerComponent* Listener : Listeners)
				{
					Listener->ValueChanged(FlagValue);
				}
			}
		);
	}
}

extern "C" void Java_com_epicgames_ue4_GameActivity_nativeJsonVariationComponentCallback(JNIEnv * Env, jobject Thiz, jstring JFlagName, jstring JFlagValue)
{
	FString FlagName = FString(Env->GetStringUTFChars(JFlagName, 0));
	TMap<FString, TArray<ULdJsonFlagListenerComponent*>>& JsonFlagListeners = FLaunchDarklyClientModule::Get()->GetJsonFlagListenerComponents();
	if(JsonFlagListeners.Contains(FlagName))
	{
		FString FlagValueAsString = FString(Env->GetStringUTFChars(JFlagValue, 0));
		TArray<ULdJsonFlagListenerComponent*>& Listeners = JsonFlagListeners[FlagName];
		AsyncTask(ENamedThreads::GameThread, [&Listeners, FlagValueAsString]()
			{
				TSharedPtr<FJsonObject> FlagJsonData = ULaunchDarklyHelpers::StringToJsonObject(FlagValueAsString);
				ULdNodeObject* FlagValue = NewObject<ULdNodeObject>();
				FlagValue->Initialize(FlagJsonData);
				for(ULdJsonFlagListenerComponent* Listener : Listeners)
				{
					Listener->ValueChanged(FlagValue);
				}
			}
		);
	}
}

extern "C" void Java_com_epicgames_ue4_GameActivity_nativeStringVariationComponentCallback(JNIEnv * Env, jobject Thiz, jstring JFlagName, jstring JFlagValue)
{
	FString FlagName = FString(Env->GetStringUTFChars(JFlagName, 0));
	TMap<FString, TArray<ULdStringFlagListenerComponent*>>& StringFlagListeners = FLaunchDarklyClientModule::Get()->GetStringFlagListenerComponents();
	if(StringFlagListeners.Contains(FlagName))
	{
		FString FlagValue = FString(Env->GetStringUTFChars(JFlagValue, 0));
		TArray<ULdStringFlagListenerComponent*>& Listeners = StringFlagListeners[FlagName];
		AsyncTask(ENamedThreads::GameThread, [&Listeners, FlagValue]()
			{
				for(ULdStringFlagListenerComponent* Listener : Listeners)
				{
					Listener->ValueChanged(FlagValue);
				}
			}
		);
	}
}



#endif
