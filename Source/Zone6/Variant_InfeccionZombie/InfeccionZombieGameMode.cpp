#include "Variant_InfeccionZombie/InfeccionZombieGameMode.h"
#include "Variant_InfeccionZombie/InfeccionZombieGameState.h"
#include "Variant_InfeccionZombie/InfeccionZombiePlayerState.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"
#include "Engine/World.h"

AInfeccionZombieGameMode::AInfeccionZombieGameMode()
{
    PrimaryActorTick.bCanEverTick = false;
    GameStateClass = AInfeccionZombieGameState::StaticClass();
    PlayerStateClass = AInfeccionZombiePlayerState::StaticClass();
}

void AInfeccionZombieGameMode::BeginPlay()
{
    Super::BeginPlay();
}

void AInfeccionZombieGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    JugadoresConectados++;

    // Asignar número de jugador en el PlayerState
    if (AInfeccionZombiePlayerState* PS = NewPlayer->GetPlayerState<AInfeccionZombiePlayerState>())
    {
        PS->NumeroJugador = JugadoresConectados; // 1 para el primero, 2 para el segundo
    }

    if (JugadoresConectados >= 2)
    {
        IniciarConteo();
    }
}

AInfeccionZombieGameState* AInfeccionZombieGameMode::GetIZGameState() const
{
    return GetGameState<AInfeccionZombieGameState>();
}

void AInfeccionZombieGameMode::IniciarConteo()
{
    GetWorldTimerManager().SetTimer(
        ConteoInicialTimer,
        this,
        &AInfeccionZombieGameMode::IniciarPartida,
        TiempoConteoInicial,
        false
    );
}

void AInfeccionZombieGameMode::IniciarPartida()
{
    AInfeccionZombieGameState* GS = GetIZGameState();
    if (!GS) return;

    GS->Servidor_IniciarPartida(TiempoPartida, TotalZonas);

    GetWorldTimerManager().SetTimer(
        PartidaTimer,
        this,
        &AInfeccionZombieGameMode::FinalizarPartida,
        TiempoPartida,
        false
    );

    GetWorldTimerManager().SetTimer(
        TickTimer,
        this,
        &AInfeccionZombieGameMode::TickTiempo,
        1.0f,
        true
    );

    // Spawn periódico del ítem de ralentización
    if (ClaseSlowItem && PosicionesSpawnItem.Num() > 0)
    {
        GetWorldTimerManager().SetTimer(
            SpawnItemTimer,
            this,
            &AInfeccionZombieGameMode::SpawnearItem,
            IntervaloSpawnItem,
            true
        );
    }
}

void AInfeccionZombieGameMode::TickTiempo()
{
    AInfeccionZombieGameState* GS = GetIZGameState();
    if (!GS) return;

    float NuevoTiempo = FMath::Max(0.0f, GS->TiempoRestante - 1.0f);
    GS->Servidor_ActualizarTiempo(NuevoTiempo);
}

void AInfeccionZombieGameMode::FinalizarPartida()
{
    AInfeccionZombieGameState* GS = GetIZGameState();
    if (!GS) return;

    GetWorldTimerManager().ClearTimer(PartidaTimer);
    GetWorldTimerManager().ClearTimer(TickTimer);
    GetWorldTimerManager().ClearTimer(SpawnItemTimer);

    GS->Servidor_FinalizarPartida();
}

void AInfeccionZombieGameMode::InfectarZona(int32 IndexZona, int32 JugadorIndex)
{
    AInfeccionZombieGameState* GS = GetIZGameState();
    if (!GS || !GS->bPartidaActiva) return;

    GS->Servidor_InfectarZona(IndexZona, JugadorIndex);
    ActualizarPuntajesPlayerState();
}

int32 AInfeccionZombieGameMode::ObtenerGanador() const
{
    AInfeccionZombieGameState* GS = GetIZGameState();
    if (!GS) return 0;

    if (GS->ZonasJugador1 > GS->ZonasJugador2) return 1;
    if (GS->ZonasJugador2 > GS->ZonasJugador1) return 2;
    return 0;
}

void AInfeccionZombieGameMode::SpawnearItem()
{
    if (!ClaseSlowItem || PosicionesSpawnItem.Num() == 0) return;

    const int32 PosRandom = FMath::RandRange(0, PosicionesSpawnItem.Num() - 1);
    FVector Pos = PosicionesSpawnItem[PosRandom];

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    GetWorld()->SpawnActor<AActor>(ClaseSlowItem, Pos, FRotator::ZeroRotator, Params);
}

void AInfeccionZombieGameMode::ActualizarPuntajesPlayerState()
{
    AInfeccionZombieGameState* GS = GetIZGameState();
    if (!GS) return;

    for (APlayerState* PS : GameState->PlayerArray)
    {
        if (AInfeccionZombiePlayerState* IPS = Cast<AInfeccionZombiePlayerState>(PS))
        {
            const int32 Zonas = (IPS->NumeroJugador == 1) ? GS->ZonasJugador1 : GS->ZonasJugador2;
            IPS->Servidor_SetZonasInfectadas(Zonas);
        }
    }
}
