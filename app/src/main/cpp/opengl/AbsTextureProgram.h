//
// Created by jay on 18-5-17.
//

#ifndef SURFACENATIVEDEMO_TEXTRUEPROGRAM_H
#define SURFACENATIVEDEMO_TEXTRUEPROGRAM_H

#include <glm/detail/type_mat4x4.hpp>
#include "Transformer.h"

namespace egl {

    class AbsTextureProgram {
    public:
        AbsTextureProgram();

        virtual ~AbsTextureProgram();

        virtual void resize(float width, float height);

        virtual void scale(float scale);

        virtual void zoom(float zoom);

        virtual void zoom(float centerX, float centerY, float zoom);

    protected:

        Transformer *mTransformer;
    private:
        glm::mat4 *mIdentityMatrix;

    };

}

#endif //SURFACENATIVEDEMO_TEXTRUEPROGRAM_H
