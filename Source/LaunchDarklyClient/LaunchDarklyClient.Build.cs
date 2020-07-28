// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class LaunchDarklyClient : ModuleRules
{
	public LaunchDarklyClient(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);

		PublicIncludePaths.Add("LaunchDarklyClient/Public");
		PublicIncludePaths.Add("LaunchDarklyClient/Classes");
		PrivateIncludePaths.Add("LaunchDarklyClient/Private");

		PrivateIncludePathModuleNames.Add("Settings");

		PublicDependencyModuleNames.AddRange(new string[] { "Core" });
		PrivateDependencyModuleNames.AddRange(new string[] { "CoreUObject", "Engine", "Http", "Json", "Slate", "SlateCore" });

		string LdSdkDirectory = ModuleDirectory + "/../../ThirdParty/LaunchDarkly";
		string LdIncludeDirectory = LdSdkDirectory + "/include";
		PublicIncludePaths.Add(LdIncludeDirectory);

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PublicDefinitions.Add("WITH_LAUNCHDARKLYCLIENT=1");

			string LdDllDestination = ModuleDirectory + "/../../Binaries/Win64";
			string LdBinaryDirectory = LdSdkDirectory + "/lib/x64";
			string LdDllName = "/ldclientapi.dll";
			string LdLibName = "/ldclientapi.lib";

			PublicAdditionalLibraries.Add(LdBinaryDirectory + LdLibName);
			PrivateIncludePaths.Add("LaunchDarklyClient/Private/Windows");

			CopyFile(LdBinaryDirectory, LdDllDestination, LdDllName);
		}
		else if (Target.Platform == UnrealTargetPlatform.Android)
		{
			PublicDefinitions.Add("WITH_LAUNCHDARKLYCLIENT=1");

			PrivateDependencyModuleNames.Add("Launch");
			PrivateIncludePaths.Add("LaunchDarklyClient/Private/Android");

			AdditionalPropertiesForReceipt.Add(new ReceiptProperty("AndroidPlugin", Path.Combine(PluginPath, "LaunchDarklyClientAndroidAPI_APL.xml")));
		}
		else
		{
			PublicDefinitions.Add("WITH_LAUNCHDARKLYCLIENT=0");
		}
	}

	private void CopyFile(string sourceDir, string destDir, string filename)
	{
		string source = sourceDir + filename;
		string dest = destDir + filename;

		System.Console.WriteLine("Copying {0} to {1}", source, dest);

		if (!System.IO.File.Exists(destDir))
		{
			System.IO.Directory.CreateDirectory(destDir);
		}

		if (System.IO.File.Exists(dest))
		{
			System.IO.File.SetAttributes(dest, System.IO.File.GetAttributes(dest) & ~System.IO.FileAttributes.ReadOnly);
		}

		try
		{
			System.IO.File.Copy(source, dest, true);
		}
		catch (System.Exception ex)
		{
			System.Console.WriteLine("Failed to copy file: {0}", ex.Message);
		}
	}
}
