// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"
#include "Components/ActorComponent.h"
#include "SM_State.h"
#include "Updatable.h"
#include "AiyaQuest.generated.h"

class UQuestStatus;

UENUM()
enum class EQuestCompletion : uint8
{
	EQC_NotStarted,
	EQC_Started,
	EQC_Succeeded,
	EQC_Failed
};

USTRUCT()
struct FQuestDetails
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(BlueprintReadOnly)
		FName StateName;

	UPROPERTY(BlueprintReadOnly)
		FText StateMessage;
};

/**
 * 
 */
UCLASS(Blueprintable)
class AIYA2_API UAiyaQuest : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// The name of the quest.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText QuestName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FName QuestId;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText QuestDescription;

	UPROPERTY(BlueprintReadOnly)
	FQuestDetails Details;

	// If this machine accepts our QuestActivities log, the quest is successful.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USM_State* QuestStateMachine;

	// If true, the InputList is a blacklist. Otherwise, it's a whitelist.
	UPROPERTY(EditAnywhere)
	uint32 bInputBlackList : 1;

	// The blacklist/whitelist (depending on bBlackList) used to filter InputAtoms this Quest recognizes.
	UPROPERTY(EditAnywhere)
	TArray<USM_InputAtom*> InputList;

	virtual void OnSucceed(UQuestStatus* QuestStatus) const;
	virtual void OnFailed(UQuestStatus* QuestStatus) const;
	
};


USTRUCT(BlueprintType)
struct FQuestInProgress
{
	GENERATED_USTRUCT_BODY()

	// Quest data asset.
	UPROPERTY(EditAnywhere)
	UAiyaQuest* Quest;

	// Current progress in the quest.
	UPROPERTY(EditAnywhere)
	EQuestCompletion QuestProgress;

	

protected:
	// All input for this quest. Filtered by the quest's blacklist/whitelist.
	UPROPERTY(EditAnywhere)
	TArray<USM_InputAtom*> QuestActivities;

public:

	// Returns true if the quest was completed (success or failure) by the new activity.
	bool UpdateQuest(UObject* ObjectRef, USM_InputAtom* QuestActivity)
	{
		// Only log activity to valid, in-progress quests. Check the blacklist/whitelist before logging activity.
		if (Quest && (QuestProgress == EQuestCompletion::EQC_Started || QuestProgress == EQuestCompletion::EQC_NotStarted)
			&& (Quest->bInputBlackList != Quest->InputList.Contains(QuestActivity)))
		{
			FStateMachineResult QuestResult;
			QuestActivities.Add(QuestActivity);
			QuestResult = Quest->QuestStateMachine->RunState(ObjectRef, QuestActivities);
			if (QuestResult.FinalState)
			{
				FQuestDetails deets;
				deets.StateName = QuestResult.FinalState->StateName;
				deets.StateMessage = QuestResult.FinalState->StateMessage;
				Quest->Details = deets;
			}
			
			const IUpdatable* updatable = Cast<IUpdatable>(ObjectRef);
			if (updatable)
			{
				updatable->Execute_DoOnUpdate (ObjectRef, Quest);
			}
			switch (QuestResult.CompletionType)
			{
			case EStateMachineCompletionType::Accepted:
				UE_LOG(LogTemp, Warning, TEXT("Succeeded"));
				QuestProgress = EQuestCompletion::EQC_Succeeded;
				return true;
			case EStateMachineCompletionType::Rejected:
				UE_LOG(LogTemp, Warning, TEXT("Failed"));
				QuestProgress = EQuestCompletion::EQC_Failed;
				return true;
			case EStateMachineCompletionType::NotAccepted:
				QuestProgress = QuestProgress == EQuestCompletion::EQC_NotStarted ? EQuestCompletion::EQC_Started : QuestProgress;
				UE_LOG(LogTemp, Warning, TEXT("Still In Progress"));
				// case NotAccepted: // Still in progress. No update needed.
			}
		}
		return false;
	}

	// Returns a new FQuestInProgress with status of Started
	static FQuestInProgress NewQuestInProgress(UAiyaQuest* _Quest)
	{
		FQuestInProgress NewQIP;
		NewQIP.Quest = _Quest;
		NewQIP.QuestProgress = EQuestCompletion::EQC_Started;
		return NewQIP;
	}
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class AIYA2_API UQuestStatus : public UActorComponent, public IUpdatable
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UQuestStatus();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Add to our quest activity log! This also automatically checks to see if any
	unfinished quests are now complete. */
	UFUNCTION(BlueprintCallable, Category = "Quests")
	void UpdateQuests(USM_InputAtom* QuestActivity);

	// Player Character
	UPROPERTY(BlueprintReadWrite)
	ACharacter* Character;

	// Get All Quests with status of Started
	UFUNCTION(BlueprintCallable, Category = "Quests")
	TArray<UAiyaQuest*> getCurrentQuests();

	// Get All Quests with status of Completed
	UFUNCTION(BlueprintCallable, Category = "Quests")
	TArray<UAiyaQuest*> getCompletedQuests();

	/** Adds a new quest in progress entry or begin the quest provided if it's already on the list and hasnt' started yet.*/
	UFUNCTION(BlueprintCallable, Category = "Quests")
	bool BeginQuest(UAiyaQuest* Quest);

protected:
	// The master list of all quest-related things we've done.
	UPROPERTY(EditAnywhere)
	TArray<USM_InputAtom*> QuestActivities;

	// The list of quests in our current game or area.
	UPROPERTY(EditAnywhere)
	TArray<FQuestInProgress> QuestList;

private:
	UFUNCTION()
	TArray<UAiyaQuest*> getQuestsByStatus(EQuestCompletion Status);

};

UCLASS()
class AIYA2_API UQuestWithResult : public UAiyaQuest
{
	GENERATED_BODY()

public:
	virtual void OnSucceed(class UQuestStatus* QuestStatus) const override;
	virtual void OnFailed(class UQuestStatus* QuestStatus) const override;

protected:
	// The quest in this list will go from Notstarted to started if the current quest succeeds
	UPROPERTY(EditAnywhere)
	TArray<UAiyaQuest*> SuccessQuests;

	// Input atoms to add if the quest succeeds;
	UPROPERTY(EditAnywhere)
	TArray<USM_InputAtom*> SuccessInputs;

	// These quests will go from notstarted to started if the current quest fails
	UPROPERTY(EditAnywhere)
	TArray<UAiyaQuest*> FailureQuests;

	// Inputs atoms to add if the quest fails
	UPROPERTY(EditAnywhere)
	TArray<USM_InputAtom*> FailureInputs;
};