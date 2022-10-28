// Fill out your copyright notice in the Description page of Project Settings.


#include "PPRifle.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "particles/PxParticleSystem.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Chaos/ChaosEngineInterface.h"
#include "Engine/EngineTypes.h"
#include <ProjectParanormal/ProjectParanormal.h>
#include "Net/UnrealNetwork.h"
#include "Sound/SoundCue.h"



static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef DebugWeapon(TEXT("MP.DebugWeapons"), DebugWeaponDrawing, TEXT("Draw Debug Lines for Weapons"), ECVF_Cheat);

// Sets default values
APPRifle::APPRifle()
{
 
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	MuzzleSocketName = "MuzzleSocket";

	ShellEjectionSocketName = "ShellEjectionSocket";

	BaseDamage = 20.f;
	RateofFire = 700;

	
	MagazineSize = 30.0f;
	TotalAmmo = 300.0f;

	bIsSingleFire = true;
	bIsEmpty = false;


	//Networking and Multiplayer
	SetReplicates(true);

	NetUpdateFrequency = 66.0f;
	MinNetUpdateFrequency = 33.0f;

}

void APPRifle::BeginPlay()
{
	Super::BeginPlay();
	TimeBetweenShots = 60 / RateofFire;
}






void APPRifle::Fire()
{
	

	if (GetLocalRole() != ROLE_Authority)
	{
		ServerFire();
		
	}

	AActor* MyOwner = GetOwner();

	MagazineSize--;

	if (MagazineSize <= 0) 
	{
		bIsEmpty = true;
	}

	if(!bIsEmpty && TotalAmmo > 0 && !bIsSingleFire)
	{
		if (MyOwner)
		{
			FVector EyeLocation;
			FRotator EyeRotation;
			MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

			FVector ShotDirection = EyeRotation.Vector();

			FVector TraceEnd = EyeLocation + (ShotDirection * 100000);

			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(MyOwner);
			QueryParams.AddIgnoredActor(this);
			QueryParams.bTraceComplex = true;
			QueryParams.bReturnPhysicalMaterial = true;

			EPhysicalSurface SurfaceType = SurfaceType_Default;

			UGameplayStatics::SpawnSoundAttached(RifleFireSound, RootComponent);

			FHitResult Hit;
			if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_WEAPON, QueryParams))
			{
				// Blocking Hit, process damage




				AActor* HitActor = Hit.GetActor();
				SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

				float ActualDamage = BaseDamage;
				if (SurfaceType == SurfaceType2)
				{
					ActualDamage *= 4.0f;
				}

				UGameplayStatics::ApplyPointDamage(HitActor, ActualDamage, ShotDirection, Hit, MyOwner->GetInstigatorController(), this, DamageType);

				PlayImpactEffects(SurfaceType, Hit.ImpactPoint);

				HitScanTrace.SurfaceType = SurfaceType;
			}

			if (DebugWeaponDrawing > 0)
			{
				DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::White, false, 1.0f, 0, 1.0f);
			}

			PlayFireEffects();
			
			if(GetLocalRole() == ROLE_Authority)
			{
				HitScanTrace.TraceTo = Hit.ImpactPoint;
				HitScanTrace.SurfaceType = SurfaceType;
			}

			

			LastFiredTime = GetWorld()->TimeSeconds;

		}
	}

	if (!bIsEmpty && TotalAmmo > 0)
	{
		if (MyOwner)
		{
			FVector EyeLocation;
			FRotator EyeRotation;
			MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

			FVector ShotDirection = EyeRotation.Vector();

			FVector TraceEnd = EyeLocation + (ShotDirection * 100000);

			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(MyOwner);
			QueryParams.AddIgnoredActor(this);
			QueryParams.bTraceComplex = true;
			QueryParams.bReturnPhysicalMaterial = true;

			EPhysicalSurface SurfaceType = SurfaceType_Default;

			UGameplayStatics::SpawnSoundAttached(RifleFireSound, RootComponent);

			FHitResult Hit;
			if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_WEAPON, QueryParams))
			{
				// Blocking Hit, process damage




				AActor* HitActor = Hit.GetActor();
				SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

				float ActualDamage = BaseDamage;
				if (SurfaceType == SurfaceType2)
				{
					ActualDamage *= 4.0f;
				}

				UGameplayStatics::ApplyPointDamage(HitActor, ActualDamage, ShotDirection, Hit, MyOwner->GetInstigatorController(), this, DamageType);

				PlayImpactEffects(SurfaceType, Hit.ImpactPoint);

				HitScanTrace.SurfaceType = SurfaceType;
			}

			if (DebugWeaponDrawing > 0)
			{
				DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::White, false, 1.0f, 0, 1.0f);
			}

			PlayFireEffects();

			if (GetLocalRole() == ROLE_Authority)
			{
				HitScanTrace.TraceTo = Hit.ImpactPoint;
				HitScanTrace.SurfaceType = SurfaceType;
			}



			LastFiredTime = GetWorld()->TimeSeconds;

		}
	}

	
	
}

