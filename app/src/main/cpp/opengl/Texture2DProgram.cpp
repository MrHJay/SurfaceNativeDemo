//
// Created by jay on 18-5-8.
//

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "Texture2DProgram.h"
#include "EGLError.h"
#include "GLUtil.h"

namespace egl {

    const char *VERTEX_SHADER =
            "uniform mat4 uMVPMatrix;\n"
                    "uniform mat4 uTexMatrix;\n"
                    "attribute vec4 aPosition;\n"
                    "attribute vec4 aTextureCoord;\n"
                    "varying vec2 vTextureCoord;\n"
                    "void main() {\n"
                    "   gl_Position = uMVPMatrix * aPosition;\n"
                    "   vTextureCoord = (uTexMatrix * aTextureCoord).xy;\n"
                    "}\n";

    const char *FRAGMENT_SHADER_EXT =
            "#extension GL_OES_EGL_image_external : require\n"
                    "precision mediump float;\n"
                    "varying vec2 vTextureCoord;\n"
                    "uniform samplerExternalOES sTexture;\n"
                    "void main() {\n"
                    "    gl_FragColor = texture2D(sTexture, vTextureCoord);\n"
                    "}\n";

    Texture2DProgram::Texture2DProgram() {
        mTextureTarget = GL_TEXTURE_EXTERNAL_OES;

        mEglProgram = GLUtil::createProgram(VERTEX_SHADER, FRAGMENT_SHADER_EXT);

        maPositionLoc = glGetAttribLocation(mEglProgram, "aPosition");
        GLUtil::checkLocation(maPositionLoc, "aPosition");
        maTextureCoordLoc = glGetAttribLocation(mEglProgram, "aTextureCoord");
        GLUtil::checkLocation(maTextureCoordLoc, "aTextureCoord");
        muMVPMatrixLoc = glGetUniformLocation(mEglProgram, "uMVPMatrix");
        GLUtil::checkLocation(muMVPMatrixLoc, "uMVPMatrix");
        muTexMatrixLoc = glGetUniformLocation(mEglProgram, "uTexMatrix");
        GLUtil::checkLocation(muTexMatrixLoc, "uTexMatrix");
        muKernelLoc = glGetUniformLocation(mEglProgram, "uKernel");
        if (muKernelLoc < 0) {
            // no kernel in this one
            muKernelLoc = -1;
            muTexOffsetLoc = -1;
            muColorAdjustLoc = -1;
        } else {
            // has kernel, must also have tex offset and color adj
            muTexOffsetLoc = glGetUniformLocation(mEglProgram, "uTexOffset");
            GLUtil::checkLocation(muTexOffsetLoc, "uTexOffset");
            muColorAdjustLoc = glGetUniformLocation(mEglProgram, "uColorAdjust");
            GLUtil::checkLocation(muColorAdjustLoc, "uColorAdjust");

            // initialize default values
            GLfloat kernels[9] = {0, 0, 0, 0, 1, 0, 0, 0, 0};
            setKernel(kernels, 0);
            setTexSize(256, 256);
        }
    }

    Texture2DProgram::~Texture2DProgram() {
        if (mEglProgram) {
            glDeleteProgram(mEglProgram);
            mEglProgram = 0;
        }
    }

    GLuint Texture2DProgram::createTexture() {
        GLuint textureId[1];
        glGenTextures(1, textureId);
        GLint error = eglGetError();
        EGLError::logError(error, "gen texture error");

        GLenum target = mTextureTarget;
        glBindTexture(target, textureId[0]);
        glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        return textureId[0];
    }

    void Texture2DProgram::setKernel(GLfloat (&kernel)[9], float colorAdjust) {
        mKernel = kernel;
        mColorAdjust = colorAdjust;
    }

    void Texture2DProgram::setTexSize(GLint width, GLint height) {
        GLfloat rw = 1.0f / width;
        GLfloat rh = 1.0f / height;

        // Don't need to create a new array here, but it's syntactically convenient.
        GLfloat arrays[] = {
                -rw, -rh, 0, -rh, rw, -rh,
                -rw, 0, 0, 0, rw, 0,
                -rw, rh, 0, rh, rw, rh
        };
        mTexOffset = &arrays[0];
    }

    void Texture2DProgram::draw(float mvpMatrix[16], const GLfloat *vertexCoords, int firstVertex,
                                int vertexCount, int coordsPerVertex, int vertexStride,
                                float texMatrix[16], const GLfloat *texCoords, GLuint textureId,
                                int texStride) {
        // Select the program.
        glUseProgram(mEglProgram);
        EGLError::checkEGLError("glUseProgram");

        // Set the texture.
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(mTextureTarget, textureId);

        // Copy the model / view / projection matrix over.
        glUniformMatrix4fv(muMVPMatrixLoc, 1, GL_FALSE, mvpMatrix);
        EGLError::checkEGLError("glUniformMatrix4fv");

        // Copy the texture transformation matrix over.
        glUniformMatrix4fv(muTexMatrixLoc, 1, GL_FALSE, texMatrix);
        EGLError::checkEGLError("glUniformMatrix4fv");

//        GLuint VBO1,VBO2;
//        glGenBuffers(1, &VBO1);
//        glGenBuffers(1, &VBO2);

//        glBindBuffer(GL_ARRAY_BUFFER, VBO1);
//        glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), vertexCoords, GL_STATIC_DRAW);

        // Connect vertexCoords to "aPosition".
        glVertexAttribPointer(static_cast<GLuint>(maPositionLoc), coordsPerVertex, GL_FLOAT,
                              GL_FALSE, vertexStride, vertexCoords);

        // Enable the "aPosition" vertex attribute.
        glEnableVertexAttribArray(static_cast<GLuint>(maPositionLoc));
        EGLError::checkEGLError("glEnableVertexAttribArray");

//        glBindBuffer(GL_ARRAY_BUFFER, VBO2);
//        glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), texCoords, GL_STATIC_DRAW);

        // Connect texCoords to "aTextureCoord".
        glVertexAttribPointer(static_cast<GLuint>(maTextureCoordLoc), 2, GL_FLOAT, GL_FALSE,
                              texStride, texCoords);
        EGLError::checkEGLError("glVertexAttribPointer");

        // Enable the "aTextureCoord" vertex attribute.
        glEnableVertexAttribArray(static_cast<GLuint>(maTextureCoordLoc));
        EGLError::checkEGLError("glEnableVertexAttribArray");

//        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Populate the convolution kernel, if present.
        if (muKernelLoc >= 0) {
            glUniform1fv(muKernelLoc, KERNEL_SIZE, mKernel);
            glUniform2fv(muTexOffsetLoc, KERNEL_SIZE, mTexOffset);
            glUniform1f(muColorAdjustLoc, mColorAdjust);
        }

        // Draw the rect.
        glDrawArrays(GL_TRIANGLE_STRIP, firstVertex, vertexCount);
        EGLError::checkEGLError("glDrawArrays");

        // Done -- disable vertex array, texture, and program.
        glDisableVertexAttribArray(static_cast<GLuint>(maPositionLoc));
        glDisableVertexAttribArray(static_cast<GLuint>(maTextureCoordLoc));
//        glDeleteBuffers(1, &VBO1);
//        glDeleteBuffers(1, &VBO2);
        glBindTexture(mTextureTarget, 0);
        glUseProgram(0);
    }

}
