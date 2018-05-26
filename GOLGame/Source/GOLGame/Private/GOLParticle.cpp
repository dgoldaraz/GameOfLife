// Fill out your copyright notice in the Description page of Project Settings.
#include "../Public/GOLParticle.h"
#include "GOLGame.h"
#include "../Public/GOLSimulator.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

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
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> ParticleMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> AliveMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> DeadMaterial;
		FConstructorStatics()
			: ParticleMesh(TEXT("/Game/Puzzle/Meshes/ParticleSphere.ParticleSphere"))
			, DeadMaterial(TEXT("/Game/Puzzle/Meshes/DeadMaterial.DeadMaterial"))
			, AliveMaterial(TEXT("/Game/Puzzle/Meshes/LiveMaterial.LiveMaterial"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	ParticleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ParticleMesh0"));
	ParticleMesh->SetStaticMesh(ConstructorStatics.ParticleMesh.Get());
	AliveMaterial = ConstructorStatics.AliveMaterial.Get();
	DeadMaterial = ConstructorStatics.DeadMaterial.Get();
	SetMaterialForState();
	ParticleMesh->SetRelativeScale3D(FVector(1.f, 1.f, 1.0f));
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

bool AGOLParticle::IsAlive()
{
	return CurrentState == EParticleState::Alive;
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

void AGOLParticle::ScaleParticle(float Size)
{
	ParticleMesh->SetRelativeScale3D(FVector(Size, Size, Size));
}

void AGOLParticle::SetSimulator(AGOLSimulator* Sim)
{
	Simulator = Sim;
}

void AGOLParticle::SetCoordinates(int R, int C)
{
	Row = R;
	Column = C;
}

void AGOLParticle::GetCoordinates(int& R, int& C) const
{
	R = Row;
	C = Column;
}

void AGOLParticle::BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	if (Simulator && Simulator->IsPaused())
	{
		if (IsAlive())
		{
			SetState(EParticleState::Dead);
			Simulator->ChangeParticleState(this, true);
		}
		else
		{
			SetState(EParticleState::Alive);
			Simulator->ChangeParticleState(this, false);
		}
	}
	
}