// Fill out your copyright notice in the Description page of Project Settings.

#include "PositionReporter2.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UPositionReporter2::UPositionReporter2()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPositionReporter2::BeginPlay()
{
	Super::BeginPlay(); // Do first what should happen up in the inheritance tree.

	FString ObjectName = GetOwner()->GetName();
	FString ObjectPos = GetOwner()->GetActorTransform().GetLocation().ToString();
	UE_LOG(LogTemp, Warning, TEXT("%s is at %s"), *ObjectName, *ObjectPos); // * infront of ObjectName converts the Fstring into TCHAR array,wut
}


// Called every frame
void UPositionReporter2::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

 