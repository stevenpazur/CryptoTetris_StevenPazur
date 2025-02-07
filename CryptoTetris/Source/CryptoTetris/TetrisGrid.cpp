#include "TetrisGrid.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
#include "Engine/Blueprint.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

ATetrisGrid::ATetrisGrid()
{
    try {
        PrimaryActorTick.bCanEverTick = true;
        GridWidth = 10;
        GridHeight = 20;
        BlockFallSpeed = 100.0f;
        bIsBlockFalling = false;
        BlockFallDelay = 0.1f;

        Grid.SetNum(GridWidth);
        for (int32 i = 0; i < GridWidth; ++i)
        {
            Grid[i].SetNum(GridHeight);
            for (int32 j = 0; j < GridHeight; ++j)
            {
                Grid[i][j] = false;
            }
        }

        SpawnLocation = FVector(0.0f, 0.0f, GridHeight * 100.0f);
    }
    catch (const std::exception& e) {
        UE_LOG(LogTemp, Error, TEXT("An exception was thrown in constructor.  e: %s"), *e.what());
    }
}

void ATetrisGrid::BeginPlay()
{
    try {
        Super::BeginPlay();

        FString Path = TEXT("/Game/Blueprints/BP_TetrisBlock.BP_TetrisBlock_C");
        TetrisBlockBP = StaticLoadClass(UObject::StaticClass(), nullptr, *Path);
        if (TetrisBlockBP)
        {
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to set TetrisBlockBP class in BeginPlay"));
        }

        FString ScoreWidgetPath = TEXT("/Game/Blueprints/ScoreWidget.ScoreWidget_C");
        ScoreWidgetClass = StaticLoadClass(UObject::StaticClass(), nullptr, *ScoreWidgetPath);
        if (!ScoreWidgetClass)
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to set ScoreWidgetBP class in BeginPlay"));
        }

        FString TitleScreenWidgetPath = TEXT("/Game/Blueprints/TitleScreenWidget.TitleScreenWidget_C");
        TitleScreenWidgetClass = StaticLoadClass(UObject::StaticClass(), nullptr, *TitleScreenWidgetPath);
        if (!TitleScreenWidgetClass)
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to set TitleScreenWidgetClass in BeginPlay"));
        }

        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            PlayerController->bShowMouseCursor = false;
            EnableInput(PlayerController);
        }

        if (ScoreWidgetClass)
        {
            ScoreWidget = CreateWidget<UUserWidget>(GetWorld(), ScoreWidgetClass);
            if (ScoreWidget)
            {
                ScoreWidget->AddToViewport();
            }
            else {
                UE_LOG(LogTemp, Warning, TEXT("Failed to create the score widget"));
            }
        }
        else {
            UE_LOG(LogTemp, Warning, TEXT("Failed to get ScoreWidgetClass"));
        }

        TetrominoShapes.Add({ { FVector2D(0, -1), FVector2D(0, 0), FVector2D(0, 1), FVector2D(0, 2) }, FColor::Cyan }); // I Shape
        TetrominoShapes.Add({ { FVector2D(0, 0), FVector2D(1, 0), FVector2D(0, 1), FVector2D(1, 1) }, FColor::Yellow }); // O Shape
        TetrominoShapes.Add({ { FVector2D(0, 0), FVector2D(-1, 0), FVector2D(1, 0), FVector2D(0, 1) }, FColor::Purple }); // T Shape
        TetrominoShapes.Add({ { FVector2D(0, 0), FVector2D(1, 0), FVector2D(0, 1), FVector2D(-1, 1) }, FColor::Green }); // S Shape
        TetrominoShapes.Add({ { FVector2D(0, 0), FVector2D(-1, 0), FVector2D(0, 1), FVector2D(1, 1) }, FColor::Red }); // Z Shape
        TetrominoShapes.Add({ { FVector2D(0, 0), FVector2D(-1, 0), FVector2D(-1, 1), FVector2D(1, 0) }, FColor::Blue }); // J Shape
        TetrominoShapes.Add({ { FVector2D(0, 0), FVector2D(1, 0), FVector2D(1, 1), FVector2D(-1, 0) }, FColor::Orange }); // L Shape

        FString bitcoinPath = TEXT("/Game/Blueprints/BP_bitcoin.BP_bitcoin_C");
        BitcoinBP = StaticLoadClass(UObject::StaticClass(), nullptr, *bitcoinPath);
        if (BitcoinBP)
        {
            TetrominoBlueprints.Add(BitcoinBP);
        }

        FString ethereumPath = TEXT("/Game/Blueprints/BP_ethereum.BP_ethereum_C");
        EthereumBP = StaticLoadClass(UObject::StaticClass(), nullptr, *ethereumPath);
        if (EthereumBP)
        {
            TetrominoBlueprints.Add(EthereumBP);
        }

        FString xrpPath = TEXT("/Game/Blueprints/BP_xrp.BP_xrp_C");
        XrpBP = StaticLoadClass(UObject::StaticClass(), nullptr, *xrpPath);
        if (XrpBP)
        {
            TetrominoBlueprints.Add(XrpBP);
        }

        FString polkadotPath = TEXT("/Game/Blueprints/BP_polkadot.BP_polkadot_C");
        PolkadotBP = StaticLoadClass(UObject::StaticClass(), nullptr, *polkadotPath);
        if (PolkadotBP)
        {
            TetrominoBlueprints.Add(PolkadotBP);
        }

        FString solanaPath = TEXT("/Game/Blueprints/BP_solana.BP_solana_C");
        SolanaBP = StaticLoadClass(UObject::StaticClass(), nullptr, *solanaPath);
        if (SolanaBP)
        {
            TetrominoBlueprints.Add(SolanaBP);
        }

        FString tetherPath = TEXT("/Game/Blueprints/BP_tether.BP_tether_C");
        TetherBP = StaticLoadClass(UObject::StaticClass(), nullptr, *tetherPath);
        if (TetherBP)
        {
            TetrominoBlueprints.Add(TetherBP);
        }

        FString usdcPath = TEXT("/Game/Blueprints/BP_usdc.BP_usdc_C");
        UsdcBP = StaticLoadClass(UObject::StaticClass(), nullptr, *usdcPath);
        if (UsdcBP)
        {
            TetrominoBlueprints.Add(UsdcBP);
        }

        GetWorld()->GetTimerManager().SetTimer(TetrominoFallTimerHandle, this, &ATetrisGrid::MoveTetrominoDown, DefaultFallInterval, true);

        SpawnTetromino();
    }
    catch (const std::exception& e) {
        UE_LOG(LogTemp, Error, TEXT("An exception was thrown in BeginPlay.  e: %s"), *e.what());
    }
}

