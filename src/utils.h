#pragma once
#include "glIncludes.h"
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

// Returns whether the given file exists
static const bool FileExists(const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}

static const std::string Vec3ToString(const glm::vec3& vec)
{
    std::ostringstream ss;
    ss << "[" << vec.x << ", " << vec.y << ", " << vec.z << "]";
    return ss.str();
}

static const glm::vec3 Vec3FromFloats(const float vec[3])
{
    return glm::vec3(vec[0], vec[1], vec[2]);
}

static const glm::vec4 Vec4FromFloats(const float vec[4])
{
    return glm::vec4(vec[0], vec[1], vec[2], vec[3]);
}

static const glm::mat3 Mat3FromFloats(const float vec1[3], const float vec2[3], const float vec3[3])
{
    return glm::mat3(Vec3FromFloats(vec1), Vec3FromFloats(vec2), Vec3FromFloats(vec3));
}

static const glm::mat4 Mat4FromFloats(const float vec1[4], const float vec2[4], const float vec3[4], const float vec4[4])
{
    return glm::mat4(Vec4FromFloats(vec1), Vec4FromFloats(vec2), Vec4FromFloats(vec3), Vec4FromFloats(vec4));
}

static float* FloatsFromVec3(const glm::vec3& vec3)
{
    float vec[3] = { vec3.x, vec3.y, vec3.z };
    return vec;
}
