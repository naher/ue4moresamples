// Fill out your copyright notice in the Description page of Project Settings.

#include "nhGrow001.h"
#include "GrowingEntity.h"


// Sets default values
AGrowingEntity::AGrowingEntity()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

AGrowingEntity::AGrowingEntity(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    BaseCollisionComponent = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("BaseSphereComponent"));
    RootComponent = BaseCollisionComponent;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("StaticMesh'/Game/Geometry/Meshes/GrowEntityMesh'"));
    EntityMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("EntityMesh"));    
    EntityMesh->SetSimulatePhysics(true);
    EntityMesh->SetStaticMesh(StaticMesh.Object);
    EntityMesh->AttachTo(RootComponent);  

    GrowingEnabled = true;
    GrowingTimeStep = 1.0f;
    GrowingProbabilities = 0.8f;
    GrowingElapsedTime = .0f;
    GrowingMaxFactor = .5f;

    SpawnEnabled = true;
    SpawnTimeStep = 3.0f;
    SpawnProbabilities = .1f;
    SpawnElapsedTime = .0f;

    SpawnMaxDistanceFactor = 3.0f;
    SpawnMaxSpawnedCount = 3;
        
}

// Called when the game starts or when spawned
void AGrowingEntity::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrowingEntity::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (GrowingEnabled)
    {
        GrowingElapsedTime += DeltaSeconds;
        bool shouldGrow = (GrowingElapsedTime > GrowingTimeStep);

        if (shouldGrow) {
            float die = FMath::FRand();
            if (die < GrowingProbabilities) {
                GrowingElapsedTime -= GrowingTimeStep;
                float growing = FMath::FRand();
                RootComponent->SetWorldScale3D(RootComponent->RelativeScale3D + growing * FVector(GrowingMaxFactor, GrowingMaxFactor, 0.0f));
            }
        }
    }

    if (SpawnEnabled)
    {
        SpawnElapsedTime += DeltaSeconds;
        bool shouldSpawn = (SpawnElapsedTime > SpawnTimeStep);

        if (shouldSpawn) {
            float die = FMath::FRand();
            if (die < SpawnProbabilities) {
                SpawnElapsedTime -= SpawnTimeStep;
                SpawnProbabilities /= 2.0f;
                if (SpawnProbabilities < 0.00001)
                    SpawnEnabled = false;
                Spawn();
            }
        }
    }
}

void AGrowingEntity::Spawn()
{    
    int SpawnCount = FMath::FRand() * SpawnMaxSpawnedCount;
    while (SpawnCount-- > 0)
    {
        UWorld *const World = GetWorld();
        if (World)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = Instigator;

            FVector SpawnLocation = GetRandomPoint();

            FRotator SpawnRotation;
            SpawnRotation.Yaw = FMath::FRand() * 360.f;
            SpawnRotation.Pitch = FMath::FRand() * 360.f;
            SpawnRotation.Roll = FMath::FRand() * 360.f;

            AGrowingEntity* const Spawned = World->SpawnActor<AGrowingEntity>(WhatToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
            Spawned->SpawnEnabled = false;
        }
    }
}

FVector AGrowingEntity::GetRandomPoint()
{
    FVector RandomLocation;
    FVector Origin;
    FVector BoundsExtent;
    GetActorBounds(false, Origin, BoundsExtent);

    float distance = FMath::FRand() * SpawnMaxDistanceFactor * BoundsExtent.Size();

    int xside = (-0.5f + FMath::FRand()) < 0 ? -1 : 1;
    int yside = (-0.5f + FMath::FRand()) < 0 ? -1 : 1;
    float p = FMath::FRand();

    RandomLocation.X = Origin.X + (p * distance * xside);
    RandomLocation.Y = Origin.Y + ((1 - p) * distance * yside);
    RandomLocation.Z = Origin.Z + 0.8*BoundsExtent.Z ;

    return RandomLocation;
}
