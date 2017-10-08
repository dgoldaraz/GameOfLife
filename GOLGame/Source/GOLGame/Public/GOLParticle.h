// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "GameFramework/Actor.h"
#include "GOLParticle.generated.h"

/*
	Game Of Life Particle, will store the current state in game and notify all its neighbours about it
*/

class AGOLSimulator;

// Enum for different states
UENUM(BlueprintType)
enum class EParticleState : uint8
{
	Alive,
	Dead, 
};

UCLASS()
class GOLGAME_API AGOLParticle : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AGOLParticle();
	AGOLParticle(int NewRow, int NewColumn);

	// Begin AActor interface
	virtual void BeginPlay() override;
	// End AActor interface

	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }
	/** Returns BlockMesh subobject **/
	FORCEINLINE class UStaticMeshComponent* GetBlockMesh() const { return ParticleMesh; }
	
	//Get/Set State
	EParticleState GetCurrentState() { return CurrentState; }
	void SetSimulator(AGOLSimulator* Sim);


	UPROPERTY(EditAnywhere, Category = "Game Of Life")
	class UMaterialInstance* DeadMaterial;

	UPROPERTY(EditAnywhere, Category = "Game Of Life")
	class UMaterialInstance* AliveMaterial;

	/** Dummy root component */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Particle)
	class USceneComponent* DummyRoot;

	/** StaticMesh component */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Particle)
	class UStaticMeshComponent* ParticleMesh;

	//This function Scales the Mesh of the particle by Size
	void ScaleParticle(float Size);

	//Set Current State on the Particle
	void SetState(EParticleState NewState);

	void SetCoordinates(int R, int C);
private:

	//Gather all the resources and sets any intial values
	void Initialize();

	//Select the best material for the Mesh based on the state
	void SetMaterialForState();

	//Current Row/Column
	int Row = 0;
	int Column = 0;

	//Current State of the particle
	UPROPERTY(EditAnywhere, Category = Particle)
	EParticleState CurrentState = EParticleState::Dead;

	AGOLSimulator* Simulator = nullptr;
};
