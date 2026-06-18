#include "Variant_InfeccionZombie/InfeccionZombieGameMode.h"
#include "Variant_InfeccionZombie/InfeccionZombieGameState.h"
#include "TimerManager.h"

AInfeccionZombieGameMode::AInfeccionZombieGameMode()
{
    PrimaryActorTick.bCanEverTick = false;
    GameStateClass = AInfeccionZombieGameState::StaticClass();
}

void AInfeccionZombieGameMode::BeginPlay()
{
    Super::BeginPlay();
}

void AInfeccionZombieGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    JugadoresConectados++;

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

    GS->Servidor_FinalizarPartida();
}

void AInfeccionZombieGameMode::InfectarZona(int32 IndexZona, int32 JugadorIndex)
{
    AInfeccionZombieGameState* GS = GetIZGameState();
    if (!GS || !GS->bPartidaActiva) return;

    GS->Servidor_InfectarZona(IndexZona, JugadorIndex);
}

int32 AInfeccionZombieGameMode::ObtenerGanador() const
{
    AInfeccionZombieGameState* GS = GetIZGameState();
    if (!GS) return 0;

    if (GS->ZonasJugador1 > GS->ZonasJugador2) return 1;
    if (GS->ZonasJugador2 > GS->ZonasJugador1) return 2;
    return 0;
}
