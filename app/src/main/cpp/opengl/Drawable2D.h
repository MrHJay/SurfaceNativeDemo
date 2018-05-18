//
// Created by jay on 18-5-9.
//

#ifndef SURFACENATIVEDEMO_DRAWABLE2D_H
#define SURFACENATIVEDEMO_DRAWABLE2D_H

#include <GLES2/gl2.h>

namespace egl {

    enum Prefab {
        RECT, FULL_RECT
    };

    class Drawable2D {
    public:
        Drawable2D(Prefab type);

        virtual ~Drawable2D();

        int getCoordsPerVertex() const;

        int getCoordsPerTex() const;

        int getVertexStride() const;

        int getVertexCount() const;

        int getTexCount() const;

        const GLfloat *getVertexArray() const;

        virtual const GLfloat *getTexCoordArray();

        int getTexCoordStride() const;

        virtual void zoom(float zoom);

        virtual void zoom(float x, float y, float zoom);

    protected:

        float getTexCenterX();

        float getTexCenterY();

    private:
        int mCoordsPerVertex;
        int mVertexStride;
        int mVertexCount;
        int mCoordsPerTex;
        int mTexCount;
        int mTexCoordStride;

        const GLfloat *mVertexArray;
        const GLfloat *mTexCoordArray;

    };

}

#endif //SURFACENATIVEDEMO_DRAWABLE2D_H
