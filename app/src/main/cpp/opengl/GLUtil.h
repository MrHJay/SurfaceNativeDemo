//
// Created by jay on 18-5-8.
//

#ifndef SURFACENATIVEDEMO_GLUTILS_H
#define SURFACENATIVEDEMO_GLUTILS_H


#include <GLES2/gl2.h>

namespace egl {
    class GLUtil {

    public:
        static GLuint createProgram(const char *vertexShaderCode, const char *fragmentShaderCode);

        static bool checkLocation(GLint location, const char *msg);

        static void logMat4(const char * msg, const float mat4[16]);

    private:
        static GLuint compileShader(GLenum type, const char *shaderCode);

    };
}

#endif //SURFACENATIVEDEMO_GLUTILS_H
