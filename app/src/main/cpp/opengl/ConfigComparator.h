//
// Created by jay on 18-5-8.
//

#ifndef SURFACENATIVEDEMO_CONFIGCOMPARATOR_H
#define SURFACENATIVEDEMO_CONFIGCOMPARATOR_H


#include <EGL/egl.h>

class ConfigComparator {

public:
    ConfigComparator(EGLDisplay display);

    bool operator()(EGLConfig const &l, EGLConfig const &r) const;

    int configWeight(EGLConfig const &config) const;

    int configAlphaSize(EGLConfig const &config) const;

private:
    EGLDisplay m_display;
};


#endif //SURFACENATIVEDEMO_CONFIGCOMPARATOR_H