void ATetrisGrid::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ATetrisGrid::SpawnTetromino()
{
    if (UWorld* World = GetWorld())
    {
        if (TetrominoShapes.Num() == 0 || TetrominoBlueprints.Num() == 0) {
            UE_LOG(LogTemp, Warning, TEXT("This should have been initialized in the constructor.. what the hell?"));
        }

        int32 ShapeIndex = FMath::RandRange(0, TetrominoShapes.Num() - 1);
        const FTetrominoShape& SelectedShape = TetrominoShapes[ShapeIndex];
        TSubclassOf<AActor> TetrominoBlueprint = TetrominoBlueprints[ShapeIndex];

        for (const FVector2D& Offset : SelectedShape.BlockOffsets)
        {
            FVector BlockLocation = SpawnLocation + FVector(Offset.X * 100.0f, 0.0f, Offset.Y * 100.0f);
            AActor* Block = World->SpawnActor<AActor>(TetrominoBlueprint, BlockLocation, FRotator::ZeroRotator);

            if (Block)
            {
                UStaticMeshComponent* Mesh = Block->FindComponentByClass<UStaticMeshComponent>();
                if (Mesh)
                {
                    Mesh->SetVectorParameterValueOnMaterials("BaseColor", FVector(SelectedShape.Color));
                }

                CurrentTetrominoBlocks.Add(Block);
            }
        }
    }
}

