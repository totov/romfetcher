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

#ifndef HOSTER_ENGINEHANDLER_H
#define HOSTER_ENGINEHANDLER_H

#include "../romfetcher.h"

hoster_t *hosterhandler_init(cache_t *cacheHandler);

result_t *hosterhandler_search(hoster_t *hosters, system_t *system, char *searchString);

void hosterhandler_download(result_t *item, downloadCallback_t downloadCallbackFunction, void *appData);

void hosterhandler_destroy(hoster_t *hoster);

#endif