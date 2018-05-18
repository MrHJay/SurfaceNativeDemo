//
// Created by jay on 18-5-17.
//

#ifndef SURFACENATIVEDEMO_TRANSFORMER_H
#define SURFACENATIVEDEMO_TRANSFORMER_H


#include <glm/detail/type_mat.hpp>
#include <GLES2/gl2.h>

namespace egl {

    class Transformer {

    public:
        Transformer();

        ~Transformer();

        void resize(float width, float height);

        void scale(float scale);

        void zoom(float centerX, float centerY, float zoom);

        float getWidth();

        float getHeight();

        GLfloat *transformedArrayV2(GLfloat *dst, GLfloat *src, int length, int stride,
                                    bool normalized);

        GLfloat *transformedArrayV3(GLfloat *dst, GLfloat *src, int length, int stride,
                                    bool normalized);

        glm::mat4 getModelViewMatrix();

        glm::mat4 getProjectionMatrix();

        glm::mat4 getIdentityMatrix();

    private:
        void recalMv();
        void recalProjection();

        glm::mat4 *mIdentityMatrix;
        glm::mat4 *mMvMatrix;
        glm::mat4 *mProjectionMatrix;
        bool mRecalMv, mRecalProjection;
        float mWidth, mHeight;
        float mZoom;
        float mScale;
        float mCenterX, mCenterY;
    };

}

#endif //SURFACENATIVEDEMO_TRANSFORMER_H
