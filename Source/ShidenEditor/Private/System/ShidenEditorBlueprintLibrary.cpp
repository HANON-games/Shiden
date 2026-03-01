// Copyright (c) 2026 HANON. All Rights Reserved.

#include "System/ShidenEditorBlueprintLibrary.h"
#include "System/ShidenStructuredLog.h"
#include "AssetViewUtils.h"
#include "IContentBrowserSingleton.h"
#include "ContentBrowserModule.h"
#include "DesktopPlatformModule.h"
#include "IContentBrowserDataModule.h"
#include "IDesktopPlatform.h"
#include "ISettingsModule.h"
#include "Internationalization/Regex.h"
#include "JsonObjectConverter.h"
#include "Command/ShidenCommandDefinition.h"
#include "System/ShidenCommandRedirector.h"
#include "Config/ShidenEditorConfig.h"
#include "Config/ShidenEditorUserConfig.h"
#include "System/ShidenEditorConstants.h"
#include "Command/ShidenStandardCommandDefinitions.h"
#include "Config/ShidenProjectConfig.h"
#include "Expression/ShidenExpressionEvaluator.h"
#include "Interfaces/IMainFrameModule.h"
#include "Save/ShidenSaveBlueprintLibrary.h"
#include "Scenario/ShidenScenario.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"
#include "Serialization/Csv/CsvParser.h"
#include "Subsystems/EditorAssetSubsystem.h"
#include "System/ShidenBlueprintLibrary.h"
#include "System/ShidenSubsystem.h"
#include "UI/ShidenSplitterBox.h"
#include "Variable/ShidenVariableBlueprintLibrary.h"

#define LOCTEXT_NAMESPACE "AssetTools"

namespace
{
	bool FixDuplicateCommandIds(TArray<FShidenCommand>& Commands, const FString& ContextLabel)
	{
		if (Commands.Num() == 0)
		{
			return false;
		}

		TSet<FGuid> SeenGuids;
		bool bFoundDuplicate = false;

		for (FShidenCommand& Command : Commands)
		{
			// Check if CommandId is missing or invalid
			if (!Command.CommandId.IsValid())
			{
				Command.CommandId = FGuid::NewGuid();
				SHIDEN_VERBOSE("Generated new CommandId for command with missing/invalid GUID in {context}: {guid}",
				               *ContextLabel, *Command.CommandId.ToString());
				bFoundDuplicate = true;
				SeenGuids.Add(Command.CommandId);
				continue;
			}

			// Check for duplicate
			if (SeenGuids.Contains(Command.CommandId))
			{
				const FGuid OldGuid = Command.CommandId;
				Command.CommandId = FGuid::NewGuid();
				SHIDEN_VERBOSE("Duplicate CommandId detected in {context}, regenerated from {oldGuid} to {newGuid}",
				               *ContextLabel, *OldGuid.ToString(), *Command.CommandId.ToString());
				bFoundDuplicate = true;
			}

			SeenGuids.Add(Command.CommandId);
		}

		return bFoundDuplicate;
	}
}

void UShidenEditorBlueprintLibrary::ShowSaveAssetDialog(const UClass* AssetClass, const FString& DefaultPath,
                                                        const FString& DefaultAssetName, FString& SavePackagePath,
                                                        FString& SaveAssetName, bool& bAssetNameWasSet)
{
	FSaveAssetDialogConfig DialogConfig;
	DialogConfig.DialogTitleOverride = ShidenEditorConstants::SaveAssetDialogTitle;
	DialogConfig.DefaultPath = DefaultPath;
	DialogConfig.DefaultAssetName = DefaultAssetName;
	DialogConfig.ExistingAssetPolicy = ESaveAssetDialogExistingAssetPolicy::AllowButWarn;
	DialogConfig.AssetClassNames.Add(AssetClass->GetClassPathName());

	const FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(ShidenEditorConstants::ContentBrowserModuleName);
	const FString SaveObjectPath = ContentBrowserModule.Get().CreateModalSaveAssetDialog(DialogConfig);

	const FString SavePackageName = FPackageName::ObjectPathToPackageName(SaveObjectPath);
	SavePackagePath = FPaths::GetPath(SavePackageName);
	SaveAssetName = FPaths::GetBaseFilename(SavePackageName);
	bAssetNameWasSet = SaveAssetName.Len() > 0;
}

bool UShidenEditorBlueprintLibrary::CanCreateFolder(const FName& Path)
{
	const TObjectPtr<UContentBrowserDataSubsystem> ContentBrowserData = IContentBrowserDataModule::Get().GetSubsystem();
	return ContentBrowserData->CanCreateFolder(Path, nullptr);
}

TArray<FString> UShidenEditorBlueprintLibrary::CreateSortedStringArray(const TArray<FString>& Array)
{
	TArray<FString> OutArray = Array;
	OutArray.Sort();
	return OutArray;
}

bool UShidenEditorBlueprintLibrary::TryLoadScenarioTextFile(const FString& Extension, FString& FileData, FString& FileName)
{
	const void* WindowHandle = nullptr;
	if (FModuleManager::Get().IsModuleLoaded("MainFrame"))
	{
		const IMainFrameModule& MainFrameModule = FModuleManager::LoadModuleChecked<IMainFrameModule>(ShidenEditorConstants::MainFrameModuleName);
		const TSharedPtr<SWindow> MainWindow = MainFrameModule.GetParentWindow();
		if (MainWindow.IsValid() && MainWindow->GetNativeWindow().IsValid())
		{
			WindowHandle = MainWindow->GetNativeWindow()->GetOSWindowHandle();
		}
	}

	if (!WindowHandle)
	{
		return false;
	}

	TArray<FString> FilePath = {};

	if (IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get())
	{
		const bool bSuccess = DesktopPlatform->OpenFileDialog(
			WindowHandle,
			ShidenEditorConstants::OpenFileDialogTitle.ToString(),
			TEXT(""),
			TEXT(""),
			TEXT("Scenario ") + Extension + TEXT(" (*.") + Extension + TEXT(")|*.") + Extension,
			EFileDialogFlags::Type::None,
			FilePath
		);

		if (!bSuccess)
		{
			return false;
		}

		if (GEngine)
		{
			const FString LoadFilePath = FPaths::ConvertRelativePathToFull(FilePath[0]);

			if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*LoadFilePath))
			{
				return false;
			}
			FileName = FPaths::GetCleanFilename(LoadFilePath);
			return FFileHelper::LoadFileToString(FileData, *LoadFilePath);
		}
	}

	return false;
}

bool UShidenEditorBlueprintLibrary::TrySaveTextFile(const FString& DefaultFileName, const FString& SaveText, const FString& Extension)
{
	const void* WindowHandle = nullptr;
	if (FModuleManager::Get().IsModuleLoaded("MainFrame"))
	{
		const IMainFrameModule& MainFrameModule = FModuleManager::LoadModuleChecked<IMainFrameModule>(ShidenEditorConstants::MainFrameModuleName);
		const TSharedPtr<SWindow> MainWindow = MainFrameModule.GetParentWindow();
		if (MainWindow.IsValid() && MainWindow->GetNativeWindow().IsValid())
		{
			WindowHandle = MainWindow->GetNativeWindow()->GetOSWindowHandle();
		}
	}

	if (!WindowHandle)
	{
		return false;
	}

	if (IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get())
	{
		TArray<FString> FilePath;

		const bool bSuccess = DesktopPlatform->SaveFileDialog(
			WindowHandle,
			ShidenEditorConstants::SaveFileDialogTitle.ToString(),
			TEXT(""),
			DefaultFileName + TEXT("." + Extension),
			TEXT("Scenario ") + Extension + TEXT(" (*.") + Extension + TEXT(")|*.") + Extension,
			EFileDialogFlags::Type::None,
			FilePath
		);

		if (!bSuccess)
		{
			return false;
		}

		return FFileHelper::SaveStringToFile(SaveText, *FPaths::ConvertRelativePathToFull(FilePath[0]),
		                                     FFileHelper::EEncodingOptions::ForceUTF8);
	}

	return false;
}

void UShidenEditorBlueprintLibrary::ParseCsvContent(const FString& CsvText, TArray<FShidenCsvParsedRow>& CsvParsedRow)
{
	const FCsvParser Parser(CsvText);
	const TArray<TArray<const TCHAR*>>& Rows = Parser.GetRows();
	bool bCommentEnd = false;

	for (const TArray<const TCHAR*>& Row : Rows)
	{
		// Skip empty rows
		if (Row.Num() == 0)
		{
			continue;
		}

		// Skip comment lines like "# comment"
		if (!bCommentEnd && FString(Row[0]).TrimStart().TrimEnd().StartsWith("#"))
		{
			continue;
		}

		bCommentEnd = true;

		FShidenCsvParsedRow ParsedRow;
		for (const TCHAR* Cell : Row)
		{
			ParsedRow.Row.Add(FString(Cell));
		}
		CsvParsedRow.Add(ParsedRow);
	}
}

void UShidenEditorBlueprintLibrary::SetDefaultClassProperty(const UClass* TargetClass, const FName& PropertyName, UClass* Value)
{
	if (!TargetClass)
	{
		SHIDEN_WARNING("SetDefaultClassProperty: TargetClass is null (PropertyName: {name})", *PropertyName.ToString());
		return;
	}
	FProperty* Prop = TargetClass->FindPropertyByName(PropertyName);
	const FClassProperty* ClassProperty = CastField<FClassProperty>(Prop);
	if (!ClassProperty)
	{
		SHIDEN_WARNING("SetDefaultClassProperty: Property '{propertyName}' not found or is not a ClassProperty in {className}", *PropertyName.ToString(), *TargetClass->GetName());
		return;
	}
	void* ValuePtr = ClassProperty->ContainerPtrToValuePtr<void>(TargetClass->GetDefaultObject());
	ClassProperty->SetObjectPropertyValue(ValuePtr, Value);
}

FString EscapeCsvItem(const FString& Item)
{
	FString EscapedItem = Item;
	if (EscapedItem.Contains(TEXT(",")) || EscapedItem.Contains(TEXT("\"")) || EscapedItem.Contains(TEXT("\n")))
	{
		EscapedItem = TEXT("\"") + EscapedItem.Replace(TEXT("\""), TEXT("\"\"")) + TEXT("\"");
	}
	return EscapedItem;
}

FShidenCommand UShidenEditorBlueprintLibrary::ExpandCommandForEditor(const FShidenCommand& SourceCommand, const bool bShouldExpandPreset, const bool bShouldExpandDefaultValue)
{
	if (!bShouldExpandPreset && bShouldExpandDefaultValue)
	{
		SHIDEN_ERROR("bShouldExpandDefaultValue=true requires bShouldExpandPreset=true when the command has a preset; default values would not be expanded for preset commands.");
		return FShidenCommand();
	}

	const TObjectPtr<const UShidenProjectConfig> Config = GetDefault<UShidenProjectConfig>();
	const TMap<FString, FShidenCommandDefinition>& CommandDefinitions = UShidenBlueprintLibrary::GetCommandDefinitionsCache();

	FShidenCommand ExpandedCommand;
	ExpandedCommand.CommandId = SourceCommand.CommandId;
	ExpandedCommand.bEnabled = SourceCommand.bEnabled;
	ExpandedCommand.CommandName = SourceCommand.CommandName;
	ExpandedCommand.PresetName = SourceCommand.PresetName;

	if (SourceCommand.PresetName.IsEmpty())
	{
		if (!bShouldExpandDefaultValue)
		{
			ExpandedCommand.Args = SourceCommand.Args;
			return ExpandedCommand;
		}

		const FShidenCommandDefinition* DefaultCommandDef = CommandDefinitions.Find(SourceCommand.CommandName);
		if (!DefaultCommandDef)
		{
			ExpandedCommand.Args = SourceCommand.Args;
			return ExpandedCommand;
		}

		for (const FShidenCommandArgument& Arg : DefaultCommandDef->Args)
		{
			FString Key = Arg.ArgName.ToString();
			if (!SourceCommand.Args.Contains(Key) && !Arg.DefaultValue.IsSet())
			{
				continue;
			}
			const FString Value = SourceCommand.Args.Contains(Key)
				                      ? SourceCommand.Args.FindRef(Key)
				                      : Arg.DefaultValue.GetValue();
			ExpandedCommand.Args.Add(Key, Value);

			// If the HasAdditionalArgs Property of CommandArguments[Index] is true, add MacroArguments
			if (Arg.EditorSettings.TemplateParameters.FindRef(TEXT("HasAdditionalArgs")).Compare(TEXT("true"), ESearchCase::IgnoreCase) == 0)
			{
				// load scenario
				UShidenScenario* MacroScenario;
				FGuid ScenarioId;
				if (UShidenScenarioBlueprintLibrary::TryGetScenarioByIdOrObjectPath(Value, ScenarioId, MacroScenario))
				{
					for (const FShidenMacroParameter& MacroParam : MacroScenario->MacroParameterDefinitions)
					{
						const FString MacroValue = SourceCommand.Args.Contains(MacroParam.Name)
							                           ? SourceCommand.Args.FindRef(MacroParam.Name)
							                           : MacroParam.DefaultValue;
						ExpandedCommand.Args.Add(MacroParam.Name, MacroValue);
					}
				}
			}
		}

		return ExpandedCommand;
	}

	if (!bShouldExpandPreset)
	{
		ExpandedCommand.Args = SourceCommand.Args;
		return ExpandedCommand;
	}

	const FShidenPreset* CommandPreset = Config->Presets.Find(SourceCommand.PresetName);
	if (!CommandPreset || CommandPreset->CommandName != SourceCommand.CommandName)
	{
		ExpandedCommand.Args = SourceCommand.Args;
		return ExpandedCommand;
	}

	const FShidenCommandDefinition* PresetCommandDef = CommandDefinitions.Find(SourceCommand.CommandName);
	if (!PresetCommandDef)
	{
		ExpandedCommand.Args = SourceCommand.Args;
		return ExpandedCommand;
	}

	for (const FShidenCommandArgument& Arg : PresetCommandDef->Args)
	{
		FString Key = Arg.ArgName.ToString();

		// Determine the value from source, preset, or default (in priority order)
		FString Value;
		if (SourceCommand.Args.Contains(Key))
		{
			Value = SourceCommand.Args.FindRef(Key);
		}
		else if (CommandPreset->Args.Contains(Key))
		{
			Value = CommandPreset->Args.FindRef(Key);
		}
		else if (bShouldExpandDefaultValue && Arg.DefaultValue.IsSet())
		{
			Value = Arg.DefaultValue.GetValue();
		}
		else
		{
			// Neither source, preset, nor default defines this arg; skip it to preserve null semantics
			continue;
		}

		ExpandedCommand.Args.Add(Key, Value);

		// If the HasAdditionalArgs Property of CommandArguments[Index] is true, add MacroArguments
		if (Arg.EditorSettings.TemplateParameters.FindRef(TEXT("HasAdditionalArgs")).Compare(TEXT("true"), ESearchCase::IgnoreCase) == 0)
		{
			// load scenario
			UShidenScenario* MacroScenario;
			FGuid ScenarioId;
			if (UShidenScenarioBlueprintLibrary::TryGetScenarioByIdOrObjectPath(Value, ScenarioId, MacroScenario))
			{
				for (const FShidenMacroParameter& MacroParam : MacroScenario->MacroParameterDefinitions)
				{
					const FString MacroValue = SourceCommand.Args.Contains(MacroParam.Name)
						                           ? SourceCommand.Args.FindRef(MacroParam.Name)
						                           : MacroParam.DefaultValue;
					ExpandedCommand.Args.Add(MacroParam.Name, MacroValue);
				}
			}
		}
	}

	return ExpandedCommand;
}

