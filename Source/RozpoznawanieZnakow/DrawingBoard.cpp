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

FString ADrawingBoard::detectChar(std::vector<std::vector<Point>>)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	FString msg = "test2";
	JsonObject->SetStringField(TEXT("some_string_field"), *FString::Printf(TEXT("%s"), *msg));

	TArray<TSharedPtr<FJsonValue>> arr1;
	TSharedPtr<FJsonObject> point11 = createNewJsonPoint(*(new Point(0.11, 0.22)));
	arr1.Add(MakeShareable(new FJsonValueObject(point11)));
	TSharedPtr<FJsonObject> point12 = createNewJsonPoint(*(new Point(0.11, 0.11)));
	arr1.Add(MakeShareable(new FJsonValueObject(point12)));

	TArray<TSharedPtr<FJsonValue>> arr2;
	TSharedPtr<FJsonObject> point21 = createNewJsonPoint(*(new Point(0.01, 0.02)));
	arr2.Add(MakeShareable(new FJsonValueObject(point21)));
	TSharedPtr<FJsonObject> point22 = createNewJsonPoint(*(new Point(0.22, 0.22)));
	arr2.Add(MakeShareable(new FJsonValueObject(point22)));

	TArray<TSharedPtr<FJsonValue>> arr;
	arr.Add(MakeShareable(new FJsonValueArray(arr1)));
	arr.Add(MakeShareable(new FJsonValueArray(arr2)));
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
	if (bWasSuccessful && Response->GetContentType() == "application/json")
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(Response->GetContentAsString());
		FJsonSerializer::Deserialize(JsonReader, JsonObject);
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