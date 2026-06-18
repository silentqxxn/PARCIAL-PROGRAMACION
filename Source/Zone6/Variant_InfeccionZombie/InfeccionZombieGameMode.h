#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InfeccionZombieGameMode.generated.h"

class AInfeccionZombieGameState;

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
    int32 TotalZonas = 6;

    // Llamado por el Actor de zona cuando el zombi toca el corazón
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

    int32 JugadoresConectados = 0;

    AInfeccionZombieGameState* GetIZGameState() const;

    void IniciarConteo();
    void IniciarPartida();
    void FinalizarPartida();
    void TickTiempo();
};
