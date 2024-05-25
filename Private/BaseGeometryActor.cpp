// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGeometryActor.h"
#include "Engine/Engine.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "TimerManager.h"

#include "WorldPartition/ContentBundle/ContentBundleLog.h"

// This macro is reponsible for creating a local logging category
// Category is only available within this .cpp file
// There are three parameters:
// 1. category name
// 2 & 3. maxium allowed logging level.
// 2. DefaultVerbosity
// 3. CompileTimeVerbosity
// DEFINE_LOG_CATEGORY_STATIC(LogBaseGeometry, Error, Error)
DEFINE_LOG_CATEGORY_STATIC(LogBaseGeometry, All, All)

// We can also create global logging category of the LogTemp type, which will be available in every project file

// Sets default values
//------------------------------------------------------------------------------------------------------------------------------------------------------
ABaseGeometryActor::ABaseGeometryActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// parameters:
	// 1. component name: FName type - used mainly to set the names of components, names of players, parameters of materials,
	// body parts in the skeleton
	// 2. don't consider the second parameter for now
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");

	// point the root component to our actor
	// 组件和Root Component
	// 在 Unreal Engine 中，一个 Actor 由多个组件组成，这些组件可以是可视的如网格（Mesh）和灯光，
	// 也可以是不可视的如碰撞体。每个 Actor 都有一个被称为“根组件”（Root Component）的主要组件，该组件在组件层次结构中位于最顶层。
	// SetRootComponent 函数
	// SetRootComponent 函数用于设置一个 Actor 的根组件。根组件是 Actor 的主体组件，其他所有的组件都直接或间接地附着在这个根组件上。
	// 设置根组件对于确保 Actor 在游戏世界中正确表现其位置、旋转和缩放至关重要。
	// 代码中的 SetRootComponent(BaseMesh)
	// 在你的代码中，BaseMesh 是通过调用 CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh")
	// 创建的一个 UStaticMeshComponent 类型的实例。这个组件用于表示具有静态网格的3D对象。
	// 这里，BaseMesh 被设置为该 Actor 的根组件，意味着所有的空间变换（位置、旋转和缩放）将基于这个组件。
	// 如果有其他组件被添加到这个 Actor 中，它们会相对于 BaseMesh 进行定位。
	// 总结: SetRootComponent(BaseMesh); 这行代码的功能是将 BaseMesh（一个静态网格组件）设置为 Actor 的根组件。
	// 这是在 Unreal Engine 中进行 Actor 设置的标准做法，它确保了 Actor 的空间关系和层次结构的正确性。
	// 通过这种方式，你的 Actor 在 Unreal Engine 的世界中具有了一个物理和可视的表现基础。
	SetRootComponent(BaseMesh);
}

// Called when the game starts or when spawned
//------------------------------------------------------------------------------------------------------------------------------------------------------
void ABaseGeometryActor::BeginPlay()
{
	Super::BeginPlay();

	Initiallocation = GetActorLocation();

	// GetActorTransform();
	// PrintType();
	// PrintStringType();
	PrintTransform();
	SetColor(GeometryData.Color);

	// para 1: reference to the timer handle
	// para 2: pointer to the object on which we want to call the function every time the timer fires.
	// we will be calling the function of our actor, so we specify "this"
	// para 3: reference to a function that will be called every time the timer fires
	// we will calling OnTimerFired function
	// para 4: frequency of the timer in seconds
	// para 5: timer loop or not - bool
	// if it's set to false, then the timer would work once and stop
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ABaseGeometryActor::OnTimerFired, GeometryData.TimeRate, true);
}

// Called every frame
//------------------------------------------------------------------------------------------------------------------------------------------------------
void ABaseGeometryActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	HandleMovement();
}

//------------------------------------------------------------------------------------------------------------------------------------------------------
void ABaseGeometryActor::PrintType()
{
	// UE_LOG macro has multiple parameters:
	// 1. log classes
	// 2. log level: Display, Warning, Error
	// 3. output message string: simple string is not allowed

	UE_LOG(LogBaseGeometry, Warning, TEXT("Actor name: %s"), *GetName());
	UE_LOG(LogBaseGeometry, Warning, TEXT("WeaponsNum: %d, KillsNum: %i"), WeaponsNum, KillsNum);
	UE_LOG(LogBaseGeometry, Warning, TEXT("Health: %f"), Health);
	UE_LOG(LogBaseGeometry, Warning, TEXT("Health: %.2f"), Health);
	UE_LOG(LogBaseGeometry, Warning, TEXT("IsDead: %d"), IsDead);
	UE_LOG(LogBaseGeometry, Warning, TEXT("HasWeapon: %d"), static_cast<int>(HasWeapon));
}

