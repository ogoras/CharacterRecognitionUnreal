// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Misc/Paths.h"
#include "fstream"
#include "DrawingBoard.generated.h"

UCLASS()
class ROZPOZNAWANIEZNAKOW_API ADrawingBoard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADrawingBoard();
	
	std::ofstream contourFile;
	FString SaveDirectory;
	FString ContourFilename;
	//UPROPERTY(VisibleAnywhere)
	//UStaticMeshComponent* VisualMesh;

	UFUNCTION(BlueprintCallable)
	void BeginContour();

	UFUNCTION(BlueprintCallable)
	void AddDot(FVector2D Offset);

	UFUNCTION(BlueprintCallable)
	FString FinishContour();

	UFUNCTION(BlueprintCallable)
	void EndStroke();

	UFUNCTION(BlueprintCallable)
	void DiscardContour();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	int contourCount;
	int nextNumber;
	int* contourCounts;
};
