// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class DragonRealmsTarget : TargetRules
{
    public DragonRealmsTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V4;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
        bLegacyEngineType = false;
        ExtraModuleNames.Add("DragonRealms");
    }
}
