//
// Created by jay on 18-5-14.
//

#ifndef SURFACENATIVEDEMO_ZOOMDRAWABLE2D_H
#define SURFACENATIVEDEMO_ZOOMDRAWABLE2D_H

#include "Drawable2D.h"
#include "Transformer.h"

namespace egl {

    class ZoomDrawable2D : public Drawable2D {
    public:
        ZoomDrawable2D(Prefab type);

        ~ZoomDrawable2D();

        void zoom(float zoom) override;

        void zoom(float x, float y, float zoom) override;

        const GLfloat *getTexCoordArray() override;

    private:
        bool mRecalculate;
        float mZoom;
        float mCenterX, mCenterY;
        GLfloat *mZoomTexCoorArray;
    };

}

#endif //SURFACENATIVEDEMO_ZOOMDRAWABLE2D_H
