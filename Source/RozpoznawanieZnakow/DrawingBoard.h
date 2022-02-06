// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Misc/Paths.h"
#include "fstream"
#include <vector>
#include "./Point.h"
#include "Runtime/Online/HTTP/Public/HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "DrawingBoard.generated.h"

UCLASS()
class ROZPOZNAWANIEZNAKOW_API ADrawingBoard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADrawingBoard();
	
	FString SaveDirectory;
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

	/*UFUNCTION(BlueprintCallable)
	void SetSubjectNumber(FString subjectNumber);*/

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	std::vector<std::vector<Point>> currentContour;
	std::vector<Point> currentStroke;

	FString detectChar(std::vector<std::vector<Point>> vec);
	void detectCharCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	TSharedPtr<FJsonObject> createNewJsonPoint(Point p);
};
