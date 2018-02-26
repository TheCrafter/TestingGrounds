// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

#include "PatrollingGuard.h" // TODO: Remove coupling

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (ensure(AIController->GetPawn()))
    {
        // Get patrol points
        APatrollingGuard* ControlledPawn = Cast<APatrollingGuard>(AIController->GetPawn());
        TArray<AActor*> PatrolPoints = ControlledPawn->PatrolPoints;

        // Set next waypoint
        UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
        int32 Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
        BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);

        // Cycle index
        int32 NewIndex = (Index + 1) % PatrolPoints.Num();
        BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, NewIndex);
    
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}