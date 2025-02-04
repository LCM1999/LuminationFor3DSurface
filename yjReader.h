#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "vtkPolyData.h"
#include "vtkNew.h"
#include "vtkSmartPointer.h"
#include "vtkXMLPolyDataReader.h"
#include "vtkXMLPolyDataWriter.h"
#include "vtkFloatArray.h"
#include "vtkDataArray.h"
#include "vtkPointData.h"
#include "vtkCellData.h"

#include "smoothing.h"

int loadingScales = 20;

class YJ {
public:
	GLuint geomVAO, quadVAO, quadVBO, positionBuffer, normalBuffer, scalarBuffer, smoothedNormalsBuffer, geomEBO; 
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	// std::vector<glm::vec2> textureCoordinates;
	std::vector<float> scalar;
	std::vector<unsigned int> indices;
	std::string path, pdPath;
	std::vector<glm::vec3> smoothedNormals[20];
	//std::vector<glm::vec3[20]> smoothedNormalsResized; -> YOU CAN'T MAKE A VECTOR OF PLAIN ARRAYS
	glm::vec4* smoothedNormalsSingleArr;
	float* sigma;
	float feature;
	bool isSet = false;

    const float quadVertices[24] = {
        // 位置        // 纹理坐标
        -1.0f,  1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,

        -1.0f,  1.0f, 0.0f, 1.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f
    };

	// std::vector<glm::vec3> maxPDs, minPDs;
	// std::vector<float> maxCurvs, minCurvs;

	YJ(std::string path, std::string base_normal_name, std::string scalar_name, float* Sigma) {
		this->path = path;
		this->sigma = Sigma;
		loadYJ(path, base_normal_name, scalar_name);
		// loadSmoothedNormals();
		//setupYJ(vertices, normals);
	}

	bool loadYJ(const std::string& path, const std::string& base_normal_name, const std::string& scalar_name) {
		//how to construct indicies for EBO..?
		//maybe mix with assimp in usage as a quick hack
		std::cout << "Loading file : " << path << "\n";
		//std::vector<glm::vec3> vertices;
		//std::vector<glm::vec3> normals;

		vtkNew<vtkXMLPolyDataReader> reader;
		reader->SetFileName(path.c_str());
		reader->Update();
		auto poly = reader->GetOutput();
		auto normals = poly->GetPointData()->GetArray(base_normal_name.c_str());
		auto pd1 = poly->GetPointData()->GetArray(scalar_name.c_str());
		float minScalar = pd1->GetRange()[0];
		float maxScalar = pd1->GetRange()[1];
		float range = maxScalar - minScalar;
		for (size_t i = 0; i < poly->GetNumberOfPoints(); i++)
		{
			double* point = poly->GetPoint(i);
			vertices.push_back(glm::vec3(point[0], point[1], point[2]));
			this->normals.push_back(glm::vec3(normals->GetTuple3(i)[0], normals->GetTuple3(i)[1], normals->GetTuple3(i)[2]));
			this->scalar.push_back(glm::clamp<float>((pd1->GetTuple1(i) - minScalar) / range, 0.0f, 1.0f));
		}
		feature = featureSize(this->vertices);
    	std::cout << "Feature size of model : " << feature << std::endl;
		// for (size_t j = 0; j < 20; j++)
		// {
	    //     sigma[j] = 0.4 * feature * pow(sqrt(2),float(j));
		// 	std::cout << "Sigma " << sigma[j] << std::endl;
		// 	if (poly->GetPointData()->HasArray(("Normals_"+std::to_string(j)).c_str()))
		// 	{
		// 		for (size_t i = 0; i < poly->GetNumberOfPoints(); i++)
		// 		{
		// 			double* normal = poly->GetPointData()->GetArray(("Normals_"+std::to_string(j)).c_str())->GetTuple3(i);
		// 			smoothedNormals[j].push_back(glm::vec3(normal[0], normal[1], normal[2]));
		// 		}
		// 	} else {
		// 		smoothedNormals[j].resize(poly->GetNumberOfPoints());
		// 		smooth_parallel(this->normals, smoothedNormals[j], invsigma2(this->sigma[j]));
		// 	}
		// }
		
		for (size_t i = 0; i < poly->GetNumberOfCells(); i++)
		{
			indices.push_back((unsigned int)poly->GetCell(i)->GetPointId(0));
			indices.push_back((unsigned int)poly->GetCell(i)->GetPointId(1));
			indices.push_back((unsigned int)poly->GetCell(i)->GetPointId(2));
		}
		
		// smoothedNormalsSingleArr = new glm::vec4[smoothedNormals[0].size() * 20];
		// for (int i = 0; i < smoothedNormals[0].size(); i++) {
		// 	//smoothedNormalsArr[i] = new glm::vec3[20];
		// 	for (int j = 0; j < loadingScales; j++) {
		// 		//smoothedNormalsArr[i][j]=smoothedNormals[j][i];
		// 		smoothedNormalsSingleArr[i + j*smoothedNormals[0].size()] = glm::vec4(smoothedNormals[j][i],0.0);
		// 	}
		// }

		std::cout << "Size of vertices : " << vertices.size() << "\n";
		std::cout << "Size of normals : " << this->normals.size() << "\n";
		std::cout << "Size of indices : " << indices.size() << "\n";
		return true;
	}
	//load preprocessed smoothed normals
	void loadSmoothedNormals() {
		//std::vector<glm::vec3> smoothedNormalsTemp ;
		std::string smoothedPath=this->path;
		smoothedPath.erase(smoothedPath.length() - 3, 3); //erases .yj, bad implementation tbh if the legth of the extension name changes it won't work
		for (int i = 0; i < loadingScales; i++) {
			smoothedPath = smoothedPath + "_k" + std::to_string(i+1) + ".yj";
			// if (!loadNormalsYJ(smoothedPath, this->smoothedNormals[i]))std::cout << "Failed to load smoothed normals at : " << smoothedPath << "\n";
			// else std::cout << "Loaded smoothed normals "<<i<< " at " << smoothedPath << "\nFirst smoothed normal : " << this->smoothedNormals[i][0].x << ", " << this->smoothedNormals[i][0].y << ", " << this->smoothedNormals[i][0].z<<"\n"<<"Size : "<<smoothedNormals[i].size()<<"\n";
			if (i > 8)smoothedPath.erase(smoothedPath.length() - 7, 7); //erases _k(number i).yj, bad implementation tbh if the legth of the extension name changes it won't work
			else smoothedPath.erase(smoothedPath.length() - 6, 6);
		}
		//Transpose because of reasons
		//smoothedNormalsArr = new glm::vec3* [smoothedNormals[0].size()] ;
		smoothedNormalsSingleArr = new glm::vec4[smoothedNormals[0].size() * 20];
		for (int i = 0; i < smoothedNormals[0].size(); i++) {
			//smoothedNormalsArr[i] = new glm::vec3[20];
			for (int j = 0; j < loadingScales; j++) {
				//smoothedNormalsArr[i][j]=smoothedNormals[j][i];
				smoothedNormalsSingleArr[i + j*smoothedNormals[0].size()] = glm::vec4(smoothedNormals[j][i],0.0);
			}
		}
	}


