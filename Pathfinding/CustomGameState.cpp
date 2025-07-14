// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameState.h"
#include "Pathfinding.h"


void ACustomGameState::UpdateHexGrid(TArray<FIntPoint> grid)
{
	HexGrid3D = grid;
	OnGridUpdated.Broadcast();
}



void ACustomGameState::UpdatePFHexGrid()
{
	Pathfinder->UpdatePFHexGrid(HexGrid3D);
}

TArray<FIntVector> ACustomGameState::GetNeighboringNodes(FIntVector node, bool bIgnoreObstacles)
{
	NeighboringNodes.Empty();
	bNeighborsSet = false;
	
	SetNeighboringNodes(node, NeighboringNodes, bIgnoreObstacles);
	
	return NeighboringNodes;
}


void ACustomGameState::BeginPlay()
{
	Super::BeginPlay();
	// Bind delegate
	FVector loc(0, 0, 0);
	FRotator rot(0, 0, 0);
	FActorSpawnParameters spawn;
	Pathfinder = GetWorld()->SpawnActor<APathfinding>(loc, rot, spawn);
	Pathfinder->SetCustomGameState(this);
	OnGridUpdated.AddDynamic(this, &ACustomGameState::UpdatePFHexGrid);
}
