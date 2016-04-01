// Fill out your copyright notice in the Description page of Project Settings.

#include "nhGrow001.h"
#include "FindClosestSpider.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AIController.h"

#include <limits>

UFindClosestSpider::UFindClosestSpider(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    bNotifyBecomeRelevant = true; // ReceiveActivationImplementations != 0;
    //bNotifyCeaseRelevant = bNotifyBecomeRelevant;
    //bNotifyOnSearch = ReceiveSearchStartImplementations != 0;
    //bNotifyTick = ReceiveTickImplementations != 0;
    //bShowPropertyDetails = true;

    bCreateNodeInstance = true;

    ConstructorHelpers::FClassFinder<AActor> FoodClassFinder(TEXT("/Game/Spiders/Enemy_Creature_Spider/Spider"));
    ClassToFind = FoodClassFinder.Class;
}

void UFindClosestSpider::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::OnBecomeRelevant(OwnerComp, NodeMemory);

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, FoundActors);

    APawn* pawn = OwnerComp.GetAIOwner()->GetPawn();

    AActor* closerActor = NULL;
    float closerDistance = std::numeric_limits<float>::max();

    for (int x = 0; x < FoundActors.Num(); ++x) {
        AActor* current = FoundActors[x];
        if (current != pawn) {
            float currentDist = pawn->GetDistanceTo(current);
            if (currentDist < closerDistance) {
                closerDistance = currentDist;
                closerActor = current;
            }
        }
    }

    AAIController* Controller = OwnerComp.GetAIOwner();
    if (closerActor) {        
        if (Controller && Controller->BrainComponent)
        {
            FVector goToDestination = pawn->GetActorLocation() + (closerActor->GetActorLocation() - pawn->GetActorLocation()) * 0.1;
            Controller->BrainComponent->GetBlackboardComponent()->SetValueAsVector(FName("GoToLocation"), goToDestination);
            Controller->BrainComponent->GetBlackboardComponent()->SetValueAsObject(FName("Target"), closerActor);
        }
    }
    else {
        Controller->BrainComponent->GetBlackboardComponent()->ClearValueAsVector(FName("GoToLocation"));
        Controller->BrainComponent->GetBlackboardComponent()->ClearValue(FName("Target"));
    }
}