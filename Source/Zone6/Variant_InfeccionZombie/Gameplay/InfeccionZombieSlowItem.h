#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InfeccionZombieSlowItem.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class AInfeccionZombieCharacter;

UCLASS()
class ZONE6_API AInfeccionZombieSlowItem : public AActor
{
    GENERATED_BODY()

public:
    AInfeccionZombieSlowItem();

    // Duración del efecto de ralentización en segundos
    UPROPERTY(EditDefaultsOnly, Category = "SlowItem")
    float DuracionEfecto = 3.f;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    USphereComponent* CollisionComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    UStaticMeshComponent* Mesh;

private:
    bool bActivo = true;

    FTimerHandle EfectoTimer;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                        bool bFromSweep, const FHitResult& SweepResult);

    // Servidor: aplica efecto al jugador y oculta el ítem
    void AplicarEfecto(AInfeccionZombieCharacter* Objetivo);

    // Quita el efecto y destruye el ítem (el GameMode lo respawnea)
    void TerminarEfecto(AInfeccionZombieCharacter* Objetivo);

    // Sincroniza visibilidad en todos los clientes
    UFUNCTION(NetMulticast, Reliable)
    void Multicast_OcultarItem();
};
