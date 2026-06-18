#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InfeccionZombieHUD.generated.h"

class AInfeccionZombieGameState;

UCLASS()
class ZONE6_API AInfeccionZombieHUD : public AHUD
{
    GENERATED_BODY()

public:
    AInfeccionZombieHUD();

protected:
    virtual void BeginPlay() override;

private:
    // Se suscribe a los delegates del GameState
    void SuscribirseAGameState();

    UFUNCTION()
    void OnTiempoActualizado(float TiempoRestante);

    UFUNCTION()
    void OnPartidaIniciada();

    UFUNCTION()
    void OnPartidaFinalizada(int32 ZonasJ1, int32 ZonasJ2);
};
