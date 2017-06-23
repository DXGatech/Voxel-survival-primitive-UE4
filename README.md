# Voxel-survival-primitive-UE4

===============================

Note: this repository does not contain any source code for now!

This is a primitive UE4 project, made to demonstrate how a voxel game can be implemented in unreal engine 4.

# Endless terrain generation system

====================================

The terrain generation system uses SimplexNoise2D, which is an enhanced version of perlin noise, to generate seamless and tileable terrain.

The pseudo code for the dual-chunk generation class:

for(chunkWidth = 0; chunkWidth <= 5; chunkWidth++)

   for(chunkHeight = 0; chunkHeight <= 5; chunkHeight++)
   
       UWorldSetWorldLocation(VoxelWorldLoc.X, VoxelWorldLoc.Y, USimplexNoise::SimplexNoise2D(VoxelWorldLoc.X, VoxelWorldLoc.y));
       
Note: All the C++ classes are exposed to the blueprint system of UE4, allowing artists to customize the procedural terrain without writing a single line of code.

INCOMPLETE
       

          

