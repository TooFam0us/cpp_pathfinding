# Hexagon Pathfinding

A lightweight C++ pathfinding solution for Unreal Engine on a hex grid.  
Provides A* based path search, movement-limited routes and range queries.  

---

## Features

- Find shortest path between two hex cells  
- Limit path length by movement speed  
- Query all reachable cells within a given range  
- Blueprint-assignable neighbor logic for custom maps  

---

## Requirements

- Unreal Engine 4.26 or higher (UE5 compatible)  
- C++ project with a module named `HEXAGON`  
- A grid defined as `TArray<FIntPoint>` (X, Y) with Z treated as height  

---

## Installation

1. Copy `CustomGameState.h/.cpp` and `Pathfinding.h/.cpp` into your project's `Source/YourModule/` folder.  
2. Add `Hexagon` (or your module name) to `PublicDependencyModuleNames` in your `.Build.cs`.  
3. In your `GameMode`, set `GameStateClass` to `ACustomGameState`.  
4. Implement the `SetNeighboringNodes` event in Blueprint or override it in C++ to feed valid neighbors.  

---

## Usage

### C++ Example

```cpp
// 1. Spawn happens automatically in BeginPlay of ACustomGameState.
// 2. Update the hex grid whenever your map changes:
ACustomGameState* GS = GetWorld()->GetGameState<ACustomGameState>();
TArray<FIntPoint> MyGrid = /* populate your grid */;
GS->UpdateHexGrid(MyGrid);

// 3. Find a path from start to target:
APathfinding* PF = GS->GetPathfinder();
TArray<FIntPoint> Path = PF->FindPath(
    FIntVector(0,0,HeightStart),
    FIntVector(3,2,HeightEnd),
    /*climbHeight=*/2,
    /*ignoreObstacles=*/false
);

// 4. Or get tiles in movement range:
TArray<FIntPoint> RangeTiles = PF->GetTilesInRange(
    FIntVector(1,1,Height),
    /*range=*/3,
    /*climbHeight=*/1
);
```

---

## Blueprint Setup

1. Drag your `CustomGameState` into a Blueprint class.
2. Override `SetNeighboringNodes` to output adjacent hex cells.
3. Call `UpdateHexGrid` after generating or loading your map.
4. Use `FindPath`, `FindPathWithMovementSpeed` or `GetTilesInRange` nodes in Blueprint graphs.

---

## API Reference

| Class            | Method                                 | Description                                |
|------------------|----------------------------------------|--------------------------------------------|
| ACustomGameState | UpdateHexGrid(TArray<FIntPoint>)       | Broadcasts grid changes to the pathfinder  |
| APathfinding     | FindPath(...)                          | Full A* path between two hex coordinates   |
| APathfinding     | GetTilesInRange(...)                   | All reachable cells within a range         |

---

## Customization

- Override `SetNeighboringNodes` to account for custom terrain, obstacles or wrapping worlds.
- Tweak `GetDistance` in `Pathfinding.cpp` for different cost heuristics.
- Expose additional properties or debug logs as needed.

---

## Contributing

Feel free to submit issues, pull requests or suggest improvements.
