// Fill out your copyright notice in the Description page of Project Settings.


#include "EvidencePickup.h"

// Sets default values
AEvidencePickup::AEvidencePickup()
{
	EvidenceInterval = 0.0f;
	TotalNumofTicks = 0.0f;



}

// Called when the game starts or when spawned
void AEvidencePickup::BeginPlay()
{
	Super::BeginPlay();

	
}

void AEvidencePickup::OnTickEvidence()
{
	TicksProcessed++;

	OnEvidenceTicked();

	if (TicksProcessed >= TotalNumofTicks)
	{
		OnExpired();

		GetWorldTimerManager().ClearTimer(TimerHandle_EvidenceTick);
	}
}

void AEvidencePickup::ActivateEvidence()
{
	if (EvidenceInterval > 0.0f)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_EvidenceTick, this, &AEvidencePickup::OnTickEvidence, EvidenceInterval, true, 0.0f);
	}
	else
	{
		OnTickEvidence();
	}
}