UShidenScenario* ExpandPresets(const UShidenScenario* SourceScenario)
{
	const TObjectPtr<UShidenScenario> ExpandedScenario = NewObject<UShidenScenario>();
	ExpandedScenario->ScenarioId = SourceScenario->ScenarioId;
	ExpandedScenario->Note = SourceScenario->Note;
	ExpandedScenario->MacroParameterDefinitions = SourceScenario->MacroParameterDefinitions;
	ExpandedScenario->LocalVariableDefinitions = SourceScenario->LocalVariableDefinitions;
	ExpandedScenario->WidgetClassOverride = SourceScenario->WidgetClassOverride;
	for (const FShidenCommand& Command : SourceScenario->Commands)
	{
		ExpandedScenario->Commands.Add(UShidenEditorBlueprintLibrary::ExpandCommandForEditor(Command, true));
	}
	return ExpandedScenario;
}

FShidenCommand UShidenEditorBlueprintLibrary::RemoveRedundantCommandArgs(const FShidenCommand& SourceCommand)
{
	const TObjectPtr<const UShidenProjectConfig> Config = GetDefault<UShidenProjectConfig>();
	const TMap<FString, FShidenCommandDefinition>& CommandDefinitions = UShidenBlueprintLibrary::GetCommandDefinitionsCache();

	FShidenCommand Result = SourceCommand;

	const FShidenPreset* CommandPreset = SourceCommand.PresetName.IsEmpty()
		                                     ? nullptr
		                                     : Config->Presets.Find(SourceCommand.PresetName);
	if (CommandPreset && CommandPreset->CommandName != SourceCommand.CommandName)
	{
		CommandPreset = nullptr;
	}

	// Build default value map from command definition
	TMap<FString, FString> DefaultValues;
	if (const FShidenCommandDefinition* CommandDef = CommandDefinitions.Find(SourceCommand.CommandName))
	{
		for (const FShidenCommandArgument& ArgDef : CommandDef->Args)
		{
			if (ArgDef.DefaultValue.IsSet())
			{
				DefaultValues.Add(ArgDef.ArgName.ToString(), ArgDef.DefaultValue.GetValue());
			}

			// For args with HasAdditionalArgs (e.g., RunMacro), also include macro parameter defaults
			if (ArgDef.EditorSettings.TemplateParameters.FindRef(TEXT("HasAdditionalArgs")).Compare(TEXT("true"), ESearchCase::IgnoreCase) == 0)
			{
				// Resolve the macro scenario value from user args, preset, or default
				FString MacroScenarioValue = SourceCommand.Args.FindRef(ArgDef.ArgName.ToString());
				if (MacroScenarioValue.IsEmpty())
				{
					if (CommandPreset)
					{
						MacroScenarioValue = CommandPreset->Args.FindRef(ArgDef.ArgName.ToString());
					}
					else if (ArgDef.DefaultValue.IsSet())
					{
						MacroScenarioValue = ArgDef.DefaultValue.GetValue();
					}
				}

				if (MacroScenarioValue == TEXT("None"))
				{
					continue;
				}

				FGuid MacroScenarioId;
				UShidenScenario* MacroScenario;
				if (UShidenScenarioBlueprintLibrary::TryGetScenarioByIdOrObjectPath(MacroScenarioValue, MacroScenarioId, MacroScenario))
				{
					for (const FShidenMacroParameter& MacroParam : MacroScenario->MacroParameterDefinitions)
					{
						DefaultValues.Add(MacroParam.Name, MacroParam.DefaultValue);
					}
				}
			}
		}
	}

	// Remove args whose value matches the effective fallback (preset takes priority over default).
	// If a preset defines the key, only compare against the preset value — even if the user value
	// is empty, it must be kept when it differs from the preset.
	TArray<FString> KeysToRemove;
	for (const auto& [Key, Value] : Result.Args)
	{
		if (CommandPreset && CommandPreset->Args.Contains(Key))
		{
			// Preset defines this key: redundant only when value matches the preset
			if (Value == CommandPreset->Args[Key])
			{
				KeysToRemove.Add(Key);
			}
		}
		else if (DefaultValues.Contains(Key) && Value == DefaultValues[Key])
		{
			// No preset for this key: redundant only when value matches the command default
			KeysToRemove.Add(Key);
		}
	}
	for (const FString& Key : KeysToRemove)
	{
		Result.Args.Remove(Key);
	}

	return Result;
}

UShidenScenario* RemoveRedundantValues(const UShidenScenario* SourceScenario)
{
	const FGuid ScenarioId = SourceScenario->ScenarioId;
	const TObjectPtr<UShidenScenario> RemovedScenario = DuplicateObject(SourceScenario, GetTransientPackage());
	// Scenario ID was lost during duplication, so restore it.
	RemovedScenario->ScenarioId = ScenarioId;
	RemovedScenario->Commands.Empty();
	for (const FShidenCommand& Command : SourceScenario->Commands)
	{
		RemovedScenario->Commands.Add(UShidenEditorBlueprintLibrary::RemoveRedundantCommandArgs(Command));
	}
	return RemovedScenario;
}

UShidenScenario* UShidenEditorBlueprintLibrary::ConvertToScenarioFromCsv(const FString& CsvString)
{
	auto GetCsvComments = [](const FString& Str)
	{
		TArray<FString> Lines;
		Str.ParseIntoArrayLines(Lines, true);
		TMap<FString, FString> Comments;
		for (const FString& Line : Lines)
		{
			if (Line.StartsWith("#"))
			{
				FString Trimmed = Line.RightChop(1).TrimStart().TrimEnd();
				FString Key, Value;
				if (Trimmed.Split(" ", &Key, &Value))
				{
					Comments.Add(Key, Value);
				}
				else
				{
					Comments.Add(Trimmed, TEXT(""));
				}
			}
		}
		return Comments;
	};

	TObjectPtr<UShidenScenario> Scenario = NewObject<UShidenScenario>();
	TMap<FString, FString> Comments = GetCsvComments(CsvString);

	// Parse comments
	FGuid ScenarioId;
	Scenario->ScenarioId = Comments.Contains(TEXT("ScenarioId")) && FGuid::Parse(Comments[TEXT("ScenarioId")], ScenarioId)
		                       ? ScenarioId
		                       : FGuid::NewGuid();
	Scenario->Note = Comments.Contains(TEXT("Note")) ? Comments[TEXT("Note")] : TEXT("");

	// Parse WidgetClassOverride
	if (Comments.Contains(TEXT("WidgetClassOverride")))
	{
		const FString& WidgetClassPath = Comments[TEXT("WidgetClassOverride")];
		if (!WidgetClassPath.IsEmpty())
		{
			if (TObjectPtr<UClass> WidgetClass = LoadObject<UClass>(nullptr, *WidgetClassPath))
			{
				Scenario->WidgetClassOverride = WidgetClass;
			}
		}
	}

	for (int32 Index = 1; Comments.Contains(TEXT("MacroParameter") + FString::FromInt(Index) + TEXT("Name")); Index++)
	{
		FShidenMacroParameter MacroParameterDefinition;
		MacroParameterDefinition.Name = Comments[TEXT("MacroParameter") + FString::FromInt(Index) + TEXT("Name")];
		FString TypeStr = Comments[TEXT("MacroParameter") + FString::FromInt(Index) + TEXT("Type")];
		MacroParameterDefinition.Type = static_cast<EShidenVariableType>(StaticEnum<EShidenVariableType>()->GetValueByNameString(TypeStr));
		MacroParameterDefinition.DefaultValue = Comments[TEXT("MacroParameter") + FString::FromInt(Index) + TEXT("DefaultValue")];
		MacroParameterDefinition.AssetPathType = MacroParameterDefinition.Type == EShidenVariableType::AssetPath
			                                         ? EShidenAssetPathType::Any
			                                         : EShidenAssetPathType::None;
		const FString AssetPathTypeKey = TEXT("MacroParameter") + FString::FromInt(Index) + TEXT("AssetPathType");

		if (Comments.Contains(AssetPathTypeKey))
		{
			MacroParameterDefinition.AssetPathType = static_cast<EShidenAssetPathType>(StaticEnum<EShidenAssetPathType>()->GetValueByNameString(Comments[AssetPathTypeKey]));
		}

		const FString IsReadOnlyKey = TEXT("MacroParameter") + FString::FromInt(Index) + TEXT("IsReadOnly");
		MacroParameterDefinition.bIsReadOnly = Comments.Contains(IsReadOnlyKey) && Comments[IsReadOnlyKey] == TEXT("true");

		const FString ToolTipKey = TEXT("MacroParameter") + FString::FromInt(Index) + TEXT("ToolTip");
		if (Comments.Contains(ToolTipKey))
		{
			MacroParameterDefinition.EditorSettings.ToolTip = FText::FromString(Comments[ToolTipKey]);
		}

		const FString VisibilityConditionKey = TEXT("MacroParameter") + FString::FromInt(Index) + TEXT("VisibilityCondition");
		if (Comments.Contains(VisibilityConditionKey))
		{
			MacroParameterDefinition.EditorSettings.VisibilityCondition = Comments[VisibilityConditionKey];
		}

		for (int32 MsgIndex = 1; Comments.Contains(TEXT("MacroParameter") + FString::FromInt(Index) + TEXT("InformationMessage") + FString::FromInt(MsgIndex) + TEXT("Condition")); MsgIndex++)
		{
			FShidenConditionalMessage Msg;
			Msg.Condition = Comments[TEXT("MacroParameter") + FString::FromInt(Index) + TEXT("InformationMessage") + FString::FromInt(MsgIndex) + TEXT("Condition")];
			Msg.Message = FText::FromString(Comments[TEXT("MacroParameter") + FString::FromInt(Index) + TEXT("InformationMessage") + FString::FromInt(MsgIndex) + TEXT("Message")]);
			MacroParameterDefinition.EditorSettings.InformationMessages.Add(Msg);
		}

		for (int32 MsgIndex = 1; Comments.Contains(TEXT("MacroParameter") + FString::FromInt(Index) + TEXT("WarningMessage") + FString::FromInt(MsgIndex) + TEXT("Condition")); MsgIndex++)
		{
			FShidenConditionalMessage Msg;
			Msg.Condition = Comments[TEXT("MacroParameter") + FString::FromInt(Index) + TEXT("WarningMessage") + FString::FromInt(MsgIndex) + TEXT("Condition")];
			Msg.Message = FText::FromString(Comments[TEXT("MacroParameter") + FString::FromInt(Index) + TEXT("WarningMessage") + FString::FromInt(MsgIndex) + TEXT("Message")]);
			MacroParameterDefinition.EditorSettings.WarningMessages.Add(Msg);
		}

		for (int32 MsgIndex = 1; Comments.Contains(TEXT("MacroParameter") + FString::FromInt(Index) + TEXT("ErrorMessage") + FString::FromInt(MsgIndex) + TEXT("Condition")); MsgIndex++)
		{
			FShidenConditionalMessage Msg;
			Msg.Condition = Comments[TEXT("MacroParameter") + FString::FromInt(Index) + TEXT("ErrorMessage") + FString::FromInt(MsgIndex) + TEXT("Condition")];
			Msg.Message = FText::FromString(Comments[TEXT("MacroParameter") + FString::FromInt(Index) + TEXT("ErrorMessage") + FString::FromInt(MsgIndex) + TEXT("Message")]);
			MacroParameterDefinition.EditorSettings.ErrorMessages.Add(Msg);
		}

		for (int32 EnumIndex = 1; Comments.Contains(TEXT("MacroParameter") + FString::FromInt(Index) + TEXT("Enum") + FString::FromInt(EnumIndex));
		     EnumIndex++)
		{
			MacroParameterDefinition.bIsEnumParameter = true;
			MacroParameterDefinition.EnumValues.Add(
				Comments[TEXT("MacroParameter") + FString::FromInt(Index) + TEXT("Enum") + FString::FromInt(EnumIndex)]);
		}

		Scenario->MacroParameterDefinitions.Add(MacroParameterDefinition);
	}

	for (int32 Index = 1; Comments.Contains(TEXT("LocalVariable") + FString::FromInt(Index) + TEXT("Name")); Index++)
	{
		FShidenVariableDefinition LocalVariableDefinition;
		LocalVariableDefinition.Name = Comments[TEXT("LocalVariable") + FString::FromInt(Index) + TEXT("Name")];
		FString TypeStr = Comments[TEXT("LocalVariable") + FString::FromInt(Index) + TEXT("Type")];
		LocalVariableDefinition.Type = static_cast<EShidenVariableType>(StaticEnum<EShidenVariableType>()->GetValueByNameString(TypeStr));
		LocalVariableDefinition.DefaultValue = Comments[TEXT("LocalVariable") + FString::FromInt(Index) + TEXT("DefaultValue")];
		LocalVariableDefinition.AssetPathType = LocalVariableDefinition.Type == EShidenVariableType::AssetPath
			                                        ? EShidenAssetPathType::Any
			                                        : EShidenAssetPathType::None;
		const FString AssetPathTypeKey = TEXT("LocalVariable") + FString::FromInt(Index) + TEXT("AssetPathType");

		if (Comments.Contains(AssetPathTypeKey))
		{
			LocalVariableDefinition.AssetPathType = static_cast<EShidenAssetPathType>(StaticEnum<EShidenAssetPathType>()->GetValueByNameString(Comments[AssetPathTypeKey]));
		}

		const FString IsReadOnlyKey = TEXT("LocalVariable") + FString::FromInt(Index) + TEXT("IsReadOnly");
		LocalVariableDefinition.bIsReadOnly = Comments.Contains(IsReadOnlyKey) && Comments[IsReadOnlyKey] == TEXT("true");

		Scenario->LocalVariableDefinitions.Add(LocalVariableDefinition);
	}

	// Parse rows
	const TMap<FString, FShidenCommandDefinition>& CommandDefinitions = UShidenBlueprintLibrary::GetCommandDefinitionsCache();
	TArray<FShidenCsvParsedRow> CsvParsedRow;
	ParseCsvContent(CsvString, CsvParsedRow);
	bool bIsHeader = true;
	for (const FShidenCsvParsedRow& ParsedRow : CsvParsedRow)
	{
		// Skip header
		if (bIsHeader)
		{
			bIsHeader = false;
			continue;
		}
		TArray<FString> Row = ParsedRow.Row;

		// Skip rows with insufficient columns (need at least: CommandId, bEnabled, CommandName, PresetName)
		if (Row.Num() < 4)
		{
			SHIDEN_WARNING("Skipping CSV row with insufficient columns (expected at least 4, got {num}).", Row.Num());
			continue;
		}

		FShidenCommand Command;
		FGuid CommandId;
		Command.CommandId = FGuid::Parse(Row[0], CommandId) ? CommandId : FGuid::NewGuid();
		Command.bEnabled = Row[1] == TEXT("true");
		Command.CommandName = Row[2];
		Command.PresetName = Row[3];

		if (!CommandDefinitions.Contains(Command.CommandName))
		{
			SHIDEN_WARNING("CommandName {name} is not found in CommandDefinitions.", *Command.CommandName);
			continue;
		}

		TArray<FShidenCommandArgument> Args = CommandDefinitions[Command.CommandName].Args;
		for (int32 Index = 0; Index < Args.Num(); Index++)
		{
			const FString CellValue = Row.IsValidIndex(Index + 4) ? Row[Index + 4] : TEXT("");
			// In CSV: empty cell = null (don't add to map), {EMPTY} = explicit empty string
			if (!CellValue.IsEmpty())
			{
				const FString Value = CellValue == TEXT("{EMPTY}") ? TEXT("") : CellValue;
				Command.Args.Add(Args[Index].ArgName.ToString(), Value);
			}

			// If the HasAdditionalArgs Property of Args[Index] is true, add MacroArguments
			if (Args[Index].EditorSettings.TemplateParameters.FindRef(TEXT("HasAdditionalArgs")).Compare(TEXT("true"), ESearchCase::IgnoreCase) == 0)
			{
				FString MacroScenarioValue = Command.GetArg(Args[Index].ArgName.ToString()).Get(TEXT(""));
				if (MacroScenarioValue.IsEmpty())
				{
					// Get scenario id or path from expanded command
					FShidenCommand TempCommand = ExpandCommandForEditor(Command, true, true);
					MacroScenarioValue = TempCommand.Args[Args[Index].ArgName.ToString()];
				}

				FGuid MacroScenarioId;
				UShidenScenario* MacroScenario;
				if (!UShidenScenarioBlueprintLibrary::TryGetScenarioByIdOrObjectPath(MacroScenarioValue, MacroScenarioId, MacroScenario))
				{
					continue;
				}

				for (const FShidenVariableDefinition& MacroParameterDefinition : MacroScenario->MacroParameterDefinitions)
				{
					Index++;
					const FString MacroCellValue = Row.IsValidIndex(Index + 4) ? Row[Index + 4] : TEXT("");
					// In CSV: empty cell = null (don't add to map), {EMPTY} = explicit empty string
					if (!MacroCellValue.IsEmpty())
					{
						const FString MacroValue = MacroCellValue == TEXT("{EMPTY}") ? TEXT("") : MacroCellValue;
						Command.Args.Add(MacroParameterDefinition.Name, MacroValue);
					}
				}
			}
		}

		Scenario->Commands.Add(Command);
	}

	FixDuplicateCommandIds(Scenario->Commands, TEXT("CSV import"));

	return RemoveRedundantValues(Scenario);
}

