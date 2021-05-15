// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Grid.generated.h"


class Node
{
public:
	bool walkable;
	FVector worldPosition;

	Node()
	{
		walkable = false;
		worldPosition = FVector::ZeroVector;
	}

	Node(bool _walkable, FVector _worldPos)
	{
		walkable = _walkable;
		worldPosition = _worldPos;
	}
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NAVIGATION_API UGrid : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGrid();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
		FVector2D gridWorldSize {1000, 1000};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
		float nodeRadius = 50.f;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	Node grid[20][20];
	float nodeDiameter = 100.f;
	int gridSizeX = 20;
	int gridSizeY = 20;

	void CreateGrid();
};
