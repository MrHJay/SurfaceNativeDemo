//
// Created by jay on 18-5-17.
//

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Transformer.h"

namespace egl {

    const char *TAG_TRANSFORM = "Transform";

    Transformer::Transformer() {
        mIdentityMatrix = new glm::mat4();
        mScale = 1.0f;
        mZoom = 1.0f;
        mCenterX = 0;
        mCenterY = 0;
        mRecalMv = true;
        mRecalProjection = true;
    }

    Transformer::~Transformer() {
        if (mIdentityMatrix) {
            delete mIdentityMatrix;
        }
        if (mMvMatrix) {
            delete mMvMatrix;
        }
        if (mProjectionMatrix) {
            delete mProjectionMatrix;
        }
    }

    void Transformer::resize(float width, float height) {
        this->mWidth = width;
        this->mHeight = height;
        mRecalProjection = true;
        mRecalMv = true;
    }

    void Transformer::scale(float scale) {
        this->mScale = scale;
        this->mRecalMv = true;
    }

    void Transformer::zoom(float zoom) {
        mCenterX = 0.5f;
        mCenterY = 0.5f;
        mZoom = zoom;
    }

    void Transformer::zoom(float centerX, float centerY, float zoom) {
        mCenterX = centerX;
        mCenterY = centerY;
        mZoom = zoom;
    }

    float Transformer::getWidth() {
        return mWidth;
    }

    float Transformer::getHeight() {
        return mHeight;
    }

    const GLfloat *Transformer::transformedArrayV2(GLfloat *dst, GLfloat *src, int length,
                                                   int stride, bool normalized) {
        glm::mat3 transformMatrix;
        glm::vec3 vec;

        float centerX, centerY;
        if (normalized) {
            centerX = mCenterX * mWidth;
            centerY = mCenterY * mHeight;
        } else {
            centerX = mCenterX;
            centerY = mCenterY;
        }

        transformMatrix = glm::translate(transformMatrix, glm::vec2(centerX, centerY));

        transformMatrix = glm::scale(transformMatrix, glm::vec2(mZoom, mZoom));

        transformMatrix = glm::translate(transformMatrix, glm::vec2(-centerX, -centerY));

        // LOG_IF(TAG_TRANSFORM, "zoom=%f, x=%f, y=%f", mZoom, centerX, centerY);

        for (int i = 0; i < length; i += stride) {
            vec = transformMatrix * glm::vec3(src[i], src[i + 1], 1.0f);
            float *xyz = glm::value_ptr(vec);
            dst[i] = xyz[0];
            dst[i + 1] = xyz[1];
            // LOG_DF(TAG_TRANSFORM, "index=%d x=%5f, y=%5f", i, xyz[0], xyz[1]);
        }
        return dst;
    }

    const GLfloat *Transformer::transformedArrayV3(GLfloat *dst, GLfloat *src, int length, int stride,
                                             bool normalized) {
        glm::mat4 transformMatrix;
        glm::vec4 vec;

        float centerX, centerY;
        if (normalized) {
            centerX = mCenterX;
            centerY = mCenterY;
        } else {
            centerX = mCenterX / mWidth;
            centerY = mCenterY / mHeight;
        }

        transformMatrix = glm::translate(transformMatrix, glm::vec3(centerX, centerY, 0.0f));

        transformMatrix = glm::scale(transformMatrix, glm::vec3(mZoom, mZoom, 0.0f));

        transformMatrix = glm::translate(transformMatrix, glm::vec3(-centerX, -centerY, 0.0f));

        // LOG_IF(TAG_TRANSFORM, "zoom=%f, x=%f, y=%f", mZoom, centerX, centerY);

        for (int i = 0; i < length; i += stride) {
            vec = transformMatrix * glm::vec4(src[i], src[i + 1], src[i + 2], 1.0f);
            float *xyzw = glm::value_ptr(vec);
            dst[i] = xyzw[0];
            dst[i + 1] = xyzw[1];
            dst[i + 2] = xyzw[2];
            // LOG_DF(TAG_TRANSFORM, "index=%d x=%5f, y=%5f, z=%5f", i, xyzw[0], xyzw[1], xyzw[2]);
        }
        return dst;
    }

    glm::mat4 Transformer::getModelViewMatrix() {
        recalMv();
        return *mMvMatrix;
    }

    glm::mat4 Transformer::getProjectionMatrix() {
        recalProjection();
        return *mProjectionMatrix;
    }

    glm::mat4 Transformer::getIdentityMatrix() {
        return *mIdentityMatrix;
    }

    void Transformer::recalMv() {
        if (mRecalMv) {
            // translate
            glm::mat4 mvMatrix = glm::translate(*mIdentityMatrix,
                                                glm::vec3(mWidth / 2, mHeight / 2, 0.0f));

            // scale
            mvMatrix = glm::scale(mvMatrix,
                                  glm::vec3(mWidth * 0.5 * mScale, mHeight * 0.5 * mScale, 1.0f));
            this->mMvMatrix = new glm::mat4(mvMatrix);
            mRecalMv = false;
        }
    }

    void Transformer::recalProjection() {
        if (mRecalProjection) {
            glm::mat4 projectionMatrix = glm::ortho(0.0f, mWidth, 0.0f, mHeight, -1.0f, 1.0f);
            this->mProjectionMatrix = new glm::mat4(projectionMatrix);
            mRecalProjection = false;
        }
    }

};