#include "BaseWebSocketReceiver.h"
#include "WebSocketsModule.h"
#include "Json.h"

ABaseWebSocketReceiver::ABaseWebSocketReceiver()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ABaseWebSocketReceiver::BeginPlay()
{
    Super::BeginPlay();
    ConnectWebSocket();
}

void ABaseWebSocketReceiver::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (WebSocket.IsValid() && WebSocket->IsConnected())
        WebSocket->Close();

    GetWorld()->GetTimerManager().ClearTimer(ReconnectTimerHandle);
    Super::EndPlay(EndPlayReason);
}

void ABaseWebSocketReceiver::ConnectWebSocket()
{
    if (!FModuleManager::Get().IsModuleLoaded("WebSockets"))
        FModuleManager::LoadModuleChecked<FWebSocketsModule>("WebSockets");

    WebSocket = FWebSocketsModule::Get().CreateWebSocket(GetWebSocketUrl());

    WebSocket->OnConnected().AddLambda([this]()
        {
            UE_LOG(LogTemp, Log, TEXT("Connected to %s"), *GetWebSocketUrl());
            GetWorld()->GetTimerManager().ClearTimer(ReconnectTimerHandle);
        });

    WebSocket->OnConnectionError().AddUObject(this, &ABaseWebSocketReceiver::OnWebSocketError);
    WebSocket->OnClosed().AddUObject(this, &ABaseWebSocketReceiver::OnWebSocketClosed);
    WebSocket->OnMessage().AddUObject(this, &ABaseWebSocketReceiver::HandleMessage);

    WebSocket->Connect();
}

void ABaseWebSocketReceiver::OnWebSocketError(const FString& Error)
{
    UE_LOG(LogTemp, Warning, TEXT("WebSocket error: %s"), *Error);
    AttemptReconnect();
}

void ABaseWebSocketReceiver::OnWebSocketClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
    UE_LOG(LogTemp, Warning, TEXT("WebSocket closed: %s"), *Reason);
    AttemptReconnect();
}

void ABaseWebSocketReceiver::AttemptReconnect()
{
    if (!WebSocket.IsValid() || WebSocket->IsConnected())
        return;

    if (!GetWorld()->GetTimerManager().IsTimerActive(ReconnectTimerHandle))
    {
        GetWorld()->GetTimerManager().SetTimer(ReconnectTimerHandle, [this]()
            {
                UE_LOG(LogTemp, Log, TEXT("Attempting reconnect..."));
                ConnectWebSocket();
            }, ReconnectInterval, false);
    }
}