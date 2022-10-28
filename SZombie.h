// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SZombie.generated.h"


class UHealthComponent;

UCLASS()
class PROJECTPARANORMAL_API ASZombie : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASZombie();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USkeletalMeshComponent* SKMeshComp;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UHealthComponent* HealthComp;

	
	
	
/* Damage and Death */
	
	UFUNCTION()
	void HandleTakeDamage(UHealthComponent* inHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Zombie")
	bool bDied;

};
