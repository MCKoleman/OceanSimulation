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

// Rounds the value to the given precision
static const std::string RoundToPrecision(float _val, size_t _precision)
{
    size_t nzero = _precision + 1;
    std::string str = std::to_string(_val);
    auto new_str = std::string(nzero - std::min(nzero, str.length()), '0') + str;
    return new_str;
}

// Help parsing a string by token from:
// https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
static const void ParseStringByDelim(std::vector<std::string>& out, const std::string& str, const std::string& delim)
{
    // Make sure out vec is cleared
    out.clear();

    // Parse each part of the string
    size_t spos = 0;
    std::string token;
    std::string tempStr = str;
    while ((spos = tempStr.find(delim)) != std::string::npos) {
        // Parse token
        token = tempStr.substr(0, spos);
        out.push_back(token);
        tempStr.erase(0, spos + delim.length());
    }
    out.push_back(tempStr);
}

// Reads a vector 3 from the given strings, skipping ahead the given offset. Strings must have size >= 4 + offset to be read, and the first element + offset are ignored
static const glm::vec3 ReadVec3FromStrings(const std::vector<std::string>& strings, int offset)
{
    // Avoid errors
    if ((int)strings.size() >= 4 + offset)
        return glm::vec3(std::stof(strings[1 + offset]), std::stof(strings[2 + offset]), std::stof(strings[3 + offset]));
    else
        return glm::vec3();
}

/// <summary>
/// Clamps the given value to be within min and max
/// </summary>
/// <param name="value">Value to clamp</param>
/// <param name="min">Minimum</param>
/// <param name="max">Maximum value</param>
/// <returns>Value in range [min, max]</returns>
static const int Clamp(int value, int min, int max)
{
    if (value > max)
        return max;
    else if (value < min)
        return min;
    else
        return value;
}

/// <summary>
/// Clamps the given value to be within min and max
/// </summary>
/// <param name="value">Value to clamp</param>
/// <param name="min">Minimum</param>
/// <param name="max">Maximum value</param>
/// <returns>Value in range [min, max]</returns>
static const float Clamp(float value, float min, float max)
{
    if (value > max)
        return max;
    else if (value < min)
        return min;
    else
        return value;
}

/// <summary>
/// Clamps the given value to be within min and max
/// </summary>
/// <param name="value">Value to clamp</param>
/// <param name="min">Minimum</param>
/// <param name="max">Maximum value</param>
/// <returns>Value in range [min, max]</returns>
static const glm::vec3 Clamp(const glm::vec3& value, float min, float max)
{
    return glm::vec3(Clamp(value.x, min, max), Clamp(value.y, min, max), Clamp(value.z, min, max));
}

/// <summary>
/// Converts the given decimal value to hexadecimal.
/// Borrowed from: 
/// https://www.geeksforgeeks.org/convert-the-given-rgb-color-code-to-hex-color-code/
/// </summary>
/// <param name="value">Decimal value</param>
/// <returns>Hexadecimal equivalent</returns>
static const std::string DecimalToHex(int value)
{
    // char array to store hexadecimal number
    char hexValue[2];

    // counter for hexadecimal number array
    int i = 0;
    while (value != 0) {

        // temporary variable to store remainder
        int temp = 0;

        // storing remainder in temp variable.
        temp = value % 16;

        // check if temp < 10
        if (temp < 10) {
            hexValue[i] = temp + 48;
            i++;
        }
        else {
            hexValue[i] = temp + 55;
            i++;
        }

        value = value / 16;
    }

    std::string hexCode = "";
    if (i == 2) {
        hexCode.push_back(hexValue[0]);
        hexCode.push_back(hexValue[1]);
    }
    else if (i == 1) {
        hexCode = "0";
        hexCode.push_back(hexValue[0]);
    }
    else if (i == 0)
        hexCode = "00";

    // Return the equivalent
    // hexadecimal color code
    return hexCode;
}

/// <summary>
/// Returns a random number between min and max
/// </summary>
/// <param name="min">Minimum value (default 0)</param>
/// <param name="max">Maximum value (default 1)</param>
/// <returns>Random float in [min, max] (default [0, 1])</returns>
static const float GetRandFloat(float min = 0.0f, float max = 1.0f)
{
    return (rand() * (max - min)) / RAND_MAX + min;
}

/// <summary>
/// Returns a vec2 of random numbers between min and max
/// </summary>
/// <param name="min">Minimum value (default 0)</param>
/// <param name="max">Maximum value (default 1)</param>
/// <returns>Random vec2 in [min, max] (default [0, 1])</returns>
static const glm::vec2 GetRandVec2(float min = 0.0f, float max = 1.0f)
{
    return glm::vec2(GetRandFloat(min, max), GetRandFloat(min, max));
}

/// <summary>
/// Returns a vec3 of random numbers between min and max
/// </summary>
/// <param name="min">Minimum value (default 0)</param>
/// <param name="max">Maximum value (default 1)</param>
/// <returns>Random vec3 in [min, max] (default [0, 1])</returns>
static const glm::vec3 GetRandVec3(float min = 0.0f, float max = 1.0f)
{
    return glm::vec3(GetRandFloat(min, max), GetRandFloat(min, max), GetRandFloat(min, max));
}

/// <summary>
/// Returns a vec4 of random numbers between min and max
/// </summary>
/// <param name="min">Minimum value (default 0)</param>
/// <param name="max">Maximum value (default 1)</param>
/// <returns>Random vec4 in [min, max] (default [0, 1])</returns>
static const glm::vec4 GetRandVec4(float min = 0.0f, float max = 1.0f)
{
    return glm::vec4(GetRandFloat(min, max), GetRandFloat(min, max), GetRandFloat(min, max), GetRandFloat(min, max));
}

/// <summary>
/// Converts the given vec3 to its hexadecimal equivalent
/// </summary>
/// <param name="vec">Vec3 to convert</param>
/// <returns>Hex value</returns>
static const std::string Vec3ToHex(const glm::vec3& vec)
{
    int r = Clamp((int)round(vec.r * 255), 0, 255);
    int g = Clamp((int)round(vec.g * 255), 0, 255);
    int b = Clamp((int)round(vec.b * 255), 0, 255);

    std::string hex = "#";
    hex += DecimalToHex(r);
    hex += DecimalToHex(g);
    hex += DecimalToHex(b);
    return hex;
}

static const glm::vec2 DegreesToDir(float degrees)
{
    return glm::normalize(glm::vec2(glm::cos(glm::radians(degrees)), glm::sin(glm::radians(degrees))));
}

static const std::string Vec2ToString(const glm::vec2& vec)
{
    std::ostringstream ss;
    ss << "[" << vec.x << ", " << vec.y << "]";
    return ss.str();
}

static const std::string Vec3ToString(const glm::vec3& vec)
{
    std::ostringstream ss;
    ss << "[" << vec.x << ", " << vec.y << ", " << vec.z << "]";
    return ss.str();
}

static const glm::vec2 Vec2FromFloats(const float vec[2])
{
    return glm::vec2(vec[0], vec[1]);
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
