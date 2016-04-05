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
    ClassSpider = FoodClassFinder.Class;
}

void UFindClosestSpider::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::OnBecomeRelevant(OwnerComp, NodeMemory);

    // getting current actor (a spider)
    APawn* pawn = OwnerComp.GetAIOwner()->GetPawn();

    AActor* closerActor = NULL;
    float closerDistance = std::numeric_limits<float>::max();

    // getting gender property
    FName GenderPropertyName("Gender");
    UProperty* UProp = FindField<UProperty>(ClassSpider, GenderPropertyName);
    FString* GenderValuePtr = UProp->ContainerPtrToValuePtr<FString>(pawn);

    // getting all spiders // TODO: possible improve here, getting just closer actors and filtering by gender
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassSpider, FoundActors);

    for (int x = 0; x < FoundActors.Num(); ++x) {
        AActor* current = FoundActors[x];
        if (current != pawn) {
            // skipping current actor

            FString* ValuePtr = UProp->ContainerPtrToValuePtr<FString>(current);
            if (ValuePtr && GenderValuePtr)
            {
                // only considering different gender spiders
                if (!ValuePtr->Compare(*GenderValuePtr)) {                    
                    continue;
                }
            }

            float currentDist = pawn->GetDistanceTo(current);
            if (currentDist < closerDistance) {
                // obtaining closer spider
                closerDistance = currentDist;
                closerActor = current;
            }
        }
    }
    
    AAIController* Controller = OwnerComp.GetAIOwner();
    if (closerActor) 
    {        
        if (Controller && Controller->BrainComponent)
        {
            // updating blackboard
            FVector goToDestination = pawn->GetActorLocation() + (closerActor->GetActorLocation() - pawn->GetActorLocation()) * 0.1;
            Controller->BrainComponent->GetBlackboardComponent()->SetValueAsVector(FName("GoToLocation"), goToDestination);
            Controller->BrainComponent->GetBlackboardComponent()->SetValueAsObject(FName("Target"), closerActor);
            // UE_LOG(LogTemp, Warning, TEXT("Closer spider: %s found for spider: %s"), *closerActor->GetName(), *pawn->GetName());
        }
        else 
        {
            // blackboard cleanup in case of not spider found
            UE_LOG(LogTemp, Warning, TEXT("No controller found for %s when searching closer spiders"), *pawn->GetName());
            Controller->BrainComponent->GetBlackboardComponent()->ClearValueAsVector(FName("GoToLocation"));
            Controller->BrainComponent->GetBlackboardComponent()->ClearValue(FName("Target"));
        }
    }
    else 
    {
        // blackboard cleanup in case of not spider found
        UE_LOG(LogTemp, Warning, TEXT("No closer spider found for %s"), *pawn->GetName());
        Controller->BrainComponent->GetBlackboardComponent()->ClearValueAsVector(FName("GoToLocation"));
        Controller->BrainComponent->GetBlackboardComponent()->ClearValue(FName("Target"));
    }
}