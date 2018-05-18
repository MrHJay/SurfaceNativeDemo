//
// Created by jay on 18-5-9.
//

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <common.h>
#include "FrameRect.h"
#include "GLUtil.h"
#include "ZoomDrawable2D.h"

egl::FrameRect::FrameRect(Texture2DProgram *texture2DProgram,
                          FontTextureProgram *fontTextureProgram, Drawable2D *drawable2D)
        : mTexture2DProgram(texture2DProgram),
          mFontTextureProgram(fontTextureProgram),
          mDrawable2D(drawable2D) {

    mIdentityMatrix = new glm::mat4();
    mMvpMatrix = *mIdentityMatrix;
    tmpMatrix = glm::value_ptr(mMvpMatrix);
}

egl::FrameRect::~FrameRect() {
    if (mTexture2DProgram) {
        delete mTexture2DProgram;
    }
    if (mDrawable2D) {
        delete mDrawable2D;
    }
    delete mIdentityMatrix;
}

GLuint egl::FrameRect::createTexture() {
    return mTexture2DProgram->createTexture();
}

void egl::FrameRect::drawFrame(GLuint textureId, float *texMatrix) {

//    GLUtil::logMat4("frame",tmpMatrix);
    mTexture2DProgram->draw(tmpMatrix, mDrawable2D->getVertexArray(), 0,
                            mDrawable2D->getVertexCount(), mDrawable2D->getCoordsPerVertex(),
                            mDrawable2D->getVertexStride(), texMatrix,
                            mDrawable2D->getTexCoordArray(), textureId,
                            mDrawable2D->getTexCoordStride());
}

void egl::FrameRect::scale(float scale) {

    // translate
    mMvpMatrix = glm::translate(*mIdentityMatrix, glm::vec3(mWidth / 2, mHeight / 2, 0.0f));
//    mMvpMatrix = *mIdentityMatrix;

    // scale
    mMvpMatrix = glm::scale(mMvpMatrix,
                            glm::vec3(mWidth * 0.5 * scale, mHeight * 0.5 * scale, 1.0f));
//    mMvpMatrix = glm::scale(mMvpMatrix, glm::vec3(mWidth * 0.5, mHeight * 0.5, 1.0f));
    LOG_IF("scale", "scale = %f", scale);

    // projection
    mMvpMatrix = mProjectionMatrix * mMvpMatrix;
    tmpMatrix = glm::value_ptr(mMvpMatrix);
    GLUtil::logMat4("projection", tmpMatrix);
}

void egl::FrameRect::projection(float width, float height) {
    this->mWidth = width;
    this->mHeight = height;
    mProjectionMatrix = glm::ortho(0.0f, width, 0.0f, height, -1.0f, 1.0f);
}

void egl::FrameRect::resetScale() {
    tmpMatrix = glm::value_ptr(*mIdentityMatrix);
}
