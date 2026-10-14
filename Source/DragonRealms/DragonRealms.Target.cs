// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class DragonRealmsEditorTarget : TargetRules
{
    public DragonRealmsEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V4;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
        bLegacyEngineType = false;
        ExtraModuleNames.Add("DragonRealms");
    }
}

