# Voxel-survival-primitive-UE4

THIS PROJECT IS NO LONGER MAINTAINED, AS AN ADVANCED VERSION IS CREATED SEPARATELY.

Content:

***Voxel terrain generation***
heightmap-independent biomes(mountains, oceans, islands, swamp), rivers(short and long), surface overhang, foliages, heightmap-dependent biomes(grassland, desert, tundra)

***Runtime effects***
Dynamic skybox with day/night cycle and weather effects

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
![alt text](https://i1.wp.com/cplusplusgametricks.files.wordpress.com/2017/12/screen-shot-2017-12-12-at-4-27-05-pm.png?ssl=1&w=450)

**Procedural river generation**


![alt text](https://i2.wp.com/cplusplusgametricks.files.wordpress.com/2017/12/screen-shot-2017-12-10-at-7-44-37-pm.png?ssl=1&w=450)

![alt text](https://i1.wp.com/cplusplusgametricks.files.wordpress.com/2017/12/screen-shot-2017-12-11-at-9-24-47-pm.png?ssl=1&w=450)

block placing & breaking( **advanced manipulations like stretch and rotation included** );

**infinite terrain generation** based on the concept of chunk( mineraft style );
![alt text](https://cplusplusgametricks.files.wordpress.com/2017/06/screen-shot-2017-06-03-at-11-53-39-pm.png?w=880&h=612)

Custom foliages:
![alt text](https://cplusplusgametricks.files.wordpress.com/2017/06/screen-shot-2017-06-04-at-10-10-51-pm1.png?w=1352)

       

          