void ATetrisGrid::MoveTetromino(const FVector2D& Direction)
{
    try {
        bool bCanMove = true;

        for (AActor* Block : CurrentTetrominoBlocks)
        {
            FVector CurrentLocation = Block->GetActorLocation();
            FVector2D GridPosition = FVector2D(CurrentLocation.X / 100.0f, CurrentLocation.Z / 100.0f);
            FVector2D NewGridPosition = GridPosition + Direction;

            if (NewGridPosition.X < -5 || NewGridPosition.X >= GridWidth - 5 || NewGridPosition.Y < 0 || IsGridOccupied(NewGridPosition.X - 5, NewGridPosition.Y))
            {
                bCanMove = false;
                break;
            }
        }

        if (bCanMove)
        {
            for (AActor* Block : CurrentTetrominoBlocks)
            {
                FVector CurrentLocation = Block->GetActorLocation();
                FVector NewLocation = CurrentLocation + FVector(Direction.X * 100.0f, 0.0f, Direction.Y * 100.0f);
                Block->SetActorLocation(NewLocation);
            }
        }
    }
    catch (const std::exception& e) {
        UE_LOG(LogTemp, Error, TEXT("An exception was thrown.  e: %s"), *e.what());
    }
}

void ATetrisGrid::MoveTetrominoLeft()
{
    MoveTetromino(FVector2D(-1, 0));
}

void ATetrisGrid::MoveTetrominoRight()
{
    MoveTetromino(FVector2D(1, 0));
}

void ATetrisGrid::MoveTetrominoDown()
{
    bool bCanMove = true;

    // Check if movement is possible
    for (AActor* Block : CurrentTetrominoBlocks)
    {
        FVector CurrentLocation = Block->GetActorLocation();
        FVector2D GridPosition = FVector2D((CurrentLocation.X + 500.0f) / 100.0f, CurrentLocation.Z / 100.0f);
        FVector2D NewGridPosition = GridPosition + FVector2D(0, -1);

        if (NewGridPosition.Y < 0 || IsGridOccupied(NewGridPosition.X, NewGridPosition.Y))
        {
            bCanMove = false;
            break;
        }
    }

    // Move Tetromino if possible
    if (bCanMove)
    {
        for (AActor* Block : CurrentTetrominoBlocks)
        {
            FVector CurrentLocation = Block->GetActorLocation();
            FVector NewLocation = CurrentLocation + FVector(0.0f, 0.0f, -100.0f);
            Block->SetActorLocation(NewLocation);
        }
    }
    else
    {
        // Set the Tetromino blocks as occupied in the grid
        for (AActor* Block : CurrentTetrominoBlocks)
        {
            FVector CurrentLocation = Block->GetActorLocation();
            int32 GridX = FMath::RoundToInt((CurrentLocation.X + 500.0f) / 100.0f);
            int32 GridY = FMath::RoundToInt(CurrentLocation.Z / 100.0f);

            // Trigger game over if a block is placed at the top of the grid
            if (GridY >= GridHeight - 1)
            {
                GameOver();
                return;
            }

            SetGrid(GridX, GridY, true);
        }

        CurrentTetrominoBlocks.Empty();

        // Clear full rows and spawn a new Tetromino
        CheckAndClearFullRows();
        SpawnTetromino();
    }
}

void ATetrisGrid::SetGrid(int32 x, int32 y, bool bIsOccupied)
{
    if (x >= 0 && x < GridWidth && y >= 0 && y < GridHeight)
    {
        Grid[x][y] = bIsOccupied;
    }
}

bool ATetrisGrid::IsGridOccupied(int32 x, int32 y) const
{
    if (x >= 0 && x < GridWidth && y >= 0 && y < GridHeight)
    {
        return Grid[x][y];
    }
    return false;
}

FVector ATetrisGrid::GridToWorld(int32 x, int32 y) const
{
    return FVector(x * 100.0f, 0.0f, y * 100.0f);
}

void ATetrisGrid::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &ATetrisGrid::MoveTetrominoLeft);
    PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &ATetrisGrid::MoveTetrominoRight);
    PlayerInputComponent->BindAction("MoveUp", IE_Pressed, this, &ATetrisGrid::RotateTetromino);
    PlayerInputComponent->BindAction("MoveDown", IE_Pressed, this, &ATetrisGrid::StartFastDrop);
    PlayerInputComponent->BindAction("MoveDown", IE_Released, this, &ATetrisGrid::StopFastDrop);
}

void ATetrisGrid::CheckAndClearFullRows()
{
    for (int32 y = 0; y < GridHeight; ++y)
    {
        bool bIsRowFull = true;
        for (int32 x = 0; x < GridWidth; ++x)
        {
            if (!IsGridOccupied(x, y))
            {
                bIsRowFull = false;
                break;
            }
        }

        if (bIsRowFull)
        {
            ClearRow(y);
            MoveRowsDown(y);

            Score += 100;
        }
    }
}

