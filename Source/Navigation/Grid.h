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

	int gCost;
	int hCost;
	int fCost;

	int gridX;
	int gridY;

	Node()
	{
		walkable = false;
		worldPosition = FVector::ZeroVector;
		gCost = 0;
		hCost = 0;
		fCost = 0;
		gridX = 0;
		gridY = 0;
	}

	Node(bool _walkable, FVector _worldPos, int _gridX, int _gridY)
	{
		walkable = _walkable;
		worldPosition = _worldPos;

		gridX = _gridX;
		gridY = _gridY;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	AActor* player;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	Node NodeFromWorldPoint(FVector worldPosition);
	TArray<Node> GetNeighbours(Node node);

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
