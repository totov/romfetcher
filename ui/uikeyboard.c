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

#include "uikeyboard.h"
#include "rendering.h"

static void renderSearchField(app_t *app);

static void renderDailPad(app_t *app);


void uikeyboard_render(app_t *app) {
    renderSearchField(app);
    renderDailPad(app);
}

static void renderSearchField(app_t *app) {
    int width, height;
    int charPos = 60;
    SDL_GL_GetDrawableSize(app->window, &width, &height);

    SDL_Rect r2 = {48, 50, width - 96, 54};
    SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 150);
    SDL_RenderFillRect(app->renderer, &r2);

    SDL_Rect r = {50, 52, width - 100, 50};
    SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 150);
    SDL_RenderFillRect(app->renderer, &r);

    if (*(app->keyboard.text) != '\0') {
        texture_t texture;
        rendering_loadText(app, &texture, app->keyboard.text, app->fonts.big, &app->themes.enabled->colors.text);
        SDL_Rect renderQuad = {60, 55, texture.w, texture.h};
        SDL_RenderCopy(app->renderer, texture.texture, NULL, &renderQuad);
        charPos += texture.w;
        SDL_DestroyTexture(texture.texture);
    }

    if (app->keyboard.activeChar != '\0') {
        char miniString[2] = {app->keyboard.activeChar, '\0'};
        texture_t texture;
        rendering_loadText(app, &texture, miniString, app->fonts.big, &app->themes.enabled->colors.highlight);
        SDL_Rect renderQuad = {charPos, 55, texture.w, texture.h};
        SDL_RenderCopy(app->renderer, texture.texture, NULL, &renderQuad);
        SDL_DestroyTexture(texture.texture);
    }
}

static void renderKey(app_t *app, int posx, int posy, int padWidth, int padHeight, uint8_t active, char *text);

static void renderDailPad(app_t *app) {
    const char keys[4][3][6] = {
            {"0-9",  "ABC",   "DEF"},
            {"GHI",  "JKL",   "MNO"},
            {"PQRS", "TUV",   "WXYZ"},
            {"DEL",  "SPACE", "SAVE"}
    };

    int width, height;
    SDL_GL_GetDrawableSize(app->window, &width, &height);

    int padWidth = (width - 100) / 4;
    int padHeight = (height - 100 - 120) / 4 - 20;
    int posX = (width / 2) - 1.5 * padWidth - 20;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            renderKey(app, posX + i * (padWidth + 20), 120 + j * (padHeight + 20), padWidth, padHeight,
                      app->keyboard.pointerPosition == i + j * 3 ? 1 : 0, (char *) keys[j][i]);
        }
    }
}

static void renderKey(app_t *app, int posx, int posy, int padWidth, int padHeight, uint8_t active, char *text) {
    SDL_Rect rectShadow = {posx, posy, padWidth, padHeight};
    SDL_SetRenderDrawColor(app->renderer, 0, 0, active ? 255 : 0, 150);
    SDL_RenderFillRect(app->renderer, &rectShadow);

    SDL_Rect rect = {posx + 2, posy + 2, padWidth - 4, padHeight - 4};
    SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 150);
    SDL_RenderFillRect(app->renderer, &rect);

    if (text != NULL && *text != '\0') {
        texture_t texture;
        rendering_loadText(app, &texture, text, app->fonts.huge, &app->themes.enabled->colors.text);
        SDL_Rect renderQuad = {posx + 20, posy + 10, padWidth - 40, padHeight - 20};
        SDL_RenderCopy(app->renderer, texture.texture, NULL, &renderQuad);
        SDL_DestroyTexture(texture.texture);
    }
}