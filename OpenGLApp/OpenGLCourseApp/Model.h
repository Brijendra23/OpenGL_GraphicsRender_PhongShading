#pragma once

#include<vector>
#include<string>

#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

#include"Mesh.h"
#include"Texture.h"



class Model
{

public: 
	Model();

	void LoadModel(const std::string& fileName);
	void RenderModel();
	void ClearModel();


	~Model();

private: 

	void LoadNode(aiNode* node, const aiScene* scene);//tree strucutre based storing data about the mesh and texture data their child with same texture and their data
		//node holds data about a specific one
		//scene holds data about every object in the scene

		void LoadMesh(aiMesh* mesh, const aiScene* scene);//mesh has to refernece the scene to get data about the texture for the mesh
		void LoadMaterial(const aiScene* scene);

	std::vector<Mesh*> meshList;
	std::vector<Texture*> textureList;
	std::vector<unsigned int> meshToTex; //each mesh is going to have a texture so instead of mesh having same texture geenrating new entry we use the already entered texture




};

