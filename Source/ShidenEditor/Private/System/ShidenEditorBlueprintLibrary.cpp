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
#include "Variable/ShidenVariableBlueprintLibrary.h"

#define LOCTEXT_NAMESPACE "AssetTools"

SHIDENEDITOR_API void UShidenEditorBlueprintLibrary::ShowSaveAssetDialog(const UClass* AssetClass, const FString& DefaultPath,
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

SHIDENEDITOR_API bool UShidenEditorBlueprintLibrary::CanCreateFolder(const FName& Path)
{
	const TObjectPtr<UContentBrowserDataSubsystem> ContentBrowserData = IContentBrowserDataModule::Get().GetSubsystem();
	return ContentBrowserData->CanCreateFolder(Path, nullptr);
}

SHIDENEDITOR_API TArray<FString> UShidenEditorBlueprintLibrary::CreateSortedStringArray(const TArray<FString>& Array)
{
	TArray<FString> OutArray = Array;
	OutArray.Sort();
	return OutArray;
}

SHIDENEDITOR_API bool UShidenEditorBlueprintLibrary::TryLoadTextFile(const FString& Extension, FString& FileData, FString& FileName)
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
			FFileHelper::LoadFileToString(FileData, *LoadFilePath);
			return true;
		}
	}

	return false;
}

SHIDENEDITOR_API bool UShidenEditorBlueprintLibrary::TrySaveTextFile(const FString& DefaultFileName, const FString& SaveText, const FString& Extension)
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

SHIDENEDITOR_API void UShidenEditorBlueprintLibrary::ParseCsvContent(const FString& CsvText, TArray<FShidenCsvParsedRow>& CsvParsedRow)
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

