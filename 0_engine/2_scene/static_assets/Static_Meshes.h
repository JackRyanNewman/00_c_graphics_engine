#ifndef STATIC_MESHES_H
#define STATIC_MESHES_H
#include "../scene.h"
#endif

/*=================================================================================================*/
  float cube_vertices[] = {
     // positions   // colors
     -0.5f, -0.5f, -0.5f, 1,0,0, //Bottom left, back
      0.5f, -0.5f, -0.5f, 0,1,0, //Bottom right, back,
      0.5f,  0.5f, -0.5f, 0,0,1, //top right, back
     -0.5f,  0.5f, -0.5f, 1,1,0, //top left, back
     -0.5f, -0.5f,  0.5f, 0,1,1, //bottom left, front
      0.5f, -0.5f,  0.5f, 1,0,1, //bottom right, front
      0.5f,  0.5f,  0.5f, 1,1,1, //top right, front
     -0.5f,  0.5f,  0.5f, 0,0,0  //top left, front
  };

  Mesh Full_Cube = {
    .vao = 0, .vbo = 0, .ebo = 0,
    .vertices = cube_vertices,
    .indices = (unsigned int[]){
      0,1,2, 2,3,0,  // back face
      4,5,6, 6,7,4,  // front face
      0,4,7, 7,3,0,  // left face
      1,5,6, 6,2,1,  // right face
      3,2,6, 6,7,3,  // top face
      0,1,5, 5,4,0   // bottom face
     },
     .vertex_count =  8*6, //amount of vertexs
     .index_count = 36, //faces* triangle per face * indices per triagle 
     //6*2*3
  };

  Mesh Wire_Cube = {
    .vao = 0, .vbo = 0, .ebo = 0,
    .vertices = cube_vertices,
    .vertex_count =  8*6, //amount of vertexs
    .index_count = 24,    //Edges of cube*indices. 
    .indices = (unsigned int[]){
      //Tells GL the order to connect the vertices in. Reduces runtime
      //Decreases memory, else it would draw the thing in the order in the cube_vertices. 
      0,1, 1,2, 2,3, 3,0,  // back face
      4,5, 5,6, 6,7, 7,4,  // front face
      0,4, 1,5, 2,6, 3,7   // connecting edges
     },
  };

  //====================================================================================================
  static float diamond_vertices[] = {
      // positions        // colors
      0.0f,  0.5f,  0.0f, 1,0,0,   // top point
     -0.5f,  0.0f,  0.0f, 0,1,0,   // left
      0.5f,  0.0f,  0.0f, 0,0,1,   // right
      0.0f,  0.0f,  0.5f, 1,1,0,   // front
      0.0f,  0.0f, -0.5f, 0,1,1,   // back
      0.0f, -0.5f,  0.0f, 1,0,1    // bottom point
  };


  Mesh Full_Diamond = {
    .vao = 0, .vbo = 0, .ebo = 0,
    .vertices = diamond_vertices, 

    .indices = (unsigned int[]){
      0,1,3,  // top-left-front
      0,3,2,  // top-front-right
      0,2,4,  // top-right-back
      0,4,1,  // top-back-left
      5,3,1,  // bottom-front-left
      5,2,3,  // bottom-right-front
      5,4,2,  // bottom-back-right
      5,1,4   // bottom-left-back
    },
    .vertex_count = 36,
    .index_count = 24,
    
  };

  Mesh Wire_Diamond = {
    .vao = 0, .vbo = 0, .ebo = 0,
    .vertices = diamond_vertices, 

    .indices = (unsigned int[]){
      0,1, 0,2, 0,3, 0,4,
      5,1, 5,2, 5,3, 5,4,
      1,3, 3,2, 2,4, 4,1
    },
    .vertex_count = 36,
    .index_count = 28,
  };

/*=================================================================================================*/
// Room vertices - a cube with inverted normals so you see it from inside
float room_vertices[] = {
    // Floor (Y = -5)
    -10.0f, -5.0f, -10.0f,  0.5f, 0.5f, 0.5f,
     10.0f, -5.0f, -10.0f,  0.5f, 0.5f, 0.5f,
     10.0f, -5.0f,  10.0f,  0.5f, 0.5f, 0.5f,
    -10.0f, -5.0f,  10.0f,  0.5f, 0.5f, 0.5f,
    
    // Ceiling (Y = 5)
    -10.0f,  5.0f, -10.0f,  0.7f, 0.7f, 0.7f,
     10.0f,  5.0f, -10.0f,  0.7f, 0.7f, 0.7f,
     10.0f,  5.0f,  10.0f,  0.7f, 0.7f, 0.7f,
    -10.0f,  5.0f,  10.0f,  0.7f, 0.7f, 0.7f,
    
    // Back wall (Z = -10)
    -10.0f, -5.0f, -10.0f,  0.8f, 0.2f, 0.2f,
     10.0f, -5.0f, -10.0f,  0.8f, 0.2f, 0.2f,
     10.0f,  5.0f, -10.0f,  0.8f, 0.2f, 0.2f,
    -10.0f,  5.0f, -10.0f,  0.8f, 0.2f, 0.2f,
    
    // Front wall (Z = 10)
    -10.0f, -5.0f,  10.0f,  0.2f, 0.8f, 0.2f,
     10.0f, -5.0f,  10.0f,  0.2f, 0.8f, 0.2f,
     10.0f,  5.0f,  10.0f,  0.2f, 0.8f, 0.2f,
    -10.0f,  5.0f,  10.0f,  0.2f, 0.8f, 0.2f,
    
    // Left wall (X = -10)
    -10.0f, -5.0f, -10.0f,  0.2f, 0.2f, 0.8f,
    -10.0f, -5.0f,  10.0f,  0.2f, 0.2f, 0.8f,
    -10.0f,  5.0f,  10.0f,  0.2f, 0.2f, 0.8f,
    -10.0f,  5.0f, -10.0f,  0.2f, 0.2f, 0.8f,
    
    // Right wall (X = 10)
    10.0f, -5.0f, -10.0f,  0.8f, 0.8f, 0.2f,
    10.0f, -5.0f,  10.0f,  0.8f, 0.8f, 0.2f,
    10.0f,  5.0f,  10.0f,  0.8f, 0.8f, 0.2f,
    10.0f,  5.0f, -10.0f,  0.8f, 0.8f, 0.2f,
};

Mesh Room_Mesh = {
    .vao = 0, .vbo = 0, .ebo = 0,
    .vertices = room_vertices,
    .indices = (unsigned int[]){
        // Floor (inverted so you see it from inside)
        3, 1, 0,  1, 3, 2,
        // Ceiling
        4, 6, 5,  6, 4, 7,
        // Back wall
        11, 9, 8,  9, 11, 10,
        // Front wall
        12, 14, 13,  14, 12, 15,
        // Left wall
        19, 17, 16,  17, 19, 18,
        // Right wall
        20, 22, 21,  22, 20, 23,
    },
    .vertex_count = 24 * 6,  // 24 vertices * 6 floats each
    .index_count = 36,
};