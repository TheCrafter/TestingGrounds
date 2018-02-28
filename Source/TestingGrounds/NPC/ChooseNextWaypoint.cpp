// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

#include "PatrolRoute.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (ensure(ControlledPawn))
    {
        UPatrolRoute* PatrolRoute = ControlledPawn->FindComponentByClass<UPatrolRoute>();
        if (PatrolRoute)
        {
            TArray<AActor*> PatrolPoints = PatrolRoute->GetPatrolPoints();
            if (PatrolPoints.Num() == 0)
            {
               UE_LOG(LogTemp, Warning, TEXT("PatrolRoute Component is missing Patrol Points. Pawn: %s"), *ControlledPawn->GetName())
            }
            else
            {
                // Set next waypoint
                UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
                int32 Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
                BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);

                // Cycle index
                int32 NewIndex = (Index + 1) % PatrolPoints.Num();
                BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, NewIndex);

                return EBTNodeResult::Succeeded;
            }
        }
    }

    return EBTNodeResult::Failed;
}