SHIDENEDITOR_API void UShidenEditorBlueprintLibrary::SetDefaultClassProperty(const UClass* TargetClass, const FName& PropertyName, UClass* Value)
{
	FProperty* Prop = TargetClass->FindPropertyByName(PropertyName);
	const FClassProperty* ClassProperty = CastField<FClassProperty>(Prop);
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

FShidenCommand UShidenEditorBlueprintLibrary::ExpandCommand(const FShidenCommand& SourceCommand, const bool bShouldExpandPreset, const bool bShouldExpandDefaultValue)
{
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

		// If all Args are empty, fill them with default values.
		const int32 NotEmptyArgCount = SourceCommand.Args.FilterByPredicate([](const TPair<FString, FString>& Pair)
		{
			return !Pair.Value.IsEmpty();
		}).Num();

		if (NotEmptyArgCount > 0)
		{
			ExpandedCommand.Args = SourceCommand.Args;
			return ExpandedCommand;
		}

		for (const FShidenCommandArgument& Arg : CommandDefinitions[SourceCommand.CommandName].Args)
		{
			FString Key = Arg.ArgName.ToString();
			ExpandedCommand.Args.Add(Key, Arg.DefaultValue);

			// If the HasAdditionalArgs Property of CommandArguments[Index] is true, add MacroArguments
			if (Arg.EditorSettings.TemplateParameters.FindRef(TEXT("HasAdditionalArgs")).Compare(TEXT("true"), ESearchCase::IgnoreCase) == 0)
			{
				// load scenario
				UShidenScenario* MacroScenario;
				FGuid ScenarioId;
				if (UShidenScenarioBlueprintLibrary::TryGetScenarioByIdOrObjectPath(Arg.DefaultValue, ScenarioId, MacroScenario))
				{
					for (const FShidenMacroParameter& MacroParam : MacroScenario->MacroParameterDefinitions)
					{
						ExpandedCommand.Args.Add(MacroParam.Name, MacroParam.DefaultValue);
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

	for (const FShidenCommandArgument& Arg : CommandDefinitions[SourceCommand.CommandName].Args)
	{
		FString Key = Arg.ArgName.ToString();
		FString Value = SourceCommand.Args.Contains(Key) && !SourceCommand.Args[Key].IsEmpty()
			                ? SourceCommand.Args[Key]
			                : CommandPreset->Args[Key];
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
					ExpandedCommand.Args.Add(MacroParam.Name, SourceCommand.GetArg(MacroParam.Name));
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
	for (const FShidenCommand& Command : SourceScenario->Commands)
	{
		ExpandedScenario->Commands.Add(UShidenEditorBlueprintLibrary::ExpandCommand(Command, true));
	}
	return ExpandedScenario;
}

UShidenScenario* RemovePresetValues(const UShidenScenario* SourceScenario)
{
	const TObjectPtr<const UShidenProjectConfig> Config = GetDefault<UShidenProjectConfig>();
	const FGuid ScenarioId = SourceScenario->ScenarioId;
	const TObjectPtr<UShidenScenario> RemovedScenario = DuplicateObject(SourceScenario, GetTransientPackage());
	// Scenario ID was lost during duplication, so restore it.
	RemovedScenario->ScenarioId = ScenarioId;
	RemovedScenario->Commands.Empty();
	for (const auto& [CommandId, bEnabled, CommandName, PresetName, Args] : SourceScenario->Commands)
	{
		FShidenCommand RemovedCommand;
		RemovedCommand.CommandId = CommandId;
		RemovedCommand.bEnabled = bEnabled;
		RemovedCommand.CommandName = CommandName;
		RemovedCommand.PresetName = PresetName;
		RemovedCommand.Args = Args;
		if (!PresetName.IsEmpty())
		{
			const FShidenPreset* CommandPreset = Config->Presets.Find(PresetName);
			if (CommandPreset && CommandPreset->CommandName == CommandName)
			{
				for (const auto& [Key, Value] : CommandPreset->Args)
				{
					if (RemovedCommand.Args.Contains(Key) && RemovedCommand.Args[Key] == Value)
					{
						RemovedCommand.Args[Key] = TEXT("");
					}
				}
			}
		}
		RemovedScenario->Commands.Add(RemovedCommand);
	}
	return RemovedScenario;
}

SHIDENEDITOR_API UShidenScenario* UShidenEditorBlueprintLibrary::ConvertToScenarioFromCsv(const FString& CsvString)
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
			FString Value = Row.IsValidIndex(Index + 4) ? Row[Index + 4] : TEXT("");
			Command.Args.Add(Args[Index].ArgName.ToString(), Value);

			// If the HasAdditionalArgs Property of Args[Index] is true, add MacroArguments
			if (Args[Index].EditorSettings.TemplateParameters.FindRef(TEXT("HasAdditionalArgs")).Compare(TEXT("true"), ESearchCase::IgnoreCase) == 0)
			{
				if (Value.IsEmpty())
				{
					// Get scenario id or path from expanded command
					FShidenCommand TempCommand = ExpandCommand(Command, true, true);
					Value = TempCommand.Args[Args[Index].ArgName.ToString()];
				}
				
				FGuid MacroScenarioId;
				UShidenScenario* MacroScenario;
				if (!UShidenScenarioBlueprintLibrary::TryGetScenarioByIdOrObjectPath(Value, MacroScenarioId, MacroScenario))
				{
					continue;
				}

				for (const FShidenVariableDefinition& MacroParameterDefinition : MacroScenario->MacroParameterDefinitions)
				{
					Index++;
					Value = Row.IsValidIndex(Index + 4) ? Row[Index + 4] : TEXT("{EMPTY}");
					Command.Args.Add(MacroParameterDefinition.Name, Value);
				}
			}
		}

		Scenario->Commands.Add(Command);
	}

	return RemovePresetValues(Scenario);
}

SHIDENEDITOR_API FString UShidenEditorBlueprintLibrary::ConvertToCsvFromScenario(const UShidenScenario* SourceScenario)
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

	// Get max column count
	int32 MaxArgCount = 0;
	for (const FShidenCommand& Command : Scenario->Commands)
	{
		MaxArgCount = FMath::Max(MaxArgCount, Command.Args.Num());
	}

	// Add header
	FString Header = TEXT("CommandId,Enabled,CommandName,PresetName");
	for (int32 Index = 0; Index < MaxArgCount; Index++)
	{
		Header += TEXT(",Arg") + FString::FromInt(Index + 1);
	}
	CsvRows.Add(Header);

	// Get Command Definitions
	const TMap<FString, FShidenCommandDefinition> CommandDefinitions = UShidenBlueprintLibrary::GetCommandDefinitionsCache();

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
				Row += TEXT(",") + EscapeCsvItem(Value);
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
				Row += TEXT(",") + EscapeCsvItem(Arg);

				// If the HasAdditionalArgs Property of CommandArguments[Index] is true, add MacroArguments
				if (CommandArguments[Index].EditorSettings.TemplateParameters.FindRef(TEXT("HasAdditionalArgs")).Compare(TEXT("true"), ESearchCase::IgnoreCase) == 0)
				{
					if (Arg.IsEmpty())
					{
						// Get scenario id or path from expanded command
						FShidenCommand TempCommand = ExpandCommand(Command, true, true);
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
							Row += TEXT(",") + EscapeCsvItem(MacroArg);
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

SHIDENEDITOR_API bool UShidenEditorBlueprintLibrary::TryConvertToScenarioFromJson(const FString& Json, UShidenScenario*& Scenario)
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

	Scenario = RemovePresetValues(ScenarioStruct.ToShidenScenario());
	return true;
}

SHIDENEDITOR_API bool UShidenEditorBlueprintLibrary::TryConvertToJsonFromScenario(const UShidenScenario* SourceScenario, FString& Json)
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
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	ShidenSubsystem->UserVariable.ListDescriptors(VariableDescriptors);
}

void UShidenEditorBlueprintLibrary::ListPredefinedSystemVariableDescriptors(TArray<FShidenVariableDescriptor>& VariableDescriptors)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	ShidenSubsystem->PredefinedSystemVariable.ListDescriptors(VariableDescriptors);
}

void UShidenEditorBlueprintLibrary::ListSystemVariableDescriptors(TArray<FShidenVariableDescriptor>& VariableDescriptors)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	ShidenSubsystem->SystemVariable.ListDescriptors(VariableDescriptors);
}

void UShidenEditorBlueprintLibrary::ListLocalVariableDescriptors(TArray<FShidenVariableDescriptor>& VariableDescriptors)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	ShidenSubsystem->LocalVariable.ListDescriptors(VariableDescriptors);
}

