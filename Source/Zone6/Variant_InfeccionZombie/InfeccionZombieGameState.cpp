#include "Variant_InfeccionZombie/InfeccionZombieGameState.h"
#include "Net/UnrealNetwork.h"

AInfeccionZombieGameState::AInfeccionZombieGameState()
{
    bReplicates = true;
}

void AInfeccionZombieGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AInfeccionZombieGameState, EstadoZonas);
    DOREPLIFETIME(AInfeccionZombieGameState, ZonasJugador1);
    DOREPLIFETIME(AInfeccionZombieGameState, ZonasJugador2);
    DOREPLIFETIME(AInfeccionZombieGameState, TiempoRestante);
    DOREPLIFETIME(AInfeccionZombieGameState, bPartidaActiva);
}

// --- Funciones llamadas SOLO desde el servidor (GameMode) ---

void AInfeccionZombieGameState::Servidor_IniciarPartida(float TiempoTotal, int32 TotalZonas)
{
    EstadoZonas.Init(0, TotalZonas);
    TiempoRestante = TiempoTotal;
    ZonasJugador1 = 0;
    ZonasJugador2 = 0;
    bPartidaActiva = true;
    // OnRep_PartidaActiva se dispara en clientes automáticamente
}

void AInfeccionZombieGameState::Servidor_InfectarZona(int32 IndexZona, int32 JugadorIndex)
{
    if (!EstadoZonas.IsValidIndex(IndexZona)) return;
    if (JugadorIndex != 1 && JugadorIndex != 2) return;

    int32 PropietarioAnterior = EstadoZonas[IndexZona];
    if (PropietarioAnterior == JugadorIndex) return;

    if (PropietarioAnterior == 1) ZonasJugador1--;
    else if (PropietarioAnterior == 2) ZonasJugador2--;

    EstadoZonas[IndexZona] = JugadorIndex;
    UltimaZonaInfectada = IndexZona;
    UltimoJugadorInfecto = JugadorIndex;

    if (JugadorIndex == 1) ZonasJugador1++;
    else ZonasJugador2++;

    // La replicación de EstadoZonas dispara OnRep_EstadoZonas en los clientes
}

void AInfeccionZombieGameState::Servidor_ActualizarTiempo(float NuevoTiempo)
{
    TiempoRestante = NuevoTiempo;
    // La replicación dispara OnRep_TiempoRestante en los clientes
}

void AInfeccionZombieGameState::Servidor_FinalizarPartida()
{
    bPartidaActiva = false;
    TiempoRestante = 0.0f;
    // La replicación dispara OnRep_PartidaActiva en los clientes
}

// --- RepNotify: se ejecutan en cada cliente ---

void AInfeccionZombieGameState::OnRep_PartidaActiva()
{
    if (bPartidaActiva)
    {
        OnPartidaIniciada.Broadcast();
    }
    else
    {
        OnPartidaFinalizada.Broadcast(ZonasJugador1, ZonasJugador2);
    }
}

void AInfeccionZombieGameState::OnRep_EstadoZonas()
{
    if (UltimaZonaInfectada >= 0)
    {
        OnZonaInfectada.Broadcast(UltimaZonaInfectada, UltimoJugadorInfecto);
    }
}

void AInfeccionZombieGameState::OnRep_Contadores()
{
    // Los Blueprints pueden bindearse directo a ZonasJugador1/2 via binding,
    // pero este RepNotify existe por si algún Widget necesita reaccionar al cambio.
}

void AInfeccionZombieGameState::OnRep_TiempoRestante()
{
    OnTiempoActualizado.Broadcast(TiempoRestante);
}
