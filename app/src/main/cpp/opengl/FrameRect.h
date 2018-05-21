//
// Created by jay on 18-5-9.
//

#ifndef SURFACENATIVEDEMO_FRAMERECT_H
#define SURFACENATIVEDEMO_FRAMERECT_H

#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include "Drawable2D.h"
#include "Texture2DProgram.h"
#include "FontTextureProgram.h"

namespace egl {

    class FrameRect {
    public:
        FrameRect(Texture2DProgram *texture2DProgram, FontTextureProgram *fontTextureProgram,
                  Drawable2D *drawable2D);

        ~FrameRect();

        GLuint createTexture();

        void drawFrame(GLuint textureId, float texMatrix[16], long timestamp);

        void scale(float scale);

        void projection(float width, float height);

        void zoom(float x, float y, float zoom);

        void resetScale();

    private:
        Drawable2D *mDrawable2D;
        Texture2DProgram *mTexture2DProgram;
        FontTextureProgram *mFontTextureProgram;

        Transformer *mTransformer;
        glm::mat4 mMvpMatrix;
    };

}

#endif //SURFACENATIVEDEMO_FRAMERECT_H
