#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InfeccionZombieGameMode.generated.h"

class AInfeccionZombieGameState;
class AInfeccionZombiePlayerState;

UCLASS()
class ZONE6_API AInfeccionZombieGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AInfeccionZombieGameMode();

    virtual void PostLogin(APlayerController* NewPlayer) override;

    UPROPERTY(EditDefaultsOnly, Category = "GameMode|Config")
    float TiempoConteoInicial = 5.0f;

    UPROPERTY(EditDefaultsOnly, Category = "GameMode|Config")
    float TiempoPartida = 600.0f;

    UPROPERTY(EditDefaultsOnly, Category = "GameMode|Config")
    int32 TotalZonas = 4;

    // Intervalo entre spawns del ítem de ralentización (segundos)
    UPROPERTY(EditDefaultsOnly, Category = "GameMode|Config")
    float IntervaloSpawnItem = 20.0f;

    // Posiciones donde puede spawnear el ítem (configurar en Blueprint)
    UPROPERTY(EditDefaultsOnly, Category = "GameMode|Config")
    TArray<FVector> PosicionesSpawnItem;

    // Clase del ítem de ralentización a spawnear
    UPROPERTY(EditDefaultsOnly, Category = "GameMode|Config")
    TSubclassOf<AActor> ClaseSlowItem;

    // Llamado por AInfeccionZombieZona cuando se completa la captura
    UFUNCTION(BlueprintCallable, Category = "GameMode")
    void InfectarZona(int32 IndexZona, int32 JugadorIndex);

    UFUNCTION(BlueprintPure, Category = "GameMode")
    int32 ObtenerGanador() const;

protected:
    virtual void BeginPlay() override;

private:
    FTimerHandle ConteoInicialTimer;
    FTimerHandle PartidaTimer;
    FTimerHandle TickTimer;
    FTimerHandle SpawnItemTimer;

    int32 JugadoresConectados = 0;

    AInfeccionZombieGameState* GetIZGameState() const;

    void IniciarConteo();
    void IniciarPartida();
    void FinalizarPartida();
    void TickTiempo();
    void SpawnearItem();

    void ActualizarPuntajesPlayerState();
};
