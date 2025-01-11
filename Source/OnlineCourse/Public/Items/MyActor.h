// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

class USphereComponent;

UCLASS()
class ONLINECOURSE_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:
	AMyActor();
	
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Oscillation")
	float Amplitude = 10.;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Oscillation")
	float Period = 1.;

	UFUNCTION(BlueprintPure, Category = "Oscillation")
	float CustomSin(float Value) const;
	
	UFUNCTION(BlueprintPure, Category = "Oscillation")
	float CustomCos(float Value) const;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Oscillation")
	float RunningTime;

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ActorMesh;

	UPROPERTY(visibleAnywhere)
	USphereComponent* SphereComponent;
};
