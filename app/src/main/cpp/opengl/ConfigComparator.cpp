//
// Created by jay on 18-5-8.
//

#include "ConfigComparator.h"

ConfigComparator::ConfigComparator(EGLDisplay display)
        : m_display(display) {
}

bool ConfigComparator::operator()(EGLConfig const &l, EGLConfig const &r) const {
    int const weight = configWeight(l) - configWeight(r);
    if (weight == 0)
        return configAlphaSize(l) < configAlphaSize(r);

    return weight < 0;
}

int ConfigComparator::configWeight(EGLConfig const &config) const {
    int val = -1;
    eglGetConfigAttrib(m_display, config, EGL_CONFIG_CAVEAT, &val);

    switch (val) {
        case EGL_NONE:
            return 0;
        case EGL_SLOW_CONFIG:
            return 1;
        case EGL_NON_CONFORMANT_CONFIG:
            return 2;
        default:
            return 0;
    }
}

int ConfigComparator::configAlphaSize(EGLConfig const &config) const {
    int val = 0;
    eglGetConfigAttrib(m_display, config, EGL_ALPHA_SIZE, &val);
    return val;
}