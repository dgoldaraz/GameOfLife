// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GOLSimulator.generated.h"

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

private:

	/** Dummy root component */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** Text component for the number of iterations */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* IterarionText;

	//Main function that perform the GOL Simulation
	void Iterate();

	//Tick accumulate time
	float AccumulateTime = 0.0f;

	//Count of iterations in the simulation
	int NumberIterations = 0;

};