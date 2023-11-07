#pragma once
#include "glIncludes.h"
#include "utils.h"
#include "files/config.h"

class Camera
{
private:
    float mFOV = 45.0f;
    float mNearClip = 0.1f;
    float mFarClip = 100.0f;

    glm::vec3 mPos = glm::vec3(0.0f);
    glm::vec3 mRot = glm::vec3(0.0f);
    glm::vec3 mPivot = glm::vec3(0.0f);
    glm::vec3 mTarget = glm::vec3(0.0f);

    glm::vec3 mDir = glm::vec3(0, 0, -1);
    glm::vec3 mUp = glm::vec3(0, 1, 0);
    glm::vec3 mRight = glm::vec3(1, 0, 0);

    float mOrthSize = 10.0f;
    bool mIsPerspective = true;
    bool mIsWireframe = false;

    glm::vec3 mBGColor = glm::vec3(0.6f);

    glm::mat4 mView = glm::mat4(1.0f);
    glm::mat4 mProjection = glm::mat4(1.0f);
    bool mRecalcView = true;
    bool mRecalcProjection = true;
    float mPrevAspect = 1.0f;

    // Calculates the basis of the camera
    void CalcBasis()
    {
        mDir = glm::normalize(glm::vec3(
            cos(mRot.y) * sin(mRot.x),
            sin(mRot.y),
            cos(mRot.y) * cos(mRot.x)
        ));

        // Right vector
        mRight = glm::normalize(glm::cross(mDir, glm::vec3(0.0f, 1.0f, 0.0f)));

        // Up vector : perpendicular to both direction and right
        mUp = glm::normalize(glm::cross(mRight, mDir));
        mTarget = mPos + mDir;

        mRecalcView = true;
    }
public:
    // Returns the position of the camera
    glm::vec3 GetPos() { return mPos; }
    // Returns the rotation of the camera
    glm::vec3 GetRot() { return mRot; }
    // Returns the direction of the camera
    glm::vec3 GetDir() { return mDir; }
    // Returns the camera's current look target
    glm::vec3 GetTarget() { return mTarget; }
    // Returns the camera's pivot
    glm::vec3 GetPivot() { return mPivot; }
    // Returns the background color of the camera
    glm::vec3 GetBGColor() { return mBGColor; }
    // Returns the orthographic size of the camera
    float GetOrthSize() { return mOrthSize; }
    // Returns the far clip plane of the camera
    float GetFarClip() { return mFarClip; }
    // Returns the near clip plane of the plane
    float GetNearClip() { return mNearClip; }
    // Returns the FOV of the camera
    float GetFOV() { return mFOV; }
    // Returns whether the camera is in perspective
    bool IsPerspective() { return mIsPerspective; }
    // Returns whether the camera is in wireframe
    bool IsWireframe() { return mIsWireframe; }

    // Sets the position of the camera
    void SetPos(const glm::vec3& pos) { mPos = pos; mRecalcView = true; }
    // Sets the orthographic size of the camera
    void SetOrthSize(float size) { mOrthSize = size; }
    // Sets the far clip plane of the camera
    void SetFarClip(float farClip) { mFarClip = farClip; }
    // Sets the near clip plane of the plane
    void SetNearClip(float nearClip) { mNearClip = nearClip; }
    // Sets the FOV of the camera
    void SetFOV(float fov) { mFOV = fov; }
    // Sets the pivot of the camera
    void SetPivot(const glm::vec3& pivot) { mPivot = pivot; }

    // Sets the camera to be perspective/orthographic
    void SetPerspective(bool isPerspective)
    {
        if (mIsPerspective == isPerspective)
            return;
        mIsPerspective = isPerspective;
        mRecalcProjection = true;
    }

    // Sets the camera to be/not be wireframe
    void SetWireframe(bool isWireframe)
    {
        if (mIsWireframe == isWireframe)
            return;
        mIsWireframe = isWireframe;
        UpdateWireframe();
    }

    // Returns the view matrix of the camera
    const glm::mat4& GetView()
    {
        if (mRecalcView)
        {
            mTarget = mPos + mDir;
            mView = glm::lookAt(mPos, mTarget, mUp);
            mRecalcView = false;
        }
        return mView;
    }
    
