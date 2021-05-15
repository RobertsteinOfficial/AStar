// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UGrid::UGrid()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	nodeDiameter = nodeRadius * 2;
	gridSizeX = FMath::RoundHalfFromZero(gridWorldSize.X / nodeDiameter);
	gridSizeY = FMath::RoundHalfFromZero(gridWorldSize.Y / nodeDiameter);

	UE_LOG(LogTemp, Warning, TEXT("%d" "%d"), gridSizeX, gridSizeY);
}


// Called when the game starts
void UGrid::BeginPlay()
{
	Super::BeginPlay();

	CreateGrid();

	DrawDebugBox(GetWorld(), GetOwner()->GetActorLocation(), FVector(gridWorldSize.X / 2, gridWorldSize.Y / 2, 100), FColor::White, true, 0, 0, 10);


	if (grid)
	{
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

				//devo creare un transform apposta, perchè quello indica la location
				FTransform trans;
				trans.SetLocation(grid[i][j].worldPosition + FVector::UpVector * 10);

				DrawDebugSolidBox
				(GetWorld(),
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

	// ...
}

void UGrid::CreateGrid()
{

	FVector worldBottomLeft = GetOwner()->GetActorLocation() - FVector::RightVector * gridWorldSize.X / 2 -
		FVector::ForwardVector * gridWorldSize.Y / 2;

	UE_LOG(LogTemp, Warning, TEXT("%s"), *worldBottomLeft.ToString());


	for (int i = 0; i < gridSizeX; i++)
	{
		for (int j = 0; j < gridSizeY; j++)
		{
			//qua era rightvector, perchè sottraggo
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

			//devo inizializare la classe
			grid[i][j] = Node(walkable, worldPoint);
		}
	}

}