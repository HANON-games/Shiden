// Copyright (c) 2025 HANON. All Rights Reserved.

#include "ShidenEditorBlueprintLibrary.h"
#include "AssetViewUtils.h"
#include "IContentBrowserSingleton.h"
#include "ContentBrowserModule.h"
#include "DesktopPlatformModule.h"
#include "IContentBrowserDataModule.h"
#include "IDesktopPlatform.h"
#include "ISettingsModule.h"
#include "JsonObjectConverter.h"
#include "Command/ShidenCommandDefinition.h"
#include "ShidenCommandRedirector.h"
#include "ShidenEditorConfig.h"
#include "ShidenEditorConstants.h"
#include "Command/ShidenStandardCommandDefinitions.h"
#include "Interfaces/IMainFrameModule.h"
#include "Save/ShidenSaveBlueprintLibrary.h"
#include "Scenario/ShidenScenario.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"
#include "Serialization/Csv/CsvParser.h"
#include "Subsystems/EditorAssetSubsystem.h"
#include "System/ShidenBlueprintLibrary.h"

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

UShidenScenario* ExpandPresets(const UShidenScenario* SourceScenario)
{
	const TMap<FString, FShidenCommandDefinition> CommandDefinitions = UShidenBlueprintLibrary::GetCommandDefinitionsCache();
	const UShidenProjectConfig* Config = GetDefault<UShidenProjectConfig>();
	TObjectPtr<UShidenScenario> ExpandedScenario = NewObject<UShidenScenario>();
	ExpandedScenario->ScenarioId = SourceScenario->ScenarioId;
	ExpandedScenario->Note = SourceScenario->Note;
	ExpandedScenario->MacroParameterDefinitions = SourceScenario->MacroParameterDefinitions;
	for (const auto& [CommandId, bEnabled, CommandName, PresetName, Args] : SourceScenario->Commands)
	{
		FShidenCommand ExpandedCommand;
		ExpandedCommand.CommandId = CommandId;
		ExpandedCommand.bEnabled = bEnabled;
		ExpandedCommand.CommandName = CommandName;
		ExpandedCommand.PresetName = PresetName;
		if (!PresetName.IsEmpty())
		{
			const FShidenPreset* CommandPreset = Config->Presets.Find(PresetName);
			if (CommandPreset && CommandPreset->CommandName == CommandName)
			{
				for (const auto& [ArgName, DisplayName, DefaultValue, TemplateWidget, Parameters, bIsAssetToBeLoaded] : CommandDefinitions[
					     CommandName].Args)
				{
					FString Key = ArgName.ToString();
					FString Value = Args.Contains(Key) && !Args[Key].IsEmpty()
						                ? Args[Key]
						                : CommandPreset->Args.Contains(Key)
											? CommandPreset->Args[Key]
											: DefaultValue;
					ExpandedCommand.Args.Add(Key, Value);
				}
			}
			else
			{
				ExpandedCommand.Args = Args;
			}
		}
		else
		{
			ExpandedCommand.Args = Args;
		}
		ExpandedScenario->Commands.Add(ExpandedCommand);
	}
	return ExpandedScenario;
}

