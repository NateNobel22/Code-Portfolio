// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EvidencePickup.generated.h"

UCLASS()
class PROJECTPARANORMAL_API AEvidencePickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEvidencePickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Evidence")
	float EvidenceInterval;

	UPROPERTY(EditDefaultsOnly, Category = "Evidence")
	float TotalNumofTicks;

	UPROPERTY(EditDefaultsOnly, Category = "Evidence")
	float TicksProcessed;


	FTimerHandle TimerHandle_EvidenceTick;

	UFUNCTION()
		void OnTickEvidence();

public:	

	void ActivateEvidence();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Evidence")
	void OnActivated();

	UFUNCTION(BlueprintImplementableEvent, Category = "Evidence")
	void OnEvidenceTicked();

	UFUNCTION(BlueprintImplementableEvent, Category = "Evidence")
	void OnExpired();
};
