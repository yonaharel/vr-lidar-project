using UnrealBuildTool;

public class new_vrTarget : TargetRules
{
	public new_vrTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		ExtraModuleNames.Add("new_vr");
	}
}
