// Copyright (c) 2024 HANON. All Rights Reserved.

#include "ShidenEditorFunctionLibrary.h"
#include "IContentBrowserSingleton.h"
#include "ContentBrowserModule.h"
#include "DesktopPlatformModule.h"
#include "IContentBrowserDataModule.h"
#include "IDesktopPlatform.h"
#include "JsonObjectConverter.h"
#include "PackageTools.h"
#include "ShidenEditorConfig.h"
#include "ShidenEditorInputDataAsset.h"
#include "Command/ShidenCommandDefinition.h"
#include "Interfaces/IMainFrameModule.h"
#include "Scenario/ShidenScenario.h"
#include "Scenario/ShidenScenarioFunctionLibrary.h"
#include "Serialization/Csv/CsvParser.h"
#include "Utility/ShidenCoreFunctionLibrary.h"

#define LOCTEXT_NAMESPACE "AssetTools"

SHIDENEDITOR_API void UShidenEditorFunctionLibrary::CreateModalForSave(UClass* AssetClass, const FString DefaultPath, const FString DefaultAssetName ,FString& SavePackagePath, FString& SaveAssetName, bool& bSuccess)
{
	FSaveAssetDialogConfig SaveAssetDialogConfig;
	SaveAssetDialogConfig.DialogTitleOverride = LOCTEXT("SaveAssetDialogTitle", "Save Asset As");
	SaveAssetDialogConfig.DefaultPath = DefaultPath;
	SaveAssetDialogConfig.DefaultAssetName = DefaultAssetName;
	SaveAssetDialogConfig.ExistingAssetPolicy = ESaveAssetDialogExistingAssetPolicy::AllowButWarn;
	SaveAssetDialogConfig.AssetClassNames.Add(AssetClass->GetClassPathName());

	const FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	const FString SaveObjectPath = ContentBrowserModule.Get().CreateModalSaveAssetDialog(SaveAssetDialogConfig);

	const FString SavePackageName = FPackageName::ObjectPathToPackageName(SaveObjectPath);
	SavePackagePath = FPaths::GetPath(SavePackageName);
	SaveAssetName = FPaths::GetBaseFilename(SavePackageName);
	bSuccess = SaveAssetName.Len() > 0;
}

SHIDENEDITOR_API bool UShidenEditorFunctionLibrary::CanCreateFolder(const FName InPath)
{
	const TObjectPtr<UContentBrowserDataSubsystem> ContentBrowserData = IContentBrowserDataModule::Get().GetSubsystem();
	return ContentBrowserData->CanCreateFolder(InPath, nullptr);
}

SHIDENEDITOR_API TArray<FString> UShidenEditorFunctionLibrary::SortStringArray(const TArray<FString>& InArray)
{
	TArray<FString> OutArray = InArray;
	OutArray.Sort();
	return OutArray;
}

SHIDENEDITOR_API void UShidenEditorFunctionLibrary::LoadTextFile(const FString& Extension, FString& FileData, FString& FileName, bool& bSuccess)
{
	const void* WindowHandle = nullptr;
	if (FModuleManager::Get().IsModuleLoaded("MainFrame"))
	{
		const FName MainFrameModuleName = "MainFrame";
		const IMainFrameModule& MainFrameModule = FModuleManager::LoadModuleChecked<IMainFrameModule>(MainFrameModuleName);
		const TSharedPtr<SWindow> MainWindow = MainFrameModule.GetParentWindow();
		if (MainWindow.IsValid() && MainWindow->GetNativeWindow().IsValid())
		{
			WindowHandle = MainWindow->GetNativeWindow()->GetOSWindowHandle();
		}
	}

	TArray<FString> FilePath = { };

	if (WindowHandle)
	{
		if (IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get())
		{
			const bool bResult = DesktopPlatform->OpenFileDialog(
				WindowHandle,
				TEXT("Open File Dialog"),
				TEXT(""),
				TEXT(""),
				TEXT("Scenario ") + Extension + TEXT(" (*.") + Extension + TEXT(")|*.") + Extension,
				EFileDialogFlags::Type::None,
				FilePath
			);

			if (bResult)
			{
				if (GEngine)
				{
					const FString LoadFilePath = FPaths::ConvertRelativePathToFull(FilePath[0]);
					
					if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*LoadFilePath))
					{
						bSuccess = false;
						return;
					}
					FileName = FPaths::GetCleanFilename(LoadFilePath);
					FFileHelper::LoadFileToString(FileData, *LoadFilePath);
					bSuccess = true;
					return;
				}
			}
		}
	}
	bSuccess = false;
}

