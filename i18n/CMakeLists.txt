if(GETTEXT_FOUND)
        add_definitions(-DENABLE_NLS)
        GETTEXT_CREATE_TRANSLATIONS(pidgin-awayonlock.pot ALL pt_BR.po)
endif()
