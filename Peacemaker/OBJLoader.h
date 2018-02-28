#pragma once
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <glm.hpp>

bool loadObj(const char* path,
	std::vector <glm::vec3> & out_vertices,
	std::vector <glm::vec2> & out_uvs,
	std::vector <glm::vec3> & out_normals
);
