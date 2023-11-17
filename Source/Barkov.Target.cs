// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class BarkovTarget : TargetRules
{
	public BarkovTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("Barkov");
		ExtraModuleNames.AddRange(new string[] {"Slate", "SlateCore", "UMG"});
		
	}
}
