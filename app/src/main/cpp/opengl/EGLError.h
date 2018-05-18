//
// Created by jay on 18-5-8.
//

#ifndef SURFACENATIVEDEMO_EGLERROR_H
#define SURFACENATIVEDEMO_EGLERROR_H

#include <EGL/eglplatform.h>

class EGLError {

public:
    static void checkEGLError(const char*msg);
    /**
     * Show openGL es error info
     * @param errorCode error code
     */
    static void logError(EGLint errorCode);

    static void logError(EGLint errorCode, const char *msg);

};


#endif //SURFACENATIVEDEMO_EGLERROR_H
