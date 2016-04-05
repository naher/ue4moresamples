// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "Reproduce.generated.h"

/**
 * 
 */
UCLASS()
class NHGROW001_API UReproduce : public UBTTaskNode
{
    GENERATED_BODY()

    UReproduce();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    //virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    //virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
    TSubclassOf<AActor> ClassSpider;

    void Spawn(APawn* parent);
    FVector GetRandomPoint(APawn* parent);
    FVector GetFarRandomPoint(APawn* parent);
	
};
