set(SOURCES_APPLICATION
        src/application.c
        src/application.h
        src/config/config.c
        src/config/config.h
        src/config/configmenu.c
        src/config/configmenu.h
        src/config/configstates.c
        src/config/configstates.h
        src/constants.h
        src/database/database.c
        src/database/database.h
        src/definitions/config.h
        src/definitions/controller.h
        src/definitions/download.h
        src/definitions/enums.h
        src/definitions/listitem.h
        src/definitions/texture.h
        src/definitions/themes.h
        src/download/download.c
        src/download/download.h
        src/download/downloader/downloader.c
        src/download/downloader/downloader.h
        src/download/downloader/downloadercurl.c
        src/download/downloader/downloadercurl.h
        src/download/downloader/downloaderinternal.c
        src/download/downloader/downloaderinternal.h
        src/download/downloadmanager.c
        src/download/downloadmanager.h
        src/download/downloadpipeline.c
        src/download/downloadpipeline.h
        src/download/postprocess.c
        src/download/postprocess.h
        src/enginecache/enginecache.c
        src/enginecache/enginecache.h
        src/helper/map.c
        src/helper/map.h
        src/helper/path.c
        src/helper/path.h
        src/helper/uihelper.c
        src/helper/uihelper.h
        src/helper/utils.c
        src/helper/utils.h
        src/input/input.c
        src/input/input.h
        src/input/keyboard.c
        src/input/keyboard.h
        src/list/list.c
        src/list/list.h
        src/modal/modal.c
        src/modal/modal.h
        src/search/search.c
        src/search/search.h
        src/state/statehandler.c
        src/state/statehandler.h
        src/state/statesystem.c
        src/state/statesystem.h
        src/themes/loading.c
        src/themes/loading.h
        src/themes/rendering.c
        src/themes/rendering.h
        src/ui/display.c
        src/ui/display.h
        src/ui/rendering.c
        src/ui/rendering.h
        src/ui/uihandler.c
        src/ui/uihandler.h
        )
add_executable(romfetcher ${SOURCES_APPLICATION})