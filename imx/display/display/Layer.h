/*
 * Copyright 2017 NXP.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _FSL_LAYER_H_
#define _FSL_LAYER_H_

#include <utils/SortedVector.h>
#include "android/Rect.h"
#include "android/Region.h"

#include "Memory.h"

namespace fsl {

#define MAX_LAYERS 64

using android::Rect;
using android::Region;
using android::SortedVector;

enum {
    LAYER_TYPE_INVALID = 0,
    LAYER_TYPE_CLIENT,
    LAYER_TYPE_DEVICE,
    LAYER_TYPE_SOLID_COLOR,
    LAYER_TYPE_CURSOR,
    LAYER_TYPE_SIDEBAND,
};

enum {
    TRANSFORM_INVALID = 0,
    TRANSFORM_FLIPH =  0x01,
    TRANSFORM_FLIPV =  0x02,
    TRANSFORM_ROT90 =  0x04,
    TRANSFORM_ROT180 = 0x03,
    TRANSFORM_ROT270 = 0x07,
};

enum {
    BLENDING_NONE     = 0x0100,
    BLENDING_PREMULT  = 0x0105,
    BLENDING_COVERAGE = 0x0405,
    BLENDING_DIM      = 0x0805,
};

class Layer
{
public:
    Layer();
    bool isSolidColor();

    bool busy;
    int zorder;
    int origType;
    int type;
    Memory* handle;
    int transform;
    int blendMode;
    int planeAlpha;
    int color;
    Rect sourceCrop;
    Rect displayFrame;
    Region visibleRegion;
    int acquireFence;
    int releaseFence;
    int index;
    void* priv;
};

class LayerVector : public SortedVector<Layer*> {
public:
    LayerVector();
    LayerVector(const LayerVector& rhs);
    virtual int do_compare(const void* lhs, const void* rhs) const;
};

}
#endif
