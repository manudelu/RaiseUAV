#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IWebSocket.h"
#include "BaseWebSocketReceiver.generated.h"

UCLASS(Abstract)
class RAISEUAV_API ABaseWebSocketReceiver : public AActor
{
    GENERATED_BODY()

public:
    ABaseWebSocketReceiver();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    // Called when a message arrives — subclasses will override this
    virtual void HandleMessage(const FString& Message) PURE_VIRTUAL(ABaseWebSocketReceiver::HandleMessage, );

    // Can be overridden for a different WebSocket URL or port
    virtual FString GetWebSocketUrl() const { return TEXT("ws://192.168.8.5:12000"); }

    // WebSocket Lifecycle
    void ConnectWebSocket();
    void AttemptReconnect();
    void OnWebSocketError(const FString& Error);
    void OnWebSocketClosed(int32 StatusCode, const FString& Reason, bool bWasClean);

protected:
    TSharedPtr<IWebSocket> WebSocket;
    FTimerHandle ReconnectTimerHandle;
    float ReconnectInterval = 2.0f; // seconds
};