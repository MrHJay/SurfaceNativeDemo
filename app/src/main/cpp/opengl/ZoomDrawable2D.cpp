//
// Created by jay on 18-5-14.
//

#include <string>
#include <glm/gtx/string_cast.hpp>
#include <glm/detail/type_mat.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include "common.h"
#include "ZoomDrawable2D.h"

egl::ZoomDrawable2D::ZoomDrawable2D(egl::Prefab type) : Drawable2D(type) {
    mZoom = 1.0f;
    mCenterX = getTexCenterX();
    mCenterY = getTexCenterY();
    mRecalculate = true;
    int length = Drawable2D::getTexCount() * 2;
    mZoomTexCoorArray = new GLfloat[length];
}

egl::ZoomDrawable2D::~ZoomDrawable2D() {
    if (mZoomTexCoorArray) {
        delete mZoomTexCoorArray;
    }
}

const GLfloat *egl::ZoomDrawable2D::getTexCoordArray() {
    if (mRecalculate) {
        int length = Drawable2D::getTexCount() * 2;
        const GLfloat *texArrays = Drawable2D::getTexCoordArray();

        glm::mat3 transformMatrix;
        glm::vec3 vec;
//        LOG_IF("Zoom","transform origin = %s", glm::to_string(transformMatrix).data());

        transformMatrix = glm::translate(transformMatrix, glm::vec2(mCenterX, mCenterY));

        transformMatrix = glm::scale(transformMatrix, glm::vec2(1 / mZoom, 1 / mZoom));

        transformMatrix = glm::translate(transformMatrix, glm::vec2(-mCenterX, -mCenterY));

        LOG_IF("Zoom", "scale=%f, x=%f, y=%f", mZoom, mCenterX, mCenterY);
        LOG_IF("Zoom", "transform = %s", glm::to_string(transformMatrix).data());

        for (int i = 0; i < length; i += 2) {
            vec = transformMatrix * glm::vec3(texArrays[i], texArrays[i + 1], 1.0f);
            float *xyz = glm::value_ptr(vec);
            mZoomTexCoorArray[i] = xyz[0];
            mZoomTexCoorArray[i + 1] = xyz[1];
            LOG_DF("Zoom", "index=%d x=%5f, y=%5f", i, xyz[0], xyz[1]);
        }
        mRecalculate = false;
    }
    return mZoomTexCoorArray;
}

void egl::ZoomDrawable2D::zoom(float zoom) {
    mZoom = zoom;
    mRecalculate = true;
}

void egl::ZoomDrawable2D::zoom(float x, float y, float zoom) {
    mZoom = zoom;
    mCenterX = x;
    mCenterY = y;
    mRecalculate = true;
}
