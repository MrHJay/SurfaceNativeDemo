//
// Created by jay on 18-5-15.
//

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "FontTextureProgram.h"
#include "GLUtil.h"
#include "EGLError.h"

namespace egl {

    const char *FONT_VERTEX_SHADER =
            "attribute vec4 vertex; // <vec2 pos, vec2 tex>\n"
                    "varying vec2 TexCoords;\n"
                    "uniform mat4 projection;\n"
                    "void main() {\n"
                    "    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
                    "    TexCoords = vertex.zw;\n"
                    "}";

    const char *FONT_FRAGMENT_SHADER =
            "precision mediump float;\n"
                    "varying vec2 TexCoords;\n"
                    "uniform sampler2D texture;\n"
                    "uniform vec4 textColor;\n"
                    "void main() {"
                    "    vec4 sampled = vec4(1.0, 1.0, 1.0, texture2D(texture, TexCoords).a);\n"
                    "    gl_FragColor = textColor * sampled;\n"
                    "}";

    FontTextureProgram::FontTextureProgram(Font *font) : mFont(font) {

        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        mEglProgram = GLUtil::createProgram(FONT_VERTEX_SHADER, FONT_FRAGMENT_SHADER);
        mProjectionLoc = glGetUniformLocation(mEglProgram, "projection");
        GLUtil::checkLocation(mProjectionLoc, "projection");
        mTextColorLoc = glGetUniformLocation(mEglProgram, "textColor");
        mVertexLoc = glGetAttribLocation(mEglProgram, "vertex");

        glUniform4f(mTextColorLoc, 1.0f, 1.0f, 1.0f, 1.0f);
    }

    void FontTextureProgram::onResize(Transformer *transformer) {
        glUseProgram(mEglProgram);
        glUniformMatrix4fv(mProjectionLoc, 1, GL_FALSE,
                           glm::value_ptr(transformer->getProjectionMatrix()));
        glUseProgram(0);
    }

    FontTextureProgram::~FontTextureProgram() {
        if (mEglProgram) {
            glDeleteProgram(mEglProgram);
            mEglProgram = 0;
        }
        if (mFont) {
            delete mFont;
        }
    }

    void
    FontTextureProgram::drawText(std::string text, float x, float y, Transformer *transformer) {
        if (mFont) {
            // Activate corresponding render state
            glUseProgram(mEglProgram);
            glActiveTexture(GL_TEXTURE0);

            // Iterate through all characters
            std::string::const_iterator c;
            for (c = text.begin(); c != text.end(); c++) {
                Character ch = mFont->getCharacter(*c);

                GLfloat xpos = x + ch.bearing.x * mScale;
                GLfloat ypos = y - (ch.size.y - ch.bearing.y) * mScale;

                GLfloat w = ch.size.x * mScale;
                GLfloat h = ch.size.y * mScale;
                GLfloat vertices[6][4] = {
                        {xpos,     ypos + h, 0.0, 0.0},
                        {xpos,     ypos,     0.0, 1.0},
                        {xpos + w, ypos,     1.0, 1.0},

                        {xpos,     ypos + h, 0.0, 0.0},
                        {xpos + w, ypos,     1.0, 1.0},
                        {xpos + w, ypos + h, 1.0, 0.0}
                };
                if (transformer != nullptr)
                    transformer->transformedArrayV2(*vertices, *vertices, 24, 4, true);
                // Render glyph texture over quad
                glBindTexture(GL_TEXTURE_2D, ch.textureID);

                glVertexAttribPointer(static_cast<GLuint>(mVertexLoc), 4, GL_FLOAT, GL_FALSE,
                                      sizeof(GLfloat) * 4, vertices);
                EGLError::checkEGLError("glVertexAttribPointer");
                glEnableVertexAttribArray(static_cast<GLuint>(mVertexLoc));
                EGLError::checkEGLError("glEnableVertexAttribArray");

                // Render quad
                glDrawArrays(GL_TRIANGLES, 0, 6);
                // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
                x += (ch.advance >> 6) *
                     mScale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
            }

            glBindTexture(GL_TEXTURE_2D, 0);
            glUseProgram(0);
        }

    }

    void FontTextureProgram::drawText(std::string text, float x, float y) {
        drawText(text, x, y, nullptr);
    }

    void FontTextureProgram::setColor(int color) {
        int a = color >> 24 & 0xFF;
        int r = color >> 16 & 0xFF;
        int g = color >> 8 & 0xFF;
        int b = color & 0xFF;

        if (mEglProgram) {
            glUseProgram(mEglProgram);
            glUniform4f(mTextColorLoc, r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
            glUseProgram(0);
        }
    }

    void FontTextureProgram::setTextSize(float size) {
        mScale = size / FONT_SIZE;
    }
}