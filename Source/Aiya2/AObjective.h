// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Info.h"
#include "AObjective.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class AIYA2_API AObjective : public AInfo
{
	GENERATED_BODY()
	
	public:
		// Sets default values for this actor's properties
		AObjective();

		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

		// called every frame
		virtual void Tick( float DeltaSeconds ) override;

		UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Quest Objectives" )
		FText Description;

		UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Quest Objectives" )
		FName ObjectiveName;

		UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Quest Objectives" )
		bool MustBeCompletedToAdvance;

		UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Quest Objectives" )
		int32 TotalProgressNeeded;

		UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Quest Objectives" )
		int32 CurrentProgress;

		UFUNCTION( BlueprintCallable, Category = "Quest Objectives")
		void Update( int32 Progress );

		UFUNCTION( BlueprintCallable, Category = "Quest Objectives")
		virtual bool IsComplete( ) const;

		UFUNCTION( BlueprintCallable, Category = "Quest Objectives")
		virtual float GetProgress( ) const;
	
	
};