//------------------------------------------------------------------------------------------------------------------------------------------------------
void ABaseGeometryActor::PrintStringType()
{
	FString Name = "John Connor";
	UE_LOG(LogBaseGeometry, Display, TEXT("Name: %s"), *Name);

	FString WeaponsNumStr = "weapons num = " + FString::FromInt(WeaponsNum);
	FString HealthStr = "Health = " + FString::SanitizeFloat(Health);
	FString IsDeadStr = "Is dead = " + FString(IsDead ? "true" : "false");

	FString State = FString::Printf(
		TEXT(" \n == All State == \n %s \n %s \n %s "), *WeaponsNumStr, *HealthStr, *IsDeadStr);

	UE_LOG(LogBaseGeometry, Warning, TEXT("%s"), *State);

	if (GEngine)
	{
		// The following function has serveral parameters:
		// 1. Unique message key. This argument allows messages with the same key not to be displayed repeatedly.
		// if you are not interested in this feature, you can set it to -1
		// 2. The time (seconds) the message will be kept on the screen.
		// 3. Display color.
		// 4. Message
		// 5. The order of output -- New lines at the top or at the bottom
		// 6. Scale -- We can slightly change the size of the message
		// Using the FVector2D type which stores two numbers: x, y. It can be used in any suitable cases.
		// Be it a vector, a coordinate in 2d space, or passing parameters of a scale
		GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Purple, Name);
		GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Green, State, true, FVector2D(1.5f, 1.5f));
	}
}

//------------------------------------------------------------------------------------------------------------------------------------------------------
void ABaseGeometryActor::PrintTransform()
{
	FTransform Transform = GetActorTransform();
	FVector Location = Transform.GetLocation();
	FRotator Rotator = Transform.Rotator();
	FVector Scale = Transform.GetScale3D();

	UE_LOG(LogBaseGeometry, Warning, TEXT("Actor name: %s"), *GetName());
	UE_LOG(LogBaseGeometry, Warning, TEXT("Transform: %s"), *Transform.ToString());
	UE_LOG(LogBaseGeometry, Warning, TEXT("Location: %s"), *Location.ToString());
	UE_LOG(LogBaseGeometry, Warning, TEXT("Rotator: %s"), *Rotator.ToString());
	UE_LOG(LogBaseGeometry, Warning, TEXT("Scale: %s"), *Scale.ToString());

	UE_LOG(LogBaseGeometry, Error, TEXT("Human transform: %s"), *Transform.ToHumanReadableString());
}

//------------------------------------------------------------------------------------------------------------------------------------------------------
void ABaseGeometryActor::HandleMovement()
{
	switch (GeometryData.MoveType)
	{
	case EMovementType::Sin:
		{
			FVector CurrentLocation = GetActorLocation();
			float Time = GetWorld()->GetTimeSeconds();
			CurrentLocation.Z = Initiallocation.Z + GeometryData.Amplitude * FMath::Sin(GeometryData.Frequency * Time);
			SetActorLocation(CurrentLocation);
		}
		break;

	case EMovementType::Static: break;
	default: break;
	}
}

void ABaseGeometryActor::SetColor(const FLinearColor& Color)
{
	if (!BaseMesh)
		return;

	UMaterialInstanceDynamic* DynMaterial = BaseMesh->CreateAndSetMaterialInstanceDynamic(0);
	if (DynMaterial)
	{
		DynMaterial->SetVectorParameterValue("Color", Color);
	}
}

void ABaseGeometryActor::OnTimerFired()
{
	if (++TimerCount <= MaxTimerCount)
	{
		const FLinearColor NewColor = FLinearColor::MakeRandomColor();
		UE_LOG(LogBaseGeometry, Display, TEXT("TimerCount: %i, Color to setup: %s"), TimerCount, *NewColor.ToString());
		SetColor(NewColor);
		// GetName's function is to get the name of the actor
		OnColorChanged.Broadcast(NewColor, GetName());
	}
	else
	{
		UE_LOG(LogBaseGeometry, Warning, TEXT("Timer has been stoped"));
		GetWorldTimerManager().ClearTimer(TimerHandle);
		OnTimerFinished.Broadcast(this);
	}
}
