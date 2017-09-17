// Copiright scheme for n!Games is under development.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void OpenDoor();
	void CloseDoor();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
private:
	float GetTotalMassOnTriggerPlate();

	UPROPERTY(EditAnywhere)
		float OpenAngle = 0.0f;

	UPROPERTY(EditAnywhere)
		float ClosedAngle = -90.0f;

	UPROPERTY(EditAnywhere) 
	ATriggerVolume* PressurePlate = nullptr;
	
	UPROPERTY(EditAnywhere)
		float DoorCloseDelay = 1.0f;

	float LastDoorOpenTime;

	UPROPERTY(EditAnywhere)
		float PlateTriggerValue = 50.f;

	class AActor* Owner = nullptr;
	
};
