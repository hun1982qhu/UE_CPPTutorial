// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseGeometryActor.h"
#include "GeometryHubActor.generated.h"

USTRUCT(BlueprintType)
struct FGeometryPayload
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseGeometryActor> GeometryClass;

	UPROPERTY(EditAnywhere)
	FGeometryData Data;

	UPROPERTY(EditAnywhere)
	FTransform InitialTransform; 
	
};

UCLASS()
class CPP_TUTORIAL_API AGeometryHubActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGeometryHubActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseGeometryActor> GeometryClass;

	// The following code is just for comparing with TSubclassOf method
	UPROPERTY(EditAnywhere)
	UClass* Class;

	// The following code is just for comparing with TSubclassOf method
	UPROPERTY(EditAnywhere)
	ABaseGeometryActor* GeometryObject;

	UPROPERTY(EditAnywhere)
	TArray<FGeometryPayload> GeometryPayloads;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
