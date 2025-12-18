#ifndef STATIC_MATERIALS_H
#define STATIC_MATERIALS_H
#include "../scene.h"
#endif

/*=================================================================================================*/
//Generic Materials  
	//Shaders------------------------------------------!!!!
		//!!Attention to 330 core, might need to dymnaic. 
		//Base Shaders
		static char vertex_shader_source[] = 
				"#version 330 core\n"
				"layout (location = 0) in vec3 aPos;\n"
				"layout (location = 1) in vec3 aColor;\n"
				"out vec3 ourColor;\n"
				"uniform mat4 model;\n"
				"uniform mat4 view;\n"
				"uniform mat4 projection;\n"
				"void main() {\n"
				"    gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
				"    ourColor = aColor;\n"  
				"}\n";

		static char fragment_shader_source[] = 
				"#version 330 core\n"
				"in vec3 ourColor;\n"
				"out vec4 FragColor;\n"
				"void main() {\n"
				"    FragColor = vec4(ourColor, 1.0);\n"
				"}\n"; 

	 Material Base_Color = {
    .shaders = (char*[]){ vertex_shader_source, fragment_shader_source },
    .GL_types = (GLenum[]){GL_VERTEX_SHADER, GL_FRAGMENT_SHADER},
		.shader_count = 2,
		.material = 0
  };
/*=================================================================================================*/

