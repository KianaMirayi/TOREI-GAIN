cmake_minimum_required(VERSION 3.15)
project(MyFirstJucePlugin VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 指向 JUCE 源码目录（需显式指定 binary 目录，因为 JUCE 不在项目子目录下）
add_subdirectory(../JUCE ${CMAKE_BINARY_DIR}/juce)

# 创建音频插件
juce_add_plugin(MyFirstJucePlugin
    COMPANY_NAME "MyCompany"
    PLUGIN_MANUFACTURER_CODE Juce
    PLUGIN_CODE Dem0
    FORMATS VST3 Standalone
    PRODUCT_NAME "TOREI GAIN"
    VERSION 1.0.0
    COPY_PLUGIN_AFTER_BUILD TRUE)

# 生成 JuceHeader.h
juce_generate_juce_header(MyFirstJucePlugin)

# 编译插件源文件
target_sources(MyFirstJucePlugin PRIVATE
    src/PluginProcessor.cpp
    src/PluginEditor.cpp)

# 设置编译定义
target_compile_definitions(MyFirstJucePlugin
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

# 链接 JUCE 模块（替换 gui_extra → audio_utils）
target_link_libraries(MyFirstJucePlugin
    PRIVATE
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
