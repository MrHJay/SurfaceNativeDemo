//
// Created by jay on 18-5-17.
//

#include "AbsTextureProgram.h"

namespace egl {

    AbsTextureProgram::AbsTextureProgram() {
        this->mIdentityMatrix = new glm::mat4();
        this->mTransformer = new Transformer();
    }

    AbsTextureProgram::~AbsTextureProgram() {
        if (mIdentityMatrix) {
            delete mIdentityMatrix;
        }
        if (mTransformer) {
            delete mTransformer;
        }
    }

    void AbsTextureProgram::resize(float width, float height) {
        mTransformer->resize(width, height);
    }

    void AbsTextureProgram::scale(float scale) {
        mTransformer->scale(scale);
    }

    void AbsTextureProgram::zoom(float zoom) {
        mTransformer->zoom(mTransformer->getWidth() / 2, mTransformer->getHeight() / 2, zoom);
    }

    void AbsTextureProgram::zoom(float centerX, float centerY, float zoom) {
        mTransformer->zoom(centerX, centerY, zoom);
    }

}
