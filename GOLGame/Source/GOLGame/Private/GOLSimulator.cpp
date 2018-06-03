// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/GOLSimulator.h"
#include "GOLGame.h"
#include "../Public/GOLParticle.h"
#include "Components/TextRenderComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"


#define LOCTEXT_NAMESPACE "GAmeOfLife"

// Sets default values
AGOLSimulator::AGOLSimulator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

}

// Called when the game starts or when spawned
void AGOLSimulator::BeginPlay()
{
	Super::BeginPlay();

	ResetGrid(false);
}

void AGOLSimulator::ResetGrid(bool bRandomize)
{
	for (auto Particle : Particles)
	{
		//Delete Particles
		Particle->Destroy();
	}

	Particles.Empty();
	PossibleAliveParticles.Empty();
	DeadParticles.Empty();
	
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
				if (bRandomize && FMath::RandRange(0.0f, 1.0f) < CreationParticleActivePer)
				{
					//Set Particle active
					NewParticle->SetState(EParticleState::Alive);
					PossibleAliveParticles.Add(NewParticle);
				}
				//Add to the array
				Particles.Add(NewParticle);
			}
		}
	}
	CenterCamera();
}

// Called every frame
void AGOLSimulator::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	//If playing and some particles alive
	if (!bPaused && PossibleAliveParticles.Num() > 0)
	{
		AccumulateTime += DeltaTime;
		if (AccumulateTime > SecondsForIteration)
		{
			AccumulateTime = 0.0f;
			Iterate();
		}
	}
}

void AGOLSimulator::Iterate()
{
	//Increment Iterations
	NumberIterations++;

	UE_LOG(LogTemp, Warning, TEXT("Iteration: %d"), NumberIterations);

	// Resolve all the Alive Particles
	int Lenght = PossibleAliveParticles.Num();
	for (int i = 0; i < Lenght; i++)
	{
		AGOLParticle* CurrentParticle = PossibleAliveParticles[i];
		ResolveParticle(CurrentParticle);
		//Update value as the array may change on resolve
		Lenght = PossibleAliveParticles.Num();
	}
	FlushBuffer();
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

void AGOLSimulator::ResolveParticle(AGOLParticle* Particle)
{
	//Look through the neighbours, decide if the negighbours and the particle should stay on the next buffer
	int R, C = 0;
	Particle->GetCoordinates(R, C);
	TArray<AGOLParticle*> ParticleNeigh;
	int AliveNeigh = 0;
	// go though all the neighbuours and count how many are active or not. If this particle is changing state, we add all the neighbours to the active list
	int RInitPos = R - 1;
	int CInitPos = C - 1;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			//Go each particle and check if is active or not, if we want the particle to 
			//Get particle

			int CurrentR = RInitPos + i;
			int CurrentC = CInitPos + j;
			//Make sure everything is in bounds
			CalculateBoundedNeig(CurrentR, Rows);
			CalculateBoundedNeig(CurrentC, Columns);

			if (CurrentR == R && CurrentC == C)
			{
				//Same partcle, dont do anything
				continue;
			}

			AGOLParticle* Neigbour = GetParticle(CurrentR, CurrentC);
			if (Neigbour)
			{
				ParticleNeigh.Add(Neigbour);
				if (Neigbour->IsAlive())
				{
					AliveNeigh++;
					if (AliveNeigh > 3) // if we have more than 3 neighs alive we are going to be dead
					{
						break;
					}
				}
			}
		}
	}

	//Check what we need to do
	// if less than 2 alive neighbours : Dead
	// if more than 3 : Dead
	// if 2 or 3 : Alive
	// if it was dead and exactly 3 : Alive

	//If we change state, add the neighbours to the resolve list
	if (!Particle->IsAlive())
	{
		if (AliveNeigh == 3)
		{
			//Return from dead by reproduction
			//Add to the next generation AddNextGen()
			//Add Neighs to the ResolveList, as they may change with the new state
			AddNextGeneration(Particle);
			AddNeighbours(ParticleNeigh);
		}
		else
		{
			//This particle shouldn't be on the aliveBuffer, so add to the dead particles
			DeadParticles.Add(Particle);
		}
	}
	else
	{
		if (AliveNeigh < 2 || AliveNeigh > 3)
		{
			// Particle dies
			// Don't add to nextGen
			//Change State
			// Add only Alive Neighs, at they may change when this particle die
			DeadParticles.Add(Particle);
			AddOnlyAliveNeighbours(ParticleNeigh);
		}
		else
		{
			//Add to next gen
			//Add all the Neigburs to the possibleAlive buffer
			AddNextGeneration(Particle);
			AddNeighbours(ParticleNeigh);
		}
	}

}

