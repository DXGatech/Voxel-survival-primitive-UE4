# Voxel-survival-primitive-UE4

===============================

Note: this repository does not contain any source code for now!

This is a primitive UE4 project, made to demonstrate how a voxel game can be implemented in unreal engine 4.

# Endless terrain generation system

====================================

The terrain generation system uses SimplexNoise2D, which is an enhanced version of perlin noise, to generate seamless and tileable terrain.

A piece of pseudo code for the dual-chunk generation class:
```
for(chunkWidth = 0; chunkWidth <= 5; chunkWidth++){
   for(chunkHeight = 0; chunkHeight <= 5; chunkHeight++){
       UWorldSetWorldLocation(VoxelWorldLoc.X, VoxelWorldLoc.Y, USimplexNoise::SimplexNoise2D(VoxelWorldLoc.X, VoxelWorldLoc.y));
       }
}
```
       
Note: For bp-code branches, which will likely be added in the future, all C++ classes are exposed to child blueprints, allowing artists to customize the procedural terrain without writing a single line of code OR editing the parent blueprint.

# Features

====================================

We have implemented: 
**basic artificial intelligence units**(templates for passive mobs);

**biomes based on geographical factors** that are illustrated in any whittaker diagram;
![alt text](https://cplusplusgametricks.files.wordpress.com/2017/06/screen-shot-2017-06-23-at-10-48-16-am.png?w=1352)

**Procedural river generation**
![alt text](https://i2.wp.com/cplusplusgametricks.files.wordpress.com/2017/12/screen-shot-2017-12-10-at-7-44-37-pm.png?ssl=1&w=450)

block placing & breaking( **advanced manipulations like stretch and rotation included** );

**infinite terrain generation** based on the concept of chunk( mineraft style );
![alt text](https://cplusplusgametricks.files.wordpress.com/2017/06/screen-shot-2017-06-03-at-11-53-39-pm.png?w=880&h=612)

Custom foliages:
![alt text](https://cplusplusgametricks.files.wordpress.com/2017/06/screen-shot-2017-06-04-at-10-10-51-pm1.png?w=1352)

       

          

