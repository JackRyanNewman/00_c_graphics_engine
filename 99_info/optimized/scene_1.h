const Scene scene_1 = {
    .max_objects = 10,
    .parent_meshes = parent_meshes,
    .material_group = scene_materials,
    .transforms = scene_transforms
};

Transform Rotating_Animations = {obj_1, obj_2, obj_3};

//Parent meshes
static const Mesh* parent_meshes[] = { &Full_Cube , &Wire_Cube, &Full_Diamond, &Wire_Diamond };
static const Material* const* scene_materials[] = {
	{Base_Color}, 
	{Base_Color}, 
	{Base_Color}, 
	{Base_Color}
};
static Transform* const* scene_transforms[] = {
		{ //Full_Cube
			{ //Base_Color
				walls
			}
		}, 
		{ //Wire_Cube
			{ //Base_Color
				obj_1
			}
		},
		{ //Full_Diamond
			{ //Base_Color
				obj_2
			}
		},		
		{ //Wire_Diamond
			{ //Base_Color
				obj_3
			}
		},

};