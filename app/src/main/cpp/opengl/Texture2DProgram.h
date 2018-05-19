//
// Created by jay on 18-5-8.
//

#ifndef SURFACENATIVEDEMO_TEXTURE2DPROGRAM_H
#define SURFACENATIVEDEMO_TEXTURE2DPROGRAM_H

namespace egl {

    class Texture2DProgram {
    public:

        Texture2DProgram();

        ~Texture2DProgram();

        GLuint createTexture();

        void setKernel(GLfloat (&kernel)[9], float colorAdjust);

        void setTexSize(GLint width, GLint height);

        void draw(float mvpMatrix[16], const GLfloat *vertexCoords, int firstVertex,
                  int vertexCount, int coordsPerVertex, int vertexStride, float texMatrix[16],
                  const GLfloat *texCoords, GLuint textureId, int texStride);

    private:
        GLenum mTextureTarget;
        GLuint mEglProgram;

        GLint muMVPMatrixLoc;
        GLint muTexMatrixLoc;
        GLint muKernelLoc;
        GLint muTexOffsetLoc;
        GLint muColorAdjustLoc;
        GLint maPositionLoc;
        GLint maTextureCoordLoc;

        GLfloat *mTexOffset;
        GLfloat *mKernel;
        GLfloat mColorAdjust;
        static const GLsizei KERNEL_SIZE = 9;
    };
}

#endif //SURFACENATIVEDEMO_TEXTURE2DPROGRAM_H
