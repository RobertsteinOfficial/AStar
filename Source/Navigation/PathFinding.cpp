// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFinding.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UPathFinding::UPathFinding()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


}


// Called when the game starts
void UPathFinding::BeginPlay()
{
	Super::BeginPlay();

	grid = GetOwner()->FindComponentByClass<UGrid>();

}

// Called every frame
void UPathFinding::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPathFinding::FindPath(FVector startPos, FVector targetPos)
{
	if (grid)
	{
		Node startNode = grid->NodeFromWorldPoint(startPos);
		Node targetNode = grid->NodeFromWorldPoint(targetPos);

		TArray<Node> openSet;
		TArray<Node> closedSet;

		openSet.Add(startNode);

		int index = -1;

		while (openSet.Num() > 0)
		{
			Node currentNode = openSet[0];

			for (int i = 0; i < openSet.Num(); i++)
			{
				if (openSet[i].fCost < currentNode.fCost ||
					openSet[i].fCost == currentNode.fCost && openSet[i].hCost < currentNode.hCost)
				{
					currentNode = openSet[i];
					index = i;
				}
			}

			if (index > -1)
			{
				//openSet.Remove(currentNode);
				openSet.RemoveAt(index);
				closedSet.Add(currentNode);
			}

			if (currentNode.worldPosition == targetNode.worldPosition)
			{
				//TO DO recuperare il path
				return;
			}



		}
	}
}
