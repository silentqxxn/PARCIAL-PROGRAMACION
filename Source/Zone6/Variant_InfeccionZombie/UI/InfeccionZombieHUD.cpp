#include "Variant_InfeccionZombie/UI/InfeccionZombieHUD.h"
#include "Variant_InfeccionZombie/InfeccionZombieGameState.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"

AInfeccionZombieHUD::AInfeccionZombieHUD()
{
}

void AInfeccionZombieHUD::BeginPlay()
{
    Super::BeginPlay();

    // Crear y mostrar el widget de HUD inmediatamente (siempre visible)
    if (ClaseWidgetHUD)
    {
        WidgetHUD = CreateWidget<UUserWidget>(GetWorld(), ClaseWidgetHUD);
        if (WidgetHUD)
        {
            WidgetHUD->AddToViewport(0);
        }
    }

    SuscribirseAGameState();
}

void AInfeccionZombieHUD::SuscribirseAGameState()
{
    AInfeccionZombieGameState* GS = GetWorld()->GetGameState<AInfeccionZombieGameState>();
    if (!GS)
    {
        FTimerHandle RetryTimer;
        GetWorldTimerManager().SetTimer(RetryTimer, this, &AInfeccionZombieHUD::SuscribirseAGameState, 0.5f, false);
        return;
    }

    GS->OnPartidaIniciada.AddDynamic(this, &AInfeccionZombieHUD::OnPartidaIniciada);
    GS->OnTiempoActualizado.AddDynamic(this, &AInfeccionZombieHUD::OnTiempoActualizado);
    GS->OnPartidaFinalizada.AddDynamic(this, &AInfeccionZombieHUD::OnPartidaFinalizada);
}

void AInfeccionZombieHUD::OnPartidaIniciada()
{
    // El widget ya está visible; nada extra que hacer aquí
}

void AInfeccionZombieHUD::OnTiempoActualizado(float TiempoRestante)
{
    // El Widget Blueprint actualiza su texto leyendo GameState->TiempoRestante
    // o bindeando directamente la variable desde el Widget
}

void AInfeccionZombieHUD::OnPartidaFinalizada(int32 ZonasJ1, int32 ZonasJ2)
{
    // Ocultar HUD de juego
    if (WidgetHUD) WidgetHUD->RemoveFromParent();

    // Mostrar pantalla de resultado
    if (ClaseWidgetResultado)
    {
        WidgetResultado = CreateWidget<UUserWidget>(GetWorld(), ClaseWidgetResultado);
        if (WidgetResultado)
        {
            WidgetResultado->AddToViewport(1);
        }
    }
}
