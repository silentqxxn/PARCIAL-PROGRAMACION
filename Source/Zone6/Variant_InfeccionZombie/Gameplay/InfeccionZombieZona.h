#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InfeccionZombieZona.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class AInfeccionZombieCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnZonaCambioEstado, int32, PropietarioNuevo, const FString&, NombreJugador);

UCLASS()
class ZONE6_API AInfeccionZombieZona : public AActor
{
    GENERATED_BODY()

public:
    AInfeccionZombieZona();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // Identificador único de esta zona en el mapa (asignado en el Editor)
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Zona")
    int32 IndexZona = 0;

    // Segundos que hay que permanecer parado para infectar
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Zona")
    float TiempoParaInfectar = 3.f;

    // 0=neutral, 1=J1, 2=J2 — replicado a todos los clientes
    UPROPERTY(ReplicatedUsing = OnRep_PropietarioActual, BlueprintReadOnly, Category = "Zona")
    int32 PropietarioActual = 0;

    // Delegate: la UI (Widget Component) se suscribe para actualizar el texto
    UPROPERTY(BlueprintAssignable, Category = "Zona|Eventos")
    FOnZonaCambioEstado OnZonaCambioEstado;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    USphereComponent* ZonaTrigger;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    UStaticMeshComponent* MeshCentro;

private:
    // Jugador que actualmente está intentando capturar (servidor)
    AInfeccionZombieCharacter* JugadorCapturando = nullptr;

    FTimerHandle CapturaTimer;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    void CompletarCaptura();

    UFUNCTION()
    void OnRep_PropietarioActual();

    FString NombreParaPropietario(int32 Propietario) const;
};
