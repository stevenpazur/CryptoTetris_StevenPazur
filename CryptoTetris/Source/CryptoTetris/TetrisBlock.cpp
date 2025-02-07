// Fill out your copyright notice in the Description page of Project Settings.


#include "TetrisBlock.h"

// Sets default values
ATetrisBlock::ATetrisBlock()
{
	PrimaryActorTick.bCanEverTick = true;

	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh"));
	RootComponent = BlockMesh;
}

// Called when the game starts or when spawned
void ATetrisBlock::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATetrisBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

