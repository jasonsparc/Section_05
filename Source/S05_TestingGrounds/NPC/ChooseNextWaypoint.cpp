// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "AIController.h"
#include "NPC/PatrolRoute.h"
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto BlackboardComp = OwnerComp.GetBlackboardComponent();

	/// Get the patrol route

	auto ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	auto PatrolRoute = ControlledPawn->FindComponentByClass<UPatrolRoute>();
	if (!ensure(PatrolRoute)) { return EBTNodeResult::Failed; }

	auto PatrolPoints = PatrolRoute->GetPatrolPoints();

	// Warn about empty patrol routes
	if (PatrolPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Guard [%s] is missing patrol points"), *ControlledPawn->GetName());
		return EBTNodeResult::Failed;
	}

	/// Set next waypoint

	auto Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);

	auto NextWaypoint = PatrolPoints.IsValidIndex(Index) ? PatrolPoints[Index] : nullptr;
	BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, NextWaypoint);

	/// Cycle the index

	auto NextIndex = PatrolPoints.Num() ? (Index + 1) % PatrolPoints.Num() : 0;
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);

	return EBTNodeResult::Succeeded;
}
