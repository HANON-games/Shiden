#include "Scenario/ShidenScenarioProperty.h"
#include "Dom/JsonValue.h"
#include "Serialization/JsonSerializer.h"

SHIDENCORE_API bool FShidenScenarioProperty::TryConvertToStringArray(TArray<FString>& Values) const
{
	TSharedPtr<FJsonValue> JsonValue;
	Values.Empty();
	if (FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(PropertyValue), JsonValue) && JsonValue.IsValid())
	{
		const TArray<TSharedPtr<FJsonValue>>* JsonArray;
		if (JsonValue->TryGetArray(JsonArray))
		{
			for (const TSharedPtr<FJsonValue>& ArrayValue : *JsonArray)
			{
				FString StringValue;
				if (ArrayValue->TryGetString(StringValue))
				{
					Values.Add(StringValue);
				}
			}
			return true;
		}
	}
	return false;
}

SHIDENCORE_API bool FShidenScenarioProperty::TryConvertToStringMap(TMap<FString, FString>& Values) const
{
	TSharedPtr<FJsonObject> JsonObject;
	Values.Empty();
	if (FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(PropertyValue), JsonObject) && JsonObject.IsValid())
	{
		for (const TPair<FString, TSharedPtr<FJsonValue>>& Pair : JsonObject->Values)
		{
			FString MapValue;
			if (Pair.Value->TryGetString(MapValue))
			{
				Values.Add(Pair.Key, MapValue);
			}
		}
		return true;
	}
	return false;
}

FString FShidenScenarioProperty::GetValueAsString() const
{
	return PropertyValue;
}
