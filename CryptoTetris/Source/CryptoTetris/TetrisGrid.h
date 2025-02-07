// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TetrominoShape.h"
#include "Blueprint/UserWidget.h"
#include "TetrisGrid.generated.h"

UCLASS()
class CRYPTOTETRIS_API ATetrisGrid : public APawn
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ATetrisGrid();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tetromino Shapes")
        TArray<FTetrominoShape> TetrominoShapes;

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tetris")
        int32 GridWidth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tetris")
        int32 GridHeight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tetris")
        TSubclassOf<AActor> TetrisBlockBP;

    UFUNCTION(BlueprintCallable, Category = "Tetris")
        void SpawnTetromino();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
        int32 Score; // Score variable

    UPROPERTY(EditAnywhere, Category = "UI")
        TSubclassOf<UUserWidget> ScoreWidgetClass;

    UPROPERTY(EditAnywhere, Category = "UI")
        TSubclassOf<UUserWidget> TitleScreenWidgetClass;

    UPROPERTY(EditAnywhere, Category = "Tetromino Blueprints")
        TArray<TSubclassOf<AActor>> TetrominoBlueprints;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tetromino Blueprint Blocks")
        TSubclassOf<AActor> BitcoinBP;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tetromino Blueprint Blocks")
        TSubclassOf<AActor> EthereumBP;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tetromino Blueprint Blocks")
        TSubclassOf<AActor> PolkadotBP;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tetromino Blueprint Blocks")
        TSubclassOf<AActor> SolanaBP;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tetromino Blueprint Blocks")
        TSubclassOf<AActor> XrpBP;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tetromino Blueprint Blocks")
        TSubclassOf<AActor> TetherBP;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tetromino Blueprint Blocks")
        TSubclassOf<AActor> UsdcBP;

    void GameOver();

    UUserWidget* ScoreWidget;

    void CheckAndClearFullRows();
    void ClearRow(int32 y);
    void MoveRowsDown(int32 ClearedRow);
    void MoveTetrominoLeft();
    void MoveTetrominoRight();
    void RotateTetromino();
    void StartFastDrop();
    void StopFastDrop();

private:
    TArray<AActor*> CurrentTetrominoBlocks;
    FVector SpawnLocation;
    float BlockFallSpeed;
    bool bIsBlockFalling;

    TArray<TArray<bool>> Grid; // 2D array to represent the grid
    FTimerHandle BlockFallTimerHandle;
    float BlockFallDelay;

    void MoveTetromino(const FVector2D& Direction);
    void MoveTetrominoDown();
    void SetGrid(int32 x, int32 y, bool bIsOccupied);
    bool IsGridOccupied(int32 x, int32 y) const;
    FVector GridToWorld(int32 x, int32 y) const;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    FTimerHandle TetrominoFallTimerHandle;
    float DefaultFallInterval = 0.2f;
    float FastFallInterval = 0.05f;

    void InitializeTetrominoShapesAndBlueprints();
};