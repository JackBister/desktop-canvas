#include <atomic>
#include <chrono>
#include <filesystem>
#include <optional>
#include <thread>

#ifdef __ANDROID__
#include <SDL2/SDL_config_android.h>
#endif
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#define DUK_USE_DEBUG
#define DUK_USE_DEBUG_LEVEL 0
#define DUK_USE_DEBUG_WRITE(level, file, line, func, msg)                                          \
    do {                                                                                           \
        logger->info("D%ld %s:%d (%s): %s", (long)(level), (file), (long)(line), (func), (msg));   \
    } while (0)

#include "Canvas/SDLCanvasRenderingContext2D.h"
#include "Canvas/SDLWindowCanvas.h"
#include "DukJavaScriptEngine.h"
#include "Input/SDLKeyToDOMKey.h"
#include "JavaScriptEngine.h"
#include "Logger/Logger.h"
#include "Navigator/SDLNavigator.h"
#include "SDLEventPump.h"
#include "slurpFile.h"
#include "watchFile.h"

static auto logger = Logger::get();

constexpr int MIN_DURATION_BETWEEN_FILE_EVALS_SECONDS = 5;

std::chrono::high_resolution_clock::time_point g_lastFileEval;
std::atomic_bool g_shouldEvalFile = false;

std::unique_ptr<EventPump> g_eventPump;
std::unique_ptr<JavaScriptEngine> g_jsEngine;

struct {
    std::filesystem::path filename;
    bool watchMode = false;
} g_options;

void parseArgs(int argc, char ** argv)
{
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--watch") == 0 || strcmp(argv[i], "-w") == 0) {
            g_options.watchMode = true;
        }
        if (argv[i][0] != '-') {
            g_options.filename = argv[i];
        }
    }
}

std::optional<std::pair<SDL_Window *, SDL_Renderer *>> initSdl()
{
    // TODO: Don't init everything.
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        return {};
    }
    if (TTF_Init()) {
        return {};
    }

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096)) {
        return {};
    }

    SDL_Window * window;
    SDL_Renderer * renderer;
    if (SDL_CreateWindowAndRenderer(960, 540, 0, &window, &renderer)) {
        return {};
    }

    return std::make_pair(window, renderer);
}

std::wstring toWString(char const * const str)
{
    const size_t fnSize = strlen(str) + 1;
    std::wstring ret(fnSize, L'#');
    mbstowcs(&ret[0], str, fnSize);
    return ret;
}

#undef main
int main(int argc, char ** argv)
{
    using namespace std::chrono_literals;
    parseArgs(argc, argv);

    auto windowAndRendererOptional = initSdl();
    if (!windowAndRendererOptional.has_value()) {
        printf("Couldn't create window and renderer: %s", SDL_GetError());
        return 1;
    }
    auto windowAndRenderer = windowAndRendererOptional.value();

    auto renderer = windowAndRenderer.second;
    auto windowSurface = SDL_GetWindowSurface(windowAndRenderer.first);

    auto canvas = new SDLWindowCanvas(windowAndRenderer.first, renderer);
    auto context = canvas->getContext();

    // TODO: Configurable
    SDL_RenderSetLogicalSize(renderer, 400, 225);
    auto numMappings = SDL_GameControllerAddMappingsFromFile("gamecontrollerdb.txt");
    if (numMappings < 0) {
        logger->info("Error loading controller mappings %s", SDL_GetError());
    }
    g_eventPump = std::make_unique<SDLEventPump>(std::pair<int, int>(400, 225));

    auto workingDir = std::filesystem::absolute(g_options.filename);
    workingDir.remove_filename();
    std::filesystem::current_path(workingDir);
    g_options.filename = g_options.filename.filename();

    dcanvas::watchFile(g_options.filename.wstring(), [&]() {
        auto now = std::chrono::high_resolution_clock::now();
        auto timeSinceLastEval =
            std::chrono::duration_cast<std::chrono::seconds>(now - g_lastFileEval);

        if (timeSinceLastEval.count() < MIN_DURATION_BETWEEN_FILE_EVALS_SECONDS) {
            return;
        }

        g_shouldEvalFile = true;
    });

    auto navigator = new SDLNavigator();

    g_jsEngine = std::make_unique<DukJavaScriptEngine>();
    g_lastFileEval = std::chrono::high_resolution_clock::now();
    g_jsEngine->evalFile(g_options.filename.u8string().c_str());

    g_jsEngine->initAudio();
    g_jsEngine->initBitmap(context);
    g_jsEngine->initNavigator(navigator);
    g_jsEngine->initWebsocket();
    g_jsEngine->initCanvas(context, renderer);

    for (;;) {
        bool shouldQuit = g_eventPump->pumpEvents(g_jsEngine.get());
        if (shouldQuit) {
            goto end;
        }

        if (g_shouldEvalFile) {
            g_lastFileEval = std::chrono::high_resolution_clock::now();
            g_shouldEvalFile = false;
            auto stashedState = g_jsEngine->callGlobalFunctionWithReturn("stashState");
            // TODO: Exceptions and that stuff
            g_jsEngine = std::make_unique<DukJavaScriptEngine>();
            g_jsEngine->evalFile(g_options.filename.u8string().c_str());

            g_jsEngine->initAudio();
            g_jsEngine->initBitmap(context);
            g_jsEngine->initNavigator(navigator);
            g_jsEngine->initWebsocket();
            g_jsEngine->initCanvas(context, renderer);

            g_jsEngine->callGlobalFunction("loadState", stashedState);
        }

        g_jsEngine->preTick();
        g_jsEngine->callGlobalFunction("tick");
        g_jsEngine->postTick();

        SDL_RenderPresent(renderer);

        // TODO: I use this to prevent my laptop from going 100% full blast with its fans, but it
        // should probably be done in a cleaner way
        std::this_thread::sleep_for(13ms);
    }

end:

    return 0;
}
