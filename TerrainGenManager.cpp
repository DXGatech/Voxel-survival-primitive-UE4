// Fill out your copyright notice in the Description page of Project Settings.

#include "PolyVox_Test.h"
#include "SimplexNoiseBPLibrary.h"
//#include "Engine.h"
#include "PolyVox/CubicSurfaceExtractor.h"
#include "PolyVox/MarchingCubesSurfaceExtractor.h"
#include "PolyVox/Mesh.h"
#include "PolyVox/Vector.h"
#include "PolyVox/BaseVolume.h"
#include "Header.h"
//#include "PolyVox/PagedVolume.h"
//#include "PolyVox/MaterialDensityPair.h"
#include "TerrainGenManager.h"


// TODO: create a location array that contains mined blocks for each ATerrainGenManagerClass
// set the voxel density in a specific location to zero and re-render the chunk when a single element is modified

// Sets default values
ATerrainGenManager::ATerrainGenManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    
    Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Terrain generation"));
    Foliage = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Foliage"));

}

void ATerrainGenManager::EditVoxelAtLoc(FVector Loc)
{
    float locx = Loc.X;
    float locy = Loc.Y;
    float locz = Loc.Z;
    
    PolyVox::MaterialDensityPair44 Air;
    Air.setMaterial(0);
    Air.setDensity(0);
    UE_LOG(LogTemp, Error, TEXT("chunk fullfilled"))
    
    FVector UpdateLoc = GetActorLocation();
    //float upx = UpdateLoc.X / 200.0f;
    //float upy = UpdateLoc.Y / 200.0f;
    float upx = genlocx;
    float upy = genlocx;
    float upz = -50.0f;
    
    PolyVox::Region ToExtract(PolyVox::Vector3DInt32(0, 0, 0), PolyVox::Vector3DInt32(sizevec_x, sizevec_y, sizevec_z + upz));
    PolyVox::PagedVolume<PolyVox::MaterialDensityPair44>* VoxChunk = VoxelVolume.Get();
    //ToExtract.getLowerX() - 5
    
    for (int x = 0; x <= 30; x++)
    {
        for (int y = 0; y <= 30; y++)
        {
            for (int z = 0; z <= 30; z++) {
                VoxChunk->setVoxel(x, y, z, Air);
            }
        }
    }

    auto Vertices = TArray<FVector>();
    auto Indices = TArray<int32>();
    auto Normals = TArray<FVector>();
    auto UV0 = TArray<FVector2D>();
    auto Colors = TArray<FColor>();
    auto Tangents = TArray<FProcMeshTangent>();
    
    auto ExtractedMesh = PolyVox::extractCubicMesh(VoxelVolume.Get(), ToExtract);
    //auto ExtractedMesh = PolyVox::extractMarchingCubesMesh(VoxelVolume.Get(), ToExtract);
    auto DecodedMesh = decodeMesh(ExtractedMesh);
    auto Index = DecodedMesh.getNoOfIndices();
    UE_LOG(LogTemp, Error, TEXT("number of indices after edit: %d"), Index)
    
    if(Index >= 2){
        for (uint32 i = 0; i < Index - 2; i+=3)
        {
            auto Index = DecodedMesh.getIndex(i + 2);
            auto Vertex2 = DecodedMesh.getVertex(Index);
            Indices.Add(Vertices.Add(FPolyVoxVector(Vertex2.position) * 100.f));
            
            Index = DecodedMesh.getIndex(i + 1);
            auto Vertex1 = DecodedMesh.getVertex(Index);
            Indices.Add(Vertices.Add(FPolyVoxVector(Vertex1.position) * 100.f));
            
            Index = DecodedMesh.getIndex(i);
            auto Vertex0 = DecodedMesh.getVertex(Index);
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
    Mesh->CreateMeshSection(0, Vertices, Indices,  Normals, UV0, Colors, Tangents, true);
}

// Called when the game starts or when spawned
void ATerrainGenManager::BeginPlay()
{
    Super::BeginPlay();
    
	
}

// Called every frame
void ATerrainGenManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

VoxelTerrainPager::VoxelTerrainPager(int NoiseSeed, int Octaves, float Frequency, float Scale, float Offset, float Height, float x, float y, float z, float inamp, float infreq, float inwaterlevel) : PolyVox::PagedVolume<PolyVox::MaterialDensityPair44>::Pager(), Seed(NoiseSeed), NoiseOctaves(Octaves), NoiseFrequency(Frequency), NoiseScale(Scale), NoiseOffset(Offset), TerrainHeight(Height)
{
    posx = x;
    posy = y;
    posz = z;
    
    amp = inamp;
    freq = infreq;
    
    waterlevel = inwaterlevel;
    
    //DeleteVoxel.SetDensity(0);
    //DeleteVoxel.SetMaterial(0);
}


void ATerrainGenManager::SetGenLoc(float x, float y, float z)
{
    
    genlocx = x;
    genlocy = y;
    
    
    PolyVox::Region ToExtract(PolyVox::Vector3DInt32(x, y, z), PolyVox::Vector3DInt32(x + sizevec_x, y + sizevec_y, z + sizevec_z));
    TSharedPtr<PolyVox::PagedVolume<PolyVox::MaterialDensityPair44>>PolyVoxelVolume = MakeShareable(new PolyVox::PagedVolume<PolyVox::MaterialDensityPair44>(new VoxelTerrainPager(0, 0, 0, 0, 0, 0, x, y, z, amplitude, NoiseFrequency, waterlevel)));
    VoxelVolume = PolyVoxelVolume;
    //auto ExtractedMesh = PolyVox::extractCubicMesh(PolyVoxelVolume.Get(), ToExtract);
    auto ExtractedMesh = PolyVox::extractMarchingCubesMesh(PolyVoxelVolume.Get(), ToExtract);
    auto DecodedMesh = decodeMesh(ExtractedMesh);
    
    
    for(int Material = 0; Material < TerrainMaterial.Num(); Material++){
    
        auto Vertices = TArray<FVector>();
        auto Indices = TArray<int32>();
        auto Normals = TArray<FVector>();
        auto UV0 = TArray<FVector2D>();
        auto Colors = TArray<FColor>();
        auto Tangents = TArray<FProcMeshTangent>();
    

        auto Index = DecodedMesh.getNoOfIndices();
        UE_LOG(LogTemp, Error, TEXT("index num: %i"), Index)
        if(Index >= 2){
            for (uint32 i = 0; i < Index - 2; i+=3)
            {
                // We need to add the vertices of each triangle in reverse or the mesh will be upside down
                auto Index = DecodedMesh.getIndex(i + 2);
                auto Vertex2 = DecodedMesh.getVertex(Index);
                auto TriangleMaterial = Vertex2.data.getMaterial();
                
                // Before we continue, we need to be sure that the triangle is the right material; we don't want to use verticies from other materials
                if (TriangleMaterial == (Material + 1))
                {
                    // If it is of the same material, then we need to add the correct indices now
                    Indices.Add(Vertices.Add(FPolyVoxVector(Vertex2.position) * 100.f));
                    
                    Index = DecodedMesh.getIndex(i + 1);
                    auto Vertex1 = DecodedMesh.getVertex(Index);
                    Indices.Add(Vertices.Add(FPolyVoxVector(Vertex1.position) * 100.f));
                    
                    Index = DecodedMesh.getIndex(i);
                    auto Vertex0 = DecodedMesh.getVertex(Index);
                    Indices.Add(Vertices.Add(FPolyVoxVector(Vertex0.position) * 100.f));
                    
                    // Calculate the tangents of our triangle
                    const FVector Edge01 = FPolyVoxVector(Vertex1.position - Vertex0.position);
                    const FVector Edge02 = FPolyVoxVector(Vertex2.position - Vertex0.position);
                    
                    const FVector TangentX = Edge01.GetSafeNormal();
                    FVector TangentZ = (Edge01 ^ Edge02).GetSafeNormal();
                    
                    for (int32 i = 0; i < 3; i++)
                    {
                        Tangents.Add(FProcMeshTangent(TangentX, false));
                        Normals.Add(TangentZ);
                    }
                }
            }
            
            // Finally create the mesh
            Mesh->CreateMeshSection(Material, Vertices, Indices, Normals, UV0, Colors, Tangents, true);
            Mesh->SetMaterial(Material, TerrainMaterial[Material]);
        }
        
        Mesh->CreateMeshSection(0, Vertices, Indices, Normals, UV0, Colors, Tangents, true);
        Mesh->SetMaterial(0, TerrainMaterial[Material]);
        

    }
    
    // Finally create the mesh
    
    // Randomly select two points to raycast
    // when hit, generate foliages
    FVector fStart = GetActorLocation() + FVector(FMath::RandRange(100.0f, 500.0f), FMath::RandRange(100.0f, 500.0f), 50000.0f);
    FVector fHit = fStart - FVector(0.0f, 0.0f, 80000.0f);
    
    FHitResult HitResult;
    FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetWorld()->GetFirstPlayerController()->GetCharacter());
    GetWorld()->LineTraceSingleByObjectType(
                                            HitResult,
                                            fStart,
                                            fHit,
                                            FCollisionObjectQueryParams(ECollisionChannel::ECC_MAX),
                                            TraceParameters
                                            );
    FVector FoliageLoc = HitResult.Location;
    FTransform FoliageTransform = FTransform(FoliageLoc);
    if(FMath::RandRange(10, 50) > 30)
        Foliage->AddInstanceWorldSpace(FoliageTransform);
    //UE_LOG(LogTemp, Error, TEXT("Foliage hit loc: %s"), *(FoliageTransform.ToString()))
}

void ATerrainGenManager::PostInitializeComponents()
{
    Super::PostInitializeComponents();
}

void VoxelTerrainPager::pageIn(const PolyVox::Region& region, PolyVox::PagedVolume<PolyVox::MaterialDensityPair44>::Chunk* Chunk)
{
    //region.getUpper_();
    for (int x = region.getLowerX(); x <= region.getUpperX(); x++)
    {
        for (int y = region.getLowerY(); y <= region.getUpperY(); y++)
        {
            for (int z = region.getLowerZ(); z <= region.getUpperZ(); z++) {
                float actx = x + posx;
                float acty = y + posy;
                float actz = z + posz;
                
                // apply 3d noise only above a certain level
                // reduce memory usage
                float tnoise = 0.0f;
                if(z > 30){
                    tnoise = FMath::Clamp(USimplexNoiseBPLibrary::SimplexNoise3D(actx * 0.0005, acty * 0.0005, actz * 0.0005), 0.5f, 1.0f) * amp * 2.0f;
                }
                
                auto EvaluatedNoise = 100000000.0f;
                
                // the same applies here
                if(z > 0){
                    EvaluatedNoise = USimplexNoiseBPLibrary::SimplexNoise2D(actx * freq, acty * freq/*, actz * freq*/) * amp + USimplexNoiseBPLibrary::SimplexNoise2D(actx * 0.000001, acty * 0.000001) * 30.0f;// + tnoise;
                }
                
                bool bSolid = actz < EvaluatedNoise;
                bool bFoliage = (EvaluatedNoise - actz) < 10.0f;
                PolyVox::MaterialDensityPair44 Voxel;
                /*if(actz < waterlevel){ //Water layer
                    Voxel.setDensity(255);
                    Voxel.setMaterial(1);
                }else if(!bSolid){
                    Voxel.setDensity(0);
                    Voxel.setMaterial(0);
                }else{
                    Voxel.setDensity(255);
                    Voxel.setMaterial(1);
                }*/
                // add foliage settings
                // if bFoliage is true, set the voxel above the current cell to some value
                Voxel.setDensity(bSolid ? 255 : 0);
                //Voxel.setMaterial(bSolid ? 1 : 0);
                
                
                //int ActualGrassZ = FMath::FloorToInt(EvaluatedNoise);
                
                //int DirtZ = ActualGrassZ - 1;
                //int DirthThickness = 3;
                
                if(bSolid){
                    
                    if(z <= 30)
                    {
                        Voxel.setMaterial(3);//3
                    }else{
                        Voxel.setMaterial(2);//2
                    }
                }else{
                    Voxel.setMaterial(0);
                }
                
                
                //Voxel.setDensity(1);
                //Voxel.setMaterial(1);

                Chunk->setVoxel(x - region.getLowerX(), y - region.getLowerY(), z - region.getLowerZ(), Voxel);
            }
        }
    }
}

// Called when a chunk is paged out
void VoxelTerrainPager::pageOut(const PolyVox::Region& region, PolyVox::PagedVolume<PolyVox::MaterialDensityPair44>::Chunk* Chunk)
{
    
    
}
