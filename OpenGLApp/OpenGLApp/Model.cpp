#include "Model.h"

Model::Model()
{

}

void Model::LoadModel(const std::string& fileName)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals|aiProcess_JoinIdenticalVertices );//ai_process triangulate converts the model into triangle
	//aiProcess flip uvs flips the texture along y axis
	//ai process gen smooth normal generates the normals for interpolation
	//4th argument treats the joined vertices as a single vertices saving us the time

	if (!scene)
	{
		printf("Model (%s) failed to load %s", fileName, importer.GetErrorString());
		return;
	}

	LoadNode(scene->mRootNode, scene);//basically starts of loading the nodes from the root node//loading meshs and textures and the childs

	LoadMaterial(scene);// loading the material for the models
}

void Model::ClearModel()
{
	for (size_t i = 0; i < meshList.size(); i++)
	{
		if (meshList[i]) {
			delete meshList[i];
			meshList[i] = nullptr;
		}
	}
	for (size_t i = 0; i < textureList.size(); i++)
	{
		if (textureList[i]) {
			delete textureList[i];
			textureList[i] = nullptr;
		}
	}
}

void Model::RenderModel()
{
	for (size_t i = 0; i < meshList.size(); i++)
	{
		unsigned int materialIndex = meshToTex[i];
		if (materialIndex < textureList.size() && textureList[materialIndex])
		{
			textureList[materialIndex]->UseTexture();
		}
		meshList[i]->RenderMesh();
	}
}












void Model::LoadNode(aiNode* node, const aiScene* scene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);// so basically this loop traverse all the meshes present in the scene 
		//loads the meshes mMeshes requires id for the nodes toload the mesh since stoore in form of id
		//to get the id for each mesh the node is used
	}

	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		LoadNode(node->mChildren[i], scene);//so basically this loop iterates through all the child nodes and loads their mesh
		//its a recursion function to load the child of all the mesh
	}
}

void Model::LoadMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		vertices.insert(vertices.end(), { mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z });//inserting the coordinaates x,y,z in vertices
		if (mesh->mTextureCoords[0])
		{
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x,mesh->mTextureCoords[0][i].y });//the texture coordinates U,Vs are stored
		}
		else
		{
			vertices.insert(vertices.end(), { 0.0f,0.0f });// dummy values to maintain the shaders readung property of vertices
		}
		vertices.insert(vertices.end(), { -mesh->mNormals[i].x,-mesh->mNormals[i].y,-mesh->mNormals[i].z });//the shaders generate the reversed value of normals to invert the normal direction
	}
	 //looping over each of the faces to store the indices
	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);//will go through each facesand store the indices
		}
	}

	Mesh* newMesh = new Mesh();
	newMesh->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());// converting the vertices vector into array and refferencing the the first data of the array location so the rest can be accwssed
	meshList.push_back(newMesh);
	meshToTex.push_back(mesh->mMaterialIndex);//linking the index of the texture



}

void Model::LoadMaterial(const aiScene* scene)
{
	textureList.resize(scene->mNumMaterials);
	for (size_t i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* material = scene->mMaterials[i];
		textureList[i] = nullptr;//initiaitng the index of the texture with the null and checking if for particular mesh any material pre exist
		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)//if the diffuse texture type present in the first index then copy it to the path 
			{
				int idx = std::string(path.data).rfind("\\");
				std::string filename = std::string(path.data).substr(idx + 1);//getting the filename

				std::string texPath = std::string("Textures/") + filename;

				textureList[i] = new Texture(texPath.c_str());

				if (!textureList[i]->LoadTexture())
				{
					printf("Failed to load texture at %s", texPath);
					delete textureList[i];
					textureList[i] = nullptr;
				}
			}
		}
		if (!textureList[i])
		{
			textureList[i] = new Texture("Textures/plain.png");
			textureList[i]->LoadTextureA();
		}


	}
}

Model::~Model()
{

}
