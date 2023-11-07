#pragma once
#include "rendering/iPrimitive.h"

class PPlane : public IPrimitive<glm::vec3, glm::uvec3>
{
public:
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