// Fill out your copyright notice in the Description page of Project Settings.


#include "SZombie.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "HealthComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASZombie::ASZombie()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SKMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SKMeshComp"));
	
	RootComponent = SKMeshComp;

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	HealthComp->OnHealthChanged.AddDynamic(this, &ASZombie::HandleTakeDamage);

	bDied = false;

}

// Called when the game starts or when spawned
void ASZombie::BeginPlay()
{
	Super::BeginPlay();


	
	
}

void ASZombie::HandleTakeDamage(UHealthComponent* inHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	

	if (Health <= 0.0f && !bDied)
	{
		bDied = true;

		


	} 
	
}



// Called every frame
void ASZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}



void ASZombie::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASZombie, bDied);
}

