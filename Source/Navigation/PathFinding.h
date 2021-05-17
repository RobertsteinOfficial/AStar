// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Grid.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PathFinding.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NAVIGATION_API UPathFinding : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPathFinding();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
		
	UGrid* grid;

	void FindPath(FVector startPos, FVector targetPos);
};
