#include "Variant_InfeccionZombie/InfeccionZombieCharacter.h"
#include "Variant_InfeccionZombie/InfeccionZombiePlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

AInfeccionZombieCharacter::AInfeccionZombieCharacter()
{
    GetCharacterMovement()->MaxWalkSpeed = VelocidadBase;
    GetCharacterMovement()->GravityScale = 1.75f;
    GetCharacterMovement()->JumpZVelocity = 700.f;
    GetCharacterMovement()->AirControl = 1.0f;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 750.f, 0.f);
    GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AInfeccionZombieCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void AInfeccionZombieCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    // Hereda el setup de movimiento, salto y cámara de AZone6Character
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AInfeccionZombieCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AInfeccionZombieCharacter, bRalentizado);
}

void AInfeccionZombieCharacter::SetRalentizado(bool bNuevoEstado)
{
    // Solo el servidor modifica el estado; la replicación lo propaga
    if (!HasAuthority()) return;

    bRalentizado = bNuevoEstado;
    OnRep_Ralentizado();

    // Sincronizar con el PlayerState
    if (AInfeccionZombiePlayerState* PS = GetPlayerState<AInfeccionZombiePlayerState>())
    {
        PS->Servidor_SetZonasInfectadas(PS->ZonasInfectadas); // refresca
        PS->bRalentizado = bNuevoEstado;
    }
}

void AInfeccionZombieCharacter::OnRep_Ralentizado()
{
    // Se ejecuta en todos los clientes cuando bRalentizado cambia
    const float NuevaVelocidad = bRalentizado ? VelocidadRalentizado : VelocidadBase;
    GetCharacterMovement()->MaxWalkSpeed = NuevaVelocidad;
}

int32 AInfeccionZombieCharacter::ObtenerNumeroJugador() const
{
    if (const AInfeccionZombiePlayerState* PS = GetPlayerState<AInfeccionZombiePlayerState>())
    {
        return PS->NumeroJugador;
    }
    return 0;
}
