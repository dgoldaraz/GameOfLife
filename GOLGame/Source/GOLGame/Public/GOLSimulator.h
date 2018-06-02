// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GOLSimulator.generated.h"

class AGOLParticle;

UCLASS()
class GOLGAME_API AGOLSimulator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGOLSimulator();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }

	/** Number of Rows along each side of grid */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulator")
	int32 Rows = 10;

	/** Number of Columns along each side of grid */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulator")
	int32 Columns = 10;

	// Time to perfom a new Iteration on the Simulation
	UPROPERTY(EditAnywhere, Category = "Simulator")
	float SecondsForIteration = 1.0f;

	// Size of each particle
	UPROPERTY(EditAnywhere, Category = "Simulator")
	float ParticleSize = 10.0f;

	// Size of each particle
	UPROPERTY(EditAnywhere, Category = "Simulator")
	float MinCameraHeight = 1.0f;

	// Size of each particle
	UPROPERTY(EditAnywhere, Category = "Simulator")
	float MaxCameraHeight = 1000.0f;

	// Size of each particle
	UPROPERTY(EditAnywhere, Category = "Simulator")
	AActor* SimCamera = nullptr;

	// Percentage to decide the posibility to set a particle active on creation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulator", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
	float CreationParticleActivePer= 0.1;

	UFUNCTION(BlueprintCallable, Category = "Simulator")
	void PauseSimulation() { bPaused = true; }

	UFUNCTION(BlueprintCallable, Category = "Simulator")
	void PlaySimulation() { bPaused = false; }

	UFUNCTION(BlueprintCallable, Category = "Simulator")
	bool IsPaused() { return bPaused; }

	UFUNCTION(BlueprintCallable, Category = "Simulator")
	void ResetSimulator(bool bRandomize);

	//Function that will change the Camera Height based on the ratio passed. This ratio need to be between 0 and 1
	UFUNCTION(BlueprintCallable, Category = "Simulation")
	void SetCameraHeight(float HeightRatio);

	UFUNCTION(BlueprintCallable, Category = "Simulation")
	float GetIteration();

	void ChangeParticleState(AGOLParticle* Particle, bool bRemove);

	AGOLParticle* GetParticle(int Row, int Column);

private:

	//Check the Neighbours and particle to decide it's state
	void ResolveParticle(AGOLParticle* Particle);

	//Main function that perform the GOL Simulation
	void Iterate();

	void FlushBuffer();

	//Adds to the next generation of particles
	void AddNextGeneration(AGOLParticle* Particle);

	//Add the list of neighbours for a new alive particle
	void AddNeighbours(const TArray<AGOLParticle*> Particles);

	//Add the particles that are alive only
	void AddOnlyAliveNeighbours(const TArray<AGOLParticle*> Particles);

	// Function to calculate a bounded position for neighbours
	// Will check based on the currentPos, which will be the first position and last. Will perform the calculations to not give negative or out bounded values, returning the correct value
	void CalculateBoundedNeig(int& CurrentPos, int MaxPos);

	//Clean previous iteration and reset grid
	void ResetGrid(bool bRandomize);

	void CenterCamera();

	/** Dummy root component */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	//Tick accumulate time
	float AccumulateTime = 0.0f;

	//Count of iterations in the simulation
	int NumberIterations = 0;

	//Array with all the particles created
	TArray<AGOLParticle*> Particles;

	//Save the possible alive particles that live one iteration, may have dead particles that will be removed at the end of the iteration
	// Every particle on this buffer has a chance to be alive at the end of the iteration, unless the particle is added to the DeadBuffer
	TArray<AGOLParticle*> PossibleAliveParticles;

	//Saves the Dead particles
	TArray<AGOLParticle*> DeadParticles;

	//TODO: Remove this fixed size, get correct size from Mesh?
	//Particle Fixed Size
	float ParticleMeshSize = 100.0f;

	bool bPaused = true;

};