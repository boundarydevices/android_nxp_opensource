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

#ifndef _FSL_DISPLAY_H_
#define _FSL_DISPLAY_H_

#include <utils/threads.h>
#include "Memory.h"
#include "Layer.h"
#include "Composer.h"
#include "Edid.h"

namespace fsl {

using android::Mutex;
using android::Thread;
using android::sp;

#define DISPLAY_PRIMARY 0

#define OVERLAY_COMPOSE_BIT 0
#define LAST_OVERLAY_BIT 1
#define CLIENT_COMPOSE_BIT 2
#define OVERLAY_COMPOSE_MASK (1 << OVERLAY_COMPOSE_BIT)
#define LAST_OVERLAY_MASK (1 << LAST_OVERLAY_BIT)
#define CLIENT_COMPOSE_MASK (1 << CLIENT_COMPOSE_BIT)

class EventListener
{
public:
    virtual ~EventListener() {}
    virtual void onVSync(int disp, nsecs_t timestamp) = 0;
    virtual void onHotplug(int disp, bool connected) = 0;
    virtual void onRefresh(int disp) = 0;
};

enum {
    DISPLAY_INVALID = 0,
    DISPLAY_LDB = 1,
    DISPLAY_HDMI = 2,
    DISPLAY_DVI = 3,
    DISPLAY_HDMI_ON_BOARD = 4,
    DISPLAY_VIRTUAL = 5,
};

enum {
    POWER_ON = 0,
    POWER_DOZE,
    POWER_DOZE_SUSPEND,
    POWER_OFF,
};

struct DisplayConfig
{
    int mXres;
    int mYres;
    int mFormat;
    int mBytespixel;
    int mStride; // in bytes.
    float mFps;
    int mVsyncPeriod;
    int mXdpi;
    int mYdpi;
};

class Display
{
public:
    Display();
    virtual ~Display();

    // layers manage.
    // release layer after its usage.
    void releaseLayer(int index);
    // get layer with index.
    Layer* getLayer(int index);
    Layer* getLayerByPriv(void* priv);
    // get empty layer.
    Layer* getFreeLayer();
    // clean and invalidate all layers.
    int invalidLayers();
    // get changed composition types for all layers.
    int getChangedTypes(uint32_t* outNumTypes, uint64_t* outLayers,
                        int32_t* outTypes);
    // get display&layer request for all layers.
    int getRequests(int32_t* outDisplayRequests, uint32_t* outNumRequests,
                    uint64_t* outLayers, int32_t* outLayerRequests);
    int getReleaseFences(uint32_t* outNumElements, uint64_t* outLayers,
                         int32_t* outFences);
    // verify all layers and marks if device can handle.
    bool verifyLayers();
    // set display composite target buffer.
    int setRenderTarget(Memory* buffer, int acquireFence);
    // to do composite all layers.
    virtual int composeLayers();

    // display property.
    // set display power on/off.
    virtual int setPowerMode(int mode);
    // enable/disable display vsync.
    virtual void setVsyncEnabled(bool enabled);
    // use software vsync.
    virtual void setFakeVSync(bool enable);
    // set display connection state.
    void setConnected(bool connected);
    // get display connection state.
    bool connected();
    // set display type.
    void setType(int type);
    // get display type.
    int type();
    // set display index of array.
    void setIndex(int index);
    // get display index of array.
    int index();

    virtual bool checkOverlay(Layer* layer);
    virtual int performOverlay();
    // update composite buffer to screen.
    virtual int updateScreen();
    // set display active config.
    virtual int setActiveConfig(int configId);
    // set display specified config parameters.
    int setConfig(int width, int height, int* format);
    // clear all display configs.
    void clearConfigs();
    // get display config with index.
    const DisplayConfig& getConfig(int config);
    // get display config with internal active index.
    const DisplayConfig& getActiveConfig();
    // get display active config index.
    int getActiveId();
    // get display config number.
    int getConfigNum();
    // check whether display support HDR or not
    bool isHdrSupported();
    // get HDR metadata
    int getHdrMetaData(HdrMetaData* hdrMetaData);

protected:
    int composeLayersLocked();
    void resetLayerLocked(Layer* layer);
    void waitOnFenceLocked();

protected:
    Mutex mLock;
    int mIndex;
    bool mConnected;
    int mType;

    int mActiveConfig;
    SortedVector<DisplayConfig> mConfigs;

    LayerVector mLayerVector;
    Layer* mLayers[MAX_LAYERS];
    Composer mComposer;
    Memory* mRenderTarget;
    int mAcquireFence;
    int mComposeFlag;
    Edid* mEdid;
};

}
#endif
