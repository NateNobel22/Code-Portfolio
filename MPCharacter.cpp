// Fill out your copyright notice in the Description page of Project Settings.


#include "MPCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "ProjectParanormal.h"
#include "PPRifle.h"
#include "HealthComponent.h"
#include "Net/UnrealNetwork.h"



// Sets default values
AMPCharacter::AMPCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);


	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	ZoomedFOV = 65;
	ZoomInterpSpeed = 20;

	WeaponAttachSocketName = "WeaponSocket";

	WeaponBackAttachSocketName = "WeaponAttachSocket";





}

// Called when the game starts or when spawned
void AMPCharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultFOV = CameraComp->FieldOfView;

	if (GetLocalRole() == ROLE_Authority) 
	{
		//Spawn Default Weapon
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			CurrentWeapon = GetWorld()->SpawnActor < APPRifle>(StarterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
			if (CurrentWeapon)
			{
				CurrentWeapon->SetOwner(this);
				CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WeaponSocket");
			}

			SecondaryWeapon = GetWorld()->SpawnActor<APPRifle>(SecondaryWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
			if (SecondaryWeapon)
			{
				SecondaryWeapon->SetOwner(this);
				SecondaryWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WeaponAttachSocket");
			}

	}

	

	HealthComp->OnHealthChanged.AddDynamic(this, &AMPCharacter::OnHealthChanged);
	


	
}

void AMPCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}

void AMPCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

void AMPCharacter::BeginCrouch()
{
	
	Crouch();
}

void AMPCharacter::EndCrouch()
{
	UnCrouch();
}

void AMPCharacter::BeginZoom()
{
	bWantstoZoom = true;
}

void AMPCharacter::EndZoom()
{
	bWantstoZoom = false;
}

void AMPCharacter::StartFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StartFire();


		bIsFiring = true;


	}
}

void AMPCharacter::StopFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
		bIsFiring = false;
	}
}

void AMPCharacter::ReloadWeapon()
{
	if (CurrentWeapon) 
	{
		CurrentWeapon->ReloadWeapon();
		
	}
}

void AMPCharacter::ToggleFireMode()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->ToggleFireMode();
	 }
}

void AMPCharacter::SwitchWeapon()
{
	if (CurrentWeapon)
	{
		CurrentWeapon = SecondaryWeapon;
	}
	if (SecondaryWeapon)
	{
		SecondaryWeapon = CurrentWeapon;
	}
	
}

void AMPCharacter::OnHealthChanged(UHealthComponent* inHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.0f && !bDied)
	{
		bDied = true;

		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		DetachFromControllerPendingDestroy();

		SetLifeSpan(10.0f);


	}
}



// Called every frame
void AMPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//bIsEmpty = CurrentWeapon->bIsEmpty;

	/* float TargetFOV = bWantstoZoom ? ZoomedFOV : DefaultFOV;
	
	float NewFOV = FMath::FInterpTo(CameraComp->FieldOfView, TargetFOV, DeltaTime, ZoomInterpSpeed);

	CameraComp->SetFieldOfView(NewFOV); */

}

// Called to bind functionality to input
void AMPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMPCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMPCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &AMPCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &AMPCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMPCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AMPCharacter::EndCrouch);



	//Weapons and Combat
	PlayerInputComponent->BindAction("ADS", IE_Pressed, this, &AMPCharacter::BeginZoom);
	PlayerInputComponent->BindAction("ADS", IE_Released, this, &AMPCharacter::EndZoom);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMPCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AMPCharacter::StopFire);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AMPCharacter::ReloadWeapon);
	PlayerInputComponent->BindAction("ChangeFireMode", IE_Pressed, this, &AMPCharacter::ToggleFireMode);
	//PlayerInputComponent->BindAction("SwitchWeapon",IE_Pressed, this, &AMPCharacter::SwitchWeapon);


	


}

FVector AMPCharacter::GetPawnViewLocation() const
{
	if (CameraComp) 
	{
		return CameraComp->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();

	
}

void AMPCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMPCharacter, CurrentWeapon);
	DOREPLIFETIME(AMPCharacter, bDied);
}

