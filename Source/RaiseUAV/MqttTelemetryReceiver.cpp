// MqttTelemetryReceiver.cpp
#include "MqttTelemetryReceiver.h"
#include "Json.h"

void AMqttTelemetryReceiver::HandleMessage(const FString& Message)
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Message);

    if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to parse JSON: %s"), *Message);
        return;
    }

    FDroneTelemetry Telemetry;

    // --- Drone info ---
    if (JsonObject->HasField("drone"))
    {
        TSharedPtr<FJsonObject> DroneObj = JsonObject->GetObjectField(TEXT("drone"));
        Telemetry.DroneId = DroneObj->GetIntegerField(TEXT("id"));
        Telemetry.Type = DroneObj->GetStringField(TEXT("type"));
    }

    // --- Status ---
    if (JsonObject->HasField("status"))
    {
        TSharedPtr<FJsonObject> StatusObj = JsonObject->GetObjectField(TEXT("status"));
        Telemetry.ErrorStatus = StatusObj->GetIntegerField(TEXT("error"));
        Telemetry.FlightStatus = StatusObj->GetIntegerField(TEXT("flight"));
        Telemetry.Gear = StatusObj->GetIntegerField(TEXT("gear"));
        Telemetry.Mode = StatusObj->GetIntegerField(TEXT("mode"));
    }

    // --- Position ---
    if (JsonObject->HasField("position"))
    {
        TSharedPtr<FJsonObject> PosObj = JsonObject->GetObjectField(TEXT("position"));
        Telemetry.GlobalPosition.X = PosObj->GetNumberField(TEXT("latitude"));
        Telemetry.GlobalPosition.Y = PosObj->GetNumberField(TEXT("longitude"));
        Telemetry.GlobalPosition.Z = PosObj->GetNumberField(TEXT("altitude"));
        Telemetry.Height = PosObj->GetNumberField(TEXT("height"));
        Telemetry.LatitudeRad = PosObj->GetNumberField(TEXT("latitudeRad"));
        Telemetry.LongitudeRad = PosObj->GetNumberField(TEXT("longitudeRad"));
    }

    // --- External position ---
    if (JsonObject->HasField("extPosition"))
    {
        TSharedPtr<FJsonObject> ExtObj = JsonObject->GetObjectField(TEXT("extPosition"));
        Telemetry.ExtPosition.X = ExtObj->GetNumberField(TEXT("latitude"));
        Telemetry.ExtPosition.Y = ExtObj->GetNumberField(TEXT("longitude"));
        Telemetry.ExtPosition.Z = ExtObj->GetNumberField(TEXT("altitude"));
        Telemetry.ExtStatus = ExtObj->GetNumberField(TEXT("status"));
    }

    // --- GPS ---
    if (JsonObject->HasField("gpsDetail"))
    {
        TSharedPtr<FJsonObject> GPSObj = JsonObject->GetObjectField(TEXT("gpsDetail"));
        Telemetry.GPScounter = GPSObj->GetIntegerField(TEXT("GPScounter"));
        Telemetry.NSV = GPSObj->GetIntegerField(TEXT("NSV"));
        Telemetry.Fix = GPSObj->GetIntegerField(TEXT("fix"));
        Telemetry.HAcc = GPSObj->GetNumberField(TEXT("hacc"));
        Telemetry.HDOP = GPSObj->GetNumberField(TEXT("hdop"));
        Telemetry.PDOP = GPSObj->GetNumberField(TEXT("pdop"));
        Telemetry.SAcc = GPSObj->GetNumberField(TEXT("sacc"));
        Telemetry.UsedGLN = GPSObj->GetIntegerField(TEXT("usedGLN"));
        Telemetry.UsedGPS = GPSObj->GetIntegerField(TEXT("usedGPS"));
        Telemetry.VAcc = GPSObj->GetNumberField(TEXT("vacc"));
    }

    // --- Velocity ---
    if (JsonObject->HasField("velocity"))
    {
        TSharedPtr<FJsonObject> VelObj = JsonObject->GetObjectField(TEXT("velocity"));
        Telemetry.LinearVelocity.X = VelObj->GetNumberField(TEXT("x"));
        Telemetry.LinearVelocity.Y = VelObj->GetNumberField(TEXT("y"));
        Telemetry.LinearVelocity.Z = VelObj->GetNumberField(TEXT("z"));
    }

    // --- Angular velocity ---
    if (JsonObject->HasField("angularVelocity"))
    {
        TSharedPtr<FJsonObject> AngVelObj = JsonObject->GetObjectField(TEXT("angularVelocity"));
        Telemetry.AngularVelocity.X = AngVelObj->GetNumberField(TEXT("x"));
        Telemetry.AngularVelocity.Y = AngVelObj->GetNumberField(TEXT("y"));
        Telemetry.AngularVelocity.Z = AngVelObj->GetNumberField(TEXT("z"));
    }

    // --- Orientation ---
    if (JsonObject->HasField("quaternion"))
    {
        TSharedPtr<FJsonObject> QuatObj = JsonObject->GetObjectField(TEXT("quaternion"));
        Telemetry.Orientation.W = QuatObj->GetNumberField(TEXT("q0"));
        Telemetry.Orientation.X = QuatObj->GetNumberField(TEXT("q1"));
        Telemetry.Orientation.Y = QuatObj->GetNumberField(TEXT("q2"));
        Telemetry.Orientation.Z = QuatObj->GetNumberField(TEXT("q3"));
    }

    // --- RC ---
    if (JsonObject->HasField("rc"))
    {
        TSharedPtr<FJsonObject> RCObj = JsonObject->GetObjectField(TEXT("rc"));
        Telemetry.Gear = RCObj->GetIntegerField(TEXT("gear"));
        Telemetry.Mode = RCObj->GetIntegerField(TEXT("mode"));
        Telemetry.RC_Throttle = RCObj->GetNumberField(TEXT("throttle"));
        Telemetry.RC_Angular.X = RCObj->GetNumberField(TEXT("roll"));
        Telemetry.RC_Angular.Y = RCObj->GetNumberField(TEXT("pitch"));
        Telemetry.RC_Angular.Z = RCObj->GetNumberField(TEXT("yaw"));
    }

    // --- Battery ---
    if (JsonObject->HasField("battery"))
    {
        TSharedPtr<FJsonObject> BatObj = JsonObject->GetObjectField(TEXT("battery"));
        Telemetry.BatteryCapacity = BatObj->GetNumberField(TEXT("capacity"));
        Telemetry.BatteryCurrent = BatObj->GetNumberField(TEXT("current"));
        Telemetry.BatteryVoltage = BatObj->GetNumberField(TEXT("voltage"));
        Telemetry.BatteryPercentage = BatObj->GetNumberField(TEXT("percentage"));
    }

    // --- Avoidance ---
    if (JsonObject->HasField("avoidance"))
    {
        TSharedPtr<FJsonObject> AvoidObj = JsonObject->GetObjectField(TEXT("avoidance"));
        Telemetry.AvoidUp = AvoidObj->GetNumberField(TEXT("up"));
        Telemetry.AvoidDown = AvoidObj->GetNumberField(TEXT("down"));
        Telemetry.AvoidLeft = AvoidObj->GetNumberField(TEXT("left"));
        Telemetry.AvoidRight = AvoidObj->GetNumberField(TEXT("right"));
        Telemetry.AvoidFront = AvoidObj->GetNumberField(TEXT("front"));
        Telemetry.AvoidBack = AvoidObj->GetNumberField(TEXT("back"));
    }

    // --- Timestamp ---
    if (JsonObject->HasField("timestamp"))
    {
        FString TSString = JsonObject->GetStringField(TEXT("timestamp"));
        Telemetry.Timestamp = FCString::Atod(*TSString);
    }

    // Trigger Blueprint event
    OnDroneTelemetryReceived(Telemetry);
}