void UShidenEditorBlueprintLibrary::ForceGC()
{
	GEngine->ForceGarbageCollection(true);
}

void UShidenEditorBlueprintLibrary::AddUserVariableDefinition(const FShidenVariableDefinition& VariableDefinition)
{
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

	ProjectConfig->SaveConfig(CPF_Config, *ProjectConfig->GetDefaultConfigFilename());
	ProjectConfig->TryUpdateDefaultConfigFile();

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	ShidenSubsystem->UserVariable.UpdateVariableDefinitions(ProjectConfig->UserVariableDefinitions);
}

void UShidenEditorBlueprintLibrary::UpdateUserVariableDefinitions(const TArray<FShidenVariableDefinition>& VariableDefinitions)
{
	const TObjectPtr<UShidenProjectConfig> ProjectConfig = GetMutableDefault<UShidenProjectConfig>();

	ProjectConfig->UserVariableDefinitions = VariableDefinitions;
	ProjectConfig->SaveConfig(CPF_Config, *ProjectConfig->GetDefaultConfigFilename());
	ProjectConfig->TryUpdateDefaultConfigFile();

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	ShidenSubsystem->UserVariable.UpdateVariableDefinitions(ProjectConfig->UserVariableDefinitions);
}

void UShidenEditorBlueprintLibrary::UpdateUserVariableDefinition(const FString& OldName, const FShidenVariableDefinition& VariableDefinition)
{
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

	ProjectConfig->SaveConfig(CPF_Config, *ProjectConfig->GetDefaultConfigFilename());
	ProjectConfig->TryUpdateDefaultConfigFile();

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	ShidenSubsystem->UserVariable.UpdateVariableDefinitions(ProjectConfig->UserVariableDefinitions);
}

void UShidenEditorBlueprintLibrary::RemoveUserVariableDefinition(const FString& Name)
{
	const TObjectPtr<UShidenProjectConfig> ProjectConfig = GetMutableDefault<UShidenProjectConfig>();

	ProjectConfig->UserVariableDefinitions.RemoveAll([Name](const FShidenVariableDefinition& VariableDefinition)
	{
		return VariableDefinition.Name == Name;
	});
	ProjectConfig->SaveConfig(CPF_Config, *ProjectConfig->GetDefaultConfigFilename());
	ProjectConfig->TryUpdateDefaultConfigFile();

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	ShidenSubsystem->UserVariable.UpdateVariableDefinitions(ProjectConfig->UserVariableDefinitions);
}

void UShidenEditorBlueprintLibrary::AddSystemVariableDefinition(const FShidenVariableDefinition& VariableDefinition)
{
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

	ProjectConfig->SaveConfig(CPF_Config, *ProjectConfig->GetDefaultConfigFilename());
	ProjectConfig->TryUpdateDefaultConfigFile();

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	ShidenSubsystem->SystemVariable.UpdateVariableDefinitions(ProjectConfig->SystemVariableDefinitions);
}

