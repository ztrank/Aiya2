// Fill out your copyright notice in the Description page of Project Settings.

#include "Aiya2.h"
#include "AObjective.h"

// Sets Default Values
AObjective::AObjective( ) :
	Description( ),
	ObjectiveName( NAME_None ),
	TotalProgressNeeded( 1 ),
	CurrentProgress( 0 ),
	MustBeCompletedToAdvance( true )
{
	// Set this acto to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AObjective::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AObjective::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void AObjective::Update( int32 Progress )
{
	CurrentProgress += Progress;
}

bool AObjective::IsComplete( ) const
{
	return CurrentProgress >= TotalProgressNeeded;
}

float AObjective::GetProgress( ) const
{
	check( TotalProgressNeeded != 0 )
	return (float)CurrentProgress / (float)TotalProgressNeeded;
}