void AGOLSimulator::CalculateBoundedNeig(int& CurrentPos, int MaxPos)
{
	if (CurrentPos < 0)
	{
		CurrentPos = MaxPos - 1;
	}

	if (CurrentPos > MaxPos)
	{
		CurrentPos = 0;
	}
}

//Adds to the next generation of particles Alive
void AGOLSimulator::AddNextGeneration(AGOLParticle* Particle)
{
	if (!PossibleAliveParticles.Contains(Particle))
	{
		PossibleAliveParticles.Add(Particle);
	}
}

void AGOLSimulator::AddNeighbours(const TArray<AGOLParticle*> Particles)
{
	//Try to add to Alive buffer if the particle is not yet and is not set to be dead
	for (AGOLParticle* CurrentParticle : Particles)
	{
		if (!PossibleAliveParticles.Contains(CurrentParticle) && !DeadParticles.Contains(CurrentParticle))
		{
			PossibleAliveParticles.Add(CurrentParticle);
		}
	}
}

void AGOLSimulator::AddOnlyAliveNeighbours(const TArray<AGOLParticle*> Particles)
{
	for (AGOLParticle* CurrentParticle : Particles)
	{
		if (!PossibleAliveParticles.Contains(CurrentParticle) && !DeadParticles.Contains(CurrentParticle) && CurrentParticle->IsAlive())
		{
			PossibleAliveParticles.Add(CurrentParticle);
		}
	}
}

void AGOLSimulator::FlushBuffer()
{
	//Remove dead particles
	for (auto DeadParticle : DeadParticles)
	{
		DeadParticle->SetState(EParticleState::Dead);
		PossibleAliveParticles.Remove(DeadParticle);
	}

	//Add alive particles
	for (AGOLParticle* Particle : PossibleAliveParticles)
	{
		Particle->SetState(EParticleState::Alive, bColourAge);
	}

	DeadParticles.Empty();
}

void AGOLSimulator::ChangeParticleState(AGOLParticle* Particle, bool bRemove)
{
	if (bRemove)
	{
		PossibleAliveParticles.Remove(Particle);
		DeadParticles.Add(Particle);
	}
	else
	{
		PossibleAliveParticles.Add(Particle);
		DeadParticles.Remove(Particle);
	}
}

void AGOLSimulator::ResetSimulator(bool bRandomize)
{
	AccumulateTime = 0.0f;
	ResetGrid(bRandomize);
	NumberIterations = 0;
}

float AGOLSimulator::GetIteration()
{
	return NumberIterations;
}

void AGOLSimulator::SetCameraHeight(float HeightRatio)
{
	if (SimCamera)
	{
		//convert from range [0-1] to new Range [Min-Max]
		// the NewValue will be => (((Value - 0) * (Max - Min) ) + Min)
		float NewRange = MaxCameraHeight - MinCameraHeight;
		float NewHeight = ((HeightRatio * NewRange)) + MinCameraHeight;
		SimCamera->SetActorLocation(FVector(SimCamera->GetActorLocation().X, SimCamera->GetActorLocation().Y, NewHeight));
	}
}

void AGOLSimulator::CenterCamera()
{
	//Update Camera position
	if (SimCamera)
	{
		//Set camera on the X/Y position of the Grid
		float NewSize = ParticleSize / ParticleMeshSize;
		FVector NewCameraLocation = GetActorLocation() + FVector(Rows * 0.5f * ParticleSize, Columns * 0.5f * ParticleSize, MinCameraHeight);
		NewCameraLocation.Z = SimCamera->GetActorLocation().Z;
		SimCamera->SetActorLocation(NewCameraLocation);
	}
}