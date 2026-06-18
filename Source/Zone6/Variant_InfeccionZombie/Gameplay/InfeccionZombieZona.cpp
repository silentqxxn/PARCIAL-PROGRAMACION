#include "Variant_InfeccionZombie/Gameplay/InfeccionZombieZona.h"
#include "Variant_InfeccionZombie/InfeccionZombieCharacter.h"
#include "Variant_InfeccionZombie/InfeccionZombieGameMode.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"

AInfeccionZombieZona::AInfeccionZombieZona()
{
    bReplicates = true;
    PrimaryActorTick.bCanEverTick = false;

    MeshCentro = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshCentro"));
    SetRootComponent(MeshCentro);

    ZonaTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("ZonaTrigger"));
    ZonaTrigger->SetupAttachment(MeshCentro);
    ZonaTrigger->SetSphereRadius(150.f);
    ZonaTrigger->SetCollisionProfileName(TEXT("Trigger"));
}

void AInfeccionZombieZona::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        ZonaTrigger->OnComponentBeginOverlap.AddDynamic(this, &AInfeccionZombieZona::OnOverlapBegin);
        ZonaTrigger->OnComponentEndOverlap.AddDynamic(this, &AInfeccionZombieZona::OnOverlapEnd);
    }
}

void AInfeccionZombieZona::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AInfeccionZombieZona, PropietarioActual);
}

void AInfeccionZombieZona::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    AInfeccionZombieCharacter* Jugador = Cast<AInfeccionZombieCharacter>(OtherActor);
    if (!Jugador || JugadorCapturando) return;

    // Si ya pertenece a este jugador, no hace falta capturar
    if (PropietarioActual == Jugador->ObtenerNumeroJugador()) return;

    JugadorCapturando = Jugador;
    GetWorldTimerManager().SetTimer(
        CapturaTimer,
        this,
        &AInfeccionZombieZona::CompletarCaptura,
        TiempoParaInfectar,
        false
    );
}

void AInfeccionZombieZona::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor == JugadorCapturando)
    {
        JugadorCapturando = nullptr;
        GetWorldTimerManager().ClearTimer(CapturaTimer);
    }
}

void AInfeccionZombieZona::CompletarCaptura()
{
    if (!JugadorCapturando) return;

    int32 NumJugador = JugadorCapturando->ObtenerNumeroJugador();
    JugadorCapturando = nullptr;

    if (AInfeccionZombieGameMode* GM = GetWorld()->GetAuthGameMode<AInfeccionZombieGameMode>())
    {
        GM->InfectarZona(IndexZona, NumJugador);
    }
}

void AInfeccionZombieZona::OnRep_PropietarioActual()
{
    // Notificar a la UI (Widget Component en Blueprint)
    const FString Nombre = NombreParaPropietario(PropietarioActual);
    OnZonaCambioEstado.Broadcast(PropietarioActual, Nombre);
}

FString AInfeccionZombieZona::NombreParaPropietario(int32 Propietario) const
{
    switch (Propietario)
    {
        case 1:  return TEXT("Infectada por J1");
        case 2:  return TEXT("Infectada por J2");
        default: return TEXT("Neutral");
    }
}
