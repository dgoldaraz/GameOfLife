// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLGame.h"
#include "../Public/GOLParticle.h"

// Sets default values
AGOLParticle::AGOLParticle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Initialize();
}

AGOLParticle::AGOLParticle(int NewRow, int NewColumn):
	Row(NewRow), Column(NewColumn)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Initialize();
}

void AGOLParticle::BeginPlay()
{
	SetMaterialForState();
}

void AGOLParticle::Initialize()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> AliveMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> DeadMaterial;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Puzzle/Meshes/PuzzleCube.PuzzleCube"))
			, AliveMaterial(TEXT("/Game/Puzzle/Meshes/AliveMaterial.AliveMaterial"))
			, DeadMaterial(TEXT("/Game/Puzzle/Meshes/DeadMaterial.DeadMaterial"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	ParticleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ParticleMesh0"));
	ParticleMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	AliveMaterial = ConstructorStatics.AliveMaterial.Get();
	DeadMaterial = ConstructorStatics.DeadMaterial.Get();
	SetMaterialForState();
	ParticleMesh->SetRelativeScale3D(FVector(1.f, 1.f, 0.25f));
	ParticleMesh->SetRelativeLocation(FVector(0.f, 0.f, 25.f));
	ParticleMesh->SetupAttachment(DummyRoot);
	
}

void AGOLParticle::SetState(EParticleState NewState)
{
	if (NewState != CurrentState)
	{
		CurrentState = NewState;
		SetMaterialForState();
	}
}

void AGOLParticle::SaveNexState(EParticleState NewState)
{
	NextState = NewState;
}

void AGOLParticle::PushNewState()
{
	//Push New State 
	SetState(NextState);
	NextState = EParticleState::Dead;
}

void AGOLParticle::SetMaterialForState()
{
	switch (CurrentState)
	{
	case EParticleState::Alive:
	{
		ParticleMesh->SetMaterial(0, AliveMaterial);
		break;
	}
	default:
	{
		ParticleMesh->SetMaterial(0, DeadMaterial);
		break;
	}
	}
}
