// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UGrid::UGrid()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


}

// Called when the game starts
void UGrid::BeginPlay()
{
	Super::BeginPlay();

	nodeDiameter = nodeRadius * 2;
	gridSizeX = FMath::RoundHalfFromZero(gridWorldSize.X / nodeDiameter);
	gridSizeY = FMath::RoundHalfFromZero(gridWorldSize.Y / nodeDiameter);


	CreateGrid();

	DrawDebugBox(GetWorld(), GetOwner()->GetActorLocation(), FVector(gridWorldSize.X / 2, gridWorldSize.Y / 2, 100), FColor::White, true, 0, 0, 10);


	if (grid)
	{
		//Node playerNode = NodeFromWorldPoint(player->GetActorLocation());


		for (int i = 0; i < gridSizeX; i++)
		{
			for (int j = 0; j < gridSizeY; j++)
			{

				FColor col;

				if (grid[i][j].walkable)
				{
					col = FColor(0, 255, 0);
				}
				else
				{
					col = FColor(255, 0, 0);
				}

				/*if (playerNode.worldPosition == grid[i][j].worldPosition)
				{
					col = FColor(255, 180, 60);
				}*/

				//devo creare un transform apposta, perchè quello indica la location
				FTransform trans;
				trans.SetLocation(grid[i][j].worldPosition + FVector::UpVector * 10);

				DrawDebugSolidBox
				(
					GetWorld(),
					FBox(FVector(0, 0, 0), FVector(nodeDiameter - 20)),		//parametro min è il centro, max è l'estensione massima
					col,
					trans,
					true,
					0,
					0
				);



			}
		}

	}
}

// Called every frame
void UGrid::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (int i = 0; i < gridSizeX; i++)
	{
		for (int j = 0; j < gridSizeY; j++)
		{
			if (path.Num() > 0)
			{
				for (size_t n = 0; n < path.Num(); n++)
				{
					if (path[i]->nodeID == grid[i][j].nodeID)
					{
						FTransform trans;
						trans.SetLocation(grid[i][j].worldPosition + FVector::UpVector * 20);

						DrawDebugSolidBox
						(
							GetWorld(),
							FBox(FVector(0, 0, 0), FVector(nodeDiameter - 20)),		//parametro min è il centro, max è l'estensione massima
							FColor(0,0,255),
							trans,
							true,
							0,
							0
						);
					}
				}
			}

		}
	}
}

void UGrid::CreateGrid()
{

	FVector worldBottomLeft = GetOwner()->GetActorLocation() - FVector::RightVector * gridWorldSize.X / 2 -
		FVector::ForwardVector * gridWorldSize.Y / 2;

	int IDCounter = 0;

	for (int i = 0; i < gridSizeX; i++)
	{
		for (int j = 0; j < gridSizeY; j++)
		{
			FVector worldPoint = worldBottomLeft + FVector::RightVector * (i  * nodeDiameter + nodeRadius) +
				FVector::ForwardVector * (j * nodeDiameter + nodeRadius);


			FHitResult hitRes;
			FCollisionQueryParams SweepParams(FName(TEXT("")), false, GetOwner());

			bool walkable = !GetWorld()->SweepSingleByChannel
			(
				hitRes,
				worldPoint,
				worldPoint,
				FQuat::Identity,
				ECollisionChannel::ECC_GameTraceChannel1,
				FCollisionShape::MakeSphere(nodeRadius),
				SweepParams,
				FCollisionResponseParams::DefaultResponseParam
			);


			grid[i][j] = Node(IDCounter++, walkable, worldPoint, i, j);
		}
	}

}

Node UGrid::NodeFromWorldPoint(FVector worldPosition)
{
	float percentX = (worldPosition.X + gridWorldSize.X / 2) / gridWorldSize.X;
	float percentY = (worldPosition.Y + gridWorldSize.Y / 2) / gridWorldSize.Y;

	percentX = FMath::Clamp(percentX, 0.f, 1.f);
	percentY = FMath::Clamp(percentY, 0.f, 1.f);

	int x = FMath::RoundHalfFromZero((gridSizeX - 1) * percentX);
	int y = FMath::RoundHalfFromZero((gridSizeY - 1) * percentY);

	return grid[y][x];
}

TArray<Node> UGrid::GetNeighbours(Node node)
{
	TArray <Node> neighbours;

	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			if (x == 0 && y == 0)
			{
				continue;
			}

			int checkX = node.gridX + x;
			int checkY = node.gridY + y;

			if (checkX >= 0 && checkX < gridSizeX && checkY >= 0 && checkY < gridSizeY)
			{
				neighbours.Add(grid[checkX][checkY]);
			}
			
		}
	}

	return neighbours;
}
