// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLGame.h"
#include "GOLSimulator.h"


// Sets default values
AGOLSimulator::AGOLSimulator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGOLSimulator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGOLSimulator::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

