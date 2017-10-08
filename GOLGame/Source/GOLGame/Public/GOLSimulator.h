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

	/** Returns ScoreText subobject **/
	FORCEINLINE class UTextRenderComponent* GetScoreText() const { return IterarionText; }

	/** Number of Rows along each side of grid */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Simulator")
	int32 Rows = 10;

	/** Number of Columns along each side of grid */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Simulator")
	int32 Columns = 10;

	// Time to perfom a new Iteration on the Simulation
	UPROPERTY(EditAnywhere, Category = "Simulator")
	float SecondsForIteration = 1.0f;

	// Size of each particle
	UPROPERTY(EditAnywhere, Category = "Simulator")
	float ParticleSize = 10.0f;

	// Percentage to decide the posibility to set a particle active on creation
	UPROPERTY(EditAnywhere, Category = "Simulator", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
	float CreationParticleActivePer= 0.3;

	AGOLParticle* GetParticle(int Row, int Column);

private:

	//Check the Neighbours and particle to decide it's state
	void ResolveParticle(const AGOLParticle* Particle);

	//Main function that perform the GOL Simulation
	void Iterate();

	void SwapBuffers();

	/** Dummy root component */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** Text component for the number of iterations */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* IterarionText;

	//Tick accumulate time
	float AccumulateTime = 0.0f;

	//Count of iterations in the simulation
	int NumberIterations = 0;

	//Array with all the particles created
	TArray<AGOLParticle*> Particles;

	//Primary Buffer for Alive Particles
	TArray<AGOLParticle*> PrimaryBuffer;

	//Secondary Buffer for Alive Particles
	TArray<AGOLParticle*> SecondaryBuffer;

	//Flag to choose Buffer
	bool bUsePrimaryBuffer = true;

	//TODO: Remove this fixed size, get correct size from Mesh?
	//Particle Fixed Size
	float ParticleMeshSize = 100.0f;

};