/*
 * Copyright 2020 - 2021 Maximilian Voss (maximilian@voss.rocks)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef DEFINITIONS_CONFIG_H
#define DEFINITIONS_CONFIG_H

#include <stdint.h>
#include "enums.h"

typedef struct configMenu_s {
    struct configMenu_s *prev;
    struct configMenu_s *next;
    char *name;
    window_t destiny;
} configMenu_t;

typedef enum {
    advancedConfig_fullscreen,
    advancedConfig_openGL,
    advancedConfig_highDPI,
    advancedConfig_downloadQueue,
    advancedConfig_externalCurl
} advancedConfigSetting_t;

typedef struct advancedConfig_s {
    struct advancedConfig_s *prev;
    struct advancedConfig_s *next;
    char *name;
    uint8_t active;
    advancedConfigSetting_t setting;
} advancedConfig_t;

typedef struct resolutionConfig_s {
    struct resolutionConfig_s *prev;
    struct resolutionConfig_s *next;
    char *name;
    uint8_t active;
    int height;
    int width;
} resolutionConfig_t;

#endif
