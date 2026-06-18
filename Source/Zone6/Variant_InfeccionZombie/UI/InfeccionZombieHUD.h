#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InfeccionZombieHUD.generated.h"

class AInfeccionZombieGameState;
class UUserWidget;

UCLASS()
class ZONE6_API AInfeccionZombieHUD : public AHUD
{
    GENERATED_BODY()

public:
    AInfeccionZombieHUD();

    // Asignar en el Blueprint hijo: widget con contadores J1/J2 y timer
    UPROPERTY(EditDefaultsOnly, Category = "HUD")
    TSubclassOf<UUserWidget> ClaseWidgetHUD;

    // Asignar en el Blueprint hijo: pantalla de resultado final
    UPROPERTY(EditDefaultsOnly, Category = "HUD")
    TSubclassOf<UUserWidget> ClaseWidgetResultado;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    UUserWidget* WidgetHUD = nullptr;

    UPROPERTY()
    UUserWidget* WidgetResultado = nullptr;

    void SuscribirseAGameState();

    UFUNCTION()
    void OnTiempoActualizado(float TiempoRestante);

    UFUNCTION()
    void OnPartidaIniciada();

    UFUNCTION()
    void OnPartidaFinalizada(int32 ZonasJ1, int32 ZonasJ2);
};
