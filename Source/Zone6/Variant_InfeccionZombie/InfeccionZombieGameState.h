#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "InfeccionZombieGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPartidaIniciada);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnZonaInfectada, int32, IndexZona, int32, JugadorIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPartidaFinalizada, int32, ZonasJ1, int32, ZonasJ2);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTiempoActualizado, float, TiempoRestante);

UCLASS()
class ZONE6_API AInfeccionZombieGameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    AInfeccionZombieGameState();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // --- Estado replicado (visible en todos los clientes) ---

    UPROPERTY(ReplicatedUsing = OnRep_EstadoZonas, BlueprintReadOnly, Category = "Estado")
    TArray<int32> EstadoZonas; // 0=neutral, 1=J1, 2=J2

    UPROPERTY(ReplicatedUsing = OnRep_Contadores, BlueprintReadOnly, Category = "Estado")
    int32 ZonasJugador1 = 0;

    UPROPERTY(ReplicatedUsing = OnRep_Contadores, BlueprintReadOnly, Category = "Estado")
    int32 ZonasJugador2 = 0;

    UPROPERTY(ReplicatedUsing = OnRep_TiempoRestante, BlueprintReadOnly, Category = "Estado")
    float TiempoRestante = 0.0f;

    UPROPERTY(ReplicatedUsing = OnRep_PartidaActiva, BlueprintReadOnly, Category = "Estado")
    bool bPartidaActiva = false;

    // --- Delegates para Blueprint/UI (se disparan en cada cliente via RepNotify) ---

    UPROPERTY(BlueprintAssignable, Category = "Eventos")
    FOnPartidaIniciada OnPartidaIniciada;

    UPROPERTY(BlueprintAssignable, Category = "Eventos")
    FOnZonaInfectada OnZonaInfectada;

    UPROPERTY(BlueprintAssignable, Category = "Eventos")
    FOnPartidaFinalizada OnPartidaFinalizada;

    UPROPERTY(BlueprintAssignable, Category = "Eventos")
    FOnTiempoActualizado OnTiempoActualizado;

    // Llamado por el GameMode (servidor) para registrar una infección
    void Servidor_InfectarZona(int32 IndexZona, int32 JugadorIndex);
    void Servidor_IniciarPartida(float TiempoTotal, int32 TotalZonas);
    void Servidor_FinalizarPartida();
    void Servidor_ActualizarTiempo(float NuevoTiempo);

private:
    // RepNotify: se ejecutan en cada cliente cuando el valor replicado cambia
    UFUNCTION()
    void OnRep_EstadoZonas();

    UFUNCTION()
    void OnRep_Contadores();

    UFUNCTION()
    void OnRep_TiempoRestante();

    UFUNCTION()
    void OnRep_PartidaActiva();

    // Índice de la última zona infectada (para pasar al delegate en el cliente)
    int32 UltimaZonaInfectada = -1;
    int32 UltimoJugadorInfecto = 0;
};
