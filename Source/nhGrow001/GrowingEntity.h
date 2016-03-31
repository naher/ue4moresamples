// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GrowingEntity.generated.h"

UCLASS()
class NHGROW001_API AGrowingEntity : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrowingEntity();
    AGrowingEntity(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    class USphereComponent* BaseCollisionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class UStaticMeshComponent* EntityMesh;
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Growing)
    bool GrowingEnabled;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Growing)
    float GrowingTimeStep;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Growing)
    float GrowingProbabilities;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Growing)
    float GrowingMaxFactor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
    bool SpawnEnabled;
    UPROPERTY(EditAnywhere, Category = Spawning)
    TSubclassOf<AGrowingEntity> WhatToSpawn;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
    float SpawnProbabilities;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
    float SpawnTimeStep;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
    float SpawnMaxDistanceFactor;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
    uint8 SpawnMaxSpawnedCount;


private:
    void Spawn();
    FVector GetRandomPoint();

private:
    float GrowingElapsedTime;
    float SpawnElapsedTime;
};