	void setup() {
		std::cout << "Setting up buffers.\n";

		glGenVertexArrays(1, &geomVAO ); //vertex array object
		glGenBuffers(1, &positionBuffer); //vertex buffer object
		glGenBuffers(1, &normalBuffer); //vertex buffer object
		glGenBuffers(1, &scalarBuffer); //vertex buffer object
		
		glGenBuffers(1, &geomEBO); 

		// glGenBuffers(1, &maxPD);
		// glGenBuffers(1, &minPD);
		// glGenBuffers(1, &maxCurv);
		// glGenBuffers(1, &minCurv);

		//VAO  
		glBindVertexArray(geomVAO); 

		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
		
		glBindBuffer(GL_ARRAY_BUFFER, scalarBuffer);
		glBufferData(GL_ARRAY_BUFFER, scalar.size() * sizeof(float), &scalar[0], GL_STATIC_DRAW);
		// maxPD, minPD, maxCurv, minCurv
		// glBindBuffer(GL_ARRAY_BUFFER, maxPD);
		// glBufferData(GL_ARRAY_BUFFER, maxPDs.size() * sizeof(glm::vec3), &maxPDs[0], GL_STATIC_DRAW);
		// glBindBuffer(GL_ARRAY_BUFFER, minPD);
		// glBufferData(GL_ARRAY_BUFFER, minPDs.size() * sizeof(glm::vec3), &minPDs[0], GL_STATIC_DRAW);
		// glBindBuffer(GL_ARRAY_BUFFER, maxCurv);
		// glBufferData(GL_ARRAY_BUFFER, maxCurvs.size() * sizeof(float), &maxCurvs[0], GL_STATIC_DRAW);
		// glBindBuffer(GL_ARRAY_BUFFER, minCurv);
		// glBufferData(GL_ARRAY_BUFFER, minCurvs.size() * sizeof(float), &minCurvs[0], GL_STATIC_DRAW);

		//EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geomEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, scalarBuffer);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);
		
		// glEnableVertexAttribArray(3);
		// glBindBuffer(GL_ARRAY_BUFFER, maxPD);
		// glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// glEnableVertexAttribArray(4);
		// glBindBuffer(GL_ARRAY_BUFFER, minPD);
		// glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// glEnableVertexAttribArray(5);
		// glBindBuffer(GL_ARRAY_BUFFER, maxCurv);
		// glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// glEnableVertexAttribArray(6);
		// glBindBuffer(GL_ARRAY_BUFFER, minCurv);
		// glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);
		//SEND SMOOTHED NORMALS TO SHADER
		//SSBO //smoothedNormalsBuffer is GLuint ID
		//glBufferData(GL_SHADER_STORAGE_BUFFER,sizeof(smoothedNormals[0])*20,smoothedNormals,GL_STATIC_DRAW);
		
		//Can't send std::vector or any other objects to glsl, they won't work.
		
		// glGenBuffers(1, &smoothedNormalsBuffer); //vertex buffer object
		// glBindBuffer(GL_SHADER_STORAGE_BUFFER,smoothedNormalsBuffer);
		// glBufferData(GL_SHADER_STORAGE_BUFFER, 20*smoothedNormals[0].size()*sizeof(glm::vec4), smoothedNormalsSingleArr, GL_DYNAMIC_DRAW);
		// glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7 ,smoothedNormalsBuffer);
		
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glBindVertexArray(0);

		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		std::cout << "Ready to render.\n";
		return;
	}

	bool geomRender(GLuint shader) {
		glUseProgram(shader);
		glBindVertexArray(geomVAO);
		glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		return true;
	}

	bool quadRender(GLuint shader) {
		glUseProgram(shader);
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		return true;
	}
};

		