FString UShidenEditorBlueprintLibrary::ConvertToCsvFromScenario(const UShidenScenario* SourceScenario)
{
	const TObjectPtr<const UShidenEditorConfig> EditorConfig = GetDefault<UShidenEditorConfig>();
	const UShidenScenario* Scenario = EditorConfig->bExpandPresets ? ExpandPresets(SourceScenario) : SourceScenario;

	TArray<FString> CsvRows;

	// Add comments
	CsvRows.Add(TEXT("#ScenarioId ") + Scenario->ScenarioId.ToString());
	CsvRows.Add(TEXT("#Note ") + Scenario->Note);
	if (Scenario->WidgetClassOverride)
	{
		CsvRows.Add(TEXT("#WidgetClassOverride ") + Scenario->WidgetClassOverride->GetPathName());
	}
	if (FShidenPluginVersion Version; TryGetCurrentPluginVersion(Version))
	{
		CsvRows.Add(TEXT("#PluginVersion ") + Version.ToString());
	}
	for (int32 Index = 0; Index < Scenario->MacroParameterDefinitions.Num(); Index++)
	{
		FString IndexStr = FString::FromInt(Index + 1);
		CsvRows.Add(TEXT("#MacroParameter") + IndexStr + TEXT("Name ") + Scenario->MacroParameterDefinitions[Index].Name);
		FText TypeText = StaticEnum<EShidenVariableType>()->GetDisplayValueAsText(Scenario->MacroParameterDefinitions[Index].Type);
		CsvRows.Add(TEXT("#MacroParameter") + IndexStr + TEXT("Type ") + TypeText.ToString());
		CsvRows.Add(TEXT("#MacroParameter") + IndexStr + TEXT("DefaultValue ") + Scenario->MacroParameterDefinitions[Index].DefaultValue);
		FText AssetPathTypeText = StaticEnum<EShidenAssetPathType>()->GetDisplayValueAsText(Scenario->MacroParameterDefinitions[Index].AssetPathType);
		CsvRows.Add(TEXT("#MacroParameter") + IndexStr + TEXT("AssetPathType ") + AssetPathTypeText.ToString());
		CsvRows.Add(TEXT("#MacroParameter") + IndexStr + TEXT("IsReadOnly ") + (Scenario->MacroParameterDefinitions[Index].bIsReadOnly ? TEXT("true") : TEXT("false")));
		CsvRows.Add(TEXT("#MacroParameter") + IndexStr + TEXT("ToolTip ") + Scenario->MacroParameterDefinitions[Index].EditorSettings.ToolTip.ToString());

		if (!Scenario->MacroParameterDefinitions[Index].EditorSettings.VisibilityCondition.IsEmpty())
		{
			CsvRows.Add(TEXT("#MacroParameter") + IndexStr + TEXT("VisibilityCondition ") + Scenario->MacroParameterDefinitions[Index].EditorSettings.VisibilityCondition);
		}

		for (int32 MsgIndex = 0; MsgIndex < Scenario->MacroParameterDefinitions[Index].EditorSettings.InformationMessages.Num(); MsgIndex++)
		{
			const FShidenConditionalMessage& Msg = Scenario->MacroParameterDefinitions[Index].EditorSettings.InformationMessages[MsgIndex];
			CsvRows.Add(TEXT("#MacroParameter") + IndexStr + TEXT("InformationMessage") + FString::FromInt(MsgIndex + 1) + TEXT("Condition ") + Msg.Condition);
			CsvRows.Add(TEXT("#MacroParameter") + IndexStr + TEXT("InformationMessage") + FString::FromInt(MsgIndex + 1) + TEXT("Message ") + Msg.Message.ToString());
		}

		for (int32 MsgIndex = 0; MsgIndex < Scenario->MacroParameterDefinitions[Index].EditorSettings.WarningMessages.Num(); MsgIndex++)
		{
			const FShidenConditionalMessage& Msg = Scenario->MacroParameterDefinitions[Index].EditorSettings.WarningMessages[MsgIndex];
			CsvRows.Add(TEXT("#MacroParameter") + IndexStr + TEXT("WarningMessage") + FString::FromInt(MsgIndex + 1) + TEXT("Condition ") + Msg.Condition);
			CsvRows.Add(TEXT("#MacroParameter") + IndexStr + TEXT("WarningMessage") + FString::FromInt(MsgIndex + 1) + TEXT("Message ") + Msg.Message.ToString());
		}

		for (int32 MsgIndex = 0; MsgIndex < Scenario->MacroParameterDefinitions[Index].EditorSettings.ErrorMessages.Num(); MsgIndex++)
		{
			const FShidenConditionalMessage& Msg = Scenario->MacroParameterDefinitions[Index].EditorSettings.ErrorMessages[MsgIndex];
			CsvRows.Add(TEXT("#MacroParameter") + IndexStr + TEXT("ErrorMessage") + FString::FromInt(MsgIndex + 1) + TEXT("Condition ") + Msg.Condition);
			CsvRows.Add(TEXT("#MacroParameter") + IndexStr + TEXT("ErrorMessage") + FString::FromInt(MsgIndex + 1) + TEXT("Message ") + Msg.Message.ToString());
		}

		if (Scenario->MacroParameterDefinitions[Index].bIsEnumParameter)
		{
			for (int32 EnumIndex = 0; EnumIndex < Scenario->MacroParameterDefinitions[Index].EnumValues.Num(); EnumIndex++)
			{
				CsvRows.Add(
					TEXT("#MacroParameter") + IndexStr + TEXT("Enum") + FString::FromInt(EnumIndex + 1) + TEXT(" ") + Scenario->
					MacroParameterDefinitions[Index].EnumValues[EnumIndex]);
			}
		}
	}

	for (int32 Index = 0; Index < Scenario->LocalVariableDefinitions.Num(); Index++)
	{
		FString IndexStr = FString::FromInt(Index + 1);
		CsvRows.Add(TEXT("#LocalVariable") + IndexStr + TEXT("Name ") + Scenario->LocalVariableDefinitions[Index].Name);
		FText TypeText = StaticEnum<EShidenVariableType>()->GetDisplayValueAsText(Scenario->LocalVariableDefinitions[Index].Type);
		CsvRows.Add(TEXT("#LocalVariable") + IndexStr + TEXT("Type ") + TypeText.ToString());
		CsvRows.Add(TEXT("#LocalVariable") + IndexStr + TEXT("DefaultValue ") + Scenario->LocalVariableDefinitions[Index].DefaultValue);
		FText AssetPathTypeText = StaticEnum<EShidenAssetPathType>()->GetDisplayValueAsText(Scenario->LocalVariableDefinitions[Index].AssetPathType);
		CsvRows.Add(TEXT("#LocalVariable") + IndexStr + TEXT("AssetPathType ") + AssetPathTypeText.ToString());
		CsvRows.Add(TEXT("#LocalVariable") + IndexStr + TEXT("IsReadOnly ") + (Scenario->LocalVariableDefinitions[Index].bIsReadOnly ? TEXT("true") : TEXT("false")));
	}

	// Get Command Definitions
	const TMap<FString, FShidenCommandDefinition>& CommandDefinitions = UShidenBlueprintLibrary::GetCommandDefinitionsCache();

	// Get max column count
	// Note: We cannot use Command.Args.Num() directly because RemoveRedundantCommandArgs may have
	// removed args matching their default values, leaving gaps (e.g., only position 4 is stored
	// while positions 0-3 were removed). We must find the highest positional index actually needed.
	int32 MaxArgCount = 0;
	for (const FShidenCommand& Command : Scenario->Commands)
	{
		if (!CommandDefinitions.Contains(Command.CommandName))
		{
			MaxArgCount = FMath::Max(MaxArgCount, Command.Args.Num());
			continue;
		}

		const TArray<FShidenCommandArgument>& CmdArgs = CommandDefinitions[Command.CommandName].Args;
		int32 EffectiveArgCount = 0;
		for (int32 ArgIndex = 0; ArgIndex < CmdArgs.Num(); ArgIndex++)
		{
			const FString ArgName = CmdArgs[ArgIndex].ArgName.ToString();
			if (Command.Args.Contains(ArgName))
			{
				EffectiveArgCount = ArgIndex + 1;
			}

			if (CmdArgs[ArgIndex].EditorSettings.TemplateParameters.FindRef(TEXT("HasAdditionalArgs")).Compare(TEXT("true"), ESearchCase::IgnoreCase) == 0)
			{
				FString MacroIdStr = Command.Args.FindRef(ArgName);
				if (MacroIdStr.IsEmpty())
				{
					const FShidenCommand TempCommand = ExpandCommandForEditor(Command, true, true);
					MacroIdStr = TempCommand.Args.FindRef(ArgName);
				}
				FGuid MacroScenarioId;
				UShidenScenario* MacroScenario;
				if (UShidenScenarioBlueprintLibrary::TryGetScenarioByIdOrObjectPath(MacroIdStr, MacroScenarioId, MacroScenario))
				{
					EffectiveArgCount = ArgIndex + 1 + MacroScenario->MacroParameterDefinitions.Num();
				}
				break;
			}
		}
		MaxArgCount = FMath::Max(MaxArgCount, EffectiveArgCount);
	}

	// Add header
	FString Header = TEXT("CommandId,Enabled,CommandName,PresetName");
	for (int32 Index = 0; Index < MaxArgCount; Index++)
	{
		Header += TEXT(",Arg") + FString::FromInt(Index + 1);
	}
	CsvRows.Add(Header);

	// Add rows
	for (const FShidenCommand& Command : Scenario->Commands)
	{
		FString Row = Command.CommandId.ToString() + TEXT(",") + (Command.bEnabled ? TEXT("true") : TEXT("false")) + TEXT(",") + EscapeCsvItem(Command.CommandName) +
			TEXT(",") + EscapeCsvItem(Command.PresetName);

		if (!CommandDefinitions.Contains(Command.CommandName))
		{
			SHIDEN_WARNING("CommandName {name} is not found in CommandDefinitions.", *Command.CommandName);
			for (const auto& [Key, Value] : Command.Args)
			{
				// In CSV: explicit empty string is written as {EMPTY}, empty cell means null
				Row += TEXT(",") + EscapeCsvItem(Value.IsEmpty() ? TEXT("{EMPTY}") : Value);
			}
			CsvRows.Add(Row);
			continue;
		}

		TArray<FShidenCommandArgument> CommandArguments = CommandDefinitions[Command.CommandName].Args;
		for (int32 Index = 0; Index < MaxArgCount; Index++)
		{
			if (CommandArguments.IsValidIndex(Index) && Command.Args.Contains(CommandArguments[Index].ArgName.ToString()))
			{
				FString Arg = Command.Args[CommandArguments[Index].ArgName.ToString()];
				// In CSV: explicit empty string is written as {EMPTY}, empty cell means null
				Row += TEXT(",") + EscapeCsvItem(Arg.IsEmpty() ? TEXT("{EMPTY}") : Arg);

				// If the HasAdditionalArgs Property of CommandArguments[Index] is true, add MacroArguments
				if (CommandArguments[Index].EditorSettings.TemplateParameters.FindRef(TEXT("HasAdditionalArgs")).Compare(TEXT("true"), ESearchCase::IgnoreCase) == 0)
				{
					if (Arg.IsEmpty())
					{
						// Get scenario id or path from expanded command
						FShidenCommand TempCommand = ExpandCommandForEditor(Command, true, true);
						Arg = TempCommand.Args[CommandArguments[Index].ArgName.ToString()];
					}

					FGuid MacroScenarioId;
					UShidenScenario* MacroScenario;
					if (!UShidenScenarioBlueprintLibrary::TryGetScenarioByIdOrObjectPath(Arg, MacroScenarioId, MacroScenario))
					{
						continue;
					}

					for (const FShidenVariableDefinition& MacroParameterDefinition : MacroScenario->MacroParameterDefinitions)
					{
						if (Command.Args.Contains(MacroParameterDefinition.Name))
						{
							FString MacroArg = Command.Args[MacroParameterDefinition.Name];
							// In CSV: explicit empty string is written as {EMPTY}, empty cell means null
							Row += TEXT(",") + EscapeCsvItem(MacroArg.IsEmpty() ? TEXT("{EMPTY}") : MacroArg);
							Index++;
						}
						else
						{
							Row += TEXT(",");
							Index++;
						}
					}
				}
			}
			else
			{
				Row += TEXT(",");
			}
		}
		CsvRows.Add(Row);
	}

	// Join rows
	return FString::Join(CsvRows, TEXT("\n"));
}

