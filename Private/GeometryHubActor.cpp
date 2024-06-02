// Fill out your copyright notice in the Description page of Project Settings.


#include "GeometryHubActor.h"
#include "Math/Transform.h"
#include "Engine/World.h"

// LogGeometryHub is the name of DEFINE_LOG_CATEGORY_STATIC 
DEFINE_LOG_CATEGORY_STATIC(LogGeometryHub, All, All)

// Sets default values
AGeometryHubActor::AGeometryHubActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGeometryHubActor::BeginPlay()
{
	Super::BeginPlay();
	DoActorSpawn();
}

// Called every frame
void AGeometryHubActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// The following need to be bound to delegate
// When we call the broadcast function of our delegate, we pass the pointer to the current actor as a parameter,
// that is, in fact, a pointer to BaseGeometryActor, but in the delegate signature we specified the parameter as
// a pointer to the base class of the actor. In fact, when the broadcast function is called, an automatic up-cast occurs
// -- the conversion of a pointer from ABaseGeometryActor to a pointer to just AActor.
// We can change the signature of the delegate and explicitly indicate the signature ABaseGeometryActor.
void AGeometryHubActor::OnColorChanged(const FLinearColor& Color, const FString& Name)
{
	UE_LOG(LogGeometryHub, Warning, TEXT("Actor name: %s Color %s"), *Name, *Color.ToString());
}

// The following need to be bound to delegate
void AGeometryHubActor::OnTimerFinished(AActor* Actor)
{
	if (!Actor) return;
	UE_LOG(LogGeometryHub, Error, TEXT("Timer finished: %s"), *Actor->GetName());

	ABaseGeometryActor* Geometry = Cast<ABaseGeometryActor>(Actor);

	if (!Geometry) return;
	UE_LOG(LogGeometryHub, Display, TEXT("Cast is success, amplitude %f"), Geometry->GetGeometryData().Amplitude);
	Geometry->Destroy();
	// Geometry->SetLifeSpan(2.0f);
}

void AGeometryHubActor::DoActorSpawn()
{
	// returns a pointer to the global game world object
	UWorld* World = GetWorld();

	if (World)
	{
		for (int32 i = 0; i < 10; ++i)
		{
			const FTransform GeometryTransform = FTransform(FRotator::ZeroRotator, FVector(0.0f, 300.0f * i, 330.0f));
			// para1: specify the class of the actor that we want to spawn
			// para2: location
			// para3: rotation
			// para4: FActorSpawnParameters - specifies additional spawn settings

			// World->SpawnActor<>():
			// World->SpawnActor<>()是一个模板函数，用于在游戏世界中生成一个新的actor。
			// 这个函数是UWorld类的一个成员函数，它负责在游戏中创建和管理actor。

			// 模板参数:
			// <ABaseGeometryActor>: 这个模板参数告诉SpawnActor函数想要生成的actor的类型。
			// 在这个例子中，它指定了ABaseGeometryActor，这意味着生成的actor将是ABaseGeometryActor类型或其子类的实例。
			ABaseGeometryActor* Geometry = World->SpawnActor<ABaseGeometryActor>(GeometryClass, GeometryTransform);

			if (Geometry)
			{
				FGeometryData Data;
				Data.MoveType = FMath::RandBool() ? EMovementType::Static : EMovementType::Sin;
				Geometry->SetGeometryData(Data);
			}
		}

		for (int32 i = 0; i < 10; ++i)
		{
			const FTransform GeometryTransform = FTransform(FRotator::ZeroRotator, FVector(0.0f, 300.0f * i, 700.0f));
			ABaseGeometryActor* Geometry = World->SpawnActorDeferred<ABaseGeometryActor>(
				GeometryClass, GeometryTransform);

			if (Geometry)
			{
				FGeometryData Data;
				Data.Color = FLinearColor::MakeRandomColor();
				Geometry->SetGeometryData(Data);
				Geometry->FinishSpawning(GeometryTransform);
			}
		}

		for (const FGeometryPayload& Payload : GeometryPayloads)
		{
			ABaseGeometryActor* Geometry = World->SpawnActorDeferred<ABaseGeometryActor>(
				Payload.GeometryClass, Payload.InitialTransform);

			if (Geometry)
			{
				Geometry->SetGeometryData(Payload.Data);
				Geometry->OnColorChanged.AddDynamic(this, &AGeometryHubActor::OnColorChanged);
				Geometry->OnTimerFinished.AddUObject(this, &AGeometryHubActor::OnTimerFinished);
				Geometry->FinishSpawning(Payload.InitialTransform);
			}
		}
	}
}
