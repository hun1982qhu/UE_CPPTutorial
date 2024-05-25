// Fill out your copyright notice in the Description page of Project Settings.

// Preprocessor directive that guarantees that during compilation this
// source file will be included strictly once.
#pragma once

// CoreMinimal.h is a header file that defines some basic types, constants, tools for logging.
#include "CoreMinimal.h"
// Inherited publicly from the actor, so we need to specify the header file where actor is declared.
#include "GameFramework/Actor.h"
#include "Components//StaticMeshComponent.h"
// BaseGeometryActor.generated.h is the auto-generated header.
// The engine generates it for each actor
// there is meta information about the class and it must go the very last in the list of all includes
// if we want to add some additional include, then we must include it before.
#include "BaseGeometryActor.generated.h"

// F：用于结构体（struct）。例如，FVector、FRotator、FLinearColor。
// A：用于表示Actor类。例如，AActor、AMyActor。
// U：用于表示UObject类或其子类。例如，UObject、UStaticMesh、UTexture。
// I：用于表示接口类。例如，IInterface。
// E：用于表示枚举类型。例如，EColor、EGameState。

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnColorChanged, const FLinearColor&, Color, const FString&, Name);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTimerFinished, AActor*);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UENUM(BlueprintType)
enum class EMovementType: uint8
{
	Sin,
	Static
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
USTRUCT(BlueprintType)
struct FGeometryData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float Amplitude = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float Frequency = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	EMovementType MoveType = EMovementType::Static;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design")
	FLinearColor Color = FLinearColor::Black;

	UPROPERTY(EditAnywhere, Category="Design")
	float TimeRate = 3.0f;
};

// class：这是 C++ 中用来声明一个类的关键字。类是创建对象的蓝图，它提供了状态（成员变量或属性）的初始值和行为（成员函数或方法）的实现。
// PROGRAMMINGPRACTICE_API：这是一个宏，通常用于 Unreal Engine 和其他大型 C++ 项目中。这个宏用于处理类和函数的导出和导入，确保在不同的模块或动态链接库（DLL）之间可以正确地共享和使用这些类和函数。在实际项目中，这个宏会根据不同的编译环境（比如是在编译库本身还是在使用库）被定义为不同的内容，如 __declspec(dllexport) 或 __declspec(dllimport)。
// ABaseGeometryActor：这是类的名称，它是你定义的一个新类。
// public AActor：这表示 ABaseGeometryActor 类是从 Unreal Engine 的 AActor 类继承而来的。AActor 是 Unreal Engine 中所有演员（游戏世界中的对象）的基类，提供了许多基本的功能和接口，例如在游戏世界中的位置、旋转等。使用 public 继承意味着基类 AActor 的公共和保护成员都可以在 ABaseGeometryActor 类中被访问。
// 总的来说，这行代码定义了一个新的类 ABaseGeometryActor，它通过继承 Unreal Engine 的 AActor 类来扩展或修改某些功能，并且使用 PROGRAMMINGPRACTICE_API 宏来确保这个类可以在不同的模块之间正确地被导出和导入。这样的设计使得 ABaseGeometryActor 可以作为游戏中的一个演员，拥有 AActor 的所有基本行为，并且可以根据需要添加新的功能或修改现有行为。
// The first macro

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UCLASS()
class CPP_TUTORIAL_API ABaseGeometryActor : public AActor
{
	// The second macro
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseGeometryActor();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	void SetGeometryData(const FGeometryData& Data)
	{
		GeometryData = Data;
	}

	// can be called in blueprint
	UFUNCTION(BlueprintCallable)
	FGeometryData GetGeometryData() const { return GeometryData; }
	
	// UPROPERTY(BlueprintAssignable) 用于将一个委托（Delegate）暴露给蓝图，使得蓝图可以绑定该委托。
	// 通常用于事件系统，当某个事件发生时，蓝图可以响应这个事件。
	UPROPERTY(BlueprintAssignable)
	FOnColorChanged OnColorChanged;

	FOnTimerFinished OnTimerFinished;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// BlueprintReadWrite specifier allows you to access the property on the blueprint graph
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GeometryData")
	FGeometryData GeometryData;

	// UPPROPERTY has following parameters:
	UPROPERTY(EditAnywhere, Category="Weapon")
	int32 WeaponsNum = 4;

	UPROPERTY(EditDefaultsOnly, Category="Stat")
	int32 KillsNum = 7;

	UPROPERTY(EditInstanceOnly, Category="Health")
	float Health = 34.435235f;

	UPROPERTY(EditAnywhere, Category="Health")
	bool IsDead = false;

	UPROPERTY(VisibleAnywhere, Category="Weapon")
	bool HasWeapon = true;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FVector Initiallocation;
	FTimerHandle TimerHandle;

	const int32 MaxTimerCount = 5;
	int32 TimerCount = 0;

	void PrintType();
	void PrintStringType();
	void PrintTransform();
	void HandleMovement();
	void SetColor(const FLinearColor& Color);
	void OnTimerFired();
};
