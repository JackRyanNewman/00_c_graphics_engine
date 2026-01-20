#GUI APP


#Tools
#Staic Storage Data-Base and backups: Interactable Gui
 # Tables: scenes, meshs, material ,actions table. 
 # Requires: GUI to insert, and view objects(Requires: Sub-engine protocol Viewing)
 # Stores Compressed back-up raw materials in single folder, with some naming protocols. 
 # Holds PNGS, Vertices data(Might require counting programs), Shader data.
 # Table uniques Meshes, Material, Shaders, Textures, Action_Groups, Actions_Types

  #U_Scene: #⟶ #J_Scene-Unique_Assets: 
  #U_Unique_Assets: Generic-Asset Key, + Location
    #⟶ #J_Unique_Assets→Shader  
    #⟶ #J_Unique_Assets→Texture 
    #⟶ #J_Unique_Assets→Action_Type: Unique_Asset_ID, Action_ID, Custom_Vector  
  
  #U_Generic-Asset: mesh, material, action type, scale
  #U_Meshes: Vertices, Indices, (AutoGen Count?) 
  #U_Materials: Material-Key: This could be broken up. 
     #⟶ #J_Material→Shader 
     #⟶ #J_Material→Texture 
  #U_Texture: Some information. 
  #U_Shader:  GPU: Shader type. 
  #U_Action-Group: ID 
    # ⟶ J_Action-Group→Action_type
  #U_Action_Types: ID Generic-Vector



# Scene Builder --> 
  # Static Storage Data-Base: 
  # Building Scene: Create scene of 3d world grid. Where you drag and drop items from db.
    #Search up: trie try complment? For search ups...
  #