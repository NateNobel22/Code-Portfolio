// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HealthComponent.h"
#include "MPCharacter.generated.h"


class UCameraComponent;
class USpringArmComponent;
class APPRifle;
class UHealthComponent;

UCLASS()
class PROJECTPARANORMAL_API AMPCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMPCharacter();

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHealthComponent* HealthComp;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void BeginCrouch();

	void EndCrouch();

	
	
/*ADS RELATED*/

	void BeginZoom();

	void EndZoom();

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	float ZoomedFOV;

	float DefaultFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0, ClampMax = 100))
	float ZoomInterpSpeed;


/* WEAPONS AND COMBAT */

	UPROPERTY(Replicated)
	APPRifle* CurrentWeapon;

	UPROPERTY(Replicated)
	APPRifle* SecondaryWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<APPRifle> SecondaryWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<APPRifle> StarterWeaponClass;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapons")
	FName WeaponAttachSocketName;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapons")
	FName WeaponBackAttachSocketName;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapons")
	FName SidearmAttachSocketName;




/*GEAR AND EQUIPMENT*/
	UPROPERTY(VisibleDefaultsOnly, Category = "Clothing")
	FName VestSocketName;

	UPROPERTY(VisibleDefaultsOnly, Category = "Clothing")
	FName ShirtSocketName;

	UPROPERTY(VisibleDefaultsOnly, Category = "Clothing")
	FName PantsSocketName;







	void StartFire();

	void StopFire();

	void ReloadWeapon();

	void ToggleFireMode();

	void SwitchWeapon();

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	bool bIsFiring;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	bool bIsEmpty;



	

/*HEALTH, DAMAGE, AND DEATH RELATED*/

	UFUNCTION()
	void OnHealthChanged(UHealthComponent* inHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	bool bDied;


/*NETWORK RELATED*/

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;

	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	bool bWantstoZoom;
};