void UShidenEditorBlueprintLibrary::UpdateSystemVariableDefinitions(const TArray<FShidenVariableDefinition>& VariableDefinitions)
{
	const TObjectPtr<UShidenProjectConfig> ProjectConfig = GetMutableDefault<UShidenProjectConfig>();

	ProjectConfig->SystemVariableDefinitions = VariableDefinitions;
	ProjectConfig->SaveConfig(CPF_Config, *ProjectConfig->GetDefaultConfigFilename());
	ProjectConfig->TryUpdateDefaultConfigFile();

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	ShidenSubsystem->SystemVariable.UpdateVariableDefinitions(ProjectConfig->SystemVariableDefinitions);
}

void UShidenEditorBlueprintLibrary::UpdateSystemVariableDefinition(const FString& OldName, const FShidenVariableDefinition& VariableDefinition)
{
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

	ProjectConfig->SaveConfig(CPF_Config, *ProjectConfig->GetDefaultConfigFilename());
	ProjectConfig->TryUpdateDefaultConfigFile();

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	ShidenSubsystem->SystemVariable.UpdateVariableDefinitions(ProjectConfig->SystemVariableDefinitions);
}

void UShidenEditorBlueprintLibrary::RemoveSystemVariableDefinition(const FString& Name)
{
	const TObjectPtr<UShidenProjectConfig> ProjectConfig = GetMutableDefault<UShidenProjectConfig>();

	ProjectConfig->SystemVariableDefinitions.RemoveAll([Name](const FShidenVariableDefinition& VariableDefinition)
	{
		return VariableDefinition.Name == Name;
	});
	ProjectConfig->SaveConfig(CPF_Config, *ProjectConfig->GetDefaultConfigFilename());
	ProjectConfig->TryUpdateDefaultConfigFile();

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	ShidenSubsystem->SystemVariable.UpdateVariableDefinitions(ProjectConfig->SystemVariableDefinitions);
}

void UShidenEditorBlueprintLibrary::GetPredefinedSystemVariableDefinitions(TArray<FShidenVariableDefinition>& VariableDefinitions)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	VariableDefinitions.Empty();
	for (const FShidenPredefinedSystemVariableDefinition& Definition : ShidenSubsystem->PredefinedSystemVariable.Definitions)
	{
		VariableDefinitions.Add(Definition);
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

	if (Redirects.Num() == 0)
	{
		AnyCommandUpdated = false;
		return;
	}

	AnyCommandUpdated = false;
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
			if (Command.GetArg(TargetCommand[Command.CommandName].Get<0>()) == TEXT("LocalVariable")
				&& Command.GetArg(TargetCommand[Command.CommandName].Get<1>()) == OldVariableName)
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
					const FString CommandVariableKindValue = Command.GetArg(TargetCommand[Command.CommandName].Get<0>());
					const FString CommandVariableNameValue = Command.GetArg(TargetCommand[Command.CommandName].Get<1>());
					if (CommandVariableKindValue == VariableKindStr && CommandVariableNameValue == OldVariableName)
					{
						Command.Args[TargetCommand[Command.CommandName].Get<1>()] = NewVariableName;
						bChanged = true;
					}
				}
			}

			if (bChanged)
			{
				EditorAssetSubsystem->SaveLoadedAsset(Scenario);
			}
		}
	}
}

