#include "SensorDataReceiver.h"
#include "Json.h"
#include "JsonUtilities.h"

void ASensorDataReceiver::HandleMessage(const FString& Message)
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Message);

    if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to parse sensor JSON"));
        return;
    }

    FString SensorId = JsonObject->GetStringField(TEXT("id"));
    float X = JsonObject->GetNumberField(TEXT("x"));
    float Y = JsonObject->GetNumberField(TEXT("y"));
    float Z = JsonObject->GetNumberField(TEXT("z"));
    FString Label = JsonObject->GetStringField(TEXT("label"));

    FVector Location(X, Y, Z);

    OnSensorDataReceived(SensorId, Location, Label);
}