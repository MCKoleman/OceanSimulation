#include "ocean.h"

Ocean::Ocean(Shader* shader, int dim, float spectrumHeight, const glm::vec2& wind, float length, bool geom)
	: mShader(shader), mDim(dim), mDimPlusOne(dim+1), mSpectrumHeight(spectrumHeight), mWind(wind), mLength(length), mGeometry(geom)
{
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mEBO);

	Generate(dim, length, geom);
}

Ocean::~Ocean()
{
	if (mHTilde)
		delete[] mHTilde;
	if (mHTildeSlopeX)
		delete[] mHTildeSlopeX;
	if (mHTildeSlopeZ)
		delete[] mHTildeSlopeZ;
	if (mHTildeDX)
		delete[] mHTildeDX;
	if (mHTildeDZ)
		delete[] mHTildeDZ;
	if (mFFT)
		delete mFFT;
}

void Ocean::Generate(int dim, float length, bool geom)
{
	mDim = dim;
	mDimPlusOne = dim + 1;
	mLength = length;
	mGeometry = geom;

	mHTilde = new Complex[mDim * mDim];
	mHTildeSlopeX = new Complex[mDim * mDim];
	mHTildeSlopeZ = new Complex[mDim * mDim];
	mHTildeDX = new Complex[mDim * mDim];
	mHTildeDZ = new Complex[mDim * mDim];
	mFFT = new FFT(mDim);

	mVertices.clear();
	mIndices.clear();

	for (int mPrime = 0; mPrime < mDimPlusOne; mPrime++)
	{
		for (int nPrime = 0; nPrime < mDimPlusOne; nPrime++)
		{
			Complex hTilde0 = HTilde0(nPrime, mPrime);
			Complex hTilde0mkConj = HTilde0(-nPrime, -mPrime).Conjugate();

			glm::vec3 pos = glm::vec3((nPrime - mDim / 2.0f) * mLength / mDim, 0.0f, (mPrime - mDim / 2.0f) * mLength / mDim);
			glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);
			mVertices.push_back(OceanVertex(pos, normal, hTilde0, hTilde0mkConj));
		}
	}

	for (int mPrime = 0; mPrime < mDim; mPrime++)
	{
		for (int nPrime = 0; nPrime < mDim; nPrime++)
		{
			int index = mPrime * mDimPlusOne + nPrime;

			if (mGeometry)
			{
				// Make 3 lines
				mIndices.push_back(index);
				mIndices.push_back(index + 1);
				mIndices.push_back(index);
				mIndices.push_back(index + mDimPlusOne);
				mIndices.push_back(index);
				mIndices.push_back(index + mDimPlusOne + 1);

				// Make a last line to close out edges
				if (nPrime == mDim - 1)
				{
					mIndices.push_back(index + 1);
					mIndices.push_back(index + mDimPlusOne + 1);
				}
				if (mPrime == mDim - 1)
				{
					mIndices.push_back(index + mDimPlusOne);
					mIndices.push_back(index + mDimPlusOne + 1);
				}
			}
			// If not geometry, then surface
			else
			{
				// Make two triangles
				mIndices.push_back(index);
				mIndices.push_back(index + mDimPlusOne);
				mIndices.push_back(index + mDimPlusOne + 1);
				mIndices.push_back(index);
				mIndices.push_back(index + mDimPlusOne + 1);
				mIndices.push_back(index + 1);
			}
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, (unsigned int)mVertices.size() * sizeof(*mVertices.data()), mVertices.data(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (unsigned int)mIndices.size() * sizeof(*mIndices.data()), mIndices.data(), GL_STATIC_DRAW);

	glBindVertexArray(mVAO);
	// vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(OceanVertex), 0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(OceanVertex), (float*)(3 * sizeof(float)));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(OceanVertex), (float*)(6 * sizeof(float)));

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

float Ocean::GetSpectrumHeight()
{
	return mSpectrumHeight;
}

void Ocean::SetSpectrumHeight(float newSpectrum)
{
	mSpectrumHeight = newSpectrum;
}

glm::vec2 Ocean::GetWind()
{
	return mWind;
}

void Ocean::SetWind(const glm::vec2& newWind)
{
	mWind = newWind;
}

// Returns the ocean's dimension
int Ocean::GetDimension()
{
	return mDim;
}

// Returns the ocean's length
float Ocean::GetLength()
{
	return mLength;
}

// Returns whether the ocean is geometry
bool Ocean::IsGeometry()
{
	return mGeometry;
}

// Releases resources
void Ocean::Release()
{
	glDeleteBuffers(1, &mEBO);
	glDeleteBuffers(1, &mVBO);
}

// Deep water
float Ocean::Dispersion(int nPrime, int mPrime)
{
	// 2pi / 200
	float w0 = PI * 0.01f;
	float kx = PI * (2 * nPrime - mDim) / mLength;
	float kz = PI * (2 * mPrime - mDim) / mLength;
	return floor(sqrt(G * sqrt(kx * kx + kz * kz)) / w0) * w0;
}

// Phillips Spectrum
float Ocean::Phillips(int nPrime, int mPrime)
{
	/* The variable names of this function are as close to the
	* mathematical formula of the Phillips Spectrum as defined 
	* in the original paper
	* https://arxiv.org/pdf/1912.03945.pdf
	*/

	glm::vec2 k = glm::vec2(PI * (2 * nPrime - mDim) / mLength, PI * (2 * mPrime - mDim) / mLength);
	float kLen = glm::length(k);

	// The Phillips Spectrum is zero near zero
	if (kLen < 0.000001)
		return 0.0f;

	float kLen2 = kLen * kLen;
	float kLen4 = kLen2 * kLen2;

	float kDotW = glm::dot(k, mWind);
	float kDotW2 = kDotW * kDotW;

	float wLen = glm::length(mWind);
	float L = wLen * wLen / G;
	float L2 = L * L;

	float damping = 0.001f;
	float l2 = L2 * damping * damping;

	return mSpectrumHeight * exp(-1.0f / (kLen2 * L2)) / kLen4 * kDotW2 * exp(-kLen2 * l2);
}

// Returns the HTilde initial
Complex Ocean::HTilde0(int nPrime, int mPrime)
{
	Complex rand = GaussianRandomVariable();
	return rand * sqrt(Phillips(nPrime, mPrime) / 2.0f);
}

// Returns the HTilde
Complex Ocean::HTilde(float t, int nPrime, int mPrime)
{
	int index = mPrime * mDimPlusOne + nPrime;

	Complex hTilde0 = Complex(mVertices[index].hTilde0);
	Complex hTilde0mkConj = Complex(mVertices[index].hTilde0mkConj);

	float omegaT = Dispersion(nPrime, mPrime) * t;

	float cosOmegaT = cos(omegaT);
	float sinOmegaT = sin(omegaT);

	Complex c0 = Complex(cosOmegaT, sinOmegaT);
	Complex c1 = Complex(cosOmegaT, -sinOmegaT);

	Complex res = hTilde0 * c0 + hTilde0mkConj * c1;

	return hTilde0 * c0 + hTilde0mkConj * c1;
}

// Returns the 
ComplexVectorNormal Ocean::GetComplexNormal(const glm::vec2& x, float t)
{
	Complex height = Complex(0.0f, 0.0f);
	glm::vec2 displacement = glm::vec2(0.0f, 0.0f);
	glm::vec3 normal = glm::vec3(0.0f, 0.0f, 0.0f);

	for (int mPrime = 0; mPrime < mDim; mPrime++)
	{
		float kz = 2.0f * PI * (mPrime - mDim / 2.0f) / mLength;
		for (int nPrime = 0; nPrime < mDim; nPrime++)
		{
			float kx = 2.0f * PI * (nPrime - mDim / 2.0f) / mLength;
			glm::vec2 k = glm::vec2(kx, kz);
			float kLen = glm::length(k);
			float kDotX = glm::dot(k, x);
			Complex c = Complex(cos(kDotX), sin(kDotX));
			Complex hTildeC = HTilde(t, nPrime, mPrime) * c;

			height = height + hTildeC;
			normal = normal + glm::vec3(-kx * hTildeC.imaginary, 0.0f, -kz * hTildeC.imaginary);

			if (kLen < 0.000001f)
				continue;

			displacement = displacement + glm::vec2(kx / kLen * hTildeC.imaginary, kz / kLen * hTildeC.imaginary);
		}
	}

	normal = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f) - normal);

	return ComplexVectorNormal(height, displacement, normal);
}