UShidenScenario* RemovePresetValues(const UShidenScenario* SourceScenario)
{
	const TObjectPtr<const UShidenProjectConfig> Config = GetDefault<UShidenProjectConfig>();
	const TObjectPtr<UShidenScenario> RemovedScenario = NewObject<UShidenScenario>();
	RemovedScenario->ScenarioId = SourceScenario->ScenarioId;
	RemovedScenario->Note = SourceScenario->Note;
	RemovedScenario->MacroParameterDefinitions = SourceScenario->MacroParameterDefinitions;
	RemovedScenario->LocalVariableDefinitions = SourceScenario->LocalVariableDefinitions;
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
	TMap<FString, TObjectPtr<UShidenScenario>> MacroScenarioCache;
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
		FShidenCommand Command;
		FGuid CommandId;
		Command.CommandId = FGuid::Parse(Row[0], CommandId) ? CommandId : FGuid::NewGuid();
		Command.bEnabled = Row[1] == TEXT("true");
		Command.CommandName = Row[2];
		Command.PresetName = Row[3];
		
		if (!CommandDefinitions.Contains(Command.CommandName))
		{
			UE_LOG(LogTemp, Warning, TEXT("CommandName %s is not found in CommandDefinitions."), *Command.CommandName);
			continue;
		}
		
		TArray<FShidenCommandArgument> Args = CommandDefinitions[Command.CommandName].Args;
		for (int32 Index = 0; Index < Args.Num(); Index++)
		{
			FString Value = Row.IsValidIndex(Index + 4) ? Row[Index + 4] : TEXT("{EMPTY}");
			Command.Args.Add(Args[Index].ArgName.ToString(), Value);

			// If the HasAdditionalArgs Property of Args[Index] is true, add MacroArguments
			if (!Value.IsEmpty() && Args[Index].TemplateParameters.FindRef(TEXT("HasAdditionalArgs")).Compare(TEXT("true"), ESearchCase::IgnoreCase) == 0)
			{
				if (!MacroScenarioCache.Contains(Value))
				{
					// Load Macro Scenario
					FGuid MacroScenarioId;
					UShidenScenario* MacroScenario;
					if (UShidenScenarioBlueprintLibrary::TryGetScenarioByIdOrObjectPath(Value, MacroScenarioId, MacroScenario))
					{
						MacroScenarioCache.Add(Value, MacroScenario);
					}
				}
				
				const TObjectPtr<UShidenScenario> MacroScenario = MacroScenarioCache.FindRef(Value);
				if (!MacroScenario)
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

SHIDENEDITOR_API FString UShidenEditorBlueprintLibrary::ConvertToCsvFromScenario(const UShidenScenario* SourceScenario, const bool bExpandPresets)
{
	const UShidenScenario* Scenario = bExpandPresets ? ExpandPresets(SourceScenario) : SourceScenario;

	TArray<FString> CsvRows;

	// Add comments
	CsvRows.Add(TEXT("#ScenarioId ") + Scenario->ScenarioId.ToString());
	CsvRows.Add(TEXT("#Note ") + Scenario->Note);
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
	TMap<FString, TObjectPtr<UShidenScenario>> MacroScenarioCache;
	for (const auto& [CommandId, bEnabled, CommandName, PresetName, Args] : Scenario->Commands)
	{
		FString Row = CommandId.ToString() + TEXT(",") + (bEnabled ? TEXT("true") : TEXT("false")) + TEXT(",") + EscapeCsvItem(CommandName) +
			TEXT(",") + EscapeCsvItem(PresetName);
		
		if (!CommandDefinitions.Contains(CommandName))
		{
			UE_LOG(LogTemp, Warning, TEXT("CommandName %s is not found in CommandDefinitions."), *CommandName);
			for (const auto& [Key, Value] : Args)
			{
				Row += TEXT(",") + EscapeCsvItem(Value);
			}
			CsvRows.Add(Row);
			continue;
		}
		
		TArray<FShidenCommandArgument> CommandArguments = CommandDefinitions[CommandName].Args;
		for (int32 Index = 0; Index < MaxArgCount; Index++)
		{
			if (CommandArguments.IsValidIndex(Index) && Args.Contains(CommandArguments[Index].ArgName.ToString()))
			{
				FString Arg = Args[CommandArguments[Index].ArgName.ToString()];
				Row += TEXT(",") + EscapeCsvItem(Arg);

				// If the HasAdditionalArgs Property of CommandArguments[Index] is true, add MacroArguments
				if (!Arg.IsEmpty() && CommandArguments[Index].TemplateParameters.FindRef(TEXT("HasAdditionalArgs")).Compare(TEXT("true"), ESearchCase::IgnoreCase) == 0)
				{
					if (!MacroScenarioCache.Contains(Arg))
					{
						// Load Macro Scenario
						FGuid MacroScenarioId;
						UShidenScenario* MacroScenario;
						if (UShidenScenarioBlueprintLibrary::TryGetScenarioByIdOrObjectPath(Arg, MacroScenarioId, MacroScenario))
						{
							MacroScenarioCache.Add(Arg, MacroScenario);
						}
					}

					const TObjectPtr<UShidenScenario> MacroScenario = MacroScenarioCache.FindRef(Arg);
					if (!MacroScenario)
					{
						continue;
					}
					
					for (const FShidenVariableDefinition& MacroParameterDefinition : MacroScenario->MacroParameterDefinitions)
					{
						if (Args.Contains(MacroParameterDefinition.Name))
						{
							FString MacroArg = Args[MacroParameterDefinition.Name];
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

SHIDENEDITOR_API bool UShidenEditorBlueprintLibrary::TryConvertToJsonFromScenario(const UShidenScenario* SourceScenario, const bool bExpandPresets,
                                                                                  FString& Json)
{
	const TObjectPtr<const UShidenScenario> Scenario = bExpandPresets ? ExpandPresets(SourceScenario) : SourceScenario;
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
	check(ShidenSubsystem);

	ShidenSubsystem->UserVariable.ListDescriptors(VariableDescriptors);
}

void UShidenEditorBlueprintLibrary::ListPredefinedSystemVariableDescriptors(TArray<FShidenVariableDescriptor>& VariableDescriptors)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	ShidenSubsystem->PredefinedSystemVariable.ListDescriptors(VariableDescriptors);
}

void UShidenEditorBlueprintLibrary::ListSystemVariableDescriptors(TArray<FShidenVariableDescriptor>& VariableDescriptors)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	ShidenSubsystem->SystemVariable.ListDescriptors(VariableDescriptors);
}

void UShidenEditorBlueprintLibrary::ListLocalVariableDescriptors(TArray<FShidenVariableDescriptor>& VariableDescriptors)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

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
	check(ShidenSubsystem);
	
	ShidenSubsystem->UserVariable.UpdateVariableDefinitions(ProjectConfig->UserVariableDefinitions);
}

void UShidenEditorBlueprintLibrary::UpdateUserVariableDefinitions(const TArray<FShidenVariableDefinition>& VariableDefinitions)
{
	const TObjectPtr<UShidenProjectConfig> ProjectConfig = GetMutableDefault<UShidenProjectConfig>();
	
	ProjectConfig->UserVariableDefinitions = VariableDefinitions;
	ProjectConfig->SaveConfig(CPF_Config, *ProjectConfig->GetDefaultConfigFilename());
	ProjectConfig->TryUpdateDefaultConfigFile();

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	
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
	check(ShidenSubsystem);
	
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
	check(ShidenSubsystem);
	
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
	check(ShidenSubsystem);
	
	ShidenSubsystem->SystemVariable.UpdateVariableDefinitions(ProjectConfig->SystemVariableDefinitions);
}

void UShidenEditorBlueprintLibrary::UpdateSystemVariableDefinitions(const TArray<FShidenVariableDefinition>& VariableDefinitions)
{
	const TObjectPtr<UShidenProjectConfig> ProjectConfig = GetMutableDefault<UShidenProjectConfig>();
	
	ProjectConfig->SystemVariableDefinitions = VariableDefinitions;
	ProjectConfig->SaveConfig(CPF_Config, *ProjectConfig->GetDefaultConfigFilename());
	ProjectConfig->TryUpdateDefaultConfigFile();

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	
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
	check(ShidenSubsystem);
	
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
	check(ShidenSubsystem);
	
	ShidenSubsystem->UserVariable.UpdateVariableDefinitions(ProjectConfig->SystemVariableDefinitions);
}

void UShidenEditorBlueprintLibrary::GetPredefinedSystemVariableDefinitions(TArray<FShidenVariableDefinition>& VariableDefinitions)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	
	VariableDefinitions.Empty();
	for (const FShidenPredefinedSystemVariableDefinition& Definition : ShidenSubsystem->PredefinedSystemVariable.Definitions)
	{
		VariableDefinitions.Add(Definition);
	}
}

bool UShidenEditorBlueprintLibrary::TryGetCurrentPluginVersion(FShidenPluginVersion& PluginVersion)
{
	const FString PluginPath = FPaths::Combine(FPaths::ProjectPluginsDir(), ShidenEditorConstants::PluginRelativePath);
	FString JsonString;
	if (!FFileHelper::LoadFileToString(JsonString, *PluginPath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load plugin file: %s"), *PluginPath);
		return false;
	}

	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

	if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON from plugin file: %s"), *PluginPath);
		return false;
	}

	if (JsonObject->HasField(TEXT("VersionName")))
	{
		const FString VersionName = JsonObject->GetStringField(TEXT("VersionName"));
		TArray<FString> Versions;
		VersionName.ParseIntoArray(Versions, TEXT("."));
		if (Versions.Num() >= 3)
		{
			PluginVersion.Major = FCString::Atoi(*Versions[0]);
			PluginVersion.Minor = FCString::Atoi(*Versions[1]);
			PluginVersion.Patch = FCString::Atoi(*Versions[2]);
			return true;
		}

		UE_LOG(LogTemp, Error, TEXT("Invalid VersionName format: %s"), *VersionName);
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("No VersionName field found"));
	return false;
}

void UShidenEditorBlueprintLibrary::RedirectCommands(UShidenScenario* Scenario, bool& AnyCommandUpdated)
{
	static const TArray<FShidenCommandRedirector> Redirects = GetRedirectDefinitions();

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
			if (Arg.TemplateWidget == UShidenStandardCommandDefinitions::VariableNameInputTemplate
				&& Arg.TemplateParameters.Contains(TEXT("VariableKindSourceIndex")))
			{
				const int32 VariableKindIndex = FCString::Atoi(*Arg.TemplateParameters[TEXT("VariableKindSourceIndex")]);
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
		TArray<FString> ArgKeys;
		Command.Args.GetKeys(ArgKeys);
		for (int32 i = 0; i < ArgKeys.Num(); i++)
		{
			const FString Key = ArgKeys[i];
			if (!Command.Args[Key].Contains(TEXT("{")) || !Command.Args[Key].Contains(TEXT("}")))
			{
				continue;
			}
			FRegexMatcher Matcher(UShidenVariableBlueprintLibrary::GetReplaceTextPattern(), Command.Args[Key]);
			while (Matcher.FindNext())
			{
				const FString Str = Matcher.GetCaptureGroup(1);
				const FString VariableName = Str.Mid(1, Str.Len() - 2).TrimStartAndEnd();
				if (VariableName == OldVariableNameWithPrefix)
				{
					Command.Args[Key].ReplaceInline(*Str, *NewReplacementVariableName, ESearchCase::CaseSensitive);
					AnyCommandUpdated = true;
				}
			}
		}
		
		if (TargetCommand.Contains(Command.CommandName))
		{
			if (Command.Args.FindRef(TargetCommand[Command.CommandName].Get<0>()) == TEXT("LocalVariable")
				&& Command.Args.FindRef(TargetCommand[Command.CommandName].Get<1>()) == OldVariableName)
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
			if (Arg.TemplateWidget == UShidenStandardCommandDefinitions::ScenarioInputTemplate
				&& Arg.TemplateParameters.FindRef(TEXT("HasAdditionalArgs")).Compare(TEXT("true"), ESearchCase::IgnoreCase) == 0)
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
			if (Arg.TemplateWidget == UShidenStandardCommandDefinitions::VariableNameInputTemplate
				&& Arg.TemplateParameters.Contains(TEXT("VariableKindSourceIndex")))
			{
				const int32 VariableKindIndex = FCString::Atoi(*Arg.TemplateParameters[TEXT("VariableKindSourceIndex")]);
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
		UE_LOG(LogTemp, Warning, TEXT("Replace local variables is not supported."));
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
				TArray<FString> ArgKeys;
				Command.Args.GetKeys(ArgKeys);
				for (int32 i = 0; i < ArgKeys.Num(); i++)
				{
					const FString Key = ArgKeys[i];
					if (!Command.Args[Key].Contains(TEXT("{")) || !Command.Args[Key].Contains(TEXT("}")))
					{
						continue;
					}
					
					FRegexMatcher Matcher(UShidenVariableBlueprintLibrary::GetReplaceTextPattern(), Command.Args[Key]);
					while (Matcher.FindNext())
					{
						const FString Str = Matcher.GetCaptureGroup(1);
						const FString VariableName = Str.Mid(1, Str.Len() - 2).TrimStartAndEnd();
						if (VariableName == OldVariableNameWithPrefix)
						{
							Command.Args[Key].ReplaceInline(*Str, *NewReplacementVariableName, ESearchCase::CaseSensitive);
							bChanged = true;
						}
					}
				}
				
				if (TargetCommand.Contains(Command.CommandName))
				{
					const FString& CommandVariableKindValue = Command.Args.FindRef(TargetCommand[Command.CommandName].Get<0>());
					const FString& CommandVariableNameValue = Command.Args.FindRef(TargetCommand[Command.CommandName].Get<1>());
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
				RedirectCommands(Scenario, bChanged);
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

	// Update pluginVersion
	FShidenPluginVersion NewVersion;
	if (!TryGetCurrentPluginVersion(NewVersion))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get current plugin version"));
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

TArray<FShidenCommandRedirector> UShidenEditorBlueprintLibrary::GetRedirectDefinitions()
{
	const TObjectPtr<const UShidenEditorConfig> EditorConfig = GetDefault<UShidenEditorConfig>();
	TArray<FShidenCommandRedirector> Redirects = UShidenCommandRedirectors::GetBuiltIn(EditorConfig->PluginVersion);
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
