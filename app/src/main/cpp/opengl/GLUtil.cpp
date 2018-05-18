//
// Created by jay on 18-5-8.
//

#include "GLUtil.h"
#include "../common.h"
#include "EGLError.h"


namespace egl {

    const char *TAG_UTIL = "GLUtil";

    GLuint GLUtil::compileShader(GLenum type, const char *shaderCode) {

        GLuint shader = glCreateShader(type);
        if (shader == 0) {
            EGLError::checkEGLError("create shader");
        }
        glShaderSource(shader, 1, &shaderCode, NULL);
        glCompileShader(shader);
        GLint compileStatus = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
        if (!compileStatus) {
            glDeleteShader(shader);
            LOG_W(TAG_UTIL, "compile shader error");
            return 0;
        }
        return shader;
    }

    GLuint GLUtil::createProgram(const char *vertexShaderCode, const char *fragmentShaderCode) {
        GLuint program = glCreateProgram();
        if (0 == program) {
            LOG_W(TAG_UTIL, "create program error");
            return 0;
        }
        LOG_D(TAG_UTIL, "create program success");
        GLuint vertexShaderID = compileShader(GL_VERTEX_SHADER, vertexShaderCode);
        GLuint fragmentShaderID = compileShader(GL_FRAGMENT_SHADER, fragmentShaderCode);

        glAttachShader(program, vertexShaderID);
        glAttachShader(program, fragmentShaderID);
        glLinkProgram(program);
        GLint linkStatus;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (!linkStatus) {
            glDeleteProgram(program);
            LOG_W(TAG_UTIL, "link program error");
            return 0;
        }
        return program;

    }

    bool GLUtil::checkLocation(GLint location, const char *msg) {
        if (location >= 0) {
            return true;
        } else {
            LOG_WF(TAG_UTIL, "%s get index failed", msg);
            return false;
        }
    }

    void GLUtil::logMat4(const char *msg, const float *mat4) {
        LOG_IF(TAG_UTIL, "%s ----------", msg);
        for (int i = 0; i < 16; i += 4) {
            LOG_IF(TAG_UTIL, "%4f,%4f,%4f,%4f", mat4[i], mat4[i + 1], mat4[i + 2], mat4[i + 3]);
        }
    }
}
