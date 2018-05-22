//
// Created by jay on 18-5-15.
//

#ifndef SURFACENATIVEDEMO_NUMBERFONT_H
#define SURFACENATIVEDEMO_NUMBERFONT_H

#include <map>
#include <string>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <glm/detail/type_vec.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <ft2build.h>
#include <android/asset_manager.h>
#include FT_FREETYPE_H

namespace egl {

#define FONT_SIZE 100;

    /// Holds all state information relevant to a character as loaded using FreeType
    struct Character {
        GLuint textureID;   // ID handle of the glyph texture
        glm::ivec2 size;    // Size of glyph
        glm::ivec2 bearing;  // Offset from baseline to left/top of glyph
        FT_Pos advance;    // Horizontal offset to advance to next glyph
    };

    class Font {

    public:
        Font();

        virtual ~Font();

        virtual void init(const char *fontPath);

        virtual void init(FT_Byte *fontData, FT_Long dataSize);

        Character getCharacter(char c);

    protected:
        virtual void initCharSet(FT_Face face)= 0;

        void insertChar(GLchar c, Character character);

    private:


        std::map<GLchar, Character> mCharacters;
    };

    class NumberFont : public Font {

    public:
        NumberFont();

        ~NumberFont() override;

        void initCharSet(FT_Face face) override;

    private:
    };

}

#endif //SURFACENATIVEDEMO_NUMBERFONT_H
