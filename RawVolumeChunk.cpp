// Fill out your copyright notice in the Description page of Project Settings.

#include "PolyVox_Test.h"
#include "SimplexNoiseBPLibrary.h"
#include "PolyVox/CubicSurfaceExtractor.h"
#include "PolyVox/MarchingCubesSurfaceExtractor.h"
#include "PolyVox/Mesh.h"
#include "PolyVox/Vector.h"
#include "RawVolumeChunk.h"



void ARawVolumeChunk::createSphereInVolume(PolyVox::RawVolume<uint8_t> &volData, float fRadius)
{
    FVector worldLoc = GetActorLocation();
    
    //This vector hold the position of the center of the volume
    PolyVox::Vector3DFloat v3dVolCenter(volData.getWidth() / 2 + worldLoc.X, volData.getHeight() / 2 + worldLoc.Y, volData.getDepth() / 2 + worldLoc.Z);
    
    //This three-level for loop iterates over every voxel in the volume
    for (int z = 0; z < volData.getDepth(); z++)
    {
        for (int y = 0; y < volData.getHeight(); y++)
        {
            for (int x = 0; x < volData.getWidth(); x++)
            {
                //Store our current position as a vector...
                // maybe worldloc later
                PolyVox::Vector3DFloat v3dCurrentPos(x, y, z);
                //And compute how far the current position is from the center of the volume
                float fDistToCenter = (v3dCurrentPos - v3dVolCenter).length();
                
                uint8_t uVoxelValue = 255;
                
                //If the current voxel is less than 'radius' units from the center then we make it solid.
                if (fDistToCenter <= fRadius)
                {
                //Our new voxel value
                  uVoxelValue = 255;
                }
                
                //Wrte the voxel value into the volume
                volData.setVoxel(x, y, z, uVoxelValue);
            }
        }
    }
}


void ARawVolumeChunk::createBlockInVolume(PolyVox::RawVolume<uint8_t>& volData, float fRadius)
{
    
     FVector worldLoc = GetActorLocation();
    
    //This vector hold the position of the center of the volume
    PolyVox::Vector3DFloat v3dVolCenter(volData.getWidth() / 2 + worldLoc.X, volData.getHeight() / 2 + worldLoc.Y, volData.getDepth() / 2 + worldLoc.Z);
    
    //This three-level for loop iterates over every voxel in the volume
    for (int z = 0; z < static_cast<uint8>(fRadius); z++)
    {
        for (int y = 0; y < static_cast<uint8>(fRadius); y++)
        {
            for (int x = 0; x < static_cast<uint8>(fRadius); x++)
            {
                //Store our current position as a vector...
                // maybe worldloc later
                PolyVox::Vector3DFloat v3dCurrentPos(x, y, z);
                //And compute how far the current position is from the center of the volume
                float fDistToCenter = (v3dCurrentPos - v3dVolCenter).length();
                
                uint8_t uVoxelValue = 255;
                
                //If the current voxel is less than 'radius' units from the center then we make it solid.
                //if (fDistToCenter <= fRadius)
                //{
                    //Our new voxel value
                  //  uVoxelValue = 255;
                //}
                
                //Wrte the voxel value into the volume	
                volData.setVoxel(x, y, z, uVoxelValue);
            }
        }
    }
}


// Sets default values
ARawVolumeChunk::ARawVolumeChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    
    Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("chunk sphere"));
}