void Ocean::EvaluateWaves(float t)
{
	float lambda = -1.0f;

	for (int mPrime = 0; mPrime < mDim; mPrime++)
	{
		for (int nPrime = 0; nPrime < mDim; nPrime++)
		{
			int index = mPrime * mDimPlusOne + nPrime;
			glm::vec2 x = glm::vec2(mVertices[index].position.x, mVertices[index].position.z);
			ComplexVectorNormal cvn = GetComplexNormal(x, t);

			mVertices[index].position = GetPositionDisplacement(mVertices[index], cvn, lambda);
			mVertices[index].normal = cvn.normal;
			
			if (nPrime == 0 && mPrime == 0)
			{
				mVertices[index + mDim + mDimPlusOne * mDim].position = GetPositionDisplacement(mVertices[index + mDim + mDimPlusOne * mDim], cvn, lambda);
				mVertices[index + mDim + mDimPlusOne * mDim].normal = cvn.normal;
			}
			if (nPrime == 0)
			{
				mVertices[index + mDim].position = GetPositionDisplacement(mVertices[index + mDim], cvn, lambda);
				mVertices[index + mDim].normal = cvn.normal;
			}
			if (mPrime == 0)
			{
				mVertices[index + mDimPlusOne * mDim].position = GetPositionDisplacement(mVertices[index + mDimPlusOne * mDim], cvn, lambda);
				mVertices[index + mDimPlusOne * mDim].normal = cvn.normal;
			}
		}
	}
}

