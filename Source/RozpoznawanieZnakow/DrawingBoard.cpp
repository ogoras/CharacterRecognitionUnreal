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
	//this->SaveDirectory = FPaths::ProjectSavedDir() + "Contours\\session" + FDateTime::Now().ToString();
	_mkdir(TCHAR_TO_UTF8(*(FPaths::ProjectSavedDir() + "Contours")));
	_mkdir(TCHAR_TO_UTF8(*(FPaths::ProjectSavedDir() + "Contours\\data")));
	this->SaveDirectory = FPaths::ProjectSavedDir() + "Contours\\data\\subTEST";
	_mkdir(TCHAR_TO_UTF8(*(this->SaveDirectory)));
	this->contourCount = 0;
	this->contourCounts = new int[characterCount]();
}

void ADrawingBoard::BeginContour()
{
	//if (this->contourCount == 0)
	//	_mkdir(TCHAR_TO_UTF8(*(this->SaveDirectory)));
	if (this->contourCounts[this->nextNumber] == 0)
		_mkdir(TCHAR_TO_UTF8(*(this->SaveDirectory + "\\" + FString::FromInt(this->nextNumber))));
	this->ContourFilename = this->SaveDirectory + "\\" + FString::FromInt(this->nextNumber) + "\\contour" + FDateTime::Now().ToString() + ".txt";
	if (contourCount != 0)
		this->contourFile.open(std::string(TCHAR_TO_UTF8(*(this->ContourFilename))));
	this->contourFile << std::setprecision(10);
}

void ADrawingBoard::AddDot(FVector2D offset)
{
	this->contourFile << offset.X << " " << offset.Y << "\n";
}

FString ADrawingBoard::FinishContour()
{
	this->contourFile.close();
	this->contourCount++;

	this->contourCounts[this->nextNumber]++;
	srand(time(NULL));
	int base_int = this->nextNumber = rand() % characterCount;
	FString return_string;
	if (base_int < 10)
		return return_string.AppendChar(base_int + '0');
	else if (base_int < 36)
		return return_string.AppendChar(base_int - 10 + 'A');
	else if (base_int < 45)
		return return_string.AppendChar(polishLetters[base_int - 36]);
	else if (base_int < 71)
		return return_string.AppendChar(base_int - 45 + 'a');
	else if (base_int < 80)
		return return_string.AppendChar(polishMinisculeLetters[base_int - 71]);
	else if (base_int < 95)
		return return_string.AppendChar(base_int - 80 + '!');
	else if (base_int < 102)
		return return_string.AppendChar(base_int - 95 + ':');
	else if (base_int < 108)
		return return_string.AppendChar(base_int - 102 + '[');
	else if (base_int < 112)
		return return_string.AppendChar(base_int - 108 + '{');
	else if (base_int == 112)
		return "spacja";
	else if (base_int == 113)
		return "tabulator";
	else
		return "nowa linia (ENTER)";
}

void ADrawingBoard::EndStroke()
{
	this->contourFile << "\n";
}

void ADrawingBoard::DiscardContour()
{
	this->contourFile.close();
	remove(TCHAR_TO_UTF8(*this->ContourFilename));
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

void ADrawingBoard::SetSubjectNumber(FString subjectNumber)
{
	this->SaveDirectory = FPaths::ProjectSavedDir() + "Contours\\data\\sub" + subjectNumber;
	_mkdir(TCHAR_TO_UTF8(*(this->SaveDirectory)));
}