void ARawVolumeChunk::ModifyChunk(FVector WorldLoc)
{
    PolyVox::Region RegionToEdit(PolyVox::Region(PolyVox::Vector3DInt32(0, 0, 0), PolyVox::Vector3DInt32(20, 20, 20)));
    PolyVox::RawVolume<uint8_t> volDataEdit(RegionToEdit);
    /*for(int x = 10; x < 20; x++){
        for(int y = 10; y < 20; y++){
            for(int z = 10; z < 20; z++){
                uint8_t uVoxelValue = 255;
                volData.Get()->setVoxel(x, y, z, uVoxelValue);
                
            }
        }
    }*/
    //FVector chunkLocation = GetActorLocation();
    
    //FVector Loc = FTransform(chunkLocation).InverseTransformPosition(WorldLoc);
    
    FVector Loc = WorldLoc / 50.0f;
    
    float x = std::abs(Loc.X);
    float y = std::abs(Loc.Y);
    float z = std::abs(Loc.Z);
    
    Loc.X = x;
    Loc.Y = y;
    Loc.Z = z;
    
    UE_LOG(LogTemp, Error, TEXT("Location: %s"), *(Loc.ToString()))
    UE_LOG(LogTemp, Error, TEXT("X: %x"), x)
    
    volData.Get()->setVoxel(x, y, z, 0);
    
    auto mesh = extractCubicMesh(volData.Get(), volData.Get()->getEnclosingRegion());
    auto decodedMesh = decodeMesh(mesh);
    int Index = decodedMesh.getNoOfIndices();
    
    UE_LOG(LogTemp, Error, TEXT("triangle num : %i"), Index)
    
    auto Vertices = TArray<FVector>();
    auto Indices = TArray<int32>();
    auto Normals = TArray<FVector>();
    auto UV0 = TArray<FVector2D>();
    auto Colors = TArray<FColor>();
    auto Tangents = TArray<FProcMeshTangent>();
    
    
    if(Index >= 2){
        for (uint32 i = 0; i < Index - 2; i+=3)
        {
            auto Index = decodedMesh.getIndex(i + 2);
            auto Vertex2 = decodedMesh.getVertex(Index);
            Indices.Add(Vertices.Add(FPolyVoxVector(Vertex2.position) * 100.f));
            Index = decodedMesh.getIndex(i + 1);
            auto Vertex1 = decodedMesh.getVertex(Index);
            Indices.Add(Vertices.Add(FPolyVoxVector(Vertex1.position) * 100.f));
            Index = decodedMesh.getIndex(i);
            auto Vertex0 = decodedMesh.getVertex(Index);
            Indices.Add(Vertices.Add(FPolyVoxVector(Vertex0.position) * 100.f));
            const FVector Edge01 = FPolyVoxVector(Vertex1.position - Vertex0.position);
            const FVector Edge02 = FPolyVoxVector(Vertex2.position - Vertex0.position);
            const FVector TangentX = Edge01.GetSafeNormal();
            FVector TangentZ = (Edge01 ^ Edge02).GetSafeNormal();
            for (int32 j = 0; j < 3; j++)
            {
                Tangents.Add(FProcMeshTangent(TangentX, false));
                Normals.Add(TangentZ);
            }
        }
    }
    Mesh->ClearMeshSection(0);
    Mesh->CreateMeshSection(0, Vertices, Indices, Normals, UV0, Colors, Tangents, true);
}


// Called when the game starts or when spawned
void ARawVolumeChunk::BeginPlay()
{
	Super::BeginPlay();
    
    FVector Location = GetActorLocation();
    float x = Location.X;
    float y = Location.Y;
    float z = Location.Z;
    
    TSharedPtr<PolyVox::RawVolume<uint8_t>>RawVolTest (new PolyVox::RawVolume<uint8_t>(PolyVox::Region(PolyVox::Vector3DInt32(0, 0, 0), PolyVox::Vector3DInt32(63, 63, 63))));
    
    volData = RawVolTest;
   // PolyVox::RawVolume<uint8_t> volData(PolyVox::Region(PolyVox::Vector3DInt32(0, 0, 0), PolyVox::Vector3DInt32(63, 63, 63)));
    //volData = new PolyVox::RawVolume<uint8_t>(PolyVox::Region(PolyVox::Vector3DInt32(0, 0, 0), PolyVox::Vector3DInt32(63, 63, 63)));
    
    //VolumePtr = &volData;
    //PolyVox::Region(PolyVox::Vector3DInt32(0, 0, 0), PolyVox::Vector3DInt32(63, 63, 63))
    
    
    createSphereInVolume(RawVolTest.ToSharedRef().Get(), 30);
    auto mesh = extractCubicMesh(RawVolTest.Get(), RawVolTest.Get()->getEnclosingRegion());
    auto decodedMesh = decodeMesh(mesh);
    int Index = decodedMesh.getNoOfIndices();
    
    auto Vertices = TArray<FVector>();
    auto Indices = TArray<int32>();
    auto Normals = TArray<FVector>();
    auto UV0 = TArray<FVector2D>();
    auto Colors = TArray<FColor>();
    auto Tangents = TArray<FProcMeshTangent>();
    
    
    if(Index >= 2){
        for (uint32 i = 0; i < Index - 2; i+=3)
        {
            auto Index = decodedMesh.getIndex(i + 2);
            auto Vertex2 = decodedMesh.getVertex(Index);
            Indices.Add(Vertices.Add(FPolyVoxVector(Vertex2.position) * 100.f));
            Index = decodedMesh.getIndex(i + 1);
            auto Vertex1 = decodedMesh.getVertex(Index);
            Indices.Add(Vertices.Add(FPolyVoxVector(Vertex1.position) * 100.f));
            Index = decodedMesh.getIndex(i);
            auto Vertex0 = decodedMesh.getVertex(Index);
            Indices.Add(Vertices.Add(FPolyVoxVector(Vertex0.position) * 100.f));
            const FVector Edge01 = FPolyVoxVector(Vertex1.position - Vertex0.position);
            const FVector Edge02 = FPolyVoxVector(Vertex2.position - Vertex0.position);
            const FVector TangentX = Edge01.GetSafeNormal();
            FVector TangentZ = (Edge01 ^ Edge02).GetSafeNormal();
            for (int32 j = 0; j < 3; j++)
            {
                Tangents.Add(FProcMeshTangent(TangentX, false));
                Normals.Add(TangentZ);
            }
        }
    }
    
    // Finally create the mesh
    Mesh->CreateMeshSection(0, Vertices, Indices, Normals, UV0, Colors, Tangents, true);
}

// Called every frame
void ARawVolumeChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