    // Returns the projection matrix of the camera
    const glm::mat4& GetProjection(float aspect)
    {
        // Only recalculate projection matrix if needed
        if (mRecalcProjection || aspect != mPrevAspect)
        {
            // Projection
            glm::mat4 projection = glm::mat4(1.0f);

            // Perspective projection
            if (mIsPerspective) 
            {
                projection = glm::perspective(glm::radians(+mFOV), aspect, +mNearClip, +mFarClip);
            }
            // Orthographic projection
            else 
            {
                projection = glm::ortho(-mOrthSize * aspect, +mOrthSize * aspect, -mOrthSize, +mOrthSize, +mNearClip, +mFarClip);
            }
            mProjection = projection;
            mRecalcProjection = false;
            mPrevAspect = aspect;
        }
        return mProjection;
    }

    // Sets the rotation of the camera to be in the given direction
    void LookAt(const glm::vec3& targetPos)
    {
        mTarget = targetPos;
        glm::vec3 lookDir = glm::normalize(targetPos - mPos);
        mRot = glm::vec3(atan2(lookDir.x, lookDir.z), asin(lookDir.y), 0.0f);
        CalcBasis();
    }

    // Translates the camera by the given delta along the basis of the camera
    void Translate(const glm::vec3& delta)
    {
        mPos += + mDir * delta.x + mRight * delta.y + mUp * delta.z;
        mRecalcView = true;
    }

    // Rotates the camera by the given euler angles
    void Rotate(const glm::vec3& delta)
    {
        mRot = glm::vec3(mRot.x + delta.x, glm::clamp(mRot.y + delta.y, -PI * 0.499f, PI * 0.499f), mRot.z);
        CalcBasis();
    }

    // Enables or disables wireframe
    void UpdateWireframe()
    {
        if (mIsWireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            // Disable culling
            glDisable(GL_CULL_FACE);
            glDisable(GL_DEPTH_TEST);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            // Enable culling
            glEnable(GL_CULL_FACE);

            // Enable depth buffer
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
    }

    /// <summary>
    /// Constructs a camera from the given parameters.
    /// </summary>
    /// <param name="fov">Field of View of perspective camera</param>
    /// <param name="nearClip">Near clip plane</param>
    /// <param name="farClip">Far clip plane</param>
    /// <param name="pos">Position of camera</param>
    /// <param name="dir">Direction that camera faces (not normalized)</param>
    /// <param name="up">Up vector of the camera</param>
    /// <param name="color">Background color displayed by the camera</param>
    /// <param name="orthSize">Orthogonal size of orth camera</param>
    /// <param name="isPerspective">Whether the camera is in perspective or orthogonal view mode</param>
    /// <param name="isWireframe">Whether the camera is rendering in wireframe or not</param>
    Camera(float fov, float nearClip, float farClip, 
        const glm::vec3& pos, const glm::vec3& dir, const glm::vec3& up, const glm::vec3& color, 
        float orthSize = 10.0f, bool isPerspective = true, bool isWireframe = false)
        : mFOV(fov), mNearClip(nearClip), mFarClip(farClip), mPos(pos), mDir(dir), mUp(up), 
        mPivot(pos + dir), mTarget(pos + dir), mOrthSize(orthSize), mIsPerspective(isPerspective), mIsWireframe(isWireframe), mBGColor(color)
    {
        LookAt(mPivot);
        UpdateWireframe();
    }

    /// <summary>
    /// Constructs a camera from the given config
    /// </summary>
    /// <param name="actionStack">Global action stack</param>
    /// <param name="config">Config to build camera from</param>
    Camera(Config* config)
        : Camera(config->GetFloat("fov"), config->GetFloat("nearClip"), config->GetFloat("farClip"),
            config->GetVec("pos"), config->GetVec("dir"), config->GetVec("up"), config->GetVec("bgColor"), 
            config->GetFloat("size"), config->GetBool("perspective"), config->GetBool("wireframe"))
    {}
};