// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTService.h"
#include "FindClosestSpider.generated.h"

/**
 * 
 */
UCLASS()
class NHGROW001_API UFindClosestSpider : public UBTService
{
    //GENERATED_BODY()
    GENERATED_UCLASS_BODY()

//    UFindClosestSpider(const FObjectInitializer& ObjectInitializer);
	
protected:
    virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    
    TSubclassOf<AActor> ClassToFind;
};
