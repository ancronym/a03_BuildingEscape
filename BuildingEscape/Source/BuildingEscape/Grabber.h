// Copiright scheme for n!Games is under development.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Grabber.generated.h"

struct PlayerViewInfo {
	FVector ViewPointLocation;
	FRotator ViewPointRotation;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	float Reach = 100.f;
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	class UPhysicsHandleComponent* PhysicsHandle = nullptr;
	class UInputComponent* InputComponent = nullptr;
	
	const FVector GetLineTraceEnd();
	PlayerViewInfo GetViewInfo();
	
	///Raycast and grab what is in reach.
	void Grab();
	void Release();
	
	// Find attached physics handle
	void FindPhysicsHandleComponent();

	// Setup attached input component
	void SetupInputComponent();

	// Return first physics body in reach
	const  FHitResult GetFirstPhysicsBodyInReach();
};