SHIDENEDITOR_API bool UShidenEditorFunctionLibrary::SaveTextFile(const FString DefaultFileName, const FString SaveText, const FString Extension)
{
	const void* WindowHandle = nullptr;
	if (FModuleManager::Get().IsModuleLoaded("MainFrame"))
	{
		const FName MainFrameModuleName = "MainFrame";
		const IMainFrameModule& MainFrameModule = FModuleManager::LoadModuleChecked<IMainFrameModule>(MainFrameModuleName);
		const TSharedPtr<SWindow> MainWindow = MainFrameModule.GetParentWindow();
		if (MainWindow.IsValid() && MainWindow->GetNativeWindow().IsValid())
		{
			WindowHandle = MainWindow->GetNativeWindow()->GetOSWindowHandle();
		}
	}

	if (WindowHandle)
	{
		if (IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get())
		{
			TArray<FString> FilePath = { };

			const bool bResult = DesktopPlatform->SaveFileDialog(
				WindowHandle,
				TEXT("Save File Dialog"),
				TEXT(""),
				DefaultFileName + TEXT("." + Extension),
				TEXT("Scenario ") + Extension + TEXT(" (*.") + Extension + TEXT(")|*.") + Extension,
				EFileDialogFlags::Type::None,
				FilePath
			);

			if (bResult)
			{
				return FFileHelper::SaveStringToFile(SaveText, *FPaths::ConvertRelativePathToFull(FilePath[0]), FFileHelper::EEncodingOptions::ForceUTF8);			
			}
		}
	}
	return false;
}