bool UShidenEditorBlueprintLibrary::TryConvertToScenarioFromJson(const FString& Json, UShidenScenario*& Scenario)
{
	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Json);
	if (!FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		return false;
	}

	FShidenScenarioStruct ScenarioStruct;
	if (!FJsonObjectConverter::JsonObjectToUStruct<FShidenScenarioStruct>(JsonObject.ToSharedRef(), &ScenarioStruct))
	{
		return false;
	}

	for (FShidenMacroParameter& MacroParameter : ScenarioStruct.MacroParameterDefinitions)
	{
		if (MacroParameter.Type == EShidenVariableType::AssetPath && MacroParameter.AssetPathType == EShidenAssetPathType::None)
		{
			MacroParameter.AssetPathType = EShidenAssetPathType::Any;
		}
	}

	for (FShidenVariableDefinition& LocalVariable : ScenarioStruct.LocalVariableDefinitions)
	{
		if (LocalVariable.Type == EShidenVariableType::AssetPath && LocalVariable.AssetPathType == EShidenAssetPathType::None)
		{
			LocalVariable.AssetPathType = EShidenAssetPathType::Any;
		}
	}

	FixDuplicateCommandIds(ScenarioStruct.Commands, TEXT("JSON import"));

	Scenario = RemoveRedundantValues(ScenarioStruct.ToShidenScenario());
	return true;
}

bool UShidenEditorBlueprintLibrary::TryConvertCommandDefinitionsToJson(FString& Json)
{
	const TMap<FString, FShidenCommandDefinition>& CommandDefinitions = UShidenBlueprintLibrary::GetCommandDefinitionsCache();

	const TSharedRef<FJsonObject> RootObject = MakeShared<FJsonObject>();

	for (const auto& [CommandName, CommandDefinition] : CommandDefinitions)
	{
		const TSharedRef<FJsonObject> CommandObject = MakeShared<FJsonObject>();

		CommandObject->SetStringField(TEXT("Note"), CommandDefinition.Note.ToString());
		CommandObject->SetBoolField(TEXT("bCanCallInMacro"), CommandDefinition.bCanCallInMacro);

		TArray<TSharedPtr<FJsonValue>> ArgsArray;
		for (const FShidenCommandArgument& Arg : CommandDefinition.Args)
		{
			const TSharedRef<FJsonObject> ArgObject = MakeShared<FJsonObject>();
			ArgObject->SetStringField(TEXT("ArgName"), Arg.ArgName.ToString());
			if (Arg.DefaultValue.IsSet())
			{
				ArgObject->SetStringField(TEXT("DefaultValue"), Arg.DefaultValue.GetValue());
			}
			ArgObject->SetBoolField(TEXT("bIsAssetToBeLoaded"), Arg.bIsAssetToBeLoaded);

			if (!Arg.EditorSettings.DisplayName.IsEmpty())
			{
				ArgObject->SetStringField(TEXT("DisplayName"), Arg.EditorSettings.DisplayName.ToString());
			}

			if (!Arg.EditorSettings.ToolTip.IsEmpty())
			{
				ArgObject->SetStringField(TEXT("ToolTip"), Arg.EditorSettings.ToolTip.ToString());
			}

			if (!Arg.EditorSettings.VisibilityCondition.IsEmpty())
			{
				ArgObject->SetStringField(TEXT("VisibilityCondition"), Arg.EditorSettings.VisibilityCondition);
			}

			auto ConvertConditionalMessages = [](const TArray<FShidenConditionalMessage>& Messages) -> TArray<TSharedPtr<FJsonValue>>
			{
				TArray<TSharedPtr<FJsonValue>> Array;
				for (const FShidenConditionalMessage& Msg : Messages)
				{
					const TSharedRef<FJsonObject> MsgObject = MakeShared<FJsonObject>();
					MsgObject->SetStringField(TEXT("Condition"), Msg.Condition);
					MsgObject->SetStringField(TEXT("Message"), Msg.Message.ToString());
					Array.Add(MakeShared<FJsonValueObject>(MsgObject));
				}
				return Array;
			};

			if (Arg.EditorSettings.InformationMessages.Num() > 0)
			{
				ArgObject->SetArrayField(TEXT("InformationMessages"), ConvertConditionalMessages(Arg.EditorSettings.InformationMessages));
			}

			if (Arg.EditorSettings.WarningMessages.Num() > 0)
			{
				ArgObject->SetArrayField(TEXT("WarningMessages"), ConvertConditionalMessages(Arg.EditorSettings.WarningMessages));
			}

			if (Arg.EditorSettings.ErrorMessages.Num() > 0)
			{
				ArgObject->SetArrayField(TEXT("ErrorMessages"), ConvertConditionalMessages(Arg.EditorSettings.ErrorMessages));
			}

			ArgsArray.Add(MakeShared<FJsonValueObject>(ArgObject));
		}
		CommandObject->SetArrayField(TEXT("Args"), ArgsArray);

		RootObject->SetObjectField(CommandName, CommandObject);
	}

	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Json);
	return FJsonSerializer::Serialize(RootObject, Writer);
}

bool UShidenEditorBlueprintLibrary::TryConvertToJsonFromScenario(const UShidenScenario* SourceScenario, FString& Json)
{
	const TObjectPtr<const UShidenEditorConfig> EditorConfig = GetDefault<UShidenEditorConfig>();
	const TObjectPtr<const UShidenScenario> Scenario = EditorConfig->bExpandPresets ? ExpandPresets(SourceScenario) : SourceScenario;
	FString VersionStr = TEXT("");
	if (FShidenPluginVersion Version; TryGetCurrentPluginVersion(Version))
	{
		VersionStr = Version.ToString();
	}
	const FShidenScenarioStruct ScenarioStruct(Scenario, VersionStr);
	const TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(ScenarioStruct);
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Json);
	return FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
}

void UShidenEditorBlueprintLibrary::ListUserVariableDescriptors(TArray<FShidenVariableDescriptor>& VariableDescriptors)
{
	if (GEngine)
	{
		const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
		ShidenSubsystem->UserVariable.ListDescriptors(VariableDescriptors);
	}
}

void UShidenEditorBlueprintLibrary::ListPredefinedSystemVariableDescriptors(TArray<FShidenVariableDescriptor>& VariableDescriptors)
{
	if (GEngine)
	{
		const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
		ShidenSubsystem->PredefinedSystemVariable.ListDescriptors(VariableDescriptors);
	}
}

void UShidenEditorBlueprintLibrary::ListSystemVariableDescriptors(TArray<FShidenVariableDescriptor>& VariableDescriptors)
{
	if (GEngine)
	{
		const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
		ShidenSubsystem->SystemVariable.ListDescriptors(VariableDescriptors);
	}
}

void UShidenEditorBlueprintLibrary::ListLocalVariableDescriptors(TArray<FShidenVariableDescriptor>& VariableDescriptors)
{
	if (GEngine)
	{
		const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
		ShidenSubsystem->LocalVariable.ListDescriptors(VariableDescriptors);
	}
}

void UShidenEditorBlueprintLibrary::ForceGC()
{
	if (GEngine)
	{
		GEngine->ForceGarbageCollection(true);
	}
}

void UShidenEditorBlueprintLibrary::AddUserVariableDefinition(const FShidenVariableDefinition& VariableDefinition)
{
	if (!GEngine)
	{
		return;
	}

	const TObjectPtr<UShidenProjectConfig> ProjectConfig = GetMutableDefault<UShidenProjectConfig>();

	const int32 Index = ProjectConfig->UserVariableDefinitions.IndexOfByPredicate(
		[VariableDefinition](const FShidenVariableDefinition& InDefinition)
		{
			return InDefinition.Name == VariableDefinition.Name;
		});

	if (Index == INDEX_NONE)
	{
		ProjectConfig->UserVariableDefinitions.Add(VariableDefinition);
	}
	else
	{
		ProjectConfig->UserVariableDefinitions[Index] = VariableDefinition;
	}

	ProjectConfig->SaveConfig();
	ProjectConfig->TryUpdateDefaultConfigFile();

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	ShidenSubsystem->UserVariable.UpdateVariableDefinitions(ProjectConfig->UserVariableDefinitions);
}

void UShidenEditorBlueprintLibrary::UpdateUserVariableDefinitions(const TArray<FShidenVariableDefinition>& VariableDefinitions)
{
	if (!GEngine)
	{
		return;
	}

	const TObjectPtr<UShidenProjectConfig> ProjectConfig = GetMutableDefault<UShidenProjectConfig>();

	ProjectConfig->UserVariableDefinitions = VariableDefinitions;
	ProjectConfig->SaveConfig();
	ProjectConfig->TryUpdateDefaultConfigFile();

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	ShidenSubsystem->UserVariable.UpdateVariableDefinitions(ProjectConfig->UserVariableDefinitions);
}

void UShidenEditorBlueprintLibrary::UpdateUserVariableDefinition(const FString& OldName, const FShidenVariableDefinition& VariableDefinition)
{
	if (!GEngine)
	{
		return;
	}

	const TObjectPtr<UShidenProjectConfig> ProjectConfig = GetMutableDefault<UShidenProjectConfig>();

	const int32 Index = ProjectConfig->UserVariableDefinitions.IndexOfByPredicate(
		[OldName](const FShidenVariableDefinition& InDefinition)
		{
			return InDefinition.Name == OldName;
		});

	if (Index == INDEX_NONE)
	{
		ProjectConfig->UserVariableDefinitions.Add(VariableDefinition);
	}
	else
	{
		ProjectConfig->UserVariableDefinitions[Index] = VariableDefinition;
	}

	ProjectConfig->SaveConfig();
	ProjectConfig->TryUpdateDefaultConfigFile();

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	ShidenSubsystem->UserVariable.UpdateVariableDefinitions(ProjectConfig->UserVariableDefinitions);
}

void UShidenEditorBlueprintLibrary::RemoveUserVariableDefinition(const FString& Name)
{
	if (!GEngine)
	{
		return;
	}

	const TObjectPtr<UShidenProjectConfig> ProjectConfig = GetMutableDefault<UShidenProjectConfig>();

	ProjectConfig->UserVariableDefinitions.RemoveAll([Name](const FShidenVariableDefinition& VariableDefinition)
	{
		return VariableDefinition.Name == Name;
	});
	ProjectConfig->SaveConfig();
	ProjectConfig->TryUpdateDefaultConfigFile();

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	ShidenSubsystem->UserVariable.UpdateVariableDefinitions(ProjectConfig->UserVariableDefinitions);
}

