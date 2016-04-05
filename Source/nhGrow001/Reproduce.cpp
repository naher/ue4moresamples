// Fill out your copyright notice in the Description page of Project Settings.

#include "nhGrow001.h"
#include "Reproduce.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "UnrealType.h"
#include <iostream>

UReproduce::UReproduce() {
    ConstructorHelpers::FClassFinder<AActor> ClassFinder(TEXT("/Game/Spiders/Enemy_Creature_Spider/Spider"));
    ClassSpider = ClassFinder.Class;
}

EBTNodeResult::Type UReproduce::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    //EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;

    //NodeResult = EBTNodeResult::Succeeded;

    // current pawn (spider)
    APawn* pawn = OwnerComp.GetAIOwner()->GetPawn();

    // check gender
    bool male = false;
    
    FName PropertyName("Gender");
    UProperty* UProp = FindField<UProperty>(ClassSpider, PropertyName);
    FString* ValuePtr = UProp->ContainerPtrToValuePtr<FString>(pawn);
    if (ValuePtr)
    {
        //UE_LOG(LogTemp, Warning, TEXT("Gender %s vs %s \n"), *FString("Male"), **ValuePtr);
        if (!ValuePtr->Compare(FString("Male"))) {
            male = true;
        }
    }
    else 
    {
        // should not happen
        return EBTNodeResult::Failed;
    }    

    // assuring partner is close enough
    AActor *parent;
    AAIController* Controller = OwnerComp.GetAIOwner();
    if (Controller && Controller->BrainComponent)
    {
        parent = (AActor*)Controller->BrainComponent->GetBlackboardComponent()->GetValueAsObject(FName("Target"));
        float currentDist = pawn->GetDistanceTo(parent);

        Controller->BrainComponent->GetBlackboardComponent()->ClearValueAsVector(FName("GoToLocation"));
        Controller->BrainComponent->GetBlackboardComponent()->ClearValue(FName("Target"));

        if (currentDist > 400) {
            return EBTNodeResult::Failed;
        }        
    }
    else
    {
        // should not happen
        return EBTNodeResult::Failed;
    }

    // rate limit for reproduction 
    FName PregnancyTimestampPropertyName("PregnancyTimestamp");
    UProperty* UPropPTS = FindField<UProperty>(ClassSpider, PregnancyTimestampPropertyName);
    FDateTime* UPropPTSValuePtr = UPropPTS->ContainerPtrToValuePtr<FDateTime>(pawn);
    if (UPropPTSValuePtr) {
        FDateTime now = FDateTime::Now();
        FTimespan timespan = now - *UPropPTSValuePtr;

        if (timespan.GetTotalSeconds() < 60) { // un hijo por minuto at most
            Controller->BrainComponent->GetBlackboardComponent()->SetValueAsVector(FName("GoToLocation"), GetFarRandomPoint(pawn));
            return EBTNodeResult::Succeeded;
        }
        else {
            *UPropPTSValuePtr = now;
        }
    }

    // spawning new spider    
    if (!male)
    {
        UE_LOG(LogTemp, Warning, TEXT("Reproducing spider %s and %s"), *pawn->GetName(), *parent->GetName());
        Spawn(pawn);
    }

    Controller->BrainComponent->GetBlackboardComponent()->SetValueAsVector(FName("GoToLocation"), GetFarRandomPoint(pawn));

    return EBTNodeResult::Succeeded;
}

void UReproduce::Spawn(APawn* parent)
{
    UWorld *const World = GetWorld();
    if (World)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = parent; 
        SpawnParams.Instigator = parent; 

        FVector SpawnLocation = GetRandomPoint(parent);

        FRotator SpawnRotation;
        SpawnRotation.Yaw = FMath::FRand() * 360.f;
        SpawnRotation.Pitch = 0; // FMath::FRand() * 360.f;
        SpawnRotation.Roll = 0; // FMath::FRand() * 360.f;

        APawn* spawned = (APawn*) World->SpawnActor<AActor>(ClassSpider, SpawnLocation, SpawnRotation, SpawnParams);
        spawned->SpawnDefaultController();

        // random gender
        FName PropertyName("Gender");
        UProperty* UProp = FindField<UProperty>(ClassSpider, PropertyName);
        FString* ValuePtr = UProp->ContainerPtrToValuePtr<FString>(spawned);
        *ValuePtr = (FMath::FRand() > 0.5) ? FString("Male") : FString("Female");

        // workaroung avoiding instantaneus reproduction, TODO: add age property
        FName PregnancyTimestampPropertyName("PregnancyTimestamp");
        UProperty* UPropPTS = FindField<UProperty>(ClassSpider, PregnancyTimestampPropertyName);
        FDateTime* UPropPTSValuePtr = UPropPTS->ContainerPtrToValuePtr<FDateTime>(spawned);
        *UPropPTSValuePtr = FDateTime::Now();

        UE_LOG(LogTemp, Warning, TEXT("New spider born %s"), *spawned->GetName());
    }
}

FVector UReproduce::GetRandomPoint(APawn* parent)
{
    FVector RandomLocation;
    FVector Origin = parent->GetActorLocation();

    float distance = 300.0 + FMath::FRand() * 200.0;

    int xside = (-0.5f + FMath::FRand()) < 0 ? -1 : 1;
    int yside = (-0.5f + FMath::FRand()) < 0 ? -1 : 1;
    float p = FMath::FRand();

    RandomLocation.X = Origin.X + (p * distance * xside);
    RandomLocation.Y = Origin.Y + ((1 - p) * distance * yside);
    RandomLocation.Z = Origin.Z;

    return RandomLocation;
}


FVector UReproduce::GetFarRandomPoint(APawn* parent)
{
    FVector RandomLocation;
    FVector Origin = parent->GetActorLocation();

    float distance = 3000.0 + FMath::FRand() * 500.0;

    int xside = (-0.5f + FMath::FRand()) < 0 ? -1 : 1;
    int yside = (-0.5f + FMath::FRand()) < 0 ? -1 : 1;
    float p = FMath::FRand();

    RandomLocation.X = Origin.X + (p * distance * xside);
    RandomLocation.Y = Origin.Y + ((1 - p) * distance * yside);
    RandomLocation.Z = Origin.Z;

    return RandomLocation;
}

