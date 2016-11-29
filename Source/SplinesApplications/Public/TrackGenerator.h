// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Actor.h"
#include "TrackGenerator.generated.h"

USTRUCT(BlueprintType)
struct FRoadData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	bool bLeftGuardRail;
	UPROPERTY(EditAnywhere)
	bool bRightGuardRail;
	UPROPERTY(EditAnywhere)
	float TrackBlank;
	UPROPERTY(EditAnywhere)
	float TrackWidth;
	UPROPERTY(EditAnywhere)
	float TrackThickness;

	FRoadData()
	{
		bLeftGuardRail = true;
		bRightGuardRail = true;
		TrackWidth = 1;
		TrackThickness = 1;
		TrackBlank = 0;
	}
};

UCLASS()
class SPLINESAPPLICATIONS_API ATrackGenerator : public AActor
{
	GENERATED_BODY()
private:
	// Sets default values for this actor's properties
	ATrackGenerator();

	virtual void OnConstruction(const FTransform& Transform) override;
		
	UPROPERTY(VisibleDefaultsOnly, Category = Road, meta = (AllowPrivateAccess = "true"))
	class USplineComponent* Spline;

	UPROPERTY(EditAnywhere, Category = "TrackMeshes")
	UStaticMesh* RoadMesh;

	UPROPERTY(EditAnywhere, Category = "TrackMeshes")
	UStaticMesh* RightGuardRailMesh;

	UPROPERTY(EditAnywhere, Category = "TrackMeshes")
	UStaticMesh* LeftGuardRailMesh;


	void BuildTrackElement(UStaticMesh* Mesh, int32 Index);
	void AddTextRenderComponent(int32 Index);

	int32 SplinePointsNum;

	UPROPERTY(EditAnywhere, Category = "Road")
	bool bRefresh;

	UPROPERTY(EditAnywhere, Category = "Road")
	bool bEnableCollision;

	UPROPERTY(EditAnywhere, Category = "Road")
	bool bDrawTrackPointNumbers;

	UPROPERTY(EditAnywhere, Category = "Road")
	bool bLoopingTrack;

	UPROPERTY(EditAnywhere, Category = "Road")
	TArray<FRoadData> RoadDataArray;
};