void UShidenEditorBlueprintLibrary::AddSystemVariableDefinition(const FShidenVariableDefinition& VariableDefinition)
{
	if (!GEngine)
	{
		return;
	}

	const TObjectPtr<UShidenProjectConfig> ProjectConfig = GetMutableDefault<UShidenProjectConfig>();

	const int32 Index = ProjectConfig->SystemVariableDefinitions.IndexOfByPredicate(
		[VariableDefinition](const FShidenVariableDefinition& InDefinition)
		{
			return InDefinition.Name == VariableDefinition.Name;
		});

	if (Index == INDEX_NONE)
	{
		ProjectConfig->SystemVariableDefinitions.Add(VariableDefinition);
	}
	else
	{
		ProjectConfig->SystemVariableDefinitions[Index] = VariableDefinition;
	}

	ProjectConfig->SaveConfig();
	ProjectConfig->TryUpdateDefaultConfigFile();

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	ShidenSubsystem->SystemVariable.UpdateVariableDefinitions(ProjectConfig->SystemVariableDefinitions);
}

void UShidenEditorBlueprintLibrary::UpdateSystemVariableDefinitions(const TArray<FShidenVariableDefinition>& VariableDefinitions)
{
	if (!GEngine)
	{
		return;
	}

	const TObjectPtr<UShidenProjectConfig> ProjectConfig = GetMutableDefault<UShidenProjectConfig>();

	ProjectConfig->SystemVariableDefinitions = VariableDefinitions;
	ProjectConfig->SaveConfig();
	ProjectConfig->TryUpdateDefaultConfigFile();

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	ShidenSubsystem->SystemVariable.UpdateVariableDefinitions(ProjectConfig->SystemVariableDefinitions);
}

void UShidenEditorBlueprintLibrary::UpdateSystemVariableDefinition(const FString& OldName, const FShidenVariableDefinition& VariableDefinition)
{
	if (!GEngine)
	{
		return;
	}

	const TObjectPtr<UShidenProjectConfig> ProjectConfig = GetMutableDefault<UShidenProjectConfig>();

	const int32 Index = ProjectConfig->SystemVariableDefinitions.IndexOfByPredicate(
		[OldName](const FShidenVariableDefinition& InDefinition)
		{
			return InDefinition.Name == OldName;
		});

	if (Index == INDEX_NONE)
	{
		ProjectConfig->SystemVariableDefinitions.Add(VariableDefinition);
	}
	else
	{
		ProjectConfig->SystemVariableDefinitions[Index] = VariableDefinition;
	}

	ProjectConfig->SaveConfig();
	ProjectConfig->TryUpdateDefaultConfigFile();

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	ShidenSubsystem->SystemVariable.UpdateVariableDefinitions(ProjectConfig->SystemVariableDefinitions);
}

void UShidenEditorBlueprintLibrary::RemoveSystemVariableDefinition(const FString& Name)
{
	if (!GEngine)
	{
		return;
	}

	const TObjectPtr<UShidenProjectConfig> ProjectConfig = GetMutableDefault<UShidenProjectConfig>();

	ProjectConfig->SystemVariableDefinitions.RemoveAll([Name](const FShidenVariableDefinition& VariableDefinition)
	{
		return VariableDefinition.Name == Name;
	});
	ProjectConfig->SaveConfig();
	ProjectConfig->TryUpdateDefaultConfigFile();

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	ShidenSubsystem->SystemVariable.UpdateVariableDefinitions(ProjectConfig->SystemVariableDefinitions);
}

void UShidenEditorBlueprintLibrary::GetPredefinedSystemVariableDefinitions(TArray<FShidenVariableDefinition>& VariableDefinitions)
{
	if (GEngine)
	{
		const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

		VariableDefinitions.Empty();
		for (const FShidenPredefinedSystemVariableDefinition& Definition : ShidenSubsystem->PredefinedSystemVariable.Definitions)
		{
			VariableDefinitions.Add(Definition);
		}
	}
}

bool UShidenEditorBlueprintLibrary::TryParseVersionString(const FString& VersionStr, FShidenPluginVersion& OutVersion)
{
	// Strip pre-release suffix first (e.g., "1.10.0-beta1" -> "1.10.0")
	FString CleanVersionStr = VersionStr;
	int32 HyphenIndex;
	if (CleanVersionStr.FindChar(TEXT('-'), HyphenIndex))
	{
		CleanVersionStr = CleanVersionStr.Left(HyphenIndex);
	}

	TArray<FString> Parts;
	CleanVersionStr.ParseIntoArray(Parts, TEXT("."));
	if (Parts.Num() >= 3)
	{
		OutVersion.Major = FCString::Atoi(*Parts[0]);
		OutVersion.Minor = FCString::Atoi(*Parts[1]);
		OutVersion.Patch = FCString::Atoi(*Parts[2]);
		return true;
	}
	return false;
}

bool UShidenEditorBlueprintLibrary::TryGetCurrentPluginVersion(FShidenPluginVersion& PluginVersion)
{
	const FString PluginPath = FPaths::Combine(FPaths::ProjectPluginsDir(), ShidenEditorConstants::PluginRelativePath);
	FString JsonString;
	if (!FFileHelper::LoadFileToString(JsonString, *PluginPath))
	{
		SHIDEN_ERROR("Failed to load plugin file: {path}", *PluginPath);
		return false;
	}

	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

	if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
	{
		SHIDEN_ERROR("Failed to parse JSON from plugin file: {path}", *PluginPath);
		return false;
	}

	if (JsonObject->HasField(TEXT("VersionName")))
	{
		const FString VersionName = JsonObject->GetStringField(TEXT("VersionName"));

		if (TryParseVersionString(VersionName, PluginVersion))
		{
			return true;
		}

		SHIDEN_ERROR("Invalid VersionName format: {version}", *VersionName);
		return false;
	}

	SHIDEN_WARNING("No VersionName field found");
	return false;
}

void UShidenEditorBlueprintLibrary::RedirectCommands(UShidenScenario* Scenario, const FShidenPluginVersion& SourcePluginVersion, bool& AnyCommandUpdated)
{
	const TArray<FShidenCommandRedirector> Redirects = GetRedirectDefinitions(SourcePluginVersion);

	AnyCommandUpdated = false;

	if (Redirects.Num() == 0)
	{
		return;
	}

	if (!IsValid(Scenario))
	{
		return;
	}

	for (FShidenCommand& Command : Scenario->Commands)
	{
		for (const auto& [TargetCommandName, NewCommandName, ArgumentNameMapping] : Redirects)
		{
			if (Command.CommandName.Compare(TargetCommandName, ESearchCase::CaseSensitive) == 0)
			{
				if (!NewCommandName.IsEmpty() && Command.CommandName.Compare(NewCommandName, ESearchCase::CaseSensitive) != 0)
				{
					Command.CommandName = NewCommandName;
					AnyCommandUpdated = true;
				}

				if (ArgumentNameMapping.Num() > 0)
				{
					TMap<FString, FString> UpdatedArgs;
					for (const TTuple<FString, FString>& Arg : Command.Args)
					{
						FString NewArgName = ArgumentNameMapping.Contains(*Arg.Key)
							                     ? ArgumentNameMapping[*Arg.Key]
							                     : *Arg.Key;
						UpdatedArgs.Add(NewArgName, Arg.Value);
						if (NewArgName.Compare(Arg.Key, ESearchCase::CaseSensitive) != 0)
						{
							AnyCommandUpdated = true;
						}
					}
					Command.Args = UpdatedArgs;
				}
			}
		}
	}
}

void UShidenEditorBlueprintLibrary::RedirectPresets(const FShidenPluginVersion& SourcePluginVersion, bool& AnyPresetUpdated)
{
	const TArray<FShidenCommandRedirector> Redirects = GetRedirectDefinitions(SourcePluginVersion);

	AnyPresetUpdated = false;

	if (Redirects.Num() == 0)
	{
		return;
	}

	const TObjectPtr<UShidenProjectConfig> ProjectConfig = GetMutableDefault<UShidenProjectConfig>();
	for (auto& [PresetName, Preset] : ProjectConfig->Presets)
	{
		for (const auto& [TargetCommandName, NewCommandName, ArgumentNameMapping] : Redirects)
		{
			if (Preset.CommandName.Compare(TargetCommandName, ESearchCase::CaseSensitive) == 0)
			{
				if (!NewCommandName.IsEmpty() && Preset.CommandName.Compare(NewCommandName, ESearchCase::CaseSensitive) != 0)
				{
					Preset.CommandName = NewCommandName;
					AnyPresetUpdated = true;
				}

				if (ArgumentNameMapping.Num() > 0)
				{
					TMap<FString, FString> UpdatedArgs;
					for (const TTuple<FString, FString>& Arg : Preset.Args)
					{
						FString NewArgName = ArgumentNameMapping.Contains(*Arg.Key)
							                     ? ArgumentNameMapping[*Arg.Key]
							                     : *Arg.Key;
						UpdatedArgs.Add(NewArgName, Arg.Value);
						if (NewArgName.Compare(Arg.Key, ESearchCase::CaseSensitive) != 0)
						{
							AnyPresetUpdated = true;
						}
					}
					Preset.Args = UpdatedArgs;
				}
			}
		}
	}
}

void UShidenEditorBlueprintLibrary::RedirectLocalVariables(UShidenScenario* Scenario, const FString& OldVariableName, const FString& NewVariableName,
                                                           bool& AnyCommandUpdated)
{
	AnyCommandUpdated = false;

	if (OldVariableName.IsEmpty())
	{
		return;
	}

	// <CommandName, <VariableKindArgName, VariableArgName>>
	TMap<FString, TTuple<FString, FString>> TargetCommand;

	const TMap<FString, FShidenCommandDefinition>& CommandDefinitions = UShidenBlueprintLibrary::GetCommandDefinitionsCache();
	for (const auto& [CommandName, CommandDefinition] : CommandDefinitions)
	{
		for (const FShidenCommandArgument& Arg : CommandDefinition.Args)
		{
			if (Arg.EditorSettings.TemplateWidget == UShidenStandardCommandDefinitions::VariableNameInputTemplate
				&& Arg.EditorSettings.TemplateParameters.Contains(TEXT("VariableKindSourceArgName")))
			{
				const FString VariableKindArgName = Arg.EditorSettings.TemplateParameters[TEXT("VariableKindSourceArgName")];
				const int32 VariableKindIndex = CommandDefinition.Args.IndexOfByPredicate([&VariableKindArgName](const FShidenCommandArgument& ArgItem)
				{
					return ArgItem.ArgName.ToString() == VariableKindArgName;
				});
				if (CommandDefinition.Args.IsValidIndex(VariableKindIndex))
				{
					const FShidenCommandArgument& VariableKindArg = CommandDefinition.Args[VariableKindIndex];
					TargetCommand.Add(CommandName, {VariableKindArg.ArgName.ToString(), Arg.ArgName.ToString()});
				}
				break;
			}
		}
	}

	const FString OldVariableNameWithPrefix = FString::Printf(TEXT("Local::%s"), *OldVariableName);
	const FString NewReplacementVariableName = FString::Printf(TEXT("{Local::%s}"), *NewVariableName);

	for (FShidenCommand& Command : Scenario->Commands)
	{
		for (TPair<FString, FString>& Pair : Command.Args)
		{
			FString& Value = Pair.Value;
			if (!Value.Contains(TEXT("{")) || !Value.Contains(TEXT("}")))
			{
				continue;
			}
			FRegexMatcher Matcher(UShidenVariableBlueprintLibrary::GetReplaceTextPattern(), Value);
			while (Matcher.FindNext())
			{
				const FString Str = Matcher.GetCaptureGroup(1);
				const FString VariableName = Str.Mid(1, Str.Len() - 2).TrimStartAndEnd();
				if (VariableName == OldVariableNameWithPrefix)
				{
					Value.ReplaceInline(*Str, *NewReplacementVariableName, ESearchCase::CaseSensitive);
					AnyCommandUpdated = true;
				}
			}
		}

		if (TargetCommand.Contains(Command.CommandName))
		{
			if (Command.GetArg(TargetCommand[Command.CommandName].Get<0>()).GetValue() == TEXT("LocalVariable")
				&& Command.GetArg(TargetCommand[Command.CommandName].Get<1>()).GetValue() == OldVariableName)
			{
				Command.Args[TargetCommand[Command.CommandName].Get<1>()] = NewVariableName;
			}
		}
	}
}

void UShidenEditorBlueprintLibrary::RedirectAllMacroParameters(const UShidenScenario* TargetMacro, const FString& TargetMacroPath,
                                                               const FString& OldParameterName, const FString& NewParameterName,
                                                               const bool bNeedTransaction, TArray<UShidenScenario*>& AssetToBeSaved)
{
	if (OldParameterName.IsEmpty() || !IsValid(TargetMacro))
	{
		return;
	}

	TMap<FString, FString> TargetCommand;

	const TMap<FString, FShidenCommandDefinition>& CommandDefinitions = UShidenBlueprintLibrary::GetCommandDefinitionsCache();
	for (const auto& [CommandName, CommandDefinition] : CommandDefinitions)
	{
		for (const FShidenCommandArgument& Arg : CommandDefinition.Args)
		{
			if (Arg.EditorSettings.TemplateWidget == UShidenStandardCommandDefinitions::ScenarioInputTemplate
				&& Arg.EditorSettings.TemplateParameters.FindRef(TEXT("HasAdditionalArgs")).Compare(TEXT("true"), ESearchCase::IgnoreCase) == 0)
			{
				TargetCommand.Add(CommandName, Arg.ArgName.ToString());
				break;
			}
		}
	}

	const TObjectPtr<const UShidenProjectConfig> ProjectConfig = GetDefault<UShidenProjectConfig>();

	TArray<FAssetData> ScenarioAssets;
	AssetViewUtils::GetAssetsInPaths({ProjectConfig->ScenarioDirectoryPath, ProjectConfig->MacroDirectoryPath}, ScenarioAssets);

	for (const FAssetData& Asset : ScenarioAssets)
	{
		if (!Asset.IsValid())
		{
			continue;
		}

		if (TObjectPtr<UShidenScenario> Scenario = Cast<UShidenScenario>(Asset.GetAsset()); IsValid(Scenario))
		{
			bool bChanged = false;
			for (FShidenCommand& Command : Scenario->Commands)
			{
				if (!TargetCommand.Contains(Command.CommandName))
				{
					continue;
				}

				const FString& TargetArgName = TargetCommand.FindRef(Command.CommandName);

				if (!Command.Args.Contains(TargetArgName))
				{
					continue;
				}

				if (Command.Args[TargetArgName] != TargetMacro->ScenarioId.ToString()
					&& Command.Args[TargetArgName] != TargetMacroPath)
				{
					continue;
				}

				if (Command.Args.Contains(OldParameterName))
				{
					if (!bChanged)
					{
						bChanged = true;
#if WITH_EDITOR
						if (bNeedTransaction)
						{
							Scenario->Modify();
						}
#endif
					}
					Command.Args.Add(NewParameterName, Command.Args[OldParameterName]);
					Command.Args.Remove(OldParameterName);
				}
			}
			if (bChanged)
			{
				AssetToBeSaved.Add(Scenario);
			}
		}
	}
}

