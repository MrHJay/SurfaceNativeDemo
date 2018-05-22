//
// Created by jay on 18-5-15.
//

#include <common.h>
#include <map>
#include <string>
#include "Font.h"
#include "GLUtil.h"
#include "EGLError.h"

namespace egl {
    using namespace std;

    const char *TAG_FONT = "FT";

    Font::Font() {

    }

    void Font::init(const char *fontPath) {
        // FreeType
        FT_Library ft;
        FT_Face face;
        // All functions return a value different than 0 whenever an error occurred
        if (FT_Init_FreeType(&ft)) {
            LOG_W(TAG_FONT, "init FreeType library failed");
            return;
        }

        if (FT_New_Face(ft, fontPath, 0, &face)) {
            LOG_WF(TAG_FONT, "new face failed for path[%s]", fontPath);
            return;
        }

        uint size = FONT_SIZE;
        FT_Set_Pixel_Sizes(face, 0, size);

        // Disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        initCharSet(face);

        // Destroy FreeType once we're finished
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }

    void Font::init(FT_Byte *fontData, FT_Long dataSize) {
        // FreeType
        FT_Library ft;
        FT_Face face;
        if (FT_Init_FreeType(&ft)) {
            LOG_W(TAG_FONT, "init FreeType library failed");
            return;
        }
        if (FT_New_Memory_Face(ft, (const FT_Byte *) fontData, dataSize, 0, &face)) {
            LOG_W(TAG_FONT, "new face failed");
            return;
        }

        uint size = FONT_SIZE;
        FT_Set_Pixel_Sizes(face, 0, size);

        // Disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        initCharSet(face);

        // Destroy FreeType once we're finished
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }

    Character Font::getCharacter(char c) {
        return mCharacters[c];
    }

    void Font::insertChar(GLchar c, Character character) {
        mCharacters.insert(make_pair(c, character));
    }

    Font::~Font() {
        map<GLchar, Character>::iterator iter;
        for (iter = mCharacters.begin(); iter != mCharacters.end(); iter++) {
            GLchar c = iter->first;
            Character character = iter->second;
            glDeleteTextures(1, &character.textureID);
        }
    }

    NumberFont::NumberFont() {
    }

    void NumberFont::initCharSet(FT_Face face) {
        LOG_I("Font", "init char set");
        const char charSets[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', ',', ':',
                                 '#', '(', ')', '-', '+', '%', '^', '*', '/', '=', ' ', '\''};
        int length = ARRAY_LENGTH(charSets);
        for (int i = 0; i < length; ++i) {
            char c = charSets[i];
            int error = FT_Load_Char(face, (FT_ULong) c, FT_LOAD_RENDER);
            if (error) {
                LOG_WF(TAG_FONT, "load char[%c] failed, error code=%d", c, error);
                continue;
            }

            // Generate texture
            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_ALPHA,
                    face->glyph->bitmap.width,
                    face->glyph->bitmap.rows,
                    0,
                    GL_ALPHA,
                    GL_UNSIGNED_BYTE,
                    face->glyph->bitmap.buffer
            );
            // Set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // Now store character for later use
            Character character = {
                    texture,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    face->glyph->advance.x
            };
            insertChar(c, character);
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    NumberFont::~NumberFont() {

    }
}
