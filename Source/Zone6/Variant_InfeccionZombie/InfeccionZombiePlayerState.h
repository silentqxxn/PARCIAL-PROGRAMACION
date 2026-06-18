#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "InfeccionZombiePlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPuntajeActualizado, int32, NuevoPuntaje);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEstadoRalentizadoCambiado, bool, bEstaRalentizado);

UCLASS()
class ZONE6_API AInfeccionZombiePlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    AInfeccionZombiePlayerState();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // Número de jugador: 1 o 2, asignado por el GameMode en PostLogin
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Jugador")
    int32 NumeroJugador = 0;

    // Puntaje individual: zonas infectadas actualmente
    UPROPERTY(ReplicatedUsing = OnRep_ZonasInfectadas, BlueprintReadOnly, Category = "Puntaje")
    int32 ZonasInfectadas = 0;

    // Estado: true si el ítem de ralentización está activo sobre este jugador
    UPROPERTY(ReplicatedUsing = OnRep_Ralentizado, BlueprintReadOnly, Category = "Estado")
    bool bRalentizado = false;

    // Delegates para que la UI reaccione en cada cliente
    UPROPERTY(BlueprintAssignable, Category = "Eventos")
    FOnPuntajeActualizado OnPuntajeActualizado;

    UPROPERTY(BlueprintAssignable, Category = "Eventos")
    FOnEstadoRalentizadoCambiado OnEstadoRalentizadoCambiado;

    // Llamado por el GameMode (servidor) para actualizar el puntaje
    void Servidor_SetZonasInfectadas(int32 Nuevas);

    // RPC servidor: el Character lo llama cuando recoge/pierde el ítem
    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Estado")
    void Server_SetRalentizado(bool bNuevoEstado);

private:
    UFUNCTION()
    void OnRep_ZonasInfectadas();

    UFUNCTION()
    void OnRep_Ralentizado();
};