void ATetrisGrid::ClearRow(int32 y)
{
    for (int32 x = 0; x < GridWidth; ++x)
    {
        FVector BlockLocation = GridToWorld(x - 5, y);
        FHitResult HitResult;
        FVector Start = BlockLocation;
        FVector End = BlockLocation + (FVector::UpVector * 5);

        if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility))
        {
            if (AActor* BlockActor = HitResult.GetActor())
            {
                BlockActor->Destroy();
            }
        }
        else {
            UE_LOG(LogTemp, Warning, TEXT("Line trace failed."));
        }

        SetGrid(x, y, false);
    }
}

void ATetrisGrid::MoveRowsDown(int32 ClearedRow)
{
    for (int32 y = ClearedRow; y < GridHeight - 1; ++y)
    {
        for (int32 x = 0; x < GridWidth; ++x)
        {
            UE_LOG(LogTemp, Warning, TEXT("Is Occupied check for clearing row.  x = %d, y = %d"), x, y + 1);
            bool bIsOccupied = IsGridOccupied(x, y + 1);

            if (bIsOccupied) {
                UE_LOG(LogTemp, Log, TEXT("An occupied piece!  Coords: %d, %d"), x, y);
            }

            SetGrid(x, y, bIsOccupied);

            if (bIsOccupied)
            {
                FVector NewLocation = GridToWorld(x - 5, y);
                FVector OldLocation = GridToWorld(x - 5, y + 1);
                UE_LOG(LogTemp, Log, TEXT("Another line trace oooh.  OldLocation = %s, NewLocation = %s"), *OldLocation.ToString(), *NewLocation.ToString());
                FHitResult HitResult;
                if (GetWorld()->LineTraceSingleByChannel(HitResult, OldLocation, OldLocation + (FVector::UpVector * 5), ECC_Visibility))
                {
                    if (AActor* BlockActor = HitResult.GetActor())
                    {
                        BlockActor->SetActorLocation(NewLocation);
                    }
                }
            }
            else
            {
                SetGrid(x, y, false);
            }
        }
    }

    // Clear the top row
    for (int32 x = 0; x < GridWidth; ++x)
    {
        SetGrid(x, GridHeight - 1, false);
    }
}

//void ATetrisGrid::RotateTetromino()
//{
//    if (CurrentTetrominoBlocks.Num() > 0)
//    {
//        // Calculate the grid-space center of the Tetromino
//        FVector2D CenterGrid(0.0f, 0.0f);
//        TArray<FVector2D> BlockGridPositions;
//
//        for (AActor* Block : CurrentTetrominoBlocks)
//        {
//            FVector WorldLocation = Block->GetActorLocation();
//
//            // Convert world coordinates to grid coordinates
//            int32 GridX = FMath::RoundToInt((WorldLocation.X + 500.0f) / 100.0f);
//            int32 GridY = FMath::RoundToInt(WorldLocation.Z / 100.0f);
//
//            CenterGrid.X += GridX;
//            CenterGrid.Y += GridY;
//
//            BlockGridPositions.Add(FVector2D(GridX, GridY));
//        }
//
//        CenterGrid /= CurrentTetrominoBlocks.Num();
//
//        bool bCanRotate = true;
//        TArray<FVector2D> NewGridPositions;
//
//        // Calculate new grid positions for the Tetromino blocks
//        for (const FVector2D& GridPos : BlockGridPositions)
//        {
//            // Calculate relative position to the center in grid space
//            FVector2D RelativePosition = GridPos - CenterGrid;
//
//            // Rotate 90 degrees clockwise in grid space
//            FVector2D RotatedPosition(-RelativePosition.Y, RelativePosition.X);
//
//            FVector2D NewGridPosition = CenterGrid + RotatedPosition;
//
//            // Check if the new grid position is valid
//            int32 NewGridX = FMath::RoundToInt(NewGridPosition.X);
//            int32 NewGridY = FMath::RoundToInt(NewGridPosition.Y);
//
//            if (NewGridX < 0 || NewGridX >= GridWidth || NewGridY < 0 || NewGridY >= GridHeight || IsGridOccupied(NewGridX, NewGridY))
//            {
//                bCanRotate = false;
//                break;
//            }
//
//            NewGridPositions.Add(NewGridPosition);
//        }
//
//        // Apply the new positions if the rotation is valid
//        if (bCanRotate)
//        {
//            for (int32 i = 0; i < CurrentTetrominoBlocks.Num(); ++i)
//            {
//                FVector2D GridPos = NewGridPositions[i];
//
//                // Corrected: Convert grid coordinates back to world coordinates
//                FVector NewWorldLocation((GridPos.X * 100.0f) - 500.0f, 0.0f, GridPos.Y * 100.0f);
//                CurrentTetrominoBlocks[i]->SetActorLocation(NewWorldLocation);
//            }
//        }
//    }
//}

