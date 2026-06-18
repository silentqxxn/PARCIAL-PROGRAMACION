#pragma once

#include "CoreMinimal.h"
#include "Zone6Character.h"
#include "InfeccionZombieCharacter.generated.h"

class UInputAction;

UCLASS()
class ZONE6_API AInfeccionZombieCharacter : public AZone6Character
{
    GENERATED_BODY()

public:
    AInfeccionZombieCharacter();

    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // --- Configuración de movimiento ---

    UPROPERTY(EditDefaultsOnly, Category = "Movimiento")
    float VelocidadBase = 500.f;

    UPROPERTY(EditDefaultsOnly, Category = "Movimiento")
    float VelocidadRalentizado = 200.f;

    // --- Estado replicado ---

    UPROPERTY(ReplicatedUsing = OnRep_Ralentizado, BlueprintReadOnly, Category = "Estado")
    bool bRalentizado = false;

    // Aplica o quita el efecto de ralentización (llamado desde SlowItem en el servidor)
    UFUNCTION(BlueprintCallable, Category = "Movimiento")
    void SetRalentizado(bool bNuevoEstado);

    // Número de jugador (1 o 2), para que la zona sepa a quién atribuir la infección
    UFUNCTION(BlueprintPure, Category = "Jugador")
    int32 ObtenerNumeroJugador() const;

protected:
    virtual void BeginPlay() override;

private:
    UFUNCTION()
    void OnRep_Ralentizado();
};
