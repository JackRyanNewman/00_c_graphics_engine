/* Jack Newman*/

/*=================================================================================================*/
/*External Libraries*/

#ifndef SCENE_H
#define SCENE_H
#include <GL/glew.h>      // GLEW MUST come first
#include <GLFW/glfw3.h>
#include "..\5_math\math3d.h"

/*=================================================================================================*/
/* Defines */
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


/*=================================================================================================*/
/* Structures */ /* Scene uses Object3D from object3d.h */



	typedef struct{
		//On scene load detect all types of meshes?, On run, we only need vao, vbo, ebo,
    //VBO — Vertex Buffer Object, EBO — Element Buffer Object (index buffer), VAO — Vertex Array Object
		GLuint vao, vbo, ebo;    			//GPU shared idea of object. 
		float *vertices;  				//Holds vertices and corresponding colors rgb. 
	  unsigned int *indices; 	//If null, then not indexed, v_count, and i_count determined within method. 
    size_t vertex_count, index_count;
	} Mesh; 

	typedef struct{
		//On scene load detect all types of shaders? On run we only need material. 
    //optional later: textures, uniform buffers. This could be freed at runtime.
			GLenum *GL_types;         //Holds array of the shaders types. 
      char **shaders; 					//Holds array of shader instructions that will get compiled.   
      int shader_count;
      GLuint material;        				//Reference to gpu compiled shader. 
	} Material;

  typedef struct{
    short *actions; //Array of actions to execute. List will be modified. 
    Vec4 quaternion; 
    Vec3 position;
    Vec3 scale;
  } Transform;

 typedef struct {
    Mesh* parent_mesh;      // pointer to a mesh (const)
    Material* material;     // pointer to a material (const)
    Transform* transform;   // pointer to mutable transform      
    GLenum draw_mode; 
  } Rendered_Object;


/*=================================================================================================*/
/* Global Variables */


/*=================================================================================================*/
/* Forward Declarations */
  void scene_update(float dt);
  void scene_cleanup();
  void Transform_update(Transform *obj, float dt);
  Mat4 Transform_get_model_matrix(Transform *obj);
  Rendered_Object* scene_1(int* total_objs, Mesh*** mesh_lst, int* num_meshes, Material*** mat_lst, int* num_mats);
  

/*=================================================================================================*/
/* MACROS */

#endif