SHIDENEDITOR_API void UShidenEditorFunctionLibrary::ParseCsv(const FString CsvText, TArray<FShidenCsvParsedRow>& CsvParsedRow)
{
	const FCsvParser Parser = FCsvParser(CsvText);
	auto& Rows = Parser.GetRows();
	bool bCommentEnd = false;

	for (auto& Row : Rows)
	{
		// Skip comment lines like "# comment"
		if (!bCommentEnd && FString(Row[0]).TrimStart().TrimEnd().StartsWith(TEXT("#")))
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

SHIDENEDITOR_API void UShidenEditorFunctionLibrary::SetDefaultClassProperty(const UClass* TargetClass, const FName PropertyName, UClass* Value)
{
	FProperty* Prop = TargetClass->FindPropertyByName(PropertyName);
	const FClassProperty* ClassProperty = CastField<FClassProperty>(Prop);
	void* ValuePtr = ClassProperty->ContainerPtrToValuePtr<void>(TargetClass->GetDefaultObject());
	ClassProperty->SetObjectPropertyValue(ValuePtr, Value);
}

TMap<FString, FString> GetCsvComments(const FString& CsvString)
{
	TArray <FString> Lines;
	CsvString.ParseIntoArrayLines(Lines, true);
	TMap<FString, FString> Comments;
	for (const FString& Line : Lines)
	{
		if (Line.StartsWith(TEXT("#")))
		{
			FString Trimmed = Line.RightChop(1).TrimStart().TrimEnd();
			FString Key, Value;
			if (Trimmed.Split(TEXT(" "), &Key, &Value))
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
	const TMap<FString, FShidenCommandDefinition> CommandDefinitions = UShidenCoreFunctionLibrary::GetCommandDefinitionsCache();
	const UShidenProjectConfig* Config = GetDefault<UShidenProjectConfig>();
	UShidenScenario* ExpandedScenario = NewObject<UShidenScenario>();
	ExpandedScenario->ScenarioId = SourceScenario->ScenarioId;
	ExpandedScenario->Note = SourceScenario->Note;
	ExpandedScenario->MacroArguments = SourceScenario->MacroArguments;
	for (const auto& [CommandId, bEnabled, CommandName, PresetName, Args] : SourceScenario->Commands)
	{
		FShidenCommand ExpandedCommand;
		ExpandedCommand.CommandId = CommandId;
		ExpandedCommand.bEnabled = bEnabled;
		ExpandedCommand.CommandName = CommandName;
		ExpandedCommand.PresetName = PresetName;
		if (!PresetName.IsEmpty())
		{
			const FShidenPreset* CommandPreset = Config->Presets.FindByPredicate([&](const FShidenPreset& Preset)
			{
				return Preset.PresetName == PresetName;
			});
			if (CommandPreset)
			{
				for (const auto [ArgName, DisplayName, InputType, DefaultValue]: CommandDefinitions[CommandName].Args)
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
	const UShidenProjectConfig* Config = GetDefault<UShidenProjectConfig>();
	UShidenScenario* RemovedScenario = NewObject<UShidenScenario>();
	RemovedScenario->ScenarioId = SourceScenario->ScenarioId;
	RemovedScenario->Note = SourceScenario->Note;
	RemovedScenario->MacroArguments = SourceScenario->MacroArguments;
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
			const FShidenPreset* CommandPreset = Config->Presets.FindByPredicate([&](const FShidenPreset& Preset)
			{
				return Preset.PresetName == PresetName;
			});
			if (CommandPreset)
			{
				for (const auto [Key, Value]: CommandPreset->Args)
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

SHIDENEDITOR_API UShidenScenario* UShidenEditorFunctionLibrary::ConvertToScenarioFromCsv(const FString& CsvString)
{
	const TMap<FString, FShidenCommandDefinition> CommandDefinitions = UShidenCoreFunctionLibrary::GetCommandDefinitionsCache();
	UShidenScenario* Scenario = NewObject<UShidenScenario>();
	TMap<FString, FString> Comments = GetCsvComments(CsvString);

	// Parse comments
	FGuid ScenarioId;
	Scenario->ScenarioId = Comments.Contains(TEXT("ScenarioId")) && FGuid::Parse(Comments[TEXT("ScenarioId")], ScenarioId) ? ScenarioId : FGuid::NewGuid();
	Scenario->Note = Comments.Contains(TEXT("Note")) ? Comments[TEXT("Note")] : TEXT("");
	for (int Index = 1; Comments.Contains(TEXT("Arg") + FString::FromInt(Index) + TEXT("Name")); Index++)
	{
		FShidenMacroArgument MacroArgument;
		MacroArgument.Name = Comments[TEXT("Arg") + FString::FromInt(Index) + TEXT("Name")];
		FString TypeStr = Comments[TEXT("Arg") + FString::FromInt(Index) + TEXT("Type")];
		MacroArgument.Type = static_cast<EShidenVariableType>(StaticEnum<EShidenVariableType>()->GetValueByNameString(TypeStr));
		MacroArgument.DefaultValue = Comments[TEXT("Arg") + FString::FromInt(Index) + TEXT("DefaultValue")];
		Scenario->MacroArguments.Add(MacroArgument);
	}
	
	// Parse rows
	TArray<FShidenCsvParsedRow> CsvParsedRow;
	ParseCsv(CsvString, CsvParsedRow);
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
		for (int Index = 0; Index < Args.Num(); Index++)
		{
			Command.Args.Add(Args[Index].ArgName.ToString(), Row.Num() > Index + 4 ? Row[Index + 4] : TEXT("{EMPTY}"));
		}
		Scenario->Commands.Add(Command);
	}

	return RemovePresetValues(Scenario);
}

SHIDENEDITOR_API FString UShidenEditorFunctionLibrary::ConvertToCsvFromScenario(const UShidenScenario* SourceScenario, const bool bExpandPresets)
{
	const UShidenScenario* Scenario = bExpandPresets ? ExpandPresets(SourceScenario) : SourceScenario;
	
	TArray<FString> CsvRows;

	// Add comments
	CsvRows.Add(TEXT("#ScenarioId ") + Scenario->ScenarioId.ToString());
	CsvRows.Add(TEXT("#Note ") + Scenario->Note);
	for (int Index = 0; Index < Scenario->MacroArguments.Num(); Index++)
	{
		FString IndexStr = FString::FromInt(Index + 1);
		CsvRows.Add(TEXT("#Arg") + IndexStr + TEXT("Name ") + Scenario->MacroArguments[Index].Name);
		FText TypeText = StaticEnum<EShidenVariableType>()->GetDisplayValueAsText(Scenario->MacroArguments[Index].Type);
		CsvRows.Add(TEXT("#Arg") + IndexStr + TEXT("Type ") + TypeText.ToString());
		CsvRows.Add(TEXT("#Arg") + IndexStr + TEXT("DefaultValue ") + Scenario->MacroArguments[Index].DefaultValue);
	}

	// Get max column count
	int MaxColumnCount = 0;
	for (const FShidenCommand& Command : Scenario->Commands)
	{
		MaxColumnCount = FMath::Max(MaxColumnCount, Command.Args.Num());
	}

	// Add header
	FString Header = TEXT("CommandId,Enabled,CommandName,PresetName");
	for (int Index = 0; Index < MaxColumnCount; Index++)
	{
		Header += TEXT(",Arg") + FString::FromInt(Index + 1);
	}
	CsvRows.Add(Header);

	// Get Command Definitions
	const TMap<FString, FShidenCommandDefinition> CommandDefinitions = UShidenCoreFunctionLibrary::GetCommandDefinitionsCache();
	
	// Add rows
	TMap<FSoftObjectPath, UShidenEditorInputDataAsset*> InputDataAssetCache;
	TMap<FString, UShidenScenario*> MacroScenarioCache;
	for (const auto& [CommandId, bEnabled, CommandName, PresetName, Args] : Scenario->Commands)
	{
		FString Row = CommandId.ToString() + TEXT(",") + (bEnabled ? TEXT("true") : TEXT("false")) + TEXT(",") + EscapeCsvItem(CommandName) + TEXT(",") + EscapeCsvItem(PresetName);
		if (!CommandDefinitions.Contains(CommandName))
		{
			UE_LOG(LogTemp, Warning, TEXT("CommandName %s is not found in CommandDefinitions."), *CommandName);
			for (const auto [Key, Value]: Args)
			{
				Row += TEXT(",") + EscapeCsvItem(Value);
			}
			CsvRows.Add(Row);
			continue;
		}
		TArray<FShidenCommandArgument> CommandArguments = CommandDefinitions[CommandName].Args;
		for (int Index = 0; Index < MaxColumnCount; Index++)
		{
			if (Index < CommandDefinitions[CommandName].Args.Num() && Args.Contains(CommandArguments[Index].ArgName.ToString()))
			{
				FString Arg = Args[CommandArguments[Index].ArgName.ToString()];
				Row += TEXT(",") + EscapeCsvItem(Arg);

				// If the HasAdditionalArgs Property of EditorInput is true, add MacroArguments
				if (!InputDataAssetCache.Contains(CommandArguments[Index].InputType))
				{
					InputDataAssetCache.Add(
						CommandArguments[Index].InputType,
						Cast<UShidenEditorInputDataAsset>(CommandArguments[Index].InputType.TryLoad())
					);
				}
				UShidenEditorInputDataAsset* InputDataAsset = InputDataAssetCache[CommandArguments[Index].InputType];
				if (!InputDataAsset)
				{
					continue;
				}
				if (!Arg.IsEmpty() && InputDataAsset->Parameters.Contains("HasAdditionalArgs") && InputDataAsset->Parameters["HasAdditionalArgs"] == TEXT("true"))
				{
					if (!MacroScenarioCache.Contains(Arg))
					{
						// Load Macro Scenario
						FGuid MacroScenarioId;
						UShidenScenario* MacroScenario;
						bool bGetScenarioSuccess;
						UShidenScenarioFunctionLibrary::GetScenarioByIdOrObjectPath(Arg, MacroScenarioId, MacroScenario, bGetScenarioSuccess);
						if (bGetScenarioSuccess)
						{
							MacroScenarioCache.Add(Arg, MacroScenario);
						}
					}
					if (!MacroScenarioCache.Contains(Arg))
					{
						continue;
					}
					const UShidenScenario* MacroScenario = MacroScenarioCache[Arg];
					if (!MacroScenario)
					{
						continue;
					}
					for (FShidenMacroArgument MacroArgument: MacroScenario->MacroArguments)
					{
						if (Args.Contains(MacroArgument.Name))
						{
							FString MacroArg = Args[MacroArgument.Name];
							Row += TEXT(",") + EscapeCsvItem(MacroArg);
						}
						else
						{
							Row += TEXT(",");
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

SHIDENEDITOR_API UShidenScenario* UShidenEditorFunctionLibrary::ConvertToScenarioFromJson(const FString& Json, bool& bSuccess)
{
	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Json);
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		FShidenScenarioStruct Scenario;
		if (FJsonObjectConverter::JsonObjectToUStruct<FShidenScenarioStruct>(JsonObject.ToSharedRef(), &Scenario))
		{
			bSuccess = true;
			return RemovePresetValues(Scenario.ToShidenScenario());
		}
	}
	bSuccess = false;
	return nullptr;
}

SHIDENEDITOR_API FString UShidenEditorFunctionLibrary::ConvertToJsonFromScenario(const UShidenScenario* SourceScenario, const bool bExpandPresets, bool& bSuccess)
{
	const UShidenScenario* Scenario = bExpandPresets ? ExpandPresets(SourceScenario) : SourceScenario;
	FShidenScenarioStruct ScenarioStruct(Scenario);
	TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(ScenarioStruct);
	FString Json;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Json);
	if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
	{
		bSuccess = true;
		return Json;
	}
	bSuccess = false;
	return TEXT("");
}
