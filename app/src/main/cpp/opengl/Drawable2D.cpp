//
// Created by jay on 18-5-9.
//

#include <common.h>
#include "Drawable2D.h"

namespace egl {
    const GLfloat RECT_COORDS[] = {
            -0.5f, -0.5f,   // 0 bottom left
            0.5f, -0.5f,   // 1 bottom right
            -0.5f, 0.5f,   // 2 top left
            0.5f, 0.5f,   // 3 top right
    };
    const GLfloat RECT_TEX_COORDS[] = {
            0.0f, 1.0f,     // 0 bottom left
            1.0f, 1.0f,     // 1 bottom right
            0.0f, 0.0f,     // 2 top left
            1.0f, 0.0f      // 3 top right
    };
    const GLfloat FULL_RECT_COORDS[] = {
            -1.0f, -1.0f,   // 0 bottom left
            1.0f, -1.0f,   // 1 bottom right
            -1.0f, 1.0f,   // 2 top left
            1.0f, 1.0f,   // 3 top right
    };
    const GLfloat FULL_RECT_TEX_COORDS[] = {
            0.0f, 0.0f,     // 0 bottom left
            1.0f, 0.0f,     // 1 bottom right
            0.0f, 1.0f,     // 2 top left
            1.0f, 1.0f      // 3 top right
    };

    Drawable2D::Drawable2D(Prefab type) {
        switch (type) {
            case RECT:
                mVertexArray = RECT_COORDS;
                mCoordsPerVertex = 2;
                mVertexCount = ARRAY_LENGTH(RECT_COORDS) / mCoordsPerVertex;
                mTexCoordArray = RECT_TEX_COORDS;
                mCoordsPerTex = 2;
                mTexCount = ARRAY_LENGTH(RECT_TEX_COORDS) / mCoordsPerTex;
                break;
            case FULL_RECT:
                mVertexArray = FULL_RECT_COORDS;
                mCoordsPerVertex = 2;
                mVertexCount = ARRAY_LENGTH(FULL_RECT_COORDS) / mCoordsPerVertex;
                mCoordsPerTex = 2;
                mTexCoordArray = FULL_RECT_TEX_COORDS;
                mTexCount = ARRAY_LENGTH(FULL_RECT_TEX_COORDS) / mCoordsPerTex;
                break;
        }
        mVertexStride = mCoordsPerVertex * sizeof(GLfloat);
        mTexCoordStride = mCoordsPerTex * sizeof(GLfloat);
    }

    Drawable2D::~Drawable2D() {

    }

    const GLfloat *Drawable2D::getVertexArray() const {
        return mVertexArray;
    }

    const GLfloat *Drawable2D::getTexCoordArray() {
        return mTexCoordArray;
    }

    int Drawable2D::getCoordsPerVertex() const {
        return mCoordsPerVertex;
    }

    int Drawable2D::getVertexStride() const {
        return mVertexStride;
    }

    int Drawable2D::getVertexCount() const {
        return mVertexCount;
    }

    int Drawable2D::getTexCoordStride() const {
        return mTexCoordStride;
    }

    int Drawable2D::getCoordsPerTex() const {
        return mCoordsPerTex;
    }

    int Drawable2D::getTexCount() const {
        return mTexCount;
    }

    float Drawable2D::getTexCenterX() {
        return (mTexCoordArray[0] + mTexCoordArray[2] + mTexCoordArray[4] + mTexCoordArray[6]) / 4;
    }

    float Drawable2D::getTexCenterY() {
        return (mTexCoordArray[1] + mTexCoordArray[3] + mTexCoordArray[5] + mTexCoordArray[7]) / 4;
    }

    void Drawable2D::zoom(float zoom) {

    }

    void Drawable2D::zoom(float x, float y, float zoom) {

    }


}