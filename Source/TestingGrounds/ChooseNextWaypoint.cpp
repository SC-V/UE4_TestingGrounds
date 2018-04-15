// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "AIController.h"
#include "PatrollingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get necessary data: Blackboard components, possessed pawn (as APatrollingGuard class instance), patrol points array and its length
	auto AIControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	auto PatrolRoute = AIControlledPawn->FindComponentByClass<UPatrollingComponent>();
	if (!ensure(PatrolRoute)) { return EBTNodeResult::Failed; }

	auto PatrolPoints = PatrolRoute->GetPatrolPoints();
	if (PatrolPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No waypoints are set on %s"), *(AIControlledPawn->GetName()));
		return EBTNodeResult::Failed;
	}

	auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	auto Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
	
	// Set waypoint
	UE_LOG(LogTemp, Warning, TEXT("Waypoint index: %i"), Index);
	BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);

	// Cycle the index
	auto NextIndex = (Index + 1) % PatrolPoints.Num();
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);

	return EBTNodeResult::Succeeded;
}




