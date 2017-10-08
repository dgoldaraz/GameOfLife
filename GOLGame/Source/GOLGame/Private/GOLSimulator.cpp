// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLGame.h"
#include "../Public/GOLSimulator.h"
#include "../Public/GOLParticle.h"
#include "Components/TextRenderComponent.h"


#define LOCTEXT_NAMESPACE "GAmeOfLife"

// Sets default values
AGOLSimulator::AGOLSimulator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	IterarionText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Iterationtext0"));
	IterarionText->SetRelativeLocation(FVector(200.0, 0.f, 0.f));
	IterarionText->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	IterarionText->SetText(FText::Format(LOCTEXT("IterationFmt", "Iterations: {0}"), FText::AsNumber(0)));
	IterarionText->SetupAttachment(DummyRoot);

}

// Called when the game starts or when spawned
void AGOLSimulator::BeginPlay()
{
	Super::BeginPlay();
	//IterarionText->SetRelativeLocation(FVector(0.0f, 2.0f, 0.0f));

	//Populate Particles 
	float XOffset = 0.0f;
	float YOffset = 0.0f;
	FAttachmentTransformRules Rules(EAttachmentRule::KeepWorld, false);
	//Calculate new Size.
	// Each capsule by default is 100, so depending on the ParticleSize, we can choose which is the right size
	float NewSize = ParticleSize / ParticleMeshSize;
	for (int i = 0; i < Rows; i++)
	{
		XOffset = i * ParticleSize;
		for (int j = 0; j < Columns; j++)
		{
			YOffset = j * ParticleSize;
			//Get Position for Particle
			const FVector ParticlePos = FVector(XOffset, YOffset, 0.0f) + GetActorLocation();

			//Spawn a particle
			AGOLParticle* NewParticle = GetWorld()->SpawnActor<AGOLParticle>(ParticlePos, FRotator(0, 0, 0));

			if (NewParticle)
			{
				NewParticle->SetSimulator(this);
				NewParticle->AttachToActor(this, Rules);
				NewParticle->ScaleParticle(NewSize);
				NewParticle->SetCoordinates(i, j);
				if (FMath::RandRange(0.0f, 1.0f) < CreationParticleActivePer)
				{
					//Set Particle active
					NewParticle->SetState(EParticleState::Alive);
					PrimaryBuffer.Add(NewParticle);
				}
				//Add to the array
				Particles.Add(NewParticle);
			}
		}
	}
}

// Called every frame
void AGOLSimulator::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	AccumulateTime += DeltaTime;
	if (AccumulateTime > SecondsForIteration)
	{
		AccumulateTime = 0.0f;
		Iterate();
	}
}

void AGOLSimulator::Iterate()
{
	//Increment Iterations
	NumberIterations++;

	// Update text
	IterarionText->SetText(FText::Format(LOCTEXT("IterationFmt", "Iterations: {0}"), FText::AsNumber(NumberIterations)));

	// Resolve all the Alive Particles
	if (bUsePrimaryBuffer)
	{
		for (const AGOLParticle* CurrentParticle : PrimaryBuffer)
		{
			ResolveParticle(CurrentParticle);
		}
	}
	else
	{
		for (const AGOLParticle* CurrentParticle : SecondaryBuffer)
		{
			ResolveParticle(CurrentParticle);
		}
	}
	
}

AGOLParticle* AGOLSimulator::GetParticle(int Row, int Column)
{
	//Convert 2D Coords into 1D
	int Index = Row * Rows + Column;
	if (Index < Particles.Num())
	{
		return Particles[Index];
	}

	return nullptr;
}

void AGOLSimulator::ResolveParticle(const AGOLParticle* Particle)
{
	//Look through the neighbours, decide if the negighbours and the particle should stay on the next buffer
}

void AGOLSimulator::SwapBuffers()
{
	if (bUsePrimaryBuffer)
	{
		PrimaryBuffer.Empty();
		for (AGOLParticle* Particle : SecondaryBuffer)
		{
			Particle->SetState(EParticleState::Alive);
		}
	}
	else
	{
		SecondaryBuffer.Empty();
		for (AGOLParticle* Particle : PrimaryBuffer)
		{
			Particle->SetState(EParticleState::Alive);
		}
	}

	bUsePrimaryBuffer = !bUsePrimaryBuffer;
}

