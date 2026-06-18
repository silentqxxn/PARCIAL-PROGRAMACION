#include "Variant_InfeccionZombie/Gameplay/InfeccionZombieSlowItem.h"
#include "Variant_InfeccionZombie/InfeccionZombieCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

AInfeccionZombieSlowItem::AInfeccionZombieSlowItem()
{
    bReplicates = true;
    PrimaryActorTick.bCanEverTick = false;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    SetRootComponent(Mesh);

    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
    CollisionComp->SetupAttachment(Mesh);
    CollisionComp->SetSphereRadius(60.f);
    CollisionComp->SetCollisionProfileName(TEXT("Trigger"));
}

void AInfeccionZombieSlowItem::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AInfeccionZombieSlowItem::OnOverlapBegin);
    }
}

void AInfeccionZombieSlowItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (!bActivo) return;

    AInfeccionZombieCharacter* Jugador = Cast<AInfeccionZombieCharacter>(OtherActor);
    if (!Jugador) return;

    AplicarEfecto(Jugador);
}

void AInfeccionZombieSlowItem::AplicarEfecto(AInfeccionZombieCharacter* Objetivo)
{
    bActivo = false;
    Objetivo->SetRalentizado(true);
    Multicast_OcultarItem();

    // Capturar referencia débil para el timer
    TWeakObjectPtr<AInfeccionZombieCharacter> ObjetivoDebil = Objetivo;
    FTimerDelegate TimerDelegate;
    TimerDelegate.BindLambda([this, ObjetivoDebil]()
    {
        if (ObjetivoDebil.IsValid())
        {
            TerminarEfecto(ObjetivoDebil.Get());
        }
        else
        {
            Destroy();
        }
    });

    GetWorldTimerManager().SetTimer(EfectoTimer, TimerDelegate, DuracionEfecto, false);
}

void AInfeccionZombieSlowItem::TerminarEfecto(AInfeccionZombieCharacter* Objetivo)
{
    Objetivo->SetRalentizado(false);
    // El GameMode se encarga del respawn; este actor se destruye
    Destroy();
}

void AInfeccionZombieSlowItem::Multicast_OcultarItem_Implementation()
{
    SetActorHiddenInGame(true);
    CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
