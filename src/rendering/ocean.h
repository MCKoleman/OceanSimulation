#pragma once
#include "glIncludes.h"
#include "utils.h"
#include "complex.h"
#include "rendering/fft.h"
#include "rendering/shader.h"
#include "rendering/material.h"
#include "rendering/light.h"
#include <vector>

/// <summary>
/// Complex vector normal for wave displacement
/// </summary>
struct ComplexVectorNormal
{
	/// <summary>
	/// Wave height
	/// </summary>
	Complex height;

	/// <summary>
	/// Displacement
	/// </summary>
	glm::vec2 displacement;

	/// <summary>
	/// Normal
	/// </summary>
	glm::vec3 normal;

	/// <summary>
	/// Create complex vector normal
	/// </summary>
	/// <param name="_height"></param>
	/// <param name="_displacement"></param>
	/// <param name="_normal"></param>
	explicit ComplexVectorNormal(Complex _height, glm::vec2 _displacement, glm::vec3 _normal)
		: height(_height), displacement(_displacement), normal(_normal)
	{}
};

/// <summary>
/// Vertex for FFT oceans
/// </summary>
struct OceanVertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 originalPosition;
	Complex hTilde0;
	Complex hTilde0mkConj;

	explicit OceanVertex(glm::vec3 _position, glm::vec3 _normal, Complex _hTilde0, Complex _hTilde0mkConj)
		: position(_position), originalPosition(_position), normal(_normal), hTilde0(_hTilde0), hTilde0mkConj(_hTilde0mkConj)
	{}
};

/// <summary>
/// Class that represents a FFT ocean
/// </summary>
class Ocean
{
private:
	bool mGeometry = false;
	int mDim;
	int mDimPlusOne;

	float mSpectrumHeight;
	glm::vec2 mWind;
	float mLength;

	Complex* mHTilde;
	Complex* mHTildeSlopeX;
	Complex* mHTildeSlopeZ;
	Complex* mHTildeDX;
	Complex* mHTildeDZ;

	FFT* mFFT;

	std::vector<OceanVertex> mVertices;
	std::vector<unsigned int> mIndices;
	
	unsigned int mVAO;
	unsigned int mVBO;
	unsigned int mEBO;

	Material* mMaterial = nullptr;
	Shader* mShader = nullptr;

	glm::vec3 mFogColor = glm::vec3(0.25f, 0.75f, 0.65f);

	const float G = 9.81f;

	static glm::vec3 GetPositionDisplacement(const OceanVertex& vert, const ComplexVectorNormal& cvn, float lambda)
	{
		glm::vec3 oPos = vert.originalPosition;
		return glm::vec3(oPos.x + lambda * cvn.displacement.x, cvn.height.real, oPos.z + lambda * cvn.displacement.y);
	}

	static glm::vec3 GetPositionDisplacement(const OceanVertex& vert, const Complex& c, const Complex& cx, const Complex& cz, float lambda)
	{
		return glm::vec3(vert.originalPosition.x + cx.real * lambda, c.real, vert.originalPosition.z + cz.real * lambda);
	}
public:
	Ocean(Shader* shader, int dim, float spectrumHeight, const glm::vec2& wind, float length, bool geom);

	~Ocean();

	void Generate(int dim, float length, bool geom);

	float GetSpectrumHeight();

	void SetSpectrumHeight(float newSpectrum);

	glm::vec2 GetWind();

	void SetWind(const glm::vec2& newWind);

	// Returns the ocean's dimension
	int GetDimension();

	// Returns the ocean's length
	float GetLength();

	// Returns whether the ocean is geometry
	bool IsGeometry();

	// Releases resources
	void Release();
	
	// Deep water
	float Dispersion(int nPrime, int mPrime);

	// Phillips Spectrum
	float Phillips(int nPrime, int mPrime);

	// Returns the HTilde initial
	Complex HTilde0(int nPrime, int mPrime);

	// Returns the HTilde
	Complex HTilde(float t, int nPrime, int mPrime);

	// Returns the 
	ComplexVectorNormal GetComplexNormal(const glm::vec2& x, float t);

	void EvaluateWaves(float t);

	void EvaluateWavesFFT(float t);

	void Draw(float t, const glm::mat4& view, const glm::mat4& proj, const glm::mat4& model, Light* light, Material* material, bool useFFT = false);
};