void UShidenEditorBlueprintLibrary::RedirectAllVariables(const EShidenVariableKind VariableKind, const FString& OldVariableName, const FString& NewVariableName)
{
	if (OldVariableName.IsEmpty())
	{
		return;
	}

	// <CommandName, <VariableKindArgName, VariableArgName>>
	TMap<FString, TTuple<FString, FString>> TargetCommand;

	const TMap<FString, FShidenCommandDefinition>& CommandDefinitions = UShidenBlueprintLibrary::GetCommandDefinitionsCache();
	for (const auto& [CommandName, CommandDefinition] : CommandDefinitions)
	{
		for (const FShidenCommandArgument& Arg : CommandDefinition.Args)
		{
			if (Arg.EditorSettings.TemplateWidget == UShidenStandardCommandDefinitions::VariableNameInputTemplate
				&& Arg.EditorSettings.TemplateParameters.Contains(TEXT("VariableKindSourceArgName")))
			{
				const FString VariableKindArgName = Arg.EditorSettings.TemplateParameters[TEXT("VariableKindSourceArgName")];
				const int32 VariableKindIndex = CommandDefinition.Args.IndexOfByPredicate([&VariableKindArgName](const FShidenCommandArgument& ArgItem)
				{
					return ArgItem.ArgName.ToString() == VariableKindArgName;
				});
				if (CommandDefinition.Args.IsValidIndex(VariableKindIndex))
				{
					const FShidenCommandArgument& VariableKindArg = CommandDefinition.Args[VariableKindIndex];
					TargetCommand.Add(CommandName, {VariableKindArg.ArgName.ToString(), Arg.ArgName.ToString()});
				}
				break;
			}
		}
	}

	const TObjectPtr<UEditorAssetSubsystem> EditorAssetSubsystem = GEditor->GetEditorSubsystem<UEditorAssetSubsystem>();
	const TObjectPtr<const UShidenProjectConfig> ProjectConfig = GetDefault<UShidenProjectConfig>();

	TArray<FAssetData> ScenarioAssets;
	AssetViewUtils::GetAssetsInPaths({ProjectConfig->ScenarioDirectoryPath, ProjectConfig->MacroDirectoryPath}, ScenarioAssets);

	const FString VariableKindStr = StaticEnum<EShidenVariableKind>()->GetDisplayValueAsText(VariableKind).ToString();

	FString OldVariableNameWithPrefix = OldVariableName;
	FString NewReplacementVariableName = NewVariableName;

	switch (VariableKind)
	{
	case EShidenVariableKind::UserVariable:
		NewReplacementVariableName = FString::Printf(TEXT("{%s}"), *NewReplacementVariableName);
		break;
	case EShidenVariableKind::LocalVariable:
		SHIDEN_WARNING("Replace local variables is not supported.");
		return;
	case EShidenVariableKind::SystemVariable:
		OldVariableNameWithPrefix = FString::Printf(TEXT("System::%s"), *OldVariableName);
		NewReplacementVariableName = FString::Printf(TEXT("{System::%s}"), *NewReplacementVariableName);
		break;
	case EShidenVariableKind::PredefinedSystemVariable:
		OldVariableNameWithPrefix = FString::Printf(TEXT("Predefined::%s"), *OldVariableName);
		NewReplacementVariableName = FString::Printf(TEXT("{Predefined::%s}"), *NewReplacementVariableName);
		break;
	}

	for (const FAssetData& Asset : ScenarioAssets)
	{
		if (!Asset.IsValid())
		{
			continue;
		}

		if (TObjectPtr<UShidenScenario> Scenario = Cast<UShidenScenario>(Asset.GetAsset()); IsValid(Scenario))
		{
			bool bChanged = false;
			for (FShidenCommand& Command : Scenario->Commands)
			{
				for (TPair<FString, FString>& Pair : Command.Args)
				{
					FString& Value = Pair.Value;
					if (!Value.Contains(TEXT("{")) || !Value.Contains(TEXT("}")))
					{
						continue;
					}

					FRegexMatcher Matcher(UShidenVariableBlueprintLibrary::GetReplaceTextPattern(), Value);
					while (Matcher.FindNext())
					{
						const FString Str = Matcher.GetCaptureGroup(1);
						const FString VariableName = Str.Mid(1, Str.Len() - 2).TrimStartAndEnd();
						if (VariableName == OldVariableNameWithPrefix)
						{
							Value.ReplaceInline(*Str, *NewReplacementVariableName, ESearchCase::CaseSensitive);
							bChanged = true;
						}
					}
				}

				if (TargetCommand.Contains(Command.CommandName))
				{
					const FString CommandVariableKindValue = Command.GetArg(TargetCommand[Command.CommandName].Get<0>()).GetValue();
					const FString CommandVariableNameValue = Command.GetArg(TargetCommand[Command.CommandName].Get<1>()).GetValue();
					if (CommandVariableKindValue == VariableKindStr && CommandVariableNameValue == OldVariableName)
					{
						Command.Args[TargetCommand[Command.CommandName].Get<1>()] = NewVariableName;
						bChanged = true;
					}
				}
			}

			if (bChanged)
			{
				// ReSharper disable once CppExpressionWithoutSideEffects
				Scenario->MarkPackageDirty();
				EditorAssetSubsystem->SaveLoadedAsset(Scenario);
			}
		}
	}
}

bool UShidenEditorBlueprintLibrary::TryMigratePlugin()
{
	const TObjectPtr<UEditorAssetSubsystem> EditorAssetSubsystem = GEditor->GetEditorSubsystem<UEditorAssetSubsystem>();
	const TObjectPtr<UShidenEditorConfig> EditorConfig = GetMutableDefault<UShidenEditorConfig>();
	const TObjectPtr<UShidenProjectConfig> ProjectConfig = GetMutableDefault<UShidenProjectConfig>();
	const FShidenPluginVersion CurrentVersion = EditorConfig->PluginVersion;

	// Migration
	if (CurrentVersion < FShidenPluginVersion(0, 16, 15))
	{
		// Migrate variables
		// Migrate UserVariableDefinitions
		for (int32 i = 0; i < ProjectConfig->UserVariableDefinitions.Num(); i++)
		{
			FShidenVariableDefinition& Definition = ProjectConfig->UserVariableDefinitions[i];
			if (Definition.Type == EShidenVariableType::AssetPath && Definition.AssetPathType == EShidenAssetPathType::None)
			{
				Definition.AssetPathType = EShidenAssetPathType::Any;
			}
		}

		// Migrate SystemVariableDefinitions
		for (int32 i = 0; i < ProjectConfig->SystemVariableDefinitions.Num(); i++)
		{
			FShidenVariableDefinition& Definition = ProjectConfig->SystemVariableDefinitions[i];
			if (Definition.Type == EShidenVariableType::AssetPath && Definition.AssetPathType == EShidenAssetPathType::None)
			{
				Definition.AssetPathType = EShidenAssetPathType::Any;
			}
		}

		// Migrate Presets
		bool bPresetsChanged = false;
		RedirectPresets(CurrentVersion, bPresetsChanged);

		ProjectConfig->SaveConfig();
		ProjectConfig->TryUpdateDefaultConfigFile();

		// Migrate Local VariableDefinitions
		TArray<FAssetData> ScenarioAssets;
		AssetViewUtils::GetAssetsInPaths({ProjectConfig->ScenarioDirectoryPath, ProjectConfig->MacroDirectoryPath}, ScenarioAssets);
		for (const FAssetData& Asset : ScenarioAssets)
		{
			if (!Asset.IsValid())
			{
				continue;
			}

			if (TObjectPtr<UShidenScenario> Scenario = Cast<UShidenScenario>(Asset.GetAsset()); IsValid(Scenario))
			{
				bool bChanged = false;
				RedirectCommands(Scenario, CurrentVersion, bChanged);
				for (FShidenVariableDefinition& Definition : Scenario->LocalVariableDefinitions)
				{
					if (Definition.Type == EShidenVariableType::AssetPath && Definition.AssetPathType == EShidenAssetPathType::None)
					{
						Definition.AssetPathType = EShidenAssetPathType::Any;
						bChanged = true;
					}
				}

				for (FShidenMacroParameter& Parameter : Scenario->MacroParameterDefinitions)
				{
					if (Parameter.Type == EShidenVariableType::AssetPath && Parameter.AssetPathType == EShidenAssetPathType::None)
					{
						Parameter.AssetPathType = EShidenAssetPathType::Any;
						bChanged = true;
					}
				}

				if (bChanged)
				{
					// ReSharper disable once CppExpressionWithoutSideEffects
					Scenario->MarkPackageDirty();
					EditorAssetSubsystem->SaveLoadedAsset(Scenario);
				}
			}
		}
	}

	if (CurrentVersion < FShidenPluginVersion(0, 17, 4))
	{
		if (UShidenSaveBlueprintLibrary::DoesSystemDataExist())
		{
			if (const TObjectPtr<USaveGame> LoadedGame = UGameplayStatics::LoadGameFromSlot("ShidenSystemSave", 0))
			{
				UGameplayStatics::SaveGameToSlot(LoadedGame, "ShidenPredefinedSystemSave", 0);
			}
		}
	}

	if (CurrentVersion < FShidenPluginVersion(1, 13, 0))
	{
		const FText DialogTitle = NSLOCTEXT("ShidenNamespace", "AdvancedModeTitle", "Enable Advanced Mode");
		const FText DialogMessage = NSLOCTEXT("ShidenNamespace", "AdvancedModeMessage",
		                                      "Do you want to enable advanced mode?\r\n\r\n"
		                                      "In advanced mode, all editing items in Shiden Visual Novel Editor are displayed.");

		const EAppReturnType::Type Response = FMessageDialog::Open(EAppMsgType::YesNo, DialogMessage, DialogTitle);

		const TObjectPtr<UShidenEditorUserConfig> UserConfig = GetMutableDefault<UShidenEditorUserConfig>();
		if (Response == EAppReturnType::No)
		{
			UserConfig->bUseAdvancedMode = false;
		}

		// Migrate from separate editor windows (pre-1.13.0 behavior)
		UserConfig->bCombineEditorWindows = false;

		// Migrate editor settings from DefaultEditor.ini
		const FString OldEditorFilename = FPaths::ProjectConfigDir() / TEXT("DefaultEditor.ini");
		static const FString EditorSectionName = TEXT("/Script/ShidenEditor.ShidenEditorConfig");

		FConfigCacheIni TempEditorConfig(EConfigCacheType::Temporary);
		TempEditorConfig.LoadFile(OldEditorFilename);

		if (TempEditorConfig.DoesSectionExist(*EditorSectionName, OldEditorFilename))
		{
			bool bTempBool;
			FString TempString;
			if (TempEditorConfig.GetString(*EditorSectionName, TEXT("ScenarioFilterPath"), TempString, OldEditorFilename))
			{
				UserConfig->ScenarioFilterPath = TempString;
			}
			if (TempEditorConfig.GetString(*EditorSectionName, TEXT("EditScenarioPath"), TempString, OldEditorFilename))
			{
				UserConfig->EditScenarioPath = TempString;
			}
			if (TempEditorConfig.GetBool(*EditorSectionName, TEXT("bPreviewSound"), bTempBool, OldEditorFilename))
			{
				UserConfig->bPreviewSound = bTempBool;
			}
			if (TempEditorConfig.GetString(*EditorSectionName, TEXT("PreviewSize"), TempString, OldEditorFilename))
			{
				UserConfig->PreviewSize.InitFromString(TempString);
			}
			if (TempEditorConfig.GetString(*EditorSectionName, TEXT("ShidenDebuggerRefreshInterval"), TempString, OldEditorFilename))
			{
				UserConfig->ShidenDebuggerRefreshInterval = FCString::Atof(*TempString);
			}
			if (TempEditorConfig.GetBool(*EditorSectionName, TEXT("bAutoSaveScenario"), bTempBool, OldEditorFilename))
			{
				UserConfig->bAutoSaveScenario = bTempBool;
			}
			if (TempEditorConfig.GetBool(*EditorSectionName, TEXT("bForceShowAllCommandArguments"), bTempBool, OldEditorFilename))
			{
				UserConfig->bForceShowAllCommandArguments = bTempBool;
			}
		}

		UserConfig->SaveConfig();

		// Migrate command arguments to null concept:
		// - Replace {EMPTY} values with empty string
		// - Remove {EMPTY} args for toggle/macro arguments (make them null)
		// - Remove args matching preset or default values (make them null)
		const TMap<FString, FShidenCommandDefinition>& CommandDefinitions = UShidenBlueprintLibrary::GetCommandDefinitionsCache();

		auto ShouldRemoveEmptyArg = [](const FShidenCommandDefinition* CommandDef, const FString& ArgKey)
		{
			if (!CommandDef)
			{
				return false;
			}

			const FShidenCommandArgument* ArgDef = CommandDef->Args.FindByPredicate([&ArgKey](const FShidenCommandArgument& InArg)
			{
				return InArg.ArgName.ToString() == ArgKey;
			});

			if (ArgDef)
			{
				return ArgDef->EditorSettings.TemplateParameters.FindRef(TEXT("HasToggle")).Compare(TEXT("true"), ESearchCase::IgnoreCase) == 0;
			}

			// Additional args (e.g., macro parameters) are not in CommandDefinition::Args.
			// The original intention was to return true unconditionally if ArgDef is not found,
			// but to be safe, we will also check if HasAdditionalArgs exists in TemplateParameters.
			for (const FShidenCommandArgument& CommandArg : CommandDef->Args)
			{
				if (CommandArg.EditorSettings.TemplateParameters.FindRef(TEXT("HasAdditionalArgs")).Compare(TEXT("true"), ESearchCase::IgnoreCase) == 0)
				{
					return true;
				}
			}

			return false;
		};

		// Migrate presets: replace/remove {EMPTY} and remove args matching command defaults
		bool bPresetsChanged = false;
		for (auto& [MigPresetName, MigPreset] : ProjectConfig->Presets)
		{
			const FShidenCommandDefinition* MigCommandDef = CommandDefinitions.Find(MigPreset.CommandName);

			TArray<FString> MigPresetEmptyKeysToRemove;
			for (auto& [Key, Value] : MigPreset.Args)
			{
				if (Value == TEXT("{EMPTY}"))
				{
					if (ShouldRemoveEmptyArg(MigCommandDef, Key))
					{
						MigPresetEmptyKeysToRemove.Add(Key);
					}
					else
					{
						Value = TEXT("");
					}
					bPresetsChanged = true;
				}
			}

			for (const FString& Key : MigPresetEmptyKeysToRemove)
			{
				MigPreset.Args.Remove(Key);
			}

			if (MigCommandDef)
			{
				TArray<FString> MigPresetKeysToRemove;
				for (const auto& [Key, Value] : MigPreset.Args)
				{
					for (const FShidenCommandArgument& ArgDef : MigCommandDef->Args)
					{
						if (ArgDef.ArgName.ToString() == Key && ArgDef.DefaultValue.IsSet() && Value == ArgDef.DefaultValue.GetValue())
						{
							MigPresetKeysToRemove.Add(Key);
							break;
						}
					}
				}
				for (const FString& Key : MigPresetKeysToRemove)
				{
					MigPreset.Args.Remove(Key);
					bPresetsChanged = true;
				}
			}
		}

		if (bPresetsChanged)
		{
			ProjectConfig->SaveConfig();
			ProjectConfig->TryUpdateDefaultConfigFile();
		}

		// Migrate scenario UAssets
		TArray<FAssetData> MigrationScenarioAssets;
		AssetViewUtils::GetAssetsInPaths(
			{ProjectConfig->ScenarioDirectoryPath, ProjectConfig->MacroDirectoryPath},
			MigrationScenarioAssets);

		for (const FAssetData& MigAsset : MigrationScenarioAssets)
		{
			if (!MigAsset.IsValid())
			{
				continue;
			}

			const TObjectPtr<UShidenScenario> MigScenario = Cast<UShidenScenario>(MigAsset.GetAsset());
			if (!IsValid(MigScenario))
			{
				continue;
			}

			bool bScenarioChanged = false;
			for (FShidenCommand& MigCommand : MigScenario->Commands)
			{
				const FShidenCommandDefinition* MigCommandDef = CommandDefinitions.Find(MigCommand.CommandName);

				// Replace/remove {EMPTY} values
				TArray<FString> MigCommandEmptyKeysToRemove;
				for (auto& [Key, Value] : MigCommand.Args)
				{
					if (Value == TEXT("{EMPTY}"))
					{
						if (ShouldRemoveEmptyArg(MigCommandDef, Key))
						{
							MigCommandEmptyKeysToRemove.Add(Key);
						}
						else
						{
							Value = TEXT("");
						}
						bScenarioChanged = true;
					}
				}

				for (const FString& Key : MigCommandEmptyKeysToRemove)
				{
					MigCommand.Args.Remove(Key);
				}

				// Remove args matching fallback values (preset > default)
				const FShidenCommand CleanedCommand = RemoveRedundantCommandArgs(MigCommand);
				if (CleanedCommand.Args.Num() != MigCommand.Args.Num())
				{
					MigCommand.Args = CleanedCommand.Args;
					bScenarioChanged = true;
				}
			}

			if (bScenarioChanged)
			{
				// ReSharper disable once CppExpressionWithoutSideEffects
				MigScenario->MarkPackageDirty();
				EditorAssetSubsystem->SaveLoadedAsset(MigScenario);
			}
		}
	}

	// Update pluginVersion
	FShidenPluginVersion NewVersion;
	if (!TryGetCurrentPluginVersion(NewVersion))
	{
		SHIDEN_ERROR("Failed to get current plugin version");
		return false;
	}

	if (EditorConfig->PluginVersion != NewVersion)
	{
		EditorConfig->PluginVersion = NewVersion;
		EditorConfig->SaveConfig();
		EditorConfig->TryUpdateDefaultConfigFile();
	}
	return true;
}

