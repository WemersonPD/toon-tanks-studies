// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

ATank::ATank()
{

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform);
}

// Function to get the input from the player
void ATank::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // The way to retrieve the input from the player
    // "MoveForward" is the name of the input in the project settings
    // This references the function Move in the Tank class
    // The function Move is defined in the Tank class
    PlayerInputComponent->BindAxis("MoveForward", this, &ATank::Move);

    // "Turn" is the name of the input in the project settings
    // This references the function Turn in the Tank class
    PlayerInputComponent->BindAxis("Turn", this, &ATank::Turn);
}

// Function to move the tank
// Value is the input from the player (1 or -1)
// 1 is forward (W), -1 is backward (S) (Defined in the project settings)
void ATank::Move(float Value)
{
    UE_LOG(LogTemp, Display, TEXT("The values is: %f"), Value);

    // One way to get the delta time
    // float DeltaTime = GetWorld()->GetDeltaSeconds();
    // Another way to get the delta time
    float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);

    float Movement = Speed * DeltaTime * Value;

    FVector DeltaLocation = FVector::ZeroVector;
    DeltaLocation.X = Movement;

    // The tank moves forward or backward
    // We are using local offset because we want to move the tank in the direction it is facing
    AddActorLocalOffset(DeltaLocation, true);
}

// Function to turn the tank
// Value is the input from the player (1 or -1)
// 1 is right (D), -1 is left (A) (Defined in the project settings)
void ATank::Turn(float Value)
{
    float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);

    float Rotation = TurnRate * DeltaTime * Value;

    FRotator DeltaRotation = FRotator::ZeroRotator;
    // Rotation around the Z axis
    DeltaRotation.Yaw = Rotation;

    // The tank turns left or right
    AddActorLocalRotation(DeltaRotation, true);
}

void ATank::BeginPlay()
{
    Super::BeginPlay();

    // The way to get the player controller
    PlayerControllerRef = Cast<APlayerController>(GetController());
}

void ATank::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (PlayerControllerRef)
    {
        FHitResult HitResult;
        // The way to get the hit result under the cursor
        PlayerControllerRef->GetHitResultUnderCursor(
            ECollisionChannel::ECC_Visibility,
            false,
            HitResult);

        // The way to draw a debug sphere at the hit location
        DrawDebugSphere(
            GetWorld(),
            HitResult.ImpactPoint,
            10.0f,
            12,
            FColor::Red,
            false,
            -1.f);

        ABasePawn::RotateTurret(HitResult.ImpactPoint);
    }
}
