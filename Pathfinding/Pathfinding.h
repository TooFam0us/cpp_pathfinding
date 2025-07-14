// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomGameState.h"
#include "Pathfinding.generated.h"

USTRUCT()
struct FHexNode {
	GENERATED_BODY()
	FIntVector worldPosition;
	bool bIsWalkable;

	int hCost;
	int gCost;

	int fCost() {
		return hCost + gCost;
	};

	FHexNode* parent;

	bool operator== (const FHexNode& B) const {
		return worldPosition == B.worldPosition;
	};

	FHexNode(): 
		worldPosition(0,0,0),
		bIsWalkable(false),
		hCost(0),
		gCost(0),
		parent(nullptr)
	{}

	FHexNode(FIntVector worldPos, bool bWalkable, int hC, int gC, FHexNode* curParent) :
		worldPosition(worldPos),
		bIsWalkable(bWalkable),
		hCost(hC),
		gCost(gC),
		parent(curParent)
	{}

};

FORCEINLINE uint32 GetTypeHash(const FHexNode& MidiTime)
{
	uint32 Hash = FCrc::MemCrc32(&MidiTime, sizeof(FHexNode));
	return Hash;
}

UCLASS()
class HEXAGON_API APathfinding : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APathfinding();



	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		TArray<FIntPoint> CurrentHexGrid;


	UFUNCTION()
		void UpdatePFHexGrid(TArray<FIntPoint> grid);

	UFUNCTION(BlueprintCallable)
		TArray<FIntPoint> FindPath(FIntVector startPos, FIntVector targetPos, int32 climbHeight, bool bIgnoreObstacles = false);

	UFUNCTION(BlueprintCallable)
		TArray<FIntPoint> FindPathWithMovementSpeed(FIntVector startPos, FIntVector targetPos, int32 movementSpeed, int32 climbHeight, bool bIgnoreObstacles = false);


	UFUNCTION(BlueprintCallable)
		TArray<FIntPoint> GetTilesInRange(FIntVector startPos, int32 range, int32 climbHeight);

	TArray<FHexNode*> RetracePath(FHexNode startNode, FHexNode endNode);

	void SetCustomGameState(ACustomGameState* gs) {
		GS = gs;
	};

	ACustomGameState* GS;

	int GetDistance(FIntVector A, FIntVector B);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};


