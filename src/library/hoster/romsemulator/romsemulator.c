/*
 * Copyright 2020 Maximilian Voss (maximilian@voss.rocks)
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

#include <csafestring.h>
#include "romsemulator.h"
#include "mapping.h"
#include "icon.h"
#include "../urlhandling.h"
#include "../../../common/utils.h"
#include "../../helper/regex.h"
#include "../results.h"

#define SHORTNAME "REN"
#define FULLNAME "https://www.romsemulator.net"
#define URL_TEMPLATE "https://romsemulator.net/roms/%system%/page/%page%/?s=%query%"

static result_t *search(system_t *system, char *searchString);

static void download(result_t *item, downloadCallback_t downloadCallbackFunction, void *appData);

static result_t *fetchingResultItems(system_t *system, result_t *resultList, char *response);

static char *fetchDownloadPageLink(char *response);

static char *fetchHiddenField(char *response, char *fieldname, int variant);

static hoster_t *hoster = NULL;

hoster_t *romsemulator_getHoster(cache_t *cacheHandler) {
    if (hoster == NULL) {
        hoster = calloc(1, sizeof(hoster_t));
        hoster->search = search;
        hoster->download = download;
        hoster->name = SHORTNAME;
        hoster->active = 1;
        hoster->fullname = FULLNAME;
        hoster->favicon = calloc(1, sizeof(memimage_t));
        hoster->favicon->binary = romemulator_icon_data;
        hoster->favicon->size = romemulator_icon_size;
    }
    return hoster;
}

static result_t *search(system_t *system, char *searchString) {
    uint32_t resultCount = 0;
    uint32_t page = 1;
    char *urlTemplate = URL_TEMPLATE;

    result_t *resultList = NULL;
    do {
        char *url = urlhandling_substitudeVariables(urlTemplate, system, &romsemulator_deviceMapping, searchString,
                                                    page);
        if (url == NULL) {
            break;
        }

        curl_response_t *response = curlling_fetch(url, NULL, GET, 1L);
        resultList = fetchingResultItems(system, resultList, response->data);
        curl_freeResponse(response);
        free(url);

        resultCount = ll_count(resultList);
        page++;
    } while (resultCount != ll_count(resultList) && resultCount % 10 == 0);

    return resultList;
}

static void download(result_t *item, downloadCallback_t downloadCallbackFunction, void *appData) {
    if (item == NULL) {
        return;
    }
    curl_response_t *detailPageResponse = curlling_fetch(item->url, NULL, GET, 1L);
    char *linkDownloadPage = fetchDownloadPageLink(detailPageResponse->data);

    curl_response_t *downloadPageResponse = curlling_fetch(linkDownloadPage, NULL, GET, 1L);

    char *pid = fetchHiddenField(downloadPageResponse->data, "pid", 0);
    char *roms = fetchHiddenField(downloadPageResponse->data, "roms_download_file_nonce_field", 1);
    char *referer = fetchHiddenField(downloadPageResponse->data, "_wp_http_referer", 0);

    csafestring_t *payload = safe_create("action=roms_download_file&pid=");
    safe_strcat(payload, pid);
    safe_strcat(payload, "&roms_download_file_nonce_field=");
    safe_strcat(payload, roms);
    safe_strcat(payload, "&_wp_http_referer=");
    safe_strcat(payload, referer);

    char *filename = str_concat(item->title, ".zip");
    downloadCallbackFunction(appData, item->system, item->title, linkDownloadPage, payload->data, filename, POST);
    free(filename);

    free(pid);
    free(roms);
    free(referer);
    curl_freeResponse(downloadPageResponse);
    free(linkDownloadPage);
    curl_freeResponse(detailPageResponse);
    safe_destroy(payload);
}

static char *fetchHiddenField(char *response, char *fieldname, int variant) {
    char *regexStringTmpl;
    if (variant == 0) {
        regexStringTmpl = "<input type=\"[^\"]+\" name=\"%fieldname%\" value=\"([^\"]+)\"";
    } else {
        regexStringTmpl = "<input type=\"[^\"]+\" id=\"[^\"]+\" name=\"%fieldname%\" value=\"([^\"]+)\"";
    };
    char *regexString = str_replace(regexStringTmpl, "%fieldname%", fieldname);

    regexMatches_t *matches = regex_getMatches(response, regexString, 1);
    if (matches == NULL) {
        return NULL;
    }
    char *link = regex_cpyGroupText(matches, 0);
    regex_destroyMatches(matches);
    free(regexString);
    return link;
}

static char *fetchDownloadPageLink(char *response) {
    char *regexString = "<a class=\"download\" href=\"([^\"]+)\" rel=\"nofollow\" id=\"download_link\">";

    regexMatches_t *matches = regex_getMatches(response, regexString, 1);
    if (matches == NULL) {
        return NULL;
    }
    char *link = regex_cpyGroupText(matches, 0);
    regex_destroyMatches(matches);
    return link;
}

static result_t *fetchingResultItems(system_t *system, result_t *resultList, char *response) {
    char *regexString = "<td><a href=\"([^\"]+)\">([^<]+)</a>";

    regexMatches_t *matches = regex_getMatches(response, regexString, 2);
    regexMatches_t *ptr = matches;

    while (ptr != NULL) {
        result_t *item = result_newItem(system, hoster);
        result_setUrl(item, ptr->groups[0]);

        char *title = str_htmlDecode(ptr->groups[1]);
        result_setTitle(item, title);
        free(title);

        resultList = ll_append(resultList, item);
        ptr = ptr->next;
    }
    regex_destroyMatches(matches);
    return resultList;
}
