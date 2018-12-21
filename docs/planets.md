# Planet Generation

## Description

Generate planets based on spherized cubes with the capability of streaming geometry
in real time using voxelization and a quadtree LOD mechanism to determine 
resolution.<br>
Feasibility in Python, C++ implementation of performance critical parts if necessary. 
 
## Implementation

### Abstraction Layers:

<div style="table, td, th{border-collapse: collapse; border: 1px solid black;}">
<table>
    <tr>
        <th align="left"><h4>Type</h4></th>
        <th align="left"><h4>Description</h4></th>
        <th align="left"><h4>Role</h4></th>
    </tr>
    <tr>
        <td valign="top"><b>Geometry Streamer</b></td>
        <td><ul>
            <li>Query Producers and process geometry</li>
            <li>Quadtree / LOD</li>
        </ul></td>
        <td valign="top">Consumer</td>
    </tr>
    <tr>
        <td valign="top"><b>Planet</b></td>
        <td><ul>
            Terrain
            <ul>
                Base Layer
                <ul>
                    <li>Continents / Water (Y/N)</li>
                    <li>Extremes (Radius + Offset)</li>
                </ul>
                Voxel Layer (Not in initial Version)
                <ul>
                    <li>Cave Noise</li>
                    <li>Solid Noise</li>
                    <li>Landscape Noise</li>
                </ul>
            </ul>
            LOD Level<br>
            Object Placement
        </ul></td>
        <td valign="top">Producer</td>
    </tr>
    <tr>
        <td valign="top"><b>Physics</b></td>
        <td><ul>
            <li>Collision Solids</li>
            <li>Fixed Resolution</li>
            <li>Quadtree around camera</li>
        </ul></td>
        <td valign="top">Producer</td>
    </tr>
    <tr>
        <td valign="top"><b>Noise</b></td>
        <td><ul>
            <li>LOD = Detail Noise</li>
            <li>Coordinates Handling</li>
        </ul></td>
        <td valign="top">Producer</td>
    </tr>
    <tr>
        <td valign="top"><b>Base Geometry</b></td>
        <td><ul>
            Shapes
            <ul>
                <li>Spherized Cube</li>
                <li>Cylinder</li>
                <li>Foliage=Doublesided Plane</li>
            </ul>
            Symmetry
        </ul></td>
        <td valign="top">Producer</td>
    </tr>
</table></div>


#### Geometry Streamer

* Is location aware, queries geometry (visual + physics) data from producers and
streams it into Panda3D.
* Has "update" method, running in separate thread (priority?), that queries for 
changes through the means of current location.
* Manages insertion and removal of geometry.


#### Planet

* Can produce terrain geometry based on provided location and LOD.
* Can produce object placement information:
    * Object Type
    * Object Location
    * Object Random Seed


#### Physics

* Can produce collision solids for quads at fixed resolution.


#### Noise

* Terrain noise, LOD = higher complexity.
* Translates position in respective quad on cube
* General Noise Functions for object generation


#### Base Geometry

* Can produce quads from a spherized cube at different resolution (LOD)
* Can produce simple base shapes with possible distortion by noise and 
selective symmetry
* Can produce L-Systems from a number of shapes
* Can produce triangles
