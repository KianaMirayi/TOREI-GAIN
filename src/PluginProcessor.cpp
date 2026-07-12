#include "PluginProcessor.h"
#include "PluginEditor.h"

// 参数布局
juce::AudioProcessorValueTreeState::ParameterLayout
MyFirstJucePluginAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "gain", 1 },
        "Gain",
        juce::NormalisableRange<float>(-15.0f, 15.0f, 0.0f),
        0.0f,
        juce::AudioParameterFloatAttributes().withStringFromValueFunction(
            [](float value, int) { return juce::String(value, 1) + " dB"; })));

    return layout;
}

MyFirstJucePluginAudioProcessor::MyFirstJucePluginAudioProcessor()
    : juce::AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                              .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                              .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
                              )
    , parameters(*this, nullptr, "PARAMETERS", createParameterLayout())
{
}

MyFirstJucePluginAudioProcessor::~MyFirstJucePluginAudioProcessor() = default;

const juce::String MyFirstJucePluginAudioProcessor::getName() const { return JucePlugin_Name; }
bool MyFirstJucePluginAudioProcessor::acceptsMidi() const { return false; }
bool MyFirstJucePluginAudioProcessor::producesMidi() const { return false; }
bool MyFirstJucePluginAudioProcessor::isMidiEffect() const { return false; }
double MyFirstJucePluginAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int MyFirstJucePluginAudioProcessor::getNumPrograms() { return 1; }
int MyFirstJucePluginAudioProcessor::getCurrentProgram() { return 0; }
void MyFirstJucePluginAudioProcessor::setCurrentProgram(int) {}
const juce::String MyFirstJucePluginAudioProcessor::getProgramName(int) { return {}; }
void MyFirstJucePluginAudioProcessor::changeProgramName(int, const juce::String&) {}

void MyFirstJucePluginAudioProcessor::prepareToPlay(double, int) {}
void MyFirstJucePluginAudioProcessor::releaseResources() {}

bool MyFirstJucePluginAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    return true;
}

void MyFirstJucePluginAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    // 从参数原子变量中读取线性增益
    const float linearGain = juce::Decibels::decibelsToGain(parameters.getRawParameterValue("gain")->load());
    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();

    // 对每个声道施加增益
    for (int ch = 0; ch < numChannels; ++ch)
    {
        float* data = buffer.getWritePointer(ch);
        for (int s = 0; s < numSamples; ++s)
            data[s] *= linearGain;
    }

    // 输出静默声道清零
    for (int ch = getTotalNumInputChannels(); ch < getTotalNumOutputChannels(); ++ch)
        buffer.clear(ch, 0, numSamples);
}

bool MyFirstJucePluginAudioProcessor::hasEditor() const { return true; }
juce::AudioProcessorEditor* MyFirstJucePluginAudioProcessor::createEditor()
{
    return new MyFirstJucePluginAudioProcessorEditor(*this);
}

void MyFirstJucePluginAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void MyFirstJucePluginAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState != nullptr)
        parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MyFirstJucePluginAudioProcessor();
}