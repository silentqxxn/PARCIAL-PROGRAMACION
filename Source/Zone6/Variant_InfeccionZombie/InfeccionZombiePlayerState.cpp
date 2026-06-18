#include "Variant_InfeccionZombie/InfeccionZombiePlayerState.h"
#include "Net/UnrealNetwork.h"

AInfeccionZombiePlayerState::AInfeccionZombiePlayerState()
{
    bReplicates = true;
}

void AInfeccionZombiePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AInfeccionZombiePlayerState, NumeroJugador);
    DOREPLIFETIME(AInfeccionZombiePlayerState, ZonasInfectadas);
    DOREPLIFETIME(AInfeccionZombiePlayerState, bRalentizado);
}

void AInfeccionZombiePlayerState::Servidor_SetZonasInfectadas(int32 Nuevas)
{
    ZonasInfectadas = Nuevas;
    // La replicación dispara OnRep_ZonasInfectadas en todos los clientes
}

void AInfeccionZombiePlayerState::Server_SetRalentizado_Implementation(bool bNuevoEstado)
{
    bRalentizado = bNuevoEstado;
    // La replicación dispara OnRep_Ralentizado en todos los clientes
}

void AInfeccionZombiePlayerState::OnRep_ZonasInfectadas()
{
    OnPuntajeActualizado.Broadcast(ZonasInfectadas);
}

void AInfeccionZombiePlayerState::OnRep_Ralentizado()
{
    OnEstadoRalentizadoCambiado.Broadcast(bRalentizado);
}
