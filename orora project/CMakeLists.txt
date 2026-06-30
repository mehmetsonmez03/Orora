# ORORA AI - Master/Oracle Build Configuration
cmake_minimum_required(VERSION 3.20)
project(ORORA VERSION 5.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui OpenGL OpenGLWidgets Multimedia Sql Network Concurrent)
find_package(OpenGL REQUIRED)

include_directories(
    ${CMAKE_SOURCE_DIR}/orora
    ${CMAKE_SOURCE_DIR}/orora/core
    ${CMAKE_SOURCE_DIR}/orora/orora_core
    ${CMAKE_SOURCE_DIR}/orora/voice_system
    ${CMAKE_SOURCE_DIR}/orora/ui
    ${CMAKE_SOURCE_DIR}/orora/gui
    ${CMAKE_SOURCE_DIR}/orora/orora_intelligence
)

set(ORORA_CORE_SOURCES
    orora/orora_app.cpp
    # Professional Core Managers
    orora/core/build_manager.cpp
    orora/core/git_manager.cpp
    orora/core/debug_manager.cpp
    orora/core/lsp_manager.cpp
    orora/core/search_manager.cpp
    orora/core/template_manager.cpp
    orora/core/performance_monitor.cpp
    orora/core/documentation_engine.cpp
    orora/core/data_lab_engine.cpp
    orora/core/remote_manager.cpp
    orora/core/auto_fix_engine.cpp
    orora/core/fullstack_lab.cpp
    # GUI & UX
    orora/gui/coding_window.cpp
    orora/gui/orora_editor.cpp
    orora/gui/cpp_highlighter.cpp
    orora/gui/terminal_widget.cpp
    orora/gui/context_menus.cpp
    orora/gui/character2d_widget.cpp
    # Intelligence Layer
    orora/orora_intelligence/orora_manager.cpp
    orora/orora_intelligence/translation_manager.cpp
    # Legacy & Specialized Consciousness (Selected)
    orora/orora_core/advanced_consciousness.cpp
    orora/orora_core/core/agent_memory.cpp
    orora/orora_core/core/autonomous_loop.cpp
)

set(ORORA_VOICE_SOURCES
    orora/voice_system/native_stt_engine.cpp
    orora/voice_system/native_tts_engine.cpp
    orora/voice_system/bark_tts_client.cpp
)

set(ORORA_UI_SOURCES
    orora/ui/chat_panel.cpp
    orora/ui/file_exporter.cpp
)

add_executable(orora
    orora/main.cpp
    ${ORORA_CORE_SOURCES}
    ${ORORA_VOICE_SOURCES}
    ${ORORA_UI_SOURCES}
)

target_link_libraries(orora PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    Qt6::Multimedia
    Qt6::Sql
    Qt6::Network
    Qt6::Concurrent
    OpenGL::GL
)

message(STATUS "Oracle Edition Build System Ready.")
