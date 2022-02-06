// Fill out your copyright notice in the Description page of Project Settings.

#include "DrawingBoard.h"
#include "Misc/DateTime.h"
#include <iomanip>
#include <direct.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

const int characterCount = 115;
const TCHAR polishLetters[] = {
	L'\u0104', L'\u0106', L'\u0118', L'\u0141', L'\u0143', L'\u00d3', L'\u015a', L'\u0179', L'\u017b'
};

const TCHAR polishMinisculeLetters[] = {
	L'\u0105', L'\u0107', L'\u0119', L'\u0142', L'\u0144', L'\u00f3', L'\u015b', L'\u017a', L'\u017c'
};

// Sets default values
ADrawingBoard::ADrawingBoard()
{
	this->SaveDirectory = FPaths::ProjectSavedDir() + "RecognitionData";
	this->currentStroke = *(new std::vector<Point>);
}

void ADrawingBoard::BeginContour()
{
	
}

void ADrawingBoard::AddDot(FVector2D offset)
{
	this->currentStroke.push_back(*(new Point(offset.X, offset.Y)));
}

FString ADrawingBoard::FinishContour()
{
	EndStroke();
	FString detectedChar = detectChar(this->currentContour);

	this->currentContour.clear();

	return detectedChar;
}

void ADrawingBoard::EndStroke()
{
	this->currentContour.push_back(this->currentStroke);
	this->currentStroke = *(new std::vector<Point>);
}

void ADrawingBoard::DiscardContour()
{
	this->currentContour.push_back(this->currentStroke);
	this->currentContour.clear();
	this->currentStroke = *(new std::vector<Point>);
}

// Called when the game starts or when spawned
void ADrawingBoard::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADrawingBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FString ADrawingBoard::detectChar(std::vector<std::vector<Point>> vec)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	TArray<TSharedPtr<FJsonValue>> arr;
	for (std::vector<Point> stroke : vec) {
		TArray<TSharedPtr<FJsonValue>> subarr;
		for (Point p : stroke) {
			TSharedPtr<FJsonObject> point = createNewJsonPoint(p);
			subarr.Add(MakeShareable(new FJsonValueObject(point)));
		}
		arr.Add(MakeShareable(new FJsonValueArray(subarr)));
	}
	JsonObject->SetArrayField(TEXT("data"), arr);

	FString OutputString;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("POST");
	HttpRequest->SetHeader("Content-Type", "application/json");
	HttpRequest->SetURL(TEXT("localhost:8000"));
	HttpRequest->SetContentAsString(OutputString);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &ADrawingBoard::detectCharCompleted);
	HttpRequest->ProcessRequest();

	return "test";
}

void ADrawingBoard::detectCharCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		FString s = Response->GetContentAsString();
		CharacterRecognized(s);
		//SomeOtherVariable = JsonObject->GetStringField("some_response_field");
	}
	else
	{
		// Handle error here
	}
}

TSharedPtr<FJsonObject> ADrawingBoard::createNewJsonPoint(Point p)
{
	TSharedPtr<FJsonObject> obj = MakeShareable(new FJsonObject());
	obj->SetNumberField(TEXT("x"), p.x);
	obj->SetNumberField(TEXT("y"), p.y);
	return obj;
}