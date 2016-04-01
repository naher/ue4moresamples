// Fill out your copyright notice in the Description page of Project Settings.

#include "nhGrow001.h"
#include "Reproduce.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UReproduce::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;

    NodeResult = EBTNodeResult::Succeeded;

    AAIController* Controller = OwnerComp.GetAIOwner();
    Controller->BrainComponent->GetBlackboardComponent()->ClearValueAsVector(FName("GoToLocation"));
    Controller->BrainComponent->GetBlackboardComponent()->ClearValue(FName("Target"));

    return NodeResult;
}

