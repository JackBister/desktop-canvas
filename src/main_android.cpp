
#include <optional>

#include <android/log.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_config_android.h>
#include <SDL2/SDL_mixer.h>

#include "Canvas/SDLCanvasRenderingContext2D.h"
#include "Canvas/SDLWindowCanvas.h"
#include "DukJavaScriptEngine.h"
#include "JavaScriptEngine.h"
#include "SDLEventPump.h"

std::unique_ptr<EventPump> g_eventPump;
std::unique_ptr<JavaScriptEngine> g_jsEngine;

int main(int argc, char * argv[])
{
    auto window = SDL_CreateWindow("SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 256,
                                   144, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (!window) {
        __android_log_print(ANDROID_LOG_INFO, "dcanvas", "window NULL! %s", SDL_GetError());
        return 1;
    }

    auto surface = SDL_GetWindowSurface(window);
    if (!surface) {
        __android_log_print(ANDROID_LOG_INFO, "dcanvas", "surface NULL! %s", SDL_GetError());
        return 1;
    }

    auto renderer = SDL_GetRenderer(window);
    if (!renderer) {
        __android_log_print(ANDROID_LOG_INFO, "dcanvas",
                            "renderer NULL! %s, trying CreateRenderer...", SDL_GetError());
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (!renderer) {
            __android_log_print(ANDROID_LOG_INFO, "dcanvas",
                                "renderer still NULL! %s, trying CreateSoftwareRenderer...",
                                SDL_GetError());
            renderer = SDL_CreateSoftwareRenderer(surface);
            if (!renderer) {
                __android_log_print(ANDROID_LOG_INFO, "dcanvas",
                                    "renderer still NULL! %s, giving up...", SDL_GetError());
                return 1;
            }
        }
    }

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096)) {
        __android_log_print(ANDROID_LOG_INFO, "dcanvas", "Mix_OpenAudio error %s", SDL_GetError());
    }

    auto canvas = new SDLWindowCanvas(window, renderer);
    auto context = canvas->getContext();

    auto backbuffer = SDL_CreateTexture(renderer, SDL_GetWindowPixelFormat(window),
                                        SDL_TEXTUREACCESS_TARGET, 400, 225);
    SDL_SetRenderTarget(renderer, backbuffer);

    // TODO: Configurable
    if (SDL_RenderSetLogicalSize(renderer, 400, 225)) {
        __android_log_print(ANDROID_LOG_INFO, "dcanvas", "RenderSetLogicalSize failed, %s",
                            SDL_GetError());
    }
    g_eventPump = std::make_unique<SDLEventPump>(std::pair<int, int>(400, 225));

    g_jsEngine = std::make_unique<DukJavaScriptEngine>();
    g_jsEngine->evalFile("app.js");
    g_jsEngine->initAudio();
    g_jsEngine->initBitmap(context);
    g_jsEngine->initWebsocket();
    g_jsEngine->initCanvas(context, renderer);

    for (;;) {
        SDL_SetRenderTarget(renderer, backbuffer);

        bool shouldQuit = g_eventPump->pumpEvents(g_jsEngine.get());
        if (shouldQuit) {
            goto end;
        }

        g_jsEngine->preTick();
        g_jsEngine->callGlobalFunction("tick");
		g_jsEngine->postTick();

        SDL_SetRenderTarget(renderer, nullptr);
        SDL_RenderCopy(renderer, backbuffer, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

end:
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return 0;
}
