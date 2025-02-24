// Copyright (c) 2024 HANON. All Rights Reserved.

#include "ShidenEditorFunctionLibrary.h"
#include "IContentBrowserSingleton.h"
#include "ContentBrowserModule.h"
#include "DesktopPlatformModule.h"
#include "IContentBrowserDataModule.h"
#include "IDesktopPlatform.h"
#include "JsonObjectConverter.h"
#include "Command/ShidenCommandDefinition.h"
#include "Interfaces/IMainFrameModule.h"
#include "Scenario/ShidenScenario.h"
#include "Scenario/ShidenScenarioFunctionLibrary.h"
#include "Serialization/Csv/CsvParser.h"
#include "Utility/ShidenCoreFunctionLibrary.h"

#define LOCTEXT_NAMESPACE "AssetTools"

SHIDENEDITOR_API void UShidenEditorFunctionLibrary::CreateModalForSave(UClass* AssetClass, const FString& DefaultPath, const FString& DefaultAssetName, FString& SavePackagePath, FString& SaveAssetName, bool& bSuccess)
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
		const FName MainFrameModuleName = TEXT("MainFrame");
		const IMainFrameModule& MainFrameModule = FModuleManager::LoadModuleChecked<IMainFrameModule>(MainFrameModuleName);
		const TSharedPtr<SWindow> MainWindow = MainFrameModule.GetParentWindow();
		if (MainWindow.IsValid() && MainWindow->GetNativeWindow().IsValid())
		{
			WindowHandle = MainWindow->GetNativeWindow()->GetOSWindowHandle();
		}
	}

	TArray<FString> FilePath = {};

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

SHIDENEDITOR_API void UShidenEditorFunctionLibrary::SaveTextFile(const FString& DefaultFileName, const FString& SaveText, const FString& Extension, bool& bSuccess)
{
	const void* WindowHandle = nullptr;
	if (FModuleManager::Get().IsModuleLoaded("MainFrame"))
	{
		const FName MainFrameModuleName = TEXT("MainFrame");
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
			TArray<FString> FilePath = {};

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
				bSuccess = FFileHelper::SaveStringToFile(SaveText, *FPaths::ConvertRelativePathToFull(FilePath[0]), FFileHelper::EEncodingOptions::ForceUTF8);
				return;
			}
		}
	}

	bSuccess = false;
}

