// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFinding.h"
#include "GameFramework/Actor.h"
#include "Algo/Reverse.h"

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

	if (seeker && target)
	{
		FVector seekerPos = seeker->GetActorLocation();
		FVector targetPos = target->GetActorLocation();

		FindPath(seekerPos, targetPos);
	}
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

		int index = 0;

		while (openSet.Num() > 0)
		{
			Node currentNode = openSet[0];

			for (int i = 0; i < openSet.Num(); i++)
			{
				if (openSet[i].FCost() < currentNode.FCost() ||
					openSet[i].FCost() == currentNode.FCost() && openSet[i].hCost < currentNode.hCost)
				{
					currentNode = openSet[i];
					index = i;
				}
			}

			//openSet.Remove(currentNode);
			openSet.RemoveAt(index);
			closedSet.Add(currentNode);


			if (currentNode.worldPosition == targetNode.worldPosition)
			{
				RetracePath(startNode, targetNode);
				return;
			}

			TArray <Node> neighbours = grid->GetNeighbours(currentNode);

			for (int i = 0; i < neighbours.Num(); i++)
			{
				if (!neighbours[i].walkable || CheckIfNodeInList(closedSet, neighbours[i].nodeID))
				{
					continue;
				}


				int newMovementCostToNeighbour = currentNode.gCost + GetDistance(currentNode, neighbours[i]);
				if (!CheckIfNodeInList(openSet, neighbours[i].nodeID) || newMovementCostToNeighbour < neighbours[i].gCost)
				{
					neighbours[i].gCost = newMovementCostToNeighbour;
					neighbours[i].hCost = GetDistance(neighbours[i], targetNode);

					neighbours[i].parentNode = &currentNode;

					if (!CheckIfNodeInList(openSet, neighbours[i].nodeID))
					{
						openSet.Add(neighbours[i]);
					}
				}


			}

		}
	}
}

int UPathFinding::GetDistance(Node nodeA, Node nodeB)
{
	int dstX = FMath::Abs(nodeA.gridX - nodeB.gridX);
	int dstY = FMath::Abs(nodeA.gridY - nodeB.gridY);

	if (dstX > dstY)
	{
		return 14 * dstY + 10 * (dstX - dstY);
	}

	return 14 * dstX + 10 * (dstY - dstX);
}

bool UPathFinding::CheckIfNodeInList(TArray<Node> list, int NodeID)
{
	for (int i = 0; i < list.Num(); i++)
	{
		if (list[i].nodeID == NodeID)
		{
			return true;
		}
	}

	return false;
}

void UPathFinding::RetracePath(Node startNode, Node endNode)
{
	TArray<Node*> path;
	Node* currentNode = &endNode;

	if (currentNode)
	{
		while (currentNode->nodeID != startNode.nodeID)
		{
			path.Add(currentNode);
			currentNode = currentNode->parentNode;
		}
	}

	Algo::Reverse(path);
	if (grid)
	{
		grid->path = path;
	}
}