void ATetrisGrid::RotateTetromino()
{
    if (CurrentTetrominoBlocks.Num() > 0)
    {
        // Use the first block as the pivot for rotation
        AActor* PivotBlock = CurrentTetrominoBlocks[0];
        FVector PivotWorldLocation = PivotBlock->GetActorLocation();

        // Convert pivot world coordinates to grid coordinates
        int32 PivotGridX = FMath::RoundToInt((PivotWorldLocation.X + 500.0f) / 100.0f);
        int32 PivotGridY = FMath::RoundToInt(PivotWorldLocation.Z / 100.0f);

        bool bCanRotate = true;
        TArray<FVector2D> NewGridPositions;

        for (AActor* Block : CurrentTetrominoBlocks)
        {
            FVector WorldLocation = Block->GetActorLocation();

            // Convert world coordinates to grid coordinates
            int32 GridX = FMath::RoundToInt((WorldLocation.X + 500.0f) / 100.0f);
            int32 GridY = FMath::RoundToInt(WorldLocation.Z / 100.0f);

            // Calculate relative position to the pivot in grid space
            FVector2D RelativePosition = FVector2D(GridX - PivotGridX, GridY - PivotGridY);

            // Rotate 90 degrees clockwise in grid space
            FVector2D RotatedPosition(-RelativePosition.Y, RelativePosition.X);

            FVector2D NewGridPosition = FVector2D(PivotGridX, PivotGridY) + RotatedPosition;

            // Check if the new grid position is valid
            int32 NewGridX = FMath::RoundToInt(NewGridPosition.X);
            int32 NewGridY = FMath::RoundToInt(NewGridPosition.Y);

            if (NewGridX < 0 || NewGridX >= GridWidth || NewGridY < 0 || NewGridY >= GridHeight || IsGridOccupied(NewGridX, NewGridY))
            {
                bCanRotate = false;
                break;
            }

            NewGridPositions.Add(NewGridPosition);
        }

        // Apply the new positions if the rotation is valid
        if (bCanRotate)
        {
            for (int32 i = 0; i < CurrentTetrominoBlocks.Num(); ++i)
            {
                FVector2D GridPos = NewGridPositions[i];

                // Convert grid coordinates back to world coordinates
                FVector NewWorldLocation((GridPos.X * 100.0f) - 500.0f, 0.0f, GridPos.Y * 100.0f);
                CurrentTetrominoBlocks[i]->SetActorLocation(NewWorldLocation);
            }
        }
    }
}

void ATetrisGrid::StartFastDrop()
{
    GetWorld()->GetTimerManager().SetTimer(TetrominoFallTimerHandle, this, &ATetrisGrid::MoveTetrominoDown, FastFallInterval, true);
}

void ATetrisGrid::StopFastDrop()
{
    GetWorld()->GetTimerManager().SetTimer(TetrominoFallTimerHandle, this, &ATetrisGrid::MoveTetrominoDown, DefaultFallInterval, true);
}

void ATetrisGrid::GameOver()
{
    // Clear the Tetromino fall timer
    GetWorld()->GetTimerManager().ClearTimer(TetrominoFallTimerHandle);

    // Ensure the player controller is valid before attempting to load the level
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        // Set the input mode to UI only and show the mouse cursor
        PlayerController->bShowMouseCursor = true;
        FInputModeUIOnly InputMode;
        PlayerController->SetInputMode(InputMode);

        // Load the main menu level
        FString LevelName = TEXT("/Game/Maps/MainMenu");
        UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName));
    }
}
