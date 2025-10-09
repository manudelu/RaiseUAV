// MqttTelemetryReceiver.h
#pragma once

#include "CoreMinimal.h"
#include "BaseWebSocketReceiver.h"
#include "MqttTelemetryReceiver.generated.h"

// Struct for full drone telemetry
USTRUCT(BlueprintType)
struct FDroneTelemetry
{
    GENERATED_BODY()

    // --- Drone info ---
    UPROPERTY(BlueprintReadOnly) int32 DroneId;
    UPROPERTY(BlueprintReadOnly) FString Type;

    // --- Status ---
    UPROPERTY(BlueprintReadOnly) int32 ErrorStatus;
    UPROPERTY(BlueprintReadOnly) int32 FlightStatus;
    UPROPERTY(BlueprintReadOnly) int32 Gear;
    UPROPERTY(BlueprintReadOnly) int32 Mode;

    // --- Global/local positions ---
    UPROPERTY(BlueprintReadOnly) FVector GlobalPosition; // Latitude, Longitude, Altitude
    UPROPERTY(BlueprintReadOnly) float Height;
    UPROPERTY(BlueprintReadOnly) float LatitudeRad;
    UPROPERTY(BlueprintReadOnly) float LongitudeRad;

    // --- External / simulation positions ---
    UPROPERTY(BlueprintReadOnly) FVector ExtPosition;
    UPROPERTY(BlueprintReadOnly) float ExtStatus;

    // --- GPS details ---
    UPROPERTY(BlueprintReadOnly) int32 GPScounter;
    UPROPERTY(BlueprintReadOnly) int32 NSV;
    UPROPERTY(BlueprintReadOnly) int32 Fix;
    UPROPERTY(BlueprintReadOnly) float HAcc;
    UPROPERTY(BlueprintReadOnly) float HDOP;
    UPROPERTY(BlueprintReadOnly) float PDOP;
    UPROPERTY(BlueprintReadOnly) float SAcc;
    UPROPERTY(BlueprintReadOnly) int32 UsedGLN;
    UPROPERTY(BlueprintReadOnly) int32 UsedGPS;
    UPROPERTY(BlueprintReadOnly) float VAcc;

    // --- RC / Control ---
    UPROPERTY(BlueprintReadOnly) FVector RC_Angular;
    UPROPERTY(BlueprintReadOnly) float RC_Throttle;

    // --- Velocity ---
    UPROPERTY(BlueprintReadOnly) FVector LinearVelocity;
    UPROPERTY(BlueprintReadOnly) FVector AngularVelocity;

    // --- Orientation ---
    UPROPERTY(BlueprintReadOnly) FQuat Orientation;
    UPROPERTY(BlueprintReadOnly) float Yaw;

    // --- Avoidance ---
    UPROPERTY(BlueprintReadOnly) float AvoidUp;
    UPROPERTY(BlueprintReadOnly) float AvoidDown;
    UPROPERTY(BlueprintReadOnly) float AvoidLeft;
    UPROPERTY(BlueprintReadOnly) float AvoidRight;
    UPROPERTY(BlueprintReadOnly) float AvoidFront;
    UPROPERTY(BlueprintReadOnly) float AvoidBack;

    // --- Battery ---
    UPROPERTY(BlueprintReadOnly) float BatteryCapacity;
    UPROPERTY(BlueprintReadOnly) float BatteryVoltage;
    UPROPERTY(BlueprintReadOnly) float BatteryCurrent;
    UPROPERTY(BlueprintReadOnly) float BatteryPercentage;

    // --- Timestamp ---
    UPROPERTY(BlueprintReadOnly) float Timestamp;
};

UCLASS()
class RAISEUAV_API AMqttTelemetryReceiver : public ABaseWebSocketReceiver
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintImplementableEvent, Category = "Drone Telemetry")
    void OnDroneTelemetryReceived(const FDroneTelemetry& Telemetry);

protected:
    virtual FString GetWebSocketUrl() const override { return TEXT("ws://192.168.8.5:12000"); }
    virtual void HandleMessage(const FString& Message) override;
};
