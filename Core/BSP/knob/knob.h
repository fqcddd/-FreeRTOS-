

#ifndef SMARTFARM_KNOB_H
#define SMARTFARM_KNOB_H

#include "main.h"

typedef enum {
    KNOB_DIR_NONE = 0, // 未旋转
    KNOB_DIR_LEFT = 1, // 左旋
    KNOB_DIR_RIGHT = 2, // 右旋
} KnobDirection;

void Knob_Init();
KnobDirection Knob_IsRotating();

#endif //SMARTFARM_KNOB_H

