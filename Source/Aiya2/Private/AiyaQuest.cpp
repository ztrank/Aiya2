// Fill out your copyright notice in the Description page of Project Settings.

#include "Aiya2.h"
#include "AiyaQuest.h"

// Sets default values for this component's properties
UQuestStatus::UQuestStatus()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UQuestStatus::BeginPlay()
{
	Super::BeginPlay();
	// ...

}


// Called every frame
void UQuestStatus::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UQuestStatus::UpdateQuests(USM_InputAtom* QuestActivity)
{
	TArray<int32> RecentlyCompletedQuests;

	//UE_LOG(LogTemp, Warning, TEXT("Update Quests"));
	// Update the master list of everything we've ever done.
	QuestActivities.Add(QuestActivity);

	// Update individual quests (if they care about this activity) and see if they are complete.
	for (int32 i = QuestList.Num() - 1; i >= 0; --i)
	{
		if (QuestList[i].UpdateQuest(this, QuestActivity))
		{
			RecentlyCompletedQuests.Add(i);

		}
	}

	for (int32 i = RecentlyCompletedQuests.Num() - 1; i >= 0; i++)
	{
		FQuestInProgress& QIP = QuestList[RecentlyCompletedQuests[i]];
		if (QIP.QuestProgress == EQuestCompletion::EQC_Succeeded)
		{
			QIP.Quest->OnSucceed(this);
		}
		else
		{
			QIP.Quest->OnFailed(this);
		}
		RecentlyCompletedQuests.RemoveAtSwap(i);
	}
}

TArray<UAiyaQuest*> UQuestStatus::getCurrentQuests()
{
	return getQuestsByStatus(EQuestCompletion::EQC_Started);
}

TArray<UAiyaQuest*> UQuestStatus::getCompletedQuests()
{
	return getQuestsByStatus(EQuestCompletion::EQC_Succeeded);
}

TArray<UAiyaQuest*> UQuestStatus::getQuestsByStatus(EQuestCompletion Status)
{
	TArray<UAiyaQuest*> compQuests;
	for (int32 i = 0; i < QuestList.Num(); ++i)
	{
		if (QuestList[i].QuestProgress == Status)
			compQuests.Add(QuestList[i].Quest);
	}

	return compQuests;
}

bool UQuestStatus::BeginQuest(UAiyaQuest* Quest)
{
	for (FQuestInProgress& QIP : QuestList)
	{
		if (QIP.Quest == Quest)
		{
			if (QIP.QuestProgress == EQuestCompletion::EQC_NotStarted)
			{
				QIP.QuestProgress = EQuestCompletion::EQC_Started;
				return true;
			}
			return false;
		}
		
	}

	QuestList.Add(FQuestInProgress::NewQuestInProgress(Quest));
	return true;
}

void UAiyaQuest::OnSucceed(UQuestStatus* QuestStatus) const
{

}

void UAiyaQuest::OnFailed(UQuestStatus* QuestStatus) const
{

}

void UQuestWithResult::OnSucceed(UQuestStatus* QuestStatus) const
{
	Super::OnSucceed(QuestStatus);
	for (UAiyaQuest* SuccessQuest : SuccessQuests)
	{
		QuestStatus->BeginQuest(SuccessQuest);
	}

	for (int32 i = 0; i < SuccessInputs.Num(); ++i)
	{
		QuestStatus->UpdateQuests(SuccessInputs[i]);
	}
}

void UQuestWithResult::OnFailed(UQuestStatus* QuestStatus) const
{
	Super::OnFailed(QuestStatus);
	for (UAiyaQuest* FailureQuest : FailureQuests)
	{
		QuestStatus->BeginQuest(FailureQuest);
	}

	for (int32 i = 0; i < FailureInputs.Num(); ++i)
	{
		QuestStatus->UpdateQuests(FailureInputs[i]);
	}
}



