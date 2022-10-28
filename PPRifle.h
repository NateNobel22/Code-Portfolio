// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPRifle.generated.h"

class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;
class USoundCue;


USTRUCT()
struct FHitScanTrace
{
	GENERATED_BODY()

public:
	
	UPROPERTY()
	TEnumAsByte<EPhysicalSurface> SurfaceType;

	UPROPERTY()
	FVector_NetQuantize TraceTo;

	
};

UCLASS()
class PROJECTPARANORMAL_API APPRifle : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APPRifle();

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USkeletalMeshComponent* MeshComp;

	
/*FIRING EFFECTS*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName ShellEjectionSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* MuzzleEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* EjectionEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UCameraShakeBase> FireCamShake;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	USoundCue* RifleFireSound;
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void PlayFireEffects();


/* IMPACT EFFECT DECLARATIONS*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* FleshImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* FleshVulImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* MetalImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* WoodImpactEffect;

	void PlayImpactEffects(EPhysicalSurface SurfaceType, FVector ImpactPoint);





	

/* DAMAGE INFORMATION */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float BaseDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		TSubclassOf<UDamageType> DamageType;




/*FIRING AND SERVER REPLICATION */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFire();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerReload();

	void Fire();
	

	UPROPERTY(ReplicatedUsing=OnRep_HitScanTrace)
	FHitScanTrace HitScanTrace;

	UFUNCTION()
	void OnRep_HitScanTrace();


/* INFORMATION RELATED TO RATE OF FIRE */
	FTimerHandle TimerHandle_TimeBetweenShots;

	float LastFiredTime;

	float TimeBetweenShots;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float RateofFire;







public:
	
	

	void StartFire();
	void StopFire();
	void ReloadWeapon();
	void ToggleFireMode();

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	bool bIsSingleFire;

	/* MAGAZINE AND AMMUNITION INFORMATION*/
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float MagazineSize;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float TotalAmmo;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	bool bIsEmpty;

};
