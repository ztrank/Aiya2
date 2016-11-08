

#pragma once

#include "GameFramework/Info.h"
#include "AObjective.h"
#include "AQuest.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class AIYA2_API AQuest : public AInfo
{
	GENERATED_BODY()
	
	public:
		// sets default values for this actors properties
		AQuest();

		// called when the game starts or when spawned
		virtual void BeginPlay() override;

		// called every frame
		virtual void Tick( float DeltaSeconds ) override;

	public:
		UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
		TArray<class AObjective*> CurrentObjectives;

		UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
		TArray<TSubclassOf<AObjective>> Objectives;

		UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
		USoundCue* QuestStartSoundCue;

		UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
		FName QuestName;

		UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
		FText QuestStartDescription;

		UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
		FText QuestEndDescription;

		UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
		FText QuestOfferDialog;

		UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
		FText QuestStartDialog;

		UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
		FText QuestAcceptedDialog;

		UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
		FText QuestCompletedDialog;

		UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Quests" )
		bool MustBeCompletedToAdvance;

		UFUNCTION( BlueprintCallable, Category = "Quests" )
		bool IsQuestComplete( ) const;

		UFUNCTION( BlueprintCallable, Category = "Quests" )
		bool CanUpdate( FName Objective );

		UFUNCTION( BlueprintCallable, Category = "Quests" )
		void Update( FName Objective, int32 Progress );

		UFUNCTION( BlueprintCallable, Category = "Quests" )
		bool TryUpdate( FName Objective, int32 Progress );

		UFUNCTION( BlueprintCallable, Category = "Quests" )
		float QuestCompletion( ) const;

		UFUNCTION(BlueprintImplementableEvent, Category = "Quests")
			bool DoOnUpdate(FName Objective);
	
};
