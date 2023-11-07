#pragma once
#include "rendering/iPrimitive.h"

class PPlane : public IPrimitive<glm::vec3, glm::uvec3>
{
public:
	// Draws the object to the screen
	void Draw(Shader* shader, State* state, const glm::mat4& viewProj, bool drawMats = false) override
	{
		// Don't draw disabled objects
		if (!mIsEnabled)
			return;

		// Set uniforms for this draw
		glm::mat4 modelMatrix = GetModelMatrix();
		glm::mat4 mvp = viewProj * modelMatrix;
		glm::mat3 normalModel = GetNormalModelMatrix();

		shader->SetMat4("MVP", mvp);
		shader->SetMat4("Model", modelMatrix);
		shader->SetMat3("NormalModel", normalModel);
		shader->SetVec3("material.ambient", glm::vec3(0.1f));
		shader->SetVec3("material.diffuse", glm::vec3(0.1f, 0.1f, 0.9f));
		shader->SetVec3("material.specular", glm::vec3(1.0f));
		shader->SetFloat("material.shininess", 8.0f);
		

		if (mDrawOver)
			glDisable(GL_DEPTH_TEST);
		glBindVertexArray(mVAO);
		if (mIsWireframe)
			glLineWidth(mLineWidth);
		glDrawElements(mIsWireframe ? GL_LINES : GL_TRIANGLES, (GLuint)mIndexLen, GL_UNSIGNED_INT, NULL);
		glBindVertexArray(0);
		glEnable(GL_DEPTH_TEST);
	}

	PPlane(const std::string& name = "", float size = 1.0f, int subDivisions = 1)
		: IPrimitive(name, 0.0f, false)
	{
        float unitSize = size / subDivisions;
        for (int x = -subDivisions; x <= subDivisions; x++)
        {
            for (int z = -subDivisions; z <= subDivisions; z++)
            {
                mVertices.push_back(unitSize * glm::vec3(x, 0, z));

                // Calculate indices
                int i = x + subDivisions;
                int j = z + subDivisions;
                unsigned int r1 = j * (2 * subDivisions + 1) + i;
                unsigned int r2 = (j + 1) * (2 * subDivisions + 1) + i;
                if (x < subDivisions && z < subDivisions)
                {
                    mIndices.push_back(glm::uvec3(r1, r2+1, r2));
                    mIndices.push_back(glm::uvec3(r2+1, r1, r1+1));
                }
            }
        }

        IPrimitive::GenBuffers();
	}
};