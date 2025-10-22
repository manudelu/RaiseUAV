#pragma once

#include "CoreMinimal.h"
#include "BaseWebSocketReceiver.h"
#include "SensorDataReceiver.generated.h"

UCLASS()
class RAISEUAV_API ASensorDataReceiver : public ABaseWebSocketReceiver
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintImplementableEvent, Category = "Sensor")
    void OnSensorDataReceived(const FString& SensorId, const FVector& Location, const FString& temp, const FString& height_level);

protected:
    /** Use different port or URL if needed */
    virtual FString GetWebSocketUrl() const override { return TEXT("ws://127.0.0.1:8000"); }

    /** Parse JSON message from WebSocket */
    virtual void HandleMessage(const FString& Message) override;
};