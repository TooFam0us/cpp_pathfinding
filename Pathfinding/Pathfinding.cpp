// Fill out your copyright notice in the Description page of Project Settings.


#include "Pathfinding.h"
#include "Algo/Reverse.h"

// Sets default values
APathfinding::APathfinding()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void APathfinding::UpdatePFHexGrid(TArray<FIntPoint> grid)
{
	CurrentHexGrid = grid;
}

TArray<FIntPoint> APathfinding::FindPath(FIntVector startPos, FIntVector targetPos, int32 climbHeight, bool bIgnoreObstacles)
{
	TArray<FHexNode> openSet;
	TArray<FHexNode> closedSet;
	TArray<FIntPoint> path = TArray<FIntPoint>();

	FHexNode startNode = FHexNode();
	startNode.worldPosition = startPos;
	FHexNode targetNode = FHexNode();
	targetNode.worldPosition = targetPos;
	openSet.Add(startNode);

	while (openSet.Num() > 0) {
		FHexNode currentNode = *openSet.begin();
		for (auto it = openSet.begin(); it != openSet.end(); ++it)
		{
			FHexNode& node = *it;
			if (node.fCost() < currentNode.fCost() || (node.fCost() == currentNode.fCost() && node.hCost < currentNode.hCost)) {
				currentNode = *it;
			}
		}

		openSet.Remove(currentNode);
		closedSet.Add(currentNode);

		if (currentNode == targetNode)
		{
			targetNode = currentNode;
			TArray<FHexNode*> nodePath = RetracePath(startNode, targetNode);
			for (int32 i = 0; i < nodePath.Num(); i++)
			{
				path.Add(FIntPoint(nodePath[i]->worldPosition.X, nodePath[i]->worldPosition.Y));
			}
			return path;
		}
		TArray<FIntVector> neigbors = GS->GetNeighboringNodes(currentNode.worldPosition, bIgnoreObstacles);
		for (const FIntVector& neighbour : neigbors) {
			FHexNode neighbourNode;
			neighbourNode.worldPosition = neighbour;

			if (closedSet.Contains(neighbourNode))
			{
				continue;
			}

			if (FMath::Abs(neighbourNode.worldPosition.Z - currentNode.worldPosition.Z) > climbHeight)
			{
				continue;
			}

			int newMovementCostToNeighbour = currentNode.gCost + GetDistance(currentNode.worldPosition, neighbourNode.worldPosition);
			if (newMovementCostToNeighbour < neighbourNode.gCost || !openSet.Contains(neighbourNode)) {
				neighbourNode.gCost = newMovementCostToNeighbour;
				neighbourNode.hCost = GetDistance(neighbourNode.worldPosition, targetNode.worldPosition);
				FHexNode* curNode = new FHexNode(currentNode.worldPosition, currentNode.bIsWalkable,
					currentNode.gCost, currentNode.hCost, currentNode.parent == nullptr ? nullptr : currentNode.parent);

				neighbourNode.parent = curNode;
				UE_LOG(LogTemp, Warning, TEXT("Assigned parent for %s to %s"), *neighbourNode.worldPosition.ToString(), *neighbourNode.parent->worldPosition.ToString());
				if (!openSet.Contains(neighbourNode))
				{
					openSet.Add(neighbourNode);
				}
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Failed to get path"));
	return path;

}

TArray<FHexNode*> APathfinding::RetracePath(FHexNode startNode, FHexNode endNode)
{
	TArray<FHexNode*> path;
	FHexNode* currentNode = &endNode;

	while (currentNode != &startNode) {
		path.Add(currentNode);
		if (currentNode->parent != nullptr)
		{
			currentNode = currentNode->parent;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No parent found for %s"), *(*currentNode).worldPosition.ToString());
			break;
		}
	}
	Algo::Reverse(path);
	return path;
}

TArray<FIntPoint> APathfinding::GetTilesInRange(FIntVector startPos, int32 range, int32 climbHeight)
{
	TArray<FIntPoint> inRangeTiles = TArray<FIntPoint>();
	int32 stepCount = 0;
	inRangeTiles.Add(FIntPoint(startPos.X, startPos.Y));
	TArray<FIntVector> tileForPreviousStep = TArray<FIntVector>();
	tileForPreviousStep.Add(startPos);

	while (stepCount < (range))
	{
		TArray<FIntVector> neighborTiles = TArray<FIntVector>();
		for (FIntVector item : tileForPreviousStep)
		{
			TArray<FIntVector> nodes = GS->GetNeighboringNodes(item, false);
			for (FIntVector tile : nodes)
			{
				if (FMath::Abs(item.Z - tile.Z) > climbHeight)
				{
					continue;
				}
				neighborTiles.AddUnique(tile);
			}
		}
		for (FIntVector tile : neighborTiles)
		{
			inRangeTiles.AddUnique(FIntPoint(tile.X, tile.Y));
		}
		tileForPreviousStep = neighborTiles;
		stepCount++;
	}
	UE_LOG(LogTemp, Warning, TEXT("inRangeTiles count %d"), inRangeTiles.Num());
	return inRangeTiles;
}

int APathfinding::GetDistance(FIntVector A, FIntVector B)
{
	int dstX = FMath::Abs(A.X - B.X);
	int dstY = FMath::Abs(A.Y - B.Y);

	if (dstX > dstY) {
		return 14 * dstY + 10 * (dstX - dstY);
	}
	return 14 * dstX + 10 * (dstY - dstX);
}

// Called when the game starts or when spawned
void APathfinding::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APathfinding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<FIntPoint> APathfinding::FindPathWithMovementSpeed(FIntVector startPos, FIntVector targetPos, int32 movementSpeed, int32 climbHeight, bool bIgnoreObstacles)
{
	TArray<FIntPoint> tempPath = FindPath(startPos, targetPos, climbHeight, bIgnoreObstacles);
	TArray<FIntPoint> path = TArray<FIntPoint>();
	if (movementSpeed >= tempPath.Num())
	{
		movementSpeed = tempPath.Num();
	}
	if (movementSpeed > 0)
	{
		for (int32 i = 0; i < movementSpeed; i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("Assigned path item %s"), *tempPath[i].ToString());
			path.Add(tempPath[i]);
		}
	}
	return path;
}

