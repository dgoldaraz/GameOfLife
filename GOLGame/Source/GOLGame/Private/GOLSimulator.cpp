// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLGame.h"
#include "../Public/GOLSimulator.h"
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
	IterarionText->SetRelativeLocation(FVector(Columns * ParticleSize, 0.f, 0.f));
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

}

