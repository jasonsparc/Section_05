// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "AIController.h"
#include "PatrollingGuard.h" // TODO remove coupling
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto BlackboardComp = OwnerComp.GetBlackboardComponent();

	/// Get the patrol points

	auto PatrollingGuard = Cast<APatrollingGuard>(OwnerComp.GetAIOwner()->GetPawn());
	if (!PatrollingGuard) { return EBTNodeResult::Failed; }

	auto PatrolPoints = PatrollingGuard->PatrolPointsCPP;

	/// Set next waypoint

	auto Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);

	auto NextWaypoint = PatrolPoints.IsValidIndex(Index) ? PatrolPoints[Index] : nullptr;
	BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, NextWaypoint);

	/// Cycle the index

	auto NextIndex = PatrolPoints.Num() ? (Index + 1) % PatrolPoints.Num() : 0;
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);

	return EBTNodeResult::Succeeded;
}
