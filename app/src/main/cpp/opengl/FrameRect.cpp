//
// Created by jay on 18-5-9.
//

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "FrameRect.h"
#include "GLUtil.h"
#include "ZoomDrawable2D.h"

egl::FrameRect::FrameRect(Texture2DProgram *texture2DProgram,
                          FontTextureProgram *fontTextureProgram, Drawable2D *drawable2D)
        : mTexture2DProgram(texture2DProgram),
          mFontTextureProgram(fontTextureProgram),
          mDrawable2D(drawable2D) {

    mTransformer = new Transformer();
}

egl::FrameRect::~FrameRect() {
    if (mTexture2DProgram) {
        delete mTexture2DProgram;
    }
    if (mDrawable2D) {
        delete mDrawable2D;
    }
    if (mTransformer) {
        delete mTransformer;
    }
}

GLuint egl::FrameRect::createTexture() {
    return mTexture2DProgram->createTexture();
}

void egl::FrameRect::drawFrame(GLuint textureId, float *texMatrix, long timestamp) {

//    GLUtil::logMat4("frame",tmpMatrix);
    mTexture2DProgram->draw(glm::value_ptr(mMvpMatrix), mDrawable2D->getVertexArray(), 0,
                            mDrawable2D->getVertexCount(), mDrawable2D->getCoordsPerVertex(),
                            mDrawable2D->getVertexStride(), texMatrix,
                            mDrawable2D->getTexCoordArray(), textureId,
                            mDrawable2D->getTexCoordStride());

    char time[32];
    strftime(time, sizeof(time), "%Y-%m-%d %H:%M:%S", localtime(&timestamp));
    mFontTextureProgram->drawText(time, mTransformer->getWidth() / 2,
                                  mTransformer->getHeight() / 2, mTransformer);
}

void egl::FrameRect::scale(float scale) {
    mTransformer->scale(scale);
    mMvpMatrix = mTransformer->getProjectionMatrix() * mTransformer->getModelViewMatrix();
}

void egl::FrameRect::projection(float width, float height) {
    mTransformer->resize(width, height);
    mFontTextureProgram->onResize(mTransformer);
    mMvpMatrix = mTransformer->getProjectionMatrix() * mTransformer->getModelViewMatrix();
}

void egl::FrameRect::zoom(float x, float y, float zoom) {
    mDrawable2D->zoom(x, 1 - y, zoom);
    mTransformer->zoom(x, 1 - y, zoom);
}

void egl::FrameRect::resetScale() {

}

