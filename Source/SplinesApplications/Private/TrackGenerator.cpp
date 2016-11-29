// Fill out your copyright notice in the Description page of Project Settings.

#include "SplinesApplications.h"
#include "TrackGenerator.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"
#include "Runtime/Engine/Classes/Components/SplineMeshComponent.h"
// Sets default values
ATrackGenerator::ATrackGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	RootComponent = Spline;
}

void ATrackGenerator::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	bRefresh = false;
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "OnConstruction");

	// Store the number of spline points
	SplinePointsNum = Spline->GetNumberOfSplinePoints();

	// Build RoadData array
	if (SplinePointsNum != RoadDataArray.Num())
	{
		RoadDataArray.SetNum(SplinePointsNum, true);
	}

	int NoOfLoops = bLoopingTrack? SplinePointsNum : SplinePointsNum - 1;

	for (int i = 0; i < NoOfLoops; i++)
	{
		// Construct the actual track


		BuildTrackElement(RoadMesh, i);

		if (RoadDataArray[i].bRightGuardRail)
		{
			BuildTrackElement(RightGuardRailMesh, i);
		}

		if (RoadDataArray[i].bLeftGuardRail)
		{
			BuildTrackElement(LeftGuardRailMesh, i);
		}

		// Draw spline point numbers
		if (bDrawTrackPointNumbers)
		{
			AddTextRenderComponent(i);
		}

	}


	RegisterAllComponents();
}

void ATrackGenerator::BuildTrackElement(UStaticMesh* Mesh, int32 Index)
{
	if (!Mesh)
	{
		return;
	}

	const int32 NextIndex = (Index + 1) % SplinePointsNum;

	// Store relevant data for each track element
	FVector StartLocation;
	FVector StartTangent;
	FVector EndLocation;
	FVector EndTangent;

	Spline->GetLocationAndTangentAtSplinePoint(Index, StartLocation, StartTangent, ESplineCoordinateSpace::World);
	Spline->GetLocationAndTangentAtSplinePoint(NextIndex, EndLocation, EndTangent, ESplineCoordinateSpace::World);

	// Use stored data to build track element
	USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this);
	SplineMesh->CreationMethod = EComponentCreationMethod::SimpleConstructionScript;
	SplineMesh->bSmoothInterpRollScale = true;
	SplineMesh->AttachTo(RootComponent);
	SplineMesh->SetStaticMesh(Mesh);
	SplineMesh->SetStartAndEnd(StartLocation, StartTangent, EndLocation, EndTangent);

	// Use the road data array
	FVector2D StartScale(RoadDataArray[Index].TrackWidth, RoadDataArray[Index].TrackThickness);
	float StartRoll = RoadDataArray[Index].TrackBlank;
	FVector2D EndScale(RoadDataArray[NextIndex].TrackWidth, RoadDataArray[NextIndex].TrackThickness);
	float EndRoll = RoadDataArray[NextIndex].TrackBlank;

	SplineMesh->SetStartRoll(StartRoll);
	SplineMesh->SetEndRoll(EndRoll);
	SplineMesh->SetStartScale(StartScale);
	SplineMesh->SetEndScale(EndScale);

	SplineMesh->SetCollisionProfileName("BlockAll");
	SplineMesh->SetCollisionEnabled(bEnableCollision ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);

	FinishAndRegisterComponent(SplineMesh);
}

void ATrackGenerator::AddTextRenderComponent(int32 Index)
{

	FVector Location;
	FVector Tangent;
	Spline->GetLocalLocationAndTangentAtSplinePoint(Index, Location, Tangent);
	Location.Z += 50;


	UTextRenderComponent* Text = NewObject<UTextRenderComponent>(this);

	Text->CreationMethod = EComponentCreationMethod::SimpleConstructionScript;
	Text->AttachTo(Spline);
	Text->RelativeLocation = Location;
	Text->RelativeRotation = (-Tangent).Rotation();
	Text->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	Text->WorldSize = 300;
	Text->SetText(FString::FromInt(Index));

	FinishAndRegisterComponent(Text);
}