void APPRifle::ToggleFireMode()
{
	if (bIsSingleFire)
	{
		bIsSingleFire = false;
	}

	else if (!bIsSingleFire)
	{
		bIsSingleFire = true;
	}
}

void APPRifle::OnRep_HitScanTrace()
{
	PlayImpactEffects(HitScanTrace.SurfaceType, HitScanTrace.TraceTo);
	PlayFireEffects();
}



void APPRifle::ServerFire_Implementation()
{
	Fire();
}

bool APPRifle::ServerFire_Validate()
{
	return true;
}

void APPRifle::ServerReload_Implementation()
{
	ReloadWeapon();
}

bool APPRifle::ServerReload_Validate()
{
	return true;
}


void APPRifle::StartFire()
{

	if (!bIsSingleFire)
	{
		float FirstDelay = FMath::Max(LastFiredTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);

		GetWorldTimerManager().SetTimer(TimerHandle_TimeBetweenShots, this, &APPRifle::Fire, TimeBetweenShots, true, FirstDelay);
		Fire();
	}
	
	else 
	{
		Fire();
	}
}

void APPRifle::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
}

void APPRifle::ReloadWeapon()
{
	if (GetLocalRole() != ROLE_Authority)
	{
		ServerReload();

	}
	
	float currentMagSize = MagazineSize;
	float ammoToSubstract = 30 - currentMagSize;

	TotalAmmo = TotalAmmo - ammoToSubstract;

	MagazineSize = 30;

	bIsEmpty = false;

}


void APPRifle::PlayFireEffects()
{

	

	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
	}

	if (EjectionEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(EjectionEffect, MeshComp, ShellEjectionSocketName);
	}

	APawn* MyOwner = Cast<APawn>(GetOwner());
	if (MyOwner)
	{
		APlayerController* PC = Cast<APlayerController>(MyOwner->GetController());
			if (PC)
			{
				PC->ClientPlayCameraShake(FireCamShake);
			}
	}
}

void APPRifle::PlayImpactEffects(EPhysicalSurface SurfaceType, FVector ImpactPoint)
{
	

	UParticleSystem* SelectedEffect = nullptr;

	switch (SurfaceType)
	{
	case SurfaceType1:  //Flesh Default
		SelectedEffect = FleshImpactEffect;
		break;
	case SurfaceType2:  //Flesh Vulnerable
		SelectedEffect = FleshVulImpactEffect;
		break;
	case SurfaceType3:  //Metal
		SelectedEffect = MetalImpactEffect;
		break;
	case SurfaceType4:  //Wood
		SelectedEffect = WoodImpactEffect;
	default:
		SelectedEffect = DefaultImpactEffect;
		break;
	}

	FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);

	FVector ShotDirection = ImpactPoint - MuzzleLocation;
	ShotDirection.Normalize();
	
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, ImpactPoint, ShotDirection.Rotation());
}


void APPRifle::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(APPRifle, HitScanTrace, COND_SkipOwner);
}