void Ocean::EvaluateWavesFFT(float t)
{
	float lambda = -1.0f;

	for (int mPrime = 0; mPrime < mDim; mPrime++)
	{
		float kz = PI * (2.0f * mPrime - mDim) / mLength;
		for (int nPrime = 0; nPrime < mDim; nPrime++) {
			float kx = PI * (2 * nPrime - mDim) / mLength;
			float len = sqrt(kx * kx + kz * kz);
			int index = mPrime * mDim + nPrime;

			mHTilde[index] = HTilde(t, nPrime, mPrime);
			mHTildeSlopeX[index] = mHTilde[index] * Complex(0, kx);
			mHTildeSlopeZ[index] = mHTilde[index] * Complex(0, kz);
			if (len < 0.000001f) {
				mHTildeDX[index] = Complex(0.0f, 0.0f);
				mHTildeDZ[index] = Complex(0.0f, 0.0f);
			}
			else {
				mHTildeDX[index] = mHTilde[index] * Complex(0, -kx / len);
				mHTildeDZ[index] = mHTilde[index] * Complex(0, -kz / len);
			}
		}
	}

	for (int mPrime = 0; mPrime < mDim; mPrime++)
	{
		mFFT->GetFFT(mHTilde, mHTilde, 1, mPrime * mDim);
		mFFT->GetFFT(mHTildeSlopeX, mHTildeSlopeX, 1, mPrime * mDim);
		mFFT->GetFFT(mHTildeSlopeZ, mHTildeSlopeZ, 1, mPrime * mDim);
		mFFT->GetFFT(mHTildeDX, mHTildeDX, 1, mPrime * mDim);
		mFFT->GetFFT(mHTildeDZ, mHTildeDZ, 1, mPrime * mDim);
	}
	for (int nPrime = 0; nPrime < mDim; nPrime++)
	{
		mFFT->GetFFT(mHTilde, mHTilde, mDim, nPrime);
		mFFT->GetFFT(mHTildeSlopeX, mHTildeSlopeX, mDim, nPrime);
		mFFT->GetFFT(mHTildeSlopeZ, mHTildeSlopeZ, mDim, nPrime);
		mFFT->GetFFT(mHTildeDX, mHTildeDX, mDim, nPrime);
		mFFT->GetFFT(mHTildeDZ, mHTildeDZ, mDim, nPrime);
	}

	int sign;
	float signs[] = { 1.0f, -1.0f };
	for (int mPrime = 0; mPrime < mDim; mPrime++)
	{
		for (int nPrime = 0; nPrime < mDim; nPrime++)
		{
			int index = mPrime * mDim + nPrime;     // index into mHTilde..
			int index1 = mPrime * mDimPlusOne + nPrime;    // index into mVertices

			sign = signs[(nPrime + mPrime) & 1];

			mHTilde[index] = mHTilde[index] * sign;

			// height
			mVertices[index1].position.y = mHTilde[index].real;

			// displacement
			mHTildeDX[index] = mHTildeDX[index] * sign;
			mHTildeDZ[index] = mHTildeDZ[index] * sign;
			mVertices[index1].position = GetPositionDisplacement(mVertices[index1], mHTilde[index], mHTildeDX[index], mHTildeDZ[index], lambda);

			// normal
			mHTildeSlopeX[index] = mHTildeSlopeX[index] * sign;
			mHTildeSlopeZ[index] = mHTildeSlopeZ[index] * sign;
			glm::vec3 normal = glm::normalize(glm::vec3(0.0f - mHTildeSlopeX[index].real, 1.0f, 0.0f - mHTildeSlopeZ[index].real));
			mVertices[index1].normal = normal;

			// for tiling
			if (nPrime == 0 && mPrime == 0)
			{
				mVertices[index1 + mDim + mDimPlusOne * mDim].position = GetPositionDisplacement(mVertices[index1 + mDim + mDimPlusOne * mDim], mHTilde[index], mHTildeDX[index], mHTildeDZ[index], lambda);
				mVertices[index1 + mDim + mDimPlusOne * mDim].normal = normal;
			}
			if (nPrime == 0)
			{
				mVertices[index1 + mDim].position = GetPositionDisplacement(mVertices[index1 + mDim], mHTilde[index], mHTildeDX[index], mHTildeDZ[index], lambda);
				mVertices[index1 + mDim].normal = normal;
			}
			if (mPrime == 0)
			{
				mVertices[index1 + mDimPlusOne * mDim].position = GetPositionDisplacement(mVertices[index1 + mDimPlusOne * mDim], mHTilde[index], mHTildeDX[index], mHTildeDZ[index], lambda);
				mVertices[index1 + mDimPlusOne * mDim].normal = normal;
			}
		}
	}
}

