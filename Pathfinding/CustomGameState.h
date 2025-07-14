// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CustomGameState.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHexGridUpdate);

class APathfinding;

UCLASS()
class HEXAGON_API ACustomGameState : public AGameStateBase
{
	GENERATED_BODY()

public: 
	

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "HexGrid")
		FHexGridUpdate OnGridUpdated;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bNeighborsSet = false;

	APathfinding* Pathfinder;

	TArray<FIntVector> NeighboringNodes;

	/**
	* Debug this shit? if it randomly loses reference to pathfinder
	*/
	UFUNCTION(BlueprintCallable)
		APathfinding* GetPathfinder() { return Pathfinder; };

	/**
	* Finding height from finding it with fintpoint
	*/
	TArray<FIntPoint> HexGrid3D;

	TArray<FIntPoint> GetHexGrid() { return HexGrid3D; };

	UFUNCTION(BlueprintImplementableEvent)
	void SetNeighboringNodes(FIntVector node, TArray<FIntVector>& nodesToReturn, bool bIgnoreObstacles = false);


	TArray<FIntVector> GetNeighboringNodes(FIntVector node, bool bIgnoreObstacles = false);

	/**
	* Update the current hexgrid
	*/
	UFUNCTION(BlueprintCallable)
		void UpdateHexGrid(TArray<FIntPoint> grid);
	
	UFUNCTION()
		void UpdatePFHexGrid();


private:

	void BeginPlay();
};
