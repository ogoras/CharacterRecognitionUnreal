// Fill out your copyright notice in the Description page of Project Settings.

#include "DrawingBoard.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"
#include "fstream"
#include <iomanip>
#include <direct.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

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
	//send data from this->currentContour to web service

	this->currentContour.clear();

	return "test";
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