void UShidenEditorBlueprintLibrary::OpenSettings(const FName& ContainerName, const FName& CategoryName, const FName& SectionName)
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->ShowViewer(ContainerName, CategoryName, SectionName);
	}
}

FString UShidenEditorBlueprintLibrary::ReplaceArgumentReferences(const TMap<FString, FString>& CommandArgs, const FString& Expression)
{
	// Single left-to-right pass over the original Expression.
	// By matching against the original string and writing into a separate Output buffer,
	// substituted values are never re-scanned, so they cannot trigger further replacements.
	// This also ensures HasVariables can reliably detect remaining {ArgName} tokens after
	// substitution (only tokens that had no matching argument will remain).
	const FRegexPattern RegexPattern(TEXT("\\{\\s*(\\w+)\\s*\\}"));

	FString Output;
	Output.Reserve(Expression.Len());
	int32 SearchStart = 0;

	FRegexMatcher Matcher(RegexPattern, Expression);
	while (Matcher.FindNext())
	{
		const int32 BeginIndex = Matcher.GetMatchBeginning();
		const int32 EndIndex = Matcher.GetMatchEnding();
		const FString ArgName = Matcher.GetCaptureGroup(1);

		// Append unmatched prefix
		Output.Append(Expression.Mid(SearchStart, BeginIndex - SearchStart));

		if (const FString* Value = CommandArgs.Find(ArgName))
		{
			// Known argument: replace with quoted value
			Output.Append(TEXT("\""));
			Output.Append(*Value);
			Output.Append(TEXT("\""));
		}
		else
		{
			// Unknown argument: treat as null and replace with empty quoted string
			Output.Append(TEXT("\"\""));
		}

		SearchStart = EndIndex;
	}

	// Append remaining suffix after the last match
	Output.Append(Expression.Mid(SearchStart));
	return Output;
}

bool UShidenEditorBlueprintLibrary::TryEvaluateConditionalMessagesCore(const FShidenExpressionVariableDefinitionContext& Context, const FString& ArgName, const TMap<FString, FString>& ExpandedCommandArgs,
                                                                       const TArray<FShidenConditionalMessage>& InformationConditionalMessages, const TArray<FShidenConditionalMessage>& WarningConditionalMessages,
                                                                       const TArray<FShidenConditionalMessage>& ErrorConditionalMessages, TArray<FText>& OutInformationMessages, TArray<FText>& OutWarningMessages,
                                                                       TArray<FText>& OutErrorMessages, FString& ErrorMessage)
{
	auto TryEvaluateMessages = [](const FShidenExpressionVariableDefinitionContext& InContext, const TMap<FString, FString>& InExpandedCommandArgs,
	                              const TArray<FShidenConditionalMessage>& InConditionalMessages,
	                              TArray<FText>& OutMessages, FString& OutErrorMessage)
	{
		OutMessages.Empty();

		for (const FShidenConditionalMessage& ConditionalMessage : InConditionalMessages)
		{
			if (ConditionalMessage.Condition.IsEmpty())
			{
				continue;
			}

			const FString ReplacedExpression = ReplaceArgumentReferences(InExpandedCommandArgs, ConditionalMessage.Condition);

			const FShidenExpressionEvaluator Evaluator(InContext);
			FShidenExpressionValue Result;
			if (!Evaluator.TryEvaluate(ReplacedExpression, Result, OutErrorMessage))
			{
				OutErrorMessage = FString::Printf(TEXT("Failed to evaluate condition '%s' / '%s': %s"), *ConditionalMessage.Condition, *ReplacedExpression, *OutErrorMessage);
				return false;
			}

			if (Result.Type != EShidenExpressionValueType::Boolean)
			{
				OutErrorMessage = FString::Printf(TEXT("Condition must evaluate to a boolean value, got '%s'"), *ReplacedExpression);
				return false;
			}

			if (Result.BoolValue)
			{
				OutMessages.Add(ConditionalMessage.Message);
			}
		}
		return true;
	};

	OutInformationMessages.Empty();
	OutWarningMessages.Empty();
	OutErrorMessages.Empty();

	const FString CommandArgValue = ExpandedCommandArgs.FindRef(ArgName);
	CheckUndefinedVariables(Context, CommandArgValue, OutErrorMessages);

	TArray<FText> InformationMessages;
	FString InformationErrorMessage;
	if (!TryEvaluateMessages(Context, ExpandedCommandArgs, InformationConditionalMessages, InformationMessages, InformationErrorMessage))
	{
		ErrorMessage = InformationErrorMessage;
		return false;
	}
	OutInformationMessages.Append(InformationMessages);

	TArray<FText> WarningMessages;
	FString WarningErrorMessage;
	if (!TryEvaluateMessages(Context, ExpandedCommandArgs, WarningConditionalMessages, WarningMessages, WarningErrorMessage))
	{
		ErrorMessage = WarningErrorMessage;
		return false;
	}
	OutWarningMessages.Append(WarningMessages);

	TArray<FText> ErrorMessages;
	FString ErrorErrorMessage;
	if (!TryEvaluateMessages(Context, ExpandedCommandArgs, ErrorConditionalMessages, ErrorMessages, ErrorErrorMessage))
	{
		ErrorMessage = ErrorErrorMessage;
		return false;
	}
	OutErrorMessages.Append(ErrorMessages);

	return true;
}

bool UShidenEditorBlueprintLibrary::TryEvaluateConditionalMessages(const UShidenScenario* Scenario, const FShidenCommandDefinition& CommandDefinition, const TMap<FString, FString>& ExpandedCommandArgs,
                                                                   const FName& ArgName, TArray<FText>& OutInformationMessages, TArray<FText>& OutWarningMessages, TArray<FText>& OutErrorMessages, FString& ErrorMessage)
{
	// Build context once for all validations
	const FShidenExpressionVariableDefinitionContext Context = BuildExpressionContext(Scenario);

	const FShidenCommandArgument* Arg = CommandDefinition.Args.FindByPredicate([&ArgName](const FShidenCommandArgument& InArg)
	{
		return InArg.ArgName == ArgName;
	});

	if (Arg)
	{
		return TryEvaluateConditionalMessagesCore(Context, ArgName.ToString(), ExpandedCommandArgs,
		                                          Arg->EditorSettings.InformationMessages, Arg->EditorSettings.WarningMessages,
		                                          Arg->EditorSettings.ErrorMessages, OutInformationMessages, OutWarningMessages, OutErrorMessages, ErrorMessage);
	}

	// find macro scenario
	const FShidenCommandArgument* MacroScenarioArg = CommandDefinition.Args.FindByPredicate([](const FShidenCommandArgument& InArg)
	{
		return InArg.EditorSettings.TemplateParameters.FindRef(TEXT("HasAdditionalArgs")).Compare(TEXT("true"), ESearchCase::IgnoreCase) == 0;
	});

	if (!MacroScenarioArg)
	{
		ErrorMessage = FString::Printf(TEXT("Argument '%s' not found in command definition"), *ArgName.ToString());
		return false;
	}

	const FString ScenarioIdOrObjectPath = ExpandedCommandArgs.FindRef(MacroScenarioArg->ArgName.ToString());
	if (ScenarioIdOrObjectPath.IsEmpty())
	{
		ErrorMessage = FString::Printf(TEXT("Macro scenario argument '%s' is empty"), *MacroScenarioArg->ArgName.ToString());
		return false;
	}

	FGuid ScenarioId;
	UShidenScenario* MacroScenario;
	if (!UShidenScenarioBlueprintLibrary::TryGetScenarioByIdOrObjectPath(ScenarioIdOrObjectPath, ScenarioId, MacroScenario))
	{
		ErrorMessage = FString::Printf(TEXT("Failed to find macro scenario for argument '%s'"), *MacroScenarioArg->ArgName.ToString());
		return false;
	}

	const FShidenMacroParameter* MacroArg = MacroScenario->MacroParameterDefinitions.FindByPredicate([&ArgName](const FShidenMacroParameter& InArg)
	{
		return InArg.Name == ArgName;
	});

	if (!MacroArg)
	{
		// Ignore if the argument is not a macro parameter
		return true;
	}

	return TryEvaluateConditionalMessagesCore(Context, ArgName.ToString(), ExpandedCommandArgs,
	                                          MacroArg->EditorSettings.InformationMessages, MacroArg->EditorSettings.WarningMessages,
	                                          MacroArg->EditorSettings.ErrorMessages, OutInformationMessages, OutWarningMessages, OutErrorMessages, ErrorMessage);
}

