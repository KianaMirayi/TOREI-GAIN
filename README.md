# TOREI GAIN

**一个简洁的音频增益插件，支持 VST3 和独立应用。**

## ✨ 特性

- 🎛️ **精细增益控制** — 可调范围 ±15 dB，步进 0.1 dB，旋钮下方显示精确 dB 数值
- ⚡ **低 CPU 占用** — 零延迟直通处理，无额外延迟
- 📦 **独立 + 插件** — 同时提供 VST3 插件和 Standalone 应用程序


## 📥 下载

从 [Releases](https://github.com/KianaMirayi/TOREI-GAIN/releases) 页面下载最新的 VST3 插件包。解压后复制 `TOREI GAIN.vst3` 到系统的 VST3 文件夹：

- **Windows:** `C:\Program Files\Common Files\VST3`

## 🔧 从源码构建

### 依赖

- [JUCE](https://github.com/juce-framework/JUCE) 8.0+
- **Visual Studio 2022/2026** (MSVC) 或 **Clang**
- **CMake** 3.15+

### 构建步骤

1. 克隆本仓库和 JUCE 源码放到同一级目录：

   ```bash
   git clone https://github.com/KianaMirayi/TOREI-GAIN.git
   git clone https://github.com/juce-framework/JUCE.git
   ```

2. 用 CMake 配置项目 (Visual Studio 生成器)：

   ```bash
   cmake -S TOREI-GAIN -B TOREI-GAIN/build -G "Visual Studio 18 2026" -A x64
   ```

3. 编译：

   ```bash
   cmake --build TOREI-GAIN/build --config Release
   ```

4. 产物位置：

   - VST3: `build/MyFirstJucePlugin_artefacts/Release/VST3/TOREI GAIN.vst3`
   - Standalone: `build/MyFirstJucePlugin_artefacts/Release/Standalone/TOREI GAIN.exe`

> **注意:** 项目使用相对路径 `../JUCE` 引用 JUCE 源码。如果你把 JUCE 放在其他位置，请修改 `CMakeLists.txt` 第 9 行。

## 📂 项目结构

```
TOREI-GAIN/
├── src/
│   ├── PluginProcessor.h/cpp    ← 音频处理核心 (增益 DSP)
│   └── PluginEditor.h/cpp       ← 用户界面
├── CMakeLists.txt               ← 构建配置
├── CMakePresets.json            ← 编译器预设 (VS 2026)
└── .gitignore
```

## 🛠️ 技术栈

- **JUCE 8.0** — 音频框架
- **C++17** — 语言标准
- **MSVC 19.51** (Visual Studio 2026) — 编译器
- **CMake** — 构建系统

## 📝 许可证

本项目采用 [MIT License](LICENSE)。

---

欢迎提交 Issue 和 Pull Request！ 🎧