bool UShidenEditorBlueprintLibrary::TryMigratePlugin()
{
	const TObjectPtr<UEditorAssetSubsystem> EditorAssetSubsystem = GEditor->GetEditorSubsystem<UEditorAssetSubsystem>();
	const TObjectPtr<UShidenEditorConfig> EditorConfig = GetMutableDefault<UShidenEditorConfig>();
	const FShidenPluginVersion CurrentVersion = EditorConfig->PluginVersion;

	// Migration
	if (CurrentVersion < FShidenPluginVersion(0, 16, 15))
	{
		// Migrate variables
		// Migrate UserVariableDefinitions
		const TObjectPtr<UShidenProjectConfig> ProjectConfig = GetMutableDefault<UShidenProjectConfig>();
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

		ProjectConfig->SaveConfig(CPF_Config, *ProjectConfig->GetDefaultConfigFilename());
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

	// if (CurrentVersion < FShidenPluginVersion(1, 10, 0))
	// {
	// 	const FText DialogTitle = NSLOCTEXT("ShidenNamespace", "AdvancedModeTitle", "Enable Advanced Mode");
	// 	const FText DialogMessage = NSLOCTEXT("ShidenNamespace", "AdvancedModeMessage",
	// 	                                      "Do you want to enable advanced mode?\r\n\r\n"
	// 	                                      "In advanced mode, all editing items in Shiden Visual Novel Editor are displayed.");
	//
	// 	const EAppReturnType::Type Response = FMessageDialog::Open(EAppMsgType::YesNo, DialogMessage, DialogTitle);
	//
	// 	if (Response == EAppReturnType::No)
	// 	{
	// 		EditorConfig->bUseAdvancedMode = false;
	// 		EditorConfig->SaveConfig(CPF_Config, *EditorConfig->GetDefaultConfigFilename());
	// 		EditorConfig->TryUpdateDefaultConfigFile();
	// 	}
	// }

	// Update pluginVersion
	FShidenPluginVersion NewVersion;
	if (!TryGetCurrentPluginVersion(NewVersion))
	{
		SHIDEN_ERROR("Failed to get current plugin version");
		return false;
	}

	EditorConfig->PluginVersion = NewVersion;
	EditorConfig->SaveConfig(CPF_Config, *EditorConfig->GetDefaultConfigFilename());
	EditorConfig->TryUpdateDefaultConfigFile();
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
	FString Result = Expression;

	// Replace each argument reference {ArgName} or { ArgName } with its value
	for (const TPair<FString, FString>& Arg : CommandArgs)
	{
		// Create regex pattern to match {  ArgName  } with optional whitespace
		const FString Pattern = FString::Printf(TEXT("\\{\\s*%s\\s*\\}"), *Arg.Key);
		FRegexPattern RegexPattern(Pattern);

		while (true)
		{
			FRegexMatcher Matcher(RegexPattern, Result);
			if (!Matcher.FindNext())
			{
				break;
			}

			const int32 BeginIndex = Matcher.GetMatchBeginning();
			const int32 EndIndex = Matcher.GetMatchEnding();

			Result = Result.Left(BeginIndex) + "\"" + Arg.Value + "\"" + Result.Mid(EndIndex);
		}
	}

	return Result;
}

bool UShidenEditorBlueprintLibrary::TryEvaluateConditionalMessagesCore(const FShidenExpressionVariableDefinitionContext& Context, const TMap<FString, FString>& CommandArgs,
                                                                       const TArray<FShidenConditionalMessage>& InformationConditionalMessages, const TArray<FShidenConditionalMessage>& WarningConditionalMessages,
                                                                       const TArray<FShidenConditionalMessage>& ErrorConditionalMessages, TArray<FText>& OutInformationMessages, TArray<FText>& OutWarningMessages,
                                                                       TArray<FText>& OutErrorMessages, FString& ErrorMessage)
{
	auto TryEvaluateMessages = [](const FShidenExpressionVariableDefinitionContext& InContext, const TMap<FString, FString>& InCommandArgs,
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

			const FString ReplacedExpression = ReplaceArgumentReferences(InCommandArgs, ConditionalMessage.Condition);

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

	TArray<FText> InformationMessages;
	FString InformationErrorMessage;
	if (!TryEvaluateMessages(Context, CommandArgs, InformationConditionalMessages, InformationMessages, InformationErrorMessage))
	{
		ErrorMessage = InformationErrorMessage;
		return false;
	}
	OutInformationMessages.Append(InformationMessages);

	TArray<FText> WarningMessages;
	FString WarningErrorMessage;
	if (!TryEvaluateMessages(Context, CommandArgs, WarningConditionalMessages, WarningMessages, WarningErrorMessage))
	{
		ErrorMessage = WarningErrorMessage;
		return false;
	}
	OutWarningMessages.Append(WarningMessages);

	TArray<FText> ErrorMessages;
	FString ErrorErrorMessage;
	if (!TryEvaluateMessages(Context, CommandArgs, ErrorConditionalMessages, ErrorMessages, ErrorErrorMessage))
	{
		ErrorMessage = ErrorErrorMessage;
		return false;
	}
	OutErrorMessages.Append(ErrorMessages);

	return true;
}

bool UShidenEditorBlueprintLibrary::TryEvaluateConditionalMessages(const UShidenScenario* Scenario, const FShidenCommandDefinition& CommandDefinition, const TMap<FString, FString>& CommandArgs,
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
		return TryEvaluateConditionalMessagesCore(Context, CommandArgs, Arg->EditorSettings.InformationMessages,
		                                          Arg->EditorSettings.WarningMessages, Arg->EditorSettings.ErrorMessages,
		                                          OutInformationMessages, OutWarningMessages, OutErrorMessages, ErrorMessage);
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

	const FString ScenarioIdOrObjectPath = CommandArgs.FindRef(MacroScenarioArg->ArgName.ToString());
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

	return TryEvaluateConditionalMessagesCore(Context, CommandArgs, MacroArg->EditorSettings.InformationMessages,
	                                          MacroArg->EditorSettings.WarningMessages, MacroArg->EditorSettings.ErrorMessages,
	                                          OutInformationMessages, OutWarningMessages, OutErrorMessages, ErrorMessage);
}

bool UShidenEditorBlueprintLibrary::TryValidateCommand(const UShidenScenario* Scenario, const FShidenCommandDefinition& CommandDefinition, const TMap<FString, FString>& CommandArgs,
                                                       bool& HasInformation, bool& HasWarning, bool& HasError, FString& ErrorMessage)
{
	// Build context once for all validations (performance optimization)
	const FShidenExpressionVariableDefinitionContext Context = BuildExpressionContext(Scenario);

	for (const FShidenCommandArgument& Arg : CommandDefinition.Args)
	{
		TArray<FText> ArgInformationMessages;
		TArray<FText> ArgWarningMessages;
		TArray<FText> ArgErrorMessages;

		if (!TryEvaluateConditionalMessagesCore(Context, CommandArgs, Arg.EditorSettings.InformationMessages,
		                                        Arg.EditorSettings.WarningMessages, Arg.EditorSettings.ErrorMessages,
		                                        ArgInformationMessages, ArgWarningMessages, ArgErrorMessages, ErrorMessage))
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
			const FString ScenarioIdOrObjectPath = CommandArgs.FindRef(Arg.ArgName.ToString());
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
					if (!TryEvaluateConditionalMessagesCore(Context, CommandArgs, MacroArg.EditorSettings.InformationMessages,
					                                        MacroArg.EditorSettings.WarningMessages, MacroArg.EditorSettings.ErrorMessages,
					                                        ArgInformationMessages, ArgWarningMessages, ArgErrorMessages, ErrorMessage))
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

bool UShidenEditorBlueprintLibrary::TryEvaluateInputVisibility(const UShidenScenario* Scenario, const TMap<FString, FString>& ExpandedCommandArgs, const FString& EditorVisibilityCondition,
                                                               bool& OutShouldShow, FString& ErrorMessage)
{
	// If bShowAllVisibilityConditionItems is enabled, always show all items
	const TObjectPtr<const UShidenEditorConfig> EditorConfig = GetDefault<UShidenEditorConfig>();
	if (EditorConfig->bShowAllVisibilityConditionItems)
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

FShidenExpressionVariableDefinitionContext UShidenEditorBlueprintLibrary::BuildExpressionContext(const UShidenScenario* Scenario)
{
	FShidenExpressionVariableDefinitionContext Context;

	// Add global variable definitions
	if (const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>())
	{
		TArray<FShidenVariableDescriptor> UserVariableDescriptors;
		ShidenSubsystem->UserVariable.ListDescriptors(UserVariableDescriptors);
		for (const FShidenVariableDescriptor& Descriptor : UserVariableDescriptors)
		{
			FShidenVariableDefinition Def
			{
				.Name = Descriptor.Name,
				.Type = Descriptor.Type,
				.AssetPathType = Descriptor.AssetPathType,
				.DefaultValue = Descriptor.DefaultValue,
				.bIsReadOnly = Descriptor.bIsReadOnly,
			};
			Context.UserVariables.Add(Descriptor.Name, Def);
		}

		TArray<FShidenVariableDescriptor> SystemVariableDescriptors;
		ShidenSubsystem->SystemVariable.ListDescriptors(SystemVariableDescriptors);
		for (const FShidenVariableDescriptor& Descriptor : SystemVariableDescriptors)
		{
			FShidenVariableDefinition Def
			{
				.Name = Descriptor.Name,
				.Type = Descriptor.Type,
				.AssetPathType = Descriptor.AssetPathType,
				.DefaultValue = Descriptor.DefaultValue,
				.bIsReadOnly = Descriptor.bIsReadOnly,
			};
			Context.SystemVariables.Add(Descriptor.Name, Def);
		}
	}

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
	}

	// Determine if scenario is in macro directory
	const TObjectPtr<const UShidenProjectConfig> ProjectConfig = GetDefault<UShidenProjectConfig>();
	const FString ScenarioPath = Scenario->GetPathName();
	Context.bIsMacro = !ProjectConfig->MacroDirectoryPath.IsEmpty() && ScenarioPath.StartsWith(ProjectConfig->MacroDirectoryPath);

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

#undef LOCTEXT_NAMESPACE