void Ocean::Draw(float t, const glm::mat4& view, const glm::mat4& proj, const glm::mat4& model, Light* light, Material* material, bool useFFT)
{
	if (useFFT)
	{
		EvaluateWavesFFT(t);
	}
	else
	{
		EvaluateWaves(t);
	}

	mShader->Use();
	mShader->SetMat4("View", view);
	mShader->SetMat4("Projection", proj);
	mShader->SetMat4("Model", model);
	mShader->SetVec3("fogColor", mFogColor);
	light->UpdateShader(mShader);
	material->UpdateShader(mShader);

	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(OceanVertex) * mDimPlusOne * mDimPlusOne, mVertices.data());
	glEnableVertexAttribArray(mShader->GetAttributeLocation("aPos"));
	glVertexAttribPointer(mShader->GetAttributeLocation("aPos"), 3, GL_FLOAT, GL_FALSE, sizeof(OceanVertex), 0);
	glEnableVertexAttribArray(mShader->GetAttributeLocation("aNorm"));
	glVertexAttribPointer(mShader->GetAttributeLocation("aNorm"), 3, GL_FLOAT, GL_FALSE, sizeof(OceanVertex), (char*)NULL + 12);
	glEnableVertexAttribArray(mShader->GetAttributeLocation("aTexCoord"));
	glVertexAttribPointer(mShader->GetAttributeLocation("aTexCoord"), 2, GL_FLOAT, GL_FALSE, sizeof(OceanVertex), (char*)NULL + 24);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	for (int j = 0; j < 10; j++)
	{
		for (int i = 0; i < 10; i++)
		{
			glm::mat4 indModel = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f));
			indModel = glm::translate(indModel, glm::vec3(mLength * i, 0.0, mLength * -j));
			mShader->SetMat4("Model", indModel);
			glDrawElements(mGeometry ? GL_LINES : GL_TRIANGLES, (unsigned int)mIndices.size(), GL_UNSIGNED_INT, 0);
		}
	}
	glBindVertexArray(0);
}