bool UShidenEditorBlueprintLibrary::TryValidateCommand(const UShidenScenario* Scenario, const FShidenCommandDefinition& CommandDefinition, const TMap<FString, FString>& ExpandedCommandArgs,
                                                       bool& HasInformation, bool& HasWarning, bool& HasError, FString& ErrorMessage)
{
	// Build context once for all validations (performance optimization)
	const FShidenExpressionVariableDefinitionContext Context = BuildExpressionContext(Scenario);

	for (const FShidenCommandArgument& Arg : CommandDefinition.Args)
	{
		TArray<FText> ArgInformationMessages;
		TArray<FText> ArgWarningMessages;
		TArray<FText> ArgErrorMessages;

		if (!TryEvaluateConditionalMessagesCore(Context, Arg.ArgName.ToString(), ExpandedCommandArgs,
		                                        Arg.EditorSettings.InformationMessages, Arg.EditorSettings.WarningMessages,
		                                        Arg.EditorSettings.ErrorMessages, ArgInformationMessages, ArgWarningMessages, ArgErrorMessages, ErrorMessage))
		{
			return false;
		}

		if (ArgInformationMessages.Num() > 0)
		{
			HasInformation = true;
		}

		if (ArgWarningMessages.Num() > 0)
		{
			HasWarning = true;
		}

		if (ArgErrorMessages.Num() > 0)
		{
			HasError = true;
		}

		if (Arg.EditorSettings.TemplateParameters.FindRef(TEXT("HasAdditionalArgs")).Compare(TEXT("true"), ESearchCase::IgnoreCase) == 0)
		{
			const FString ScenarioIdOrObjectPath = ExpandedCommandArgs.FindRef(Arg.ArgName.ToString());
			if (ScenarioIdOrObjectPath.IsEmpty())
			{
				return true;
			}

			FGuid ScenarioId;
			UShidenScenario* MacroScenario;
			if (UShidenScenarioBlueprintLibrary::TryGetScenarioByIdOrObjectPath(ScenarioIdOrObjectPath, ScenarioId, MacroScenario))
			{
				for (const FShidenMacroParameter& MacroArg : MacroScenario->MacroParameterDefinitions)
				{
					if (!TryEvaluateConditionalMessagesCore(Context, MacroArg.Name, ExpandedCommandArgs,
					                                        MacroArg.EditorSettings.InformationMessages, MacroArg.EditorSettings.WarningMessages,
					                                        MacroArg.EditorSettings.ErrorMessages, ArgInformationMessages, ArgWarningMessages, ArgErrorMessages, ErrorMessage))
					{
						return false;
					}

					if (ArgInformationMessages.Num() > 0)
					{
						HasInformation = true;
					}

					if (ArgWarningMessages.Num() > 0)
					{
						HasWarning = true;
					}

					if (ArgErrorMessages.Num() > 0)
					{
						HasError = true;
					}
				}
			}
		}
	}

	return true;
}

UShidenSplitterBoxSlot* UShidenEditorBlueprintLibrary::SlotAsShidenSplitterBoxSlot(const UWidget* Widget)
{
	if (Widget)
	{
		return Cast<UShidenSplitterBoxSlot>(Widget->Slot);
	}

	return nullptr;
}

bool UShidenEditorBlueprintLibrary::TryEvaluateInputVisibility(const UShidenScenario* Scenario, const TMap<FString, FString>& ExpandedCommandArgs, const FString& EditorVisibilityCondition,
                                                               bool& OutShouldShow, FString& ErrorMessage)
{
	// If bShowAllVisibilityConditionItems is enabled, always show all items
	const TObjectPtr<const UShidenEditorUserConfig> UserConfig = GetDefault<UShidenEditorUserConfig>();
	if (UserConfig->bForceShowAllCommandArguments)
	{
		OutShouldShow = true;
		return true;
	}

	// Empty condition means always visible
	if (EditorVisibilityCondition.IsEmpty())
	{
		OutShouldShow = true;
		return true;
	}

	const FShidenExpressionVariableDefinitionContext Context = BuildExpressionContext(Scenario);

	const FString ReplacedExpression = ReplaceArgumentReferences(ExpandedCommandArgs, EditorVisibilityCondition);

	const FShidenExpressionEvaluator Evaluator(Context);
	FShidenExpressionValue Result;
	if (!Evaluator.TryEvaluate(ReplacedExpression, Result, ErrorMessage))
	{
		OutShouldShow = false;
		return false;
	}

	if (Result.Type != EShidenExpressionValueType::Boolean)
	{
		ErrorMessage = FString::Printf(TEXT("EditorVisibilityCondition must evaluate to a boolean value, got %s"), *ReplacedExpression);
		OutShouldShow = false;
		return false;
	}

	OutShouldShow = Result.BoolValue;
	return true;
}

void UShidenEditorBlueprintLibrary::CheckUndefinedVariables(const FShidenExpressionVariableDefinitionContext& Context, const FString& CommandArg,
                                                            TArray<FText>& OutErrorMessages)
{
	const TSet VariableNames(UShidenVariableBlueprintLibrary::GetVariableNamesFromText(CommandArg));
	FShidenVariableDefinition Definition;

	for (const FString& VariableRef : VariableNames)
	{
		if (VariableRef.Contains(TEXT("::")))
		{
			FString Kind, Name;
			VariableRef.Split(TEXT("::"), &Kind, &Name, ESearchCase::CaseSensitive);
			Kind = Kind.TrimStartAndEnd();
			Name = Name.TrimStartAndEnd().Replace(TEXT("\\:"), TEXT(":"));
			if (!Context.TryFindVariable(Kind, Name, Definition))
			{
				OutErrorMessages.Add(FText::Format(
					LOCTEXT("UndefinedVariable", "Undefined {0} Variable: \"{1}\""),
					FText::FromString(Kind),
					FText::FromString(Name)
				));
			}
		}
		else
		{
			const FString Name = VariableRef.TrimStartAndEnd().Replace(TEXT("\\:"), TEXT(":"));
			if (!Context.TryFindVariable(TEXT(""), Name, Definition))
			{
				OutErrorMessages.Add(FText::Format(
					LOCTEXT("UndefinedUserVariable", "Undefined User Variable: \"{0}\""),
					FText::FromString(Name)
				));
			}
		}
	}
}

FShidenExpressionVariableDefinitionContext UShidenEditorBlueprintLibrary::BuildExpressionContext(const UShidenScenario* Scenario)
{
	FShidenExpressionVariableDefinitionContext Context;

	// Add scenario-specific variables if provided
	if (IsValid(Scenario))
	{
		for (const FShidenVariableDefinition& Def : Scenario->LocalVariableDefinitions)
		{
			Context.LocalVariables.Add(Def.Name, Def);
		}

		for (const FShidenMacroParameter& Param : Scenario->MacroParameterDefinitions)
		{
			Context.MacroParameters.Add(Param.Name, Param);
		}

		// Determine if scenario is in macro directory
		const TObjectPtr<const UShidenProjectConfig> ProjectConfig = GetDefault<UShidenProjectConfig>();
		const FString ScenarioPath = Scenario->GetPathName();
		Context.bIsMacro = !ProjectConfig->MacroDirectoryPath.IsEmpty() && ScenarioPath.StartsWith(ProjectConfig->MacroDirectoryPath);
	}
	else
	{
		Context.bIsMacro = false;
	}

	return Context;
}

TArray<FShidenCommandRedirector> UShidenEditorBlueprintLibrary::GetRedirectDefinitions(const FShidenPluginVersion& SourcePluginVersion)
{
	const TObjectPtr<const UShidenEditorConfig> EditorConfig = GetDefault<UShidenEditorConfig>();
	TArray<FShidenCommandRedirector> Redirects = UShidenCommandRedirectors::GetBuiltIn(SourcePluginVersion);
	for (const FSoftObjectPath& RedirectorPath : EditorConfig->CommandRedirectors)
	{
		if (const TObjectPtr<UShidenCommandRedirectors> RedirectAsset = Cast<UShidenCommandRedirectors>(RedirectorPath.TryLoad()))
		{
			for (const FShidenCommandRedirector& Redirect : RedirectAsset->Redirectors)
			{
				Redirects.Add(Redirect);
			}
		}
	}
	return Redirects;
}

bool UShidenEditorBlueprintLibrary::TryCreateOrReplaceScenarioAsset(const FString& SourceFileName, const FString& Content, const FString& Format, FString& OutPackageName, UShidenScenario*& OutScenario)
{
	OutScenario = nullptr;
	FShidenPluginVersion SourceVersion = FShidenPluginVersion(0, 0, 0);

	// Parse content into a transient scenario object
	UShidenScenario* ParsedScenario = nullptr;
	if (Format.Equals(TEXT("CSV"), ESearchCase::IgnoreCase))
	{
		ParsedScenario = ConvertToScenarioFromCsv(Content);

		// Extract plugin version from CSV comment (e.g., #PluginVersion 1.0.0)
		TArray<FString> Lines;
		Content.ParseIntoArrayLines(Lines, true);
		for (const FString& Line : Lines)
		{
			if (Line.StartsWith(TEXT("#PluginVersion ")))
			{
				const FString VersionStr = Line.RightChop(15).TrimStartAndEnd();
				TryParseVersionString(VersionStr, SourceVersion);
				break;
			}
		}
	}
	else
	{
		if (!TryConvertToScenarioFromJson(Content, ParsedScenario))
		{
			return false;
		}

		// Extract plugin version from JSON field "PluginVersion"
		TSharedPtr<FJsonObject> JsonObject;
		const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Content);
		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			FString VersionStr;
			if (JsonObject->TryGetStringField(TEXT("PluginVersion"), VersionStr) && !VersionStr.IsEmpty())
			{
				TryParseVersionString(VersionStr, SourceVersion);
			}
		}
	}

	if (!ParsedScenario)
	{
		return false;
	}

	const bool bIsMacro = ParsedScenario->MacroParameterDefinitions.Num() > 0;
	const FString DefaultPath = bIsMacro ? GetDefault<UShidenProjectConfig>()->MacroDirectoryPath : GetDefault<UShidenProjectConfig>()->ScenarioDirectoryPath;
	FString DefaultAssetName(SourceFileName);
	DefaultAssetName.Split(TEXT("."), &DefaultAssetName, nullptr);
	FString PackagePath, AssetName;
	bool bAssetNameWasSet;
	ShowSaveAssetDialog(UShidenScenario::StaticClass(), DefaultPath, DefaultAssetName, PackagePath, AssetName, bAssetNameWasSet);
	if (!bAssetNameWasSet)
	{
		return false;
	}

	// Redirect commands to the current plugin version
	bool bAnyCommandUpdated = false;
	RedirectCommands(ParsedScenario, SourceVersion, bAnyCommandUpdated);

	// Build full package/object paths e.g. /Game/Shiden/Scenarios/MyScenario.MyScenario
	OutPackageName = PackagePath / AssetName;
	const FString ObjectPath = FString::Printf(TEXT("%s.%s"), *OutPackageName, *AssetName);
	if (!FPackageName::IsValidObjectPath(ObjectPath))
	{
		SHIDEN_ERROR("Invalid scenario asset path: {objectPath}", *ObjectPath);
		return false;
	}

	const TObjectPtr<UEditorAssetSubsystem> EditorAssetSubsystem = GEditor->GetEditorSubsystem<UEditorAssetSubsystem>();
	if (!EditorAssetSubsystem)
	{
		return false;
	}

	// Check if an asset already exists at the target path
	if (EditorAssetSubsystem->DoesAssetExist(OutPackageName))
	{
		const FSoftObjectPath ExistingScenarioPath(ObjectPath);
		const TObjectPtr<UShidenScenario> ExistingScenario = Cast<UShidenScenario>(ExistingScenarioPath.TryLoad());
		if (!ExistingScenario)
		{
			return false;
		}

		// Overwrite existing asset: preserve its ScenarioId and update data
		FScopedTransaction Transaction(NSLOCTEXT("ShidenNamespace", "ImportScenario", "Import Scenario"));
		ExistingScenario->Modify();
		ExistingScenario->Note = ParsedScenario->Note;
		ExistingScenario->Commands = ParsedScenario->Commands;
		ExistingScenario->MacroParameterDefinitions = ParsedScenario->MacroParameterDefinitions;
		ExistingScenario->LocalVariableDefinitions = ParsedScenario->LocalVariableDefinitions;
		ExistingScenario->WidgetClassOverride = ParsedScenario->WidgetClassOverride;
		// ReSharper disable once CppExpressionWithoutSideEffects
		ExistingScenario->MarkPackageDirty();

		if (!EditorAssetSubsystem->SaveLoadedAsset(ExistingScenario))
		{
			return false;
		}

		OutScenario = ExistingScenario;
		return true;
	}

	// Create a new asset at the target path
	const TObjectPtr<UPackage> Package = CreatePackage(*OutPackageName);
	if (!Package)
	{
		return false;
	}

	const TObjectPtr<UShidenScenario> NewScenario = NewObject<UShidenScenario>(Package, *AssetName, RF_Public | RF_Standalone);
	if (!NewScenario)
	{
		return false;
	}

	// Handle ScenarioId conflicts: regenerate if the same GUID is already registered at a different path
	const TObjectPtr<const UShidenProjectConfig> ProjectConfig = GetDefault<UShidenProjectConfig>();
	if (ProjectConfig && ProjectConfig->ScenarioPaths.Contains(ParsedScenario->ScenarioId))
	{
		ParsedScenario->ScenarioId = FGuid::NewGuid();
	}

	NewScenario->ScenarioId = ParsedScenario->ScenarioId;
	NewScenario->Note = ParsedScenario->Note;
	NewScenario->Commands = ParsedScenario->Commands;
	NewScenario->MacroParameterDefinitions = ParsedScenario->MacroParameterDefinitions;
	NewScenario->LocalVariableDefinitions = ParsedScenario->LocalVariableDefinitions;
	NewScenario->WidgetClassOverride = ParsedScenario->WidgetClassOverride;
	// ReSharper disable once CppExpressionWithoutSideEffects
	Package->MarkPackageDirty();

	if (!EditorAssetSubsystem->SaveLoadedAsset(NewScenario))
	{
		return false;
	}

	OutScenario = NewScenario;
	return true;
}

#undef LOCTEXT_NAMESPACE
