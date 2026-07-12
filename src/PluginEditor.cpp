#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace
{
    constexpr auto editorWidth = 440;
    constexpr auto editorHeight = 350;

    void configureLabel(juce::Label& label, const juce::String& text, float size,
                        juce::Colour colour, juce::Justification justification)
    {
        label.setText(text, juce::dontSendNotification);
        label.setFont(juce::Font(juce::FontOptions(size)));
        label.setColour(juce::Label::textColourId, colour);
        label.setJustificationType(justification);
        label.setInterceptsMouseClicks(false, false);
    }
}

MyFirstJucePluginAudioProcessorEditor::MyFirstJucePluginAudioProcessorEditor(MyFirstJucePluginAudioProcessor& p)
    : AudioProcessorEditor(&p)
    , processorRef(p)
{
    configureLabel(titleLabel, "TOREI GAIN", 24.0f, juce::Colour(0xfff4f0ea), juce::Justification::centred);
    titleLabel.setFont(juce::Font(juce::FontOptions(24.0f, juce::Font::bold)));
    addAndMakeVisible(titleLabel);

    configureLabel(subtitleLabel, "Clean input trim", 13.0f, juce::Colour(0xff8f96a3), juce::Justification::centred);
    addAndMakeVisible(subtitleLabel);

    gainSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 108, 28);
    gainSlider.setRange(-15.0, 15.0, 0.0);
    gainSlider.setValue(0.0);
    gainSlider.setDoubleClickReturnValue(true, 0.0);
    gainSlider.setMouseDragSensitivity(170);
    gainSlider.setRotaryParameters(7.0f * juce::MathConstants<float>::pi / 6.0f,
                                    17.0f * juce::MathConstants<float>::pi / 6.0f,
                                    true);
    gainSlider.setLookAndFeel(&modernLAF);
    addAndMakeVisible(gainSlider);

    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.parameters, "gain", gainSlider);

    configureLabel(gainLabel, "OUTPUT GAIN", 12.0f, juce::Colour(0xffaeb4bf), juce::Justification::centred);
    addAndMakeVisible(gainLabel);

    configureLabel(minLabel, "-15 dB", 12.0f, juce::Colour(0xff6f7784), juce::Justification::centredLeft);
    configureLabel(zeroLabel, "0", 12.0f, juce::Colour(0xffdfe3ea), juce::Justification::centred);
    configureLabel(maxLabel, "+15 dB", 12.0f, juce::Colour(0xff6f7784), juce::Justification::centredRight);
    addAndMakeVisible(minLabel);
    addAndMakeVisible(zeroLabel);
    addAndMakeVisible(maxLabel);

    setSize(editorWidth, editorHeight);
}

MyFirstJucePluginAudioProcessorEditor::~MyFirstJucePluginAudioProcessorEditor()
{
    gainSlider.setLookAndFeel(nullptr);
}

void MyFirstJucePluginAudioProcessorEditor::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    juce::ColourGradient background(juce::Colour(0xff20242d), bounds.getX(), bounds.getY(),
                                    juce::Colour(0xff101217), bounds.getRight(), bounds.getBottom(), false);
    g.setGradientFill(background);
    g.fillRoundedRectangle(bounds, 12.0f);

    auto panel = bounds.reduced(22.0f, 44.0f);
    panel.removeFromTop(34.0f);

    g.setColour(juce::Colour(0xff252a34).withAlpha(0.72f));
    g.fillRoundedRectangle(panel, 18.0f);

    g.setColour(juce::Colour(0xff49515f).withAlpha(0.65f));
    g.drawRoundedRectangle(panel, 18.0f, 1.0f);

    auto glow = panel.withSizeKeepingCentre(panel.getWidth() * 0.72f, panel.getHeight() * 0.82f);
    juce::ColourGradient accentGlow(juce::Colour(0x22f0a84a), glow.getCentreX(), glow.getY(),
                                    juce::Colour(0x0055c7ff), glow.getCentreX(), glow.getBottom(), true);
    g.setGradientFill(accentGlow);
    g.fillEllipse(glow);

    auto topLine = bounds.reduced(28.0f, 0.0f).removeFromTop(1.0f).translated(0.0f, 74.0f);
    g.setColour(juce::Colour(0xfff0a84a).withAlpha(0.32f));
    g.fillRect(topLine);
}

void MyFirstJucePluginAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();

    titleLabel.setBounds(24, 22, area.getWidth() - 48, 30);
    subtitleLabel.setBounds(24, 52, area.getWidth() - 48, 20);

    auto knobSize = 178;
    auto knobX = (area.getWidth() - knobSize) / 2;
    auto knobY = 98;
    gainSlider.setBounds(knobX, knobY, knobSize, knobSize + 40);

    auto scaleY = knobY + knobSize + 1;
    minLabel.setBounds(knobX - 32, scaleY, 76, 20);
    zeroLabel.setBounds(knobX + knobSize / 2 - 20, knobY - 22, 40, 18);
    maxLabel.setBounds(knobX + knobSize - 44, scaleY, 76, 20);

    gainLabel.setBounds(knobX, area.getHeight() - 29, knobSize, 18);
}
