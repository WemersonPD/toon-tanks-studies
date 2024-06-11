// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABasePawn::ABasePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Capsule Collider and set it as the root component
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	// Create Base Mesh and attach it to the root component
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	// Create Turret Mesh and attach it to the Base Mesh
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	// Create Projectile Spawn Point and attach it to the Turret Mesh
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

void ABasePawn::RotateTurret(FVector LookAtTarget)
{
	FVector StartLocation = TurretMesh->GetComponentLocation();
	FRotator StartRotation = TurretMesh->GetComponentRotation();
	FVector TargetLocation = FVector(LookAtTarget - StartLocation);

	// Make the turret face the target
	// Rotate only the yaw (Z axis), not the pitch or roll
	FRotator TargetRotation = FRotator(0.f, TargetLocation.Rotation().Yaw, 0.f);

	// Get delta time
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);

	// Rotate the turret
	TurretMesh->SetWorldRotation(
		FMath::RInterpTo(
			StartRotation,
			TargetRotation,
			DeltaTime,
			25.f));
}