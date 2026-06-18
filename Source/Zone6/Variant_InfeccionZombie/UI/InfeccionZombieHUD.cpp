#include "Variant_InfeccionZombie/UI/InfeccionZombieHUD.h"
#include "Variant_InfeccionZombie/InfeccionZombieGameState.h"
#include "Kismet/GameplayStatics.h"

AInfeccionZombieHUD::AInfeccionZombieHUD()
{
}

void AInfeccionZombieHUD::BeginPlay()
{
    Super::BeginPlay();
    SuscribirseAGameState();
}

void AInfeccionZombieHUD::SuscribirseAGameState()
{
    AInfeccionZombieGameState* GS = GetWorld()->GetGameState<AInfeccionZombieGameState>();
    if (!GS)
    {
        // El GameState puede no estar listo aún; intentar en el siguiente tick
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
    // Señal para que el Widget Blueprint muestre el HUD de juego
}

void AInfeccionZombieHUD::OnTiempoActualizado(float TiempoRestante)
{
    // Señal para que el Widget Blueprint actualice el cronómetro
    // El tiempo en segundos está disponible en TiempoRestante
}

void AInfeccionZombieHUD::OnPartidaFinalizada(int32 ZonasJ1, int32 ZonasJ2)
{
    // Señal para que el Widget Blueprint muestre la pantalla de resultado
    // ZonasJ1 y ZonasJ2 contienen los puntajes finales
}