SHIDENEDITOR_API void UShidenEditorFunctionLibrary::ParseCsv(const FString& CsvText, TArray<FShidenCsvParsedRow>& CsvParsedRow)
{
	const FCsvParser Parser = FCsvParser(CsvText);
	auto& Rows = Parser.GetRows();
	bool bCommentEnd = false;

	for (auto& Row : Rows)
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

SHIDENEDITOR_API void UShidenEditorFunctionLibrary::SetDefaultClassProperty(const UClass* TargetClass, const FName& PropertyName, UClass* Value)
{
	FProperty* Prop = TargetClass->FindPropertyByName(PropertyName);
	const FClassProperty* ClassProperty = CastField<FClassProperty>(Prop);
	void* ValuePtr = ClassProperty->ContainerPtrToValuePtr<void>(TargetClass->GetDefaultObject());
	ClassProperty->SetObjectPropertyValue(ValuePtr, Value);
}

TMap<FString, FString> GetCsvComments(const FString& CsvString)
{
	TArray<FString> Lines;
	CsvString.ParseIntoArrayLines(Lines, true);
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
				for (const auto& [ArgName, DisplayName, DefaultValue, TemplateWidget, Parameters, bIsAssetToBeLoaded] : CommandDefinitions[CommandName].Args)
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
	RemovedScenario->MacroParameterDefinitions = SourceScenario->MacroParameterDefinitions;
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

SHIDENEDITOR_API UShidenScenario* UShidenEditorFunctionLibrary::ConvertToScenarioFromCsv(const FString& CsvString)
{
	const TMap<FString, FShidenCommandDefinition> CommandDefinitions = UShidenCoreFunctionLibrary::GetCommandDefinitionsCache();
	TObjectPtr<UShidenScenario> Scenario = NewObject<UShidenScenario>();
	TMap<FString, FString> Comments = GetCsvComments(CsvString);

	// Parse comments
	FGuid ScenarioId;
	Scenario->ScenarioId = Comments.Contains(TEXT("ScenarioId")) && FGuid::Parse(Comments[TEXT("ScenarioId")], ScenarioId)
							   ? ScenarioId
							   : FGuid::NewGuid();
	Scenario->Note = Comments.Contains(TEXT("Note")) ? Comments[TEXT("Note")] : TEXT("");
	for (int Index = 1; Comments.Contains(TEXT("MacroParameter") + FString::FromInt(Index) + TEXT("Name")); Index++)
	{
		FShidenMacroParameter MacroParameterDefinition;
		MacroParameterDefinition.Name = Comments[TEXT("MacroParameter") + FString::FromInt(Index) + TEXT("Name")];
		FString TypeStr = Comments[TEXT("MacroParameter") + FString::FromInt(Index) + TEXT("Type")];
		MacroParameterDefinition.Type = static_cast<EShidenVariableType>(StaticEnum<EShidenVariableType>()->GetValueByNameString(TypeStr));
		MacroParameterDefinition.DefaultValue = Comments[TEXT("MacroParameter") + FString::FromInt(Index) + TEXT("DefaultValue")];
		for (int EnumIndex = 1; Comments.Contains(TEXT("MacroParameter") + FString::FromInt(Index) + TEXT("Enum") + FString::FromInt(EnumIndex)); EnumIndex++)
		{
			MacroParameterDefinition.IsEnum = true;
			MacroParameterDefinition.EnumValues.Add(Comments[TEXT("MacroParameter") + FString::FromInt(Index) + TEXT("Enum") + FString::FromInt(EnumIndex)]);
		}
		Scenario->MacroParameterDefinitions.Add(MacroParameterDefinition);
	}
	for (int Index = 1; Comments.Contains(TEXT("LocalVariable") + FString::FromInt(Index) + TEXT("Name")); Index++)
	{
		FShidenVariableDefinition LocalVariableDefinition;
		LocalVariableDefinition.Name = Comments[TEXT("LocalVariable") + FString::FromInt(Index) + TEXT("Name")];
		FString TypeStr = Comments[TEXT("LocalVariable") + FString::FromInt(Index) + TEXT("Type")];
		LocalVariableDefinition.Type = static_cast<EShidenVariableType>(StaticEnum<EShidenVariableType>()->GetValueByNameString(TypeStr));
		LocalVariableDefinition.DefaultValue = Comments[TEXT("LocalVariable") + FString::FromInt(Index) + TEXT("DefaultValue")];
		Scenario->LocalVariableDefinitions.Add(LocalVariableDefinition);
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
	for (int Index = 0; Index < Scenario->MacroParameterDefinitions.Num(); Index++)
	{
		FString IndexStr = FString::FromInt(Index + 1);
		CsvRows.Add(TEXT("#MacroParameter") + IndexStr + TEXT("Name ") + Scenario->MacroParameterDefinitions[Index].Name);
		FText TypeText = StaticEnum<EShidenVariableType>()->GetDisplayValueAsText(Scenario->MacroParameterDefinitions[Index].Type);
		CsvRows.Add(TEXT("#MacroParameter") + IndexStr + TEXT("Type ") + TypeText.ToString());
		CsvRows.Add(TEXT("#MacroParameter") + IndexStr + TEXT("DefaultValue ") + Scenario->MacroParameterDefinitions[Index].DefaultValue);
		if (Scenario->MacroParameterDefinitions[Index].IsEnum)
		{
			for (int EnumIndex = 0; EnumIndex < Scenario->MacroParameterDefinitions[Index].EnumValues.Num(); EnumIndex++)
			{
				CsvRows.Add(TEXT("#MacroParameter") + IndexStr + TEXT("Enum") + FString::FromInt(EnumIndex + 1) + TEXT(" ") + Scenario->MacroParameterDefinitions[Index].EnumValues[EnumIndex]);
			}
		}
	}
	for (int Index = 0; Index < Scenario->LocalVariableDefinitions.Num(); Index++)
	{
		FString IndexStr = FString::FromInt(Index + 1);
		CsvRows.Add(TEXT("#LocalVariable") + IndexStr + TEXT("Name ") + Scenario->LocalVariableDefinitions[Index].Name);
		FText TypeText = StaticEnum<EShidenVariableType>()->GetDisplayValueAsText(Scenario->LocalVariableDefinitions[Index].Type);
		CsvRows.Add(TEXT("#LocalVariable") + IndexStr + TEXT("Type ") + TypeText.ToString());
		CsvRows.Add(TEXT("#LocalVariable") + IndexStr + TEXT("DefaultValue ") + Scenario->LocalVariableDefinitions[Index].DefaultValue);
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
	TMap<FString, TObjectPtr<UShidenScenario>> MacroScenarioCache;
	for (const auto& [CommandId, bEnabled, CommandName, PresetName, Args] : Scenario->Commands)
	{
		FString Row = CommandId.ToString() + TEXT(",") + (bEnabled ? TEXT("true") : TEXT("false")) + TEXT(",") + EscapeCsvItem(CommandName) + TEXT(",") + EscapeCsvItem(PresetName);
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
		for (int Index = 0; Index < MaxColumnCount; Index++)
		{
			if (Index < CommandDefinitions[CommandName].Args.Num() && Args.Contains(CommandArguments[Index].ArgName.ToString()))
			{
				FString Arg = Args[CommandArguments[Index].ArgName.ToString()];
				Row += TEXT(",") + EscapeCsvItem(Arg);

				// If the HasAdditionalArgs Property of CommandArguments[Index] is true, add MacroArguments
				if (!Arg.IsEmpty() && CommandArguments[Index].TemplateParameters.Contains("HasAdditionalArgs")
					&& CommandArguments[Index].TemplateParameters["HasAdditionalArgs"] == TEXT("true"))
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
					const TObjectPtr<UShidenScenario> MacroScenario = MacroScenarioCache[Arg];
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
	const FShidenScenarioStruct ScenarioStruct(Scenario);
	const TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(ScenarioStruct);
	FString Json;
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Json);
	if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
	{
		bSuccess = true;
		return Json;
	}
	bSuccess = false;
	return TEXT("");
}

void UShidenEditorFunctionLibrary::ListUserVariableDescriptors(TArray<FShidenVariableDescriptor>& VariableDescriptors)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->UserVariable.ListDescriptors(VariableDescriptors);
}

void UShidenEditorFunctionLibrary::ListPredefinedSystemVariableDescriptors(TArray<FShidenVariableDescriptor>& VariableDescriptors)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->PredefinedSystemVariable.ListDescriptors(VariableDescriptors);
}

void UShidenEditorFunctionLibrary::ListSystemVariableDescriptors(TArray<FShidenVariableDescriptor>& VariableDescriptors)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->SystemVariable.ListDescriptors(VariableDescriptors);
}

void UShidenEditorFunctionLibrary::ListLocalVariableDescriptors(TArray<FShidenVariableDescriptor>& VariableDescriptors)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->LocalVariable.ListDescriptors(VariableDescriptors);
}

void UShidenEditorFunctionLibrary::AddUserVariableDefinition(const FShidenVariableDefinition& VariableDefinition)
{
	UShidenProjectConfig* ProjectConfig = GetMutableDefault<UShidenProjectConfig>();
	
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

void UShidenEditorFunctionLibrary::UpdateUserVariableDefinition(const FString& OldName, const FShidenVariableDefinition& VariableDefinition)
{
	UShidenProjectConfig* ProjectConfig = GetMutableDefault<UShidenProjectConfig>();
	
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

void UShidenEditorFunctionLibrary::RemoveUserVariableDefinition(const FString& Name)
{
	UShidenProjectConfig* ProjectConfig = GetMutableDefault<UShidenProjectConfig>();
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

void UShidenEditorFunctionLibrary::AddSystemVariableDefinition(const FShidenVariableDefinition& VariableDefinition)
{
	UShidenProjectConfig* ProjectConfig = GetMutableDefault<UShidenProjectConfig>();

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

void UShidenEditorFunctionLibrary::UpdateSystemVariableDefinition(const FString& OldName, const FShidenVariableDefinition& VariableDefinition)
{
	UShidenProjectConfig* ProjectConfig = GetMutableDefault<UShidenProjectConfig>();

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

void UShidenEditorFunctionLibrary::RemoveSystemVariableDefinition(const FString& Name)
{
	UShidenProjectConfig* ProjectConfig = GetMutableDefault<UShidenProjectConfig>();
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

void UShidenEditorFunctionLibrary::GetPredefinedSystemVariableDefinitions(TArray<FShidenVariableDefinition>& VariableDefinitions)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	VariableDefinitions.Empty();
	for (const FShidenPredefinedSystemVariableDefinition& Definition : ShidenSubsystem->PredefinedSystemVariable.Definitions)
	{
		VariableDefinitions.Add(Definition);
	}
}
