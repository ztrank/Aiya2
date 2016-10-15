// Fill out your copyright notice in the Description page of Project Settings.

#include "Aiya2.h"
#include "AObjective.h"
#include "AQuest.h"


AQuest::AQuest() :
	QuestName( ),
	CurrentObjectives( ),
	QuestStartDescription( ),
	QuestEndDescription( ),
	QuestOfferDialog( ),
	QuestStartDialog( ),
	QuestAcceptedDialog( ),
	QuestCompletedDialog( ),
	MustBeCompletedToAdvance( )
{

}

void AQuest::BeginPlay()
{
	Super::BeginPlay();
	UWorld* World = GetWorld();
	if( World )
	{
		for ( auto Objective : Objectives )
		{
			CurrentObjectives.Add((AObjective*)World->SpawnActor(Objective));
		}
	}
}

void AQuest::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

bool AQuest::IsQuestComplete() const
{
	bool result = true;
	for( auto Objective : CurrentObjectives )
	{
		result &= Objective->IsComplete();
	}
	return result;
}

bool AQuest::CanUpdate( FName Objective )
{
	bool PreviousIsComplete = true;
	for( auto Obj : CurrentObjectives )
	{
		if ( PreviousIsComplete )
		{
			if ( Objective == Obj->ObjectiveName )
				return true;
			else
				PreviousIsComplete = Obj->IsComplete() |
				!Obj->MustBeCompletedToAdvance;
		}
		else
		{
			return false;
		}
	}
	return true;
}

void AQuest::Update( FName Objective, int32 Progress )
{
	for ( auto Obj : CurrentObjectives )
	{
		if ( Obj->ObjectiveName == Objective )
		{
			Obj->Update( Progress );
			return;
		}
	}
}

bool AQuest::TryUpdate(FName Objective, int32 Progress )
{
	bool result = CanUpdate( Objective );
	if ( result )
	{
		Update( Objective, Progress );
	}
	return result;
}

float AQuest::QuestCompletion( ) const
{
	int32 NumObjectives = CurrentObjectives.Num( );
	if( NumObjectives == 0 ) return 1.0f;

	float AggregateComplete = 0.0f;
	for( auto Objective : CurrentObjectives )
	{
		AggregateComplete += Objective->GetProgress( );
	}
	return AggregateComplete / (float)NumObjectives;
}



