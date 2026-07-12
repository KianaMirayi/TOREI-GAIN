#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class ModernLookAndFeel : public juce::LookAndFeel_V4
{
public:
    ModernLookAndFeel()
    {
        setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xfff0a84a));
        setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xff343945));
        setColour(juce::Slider::thumbColourId, juce::Colour(0xffffd08a));
        setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xfff3f4f6));
        setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xff20242c));
        setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0x00000000));
        setColour(juce::Slider::textBoxHighlightColourId, juce::Colour(0x33f0a84a));
    }

    juce::Label* createSliderTextBox(juce::Slider& slider) override
    {
        auto* label = juce::LookAndFeel_V4::createSliderTextBox(slider);
        label->setFont(juce::Font(juce::FontOptions(15.0f, juce::Font::bold)));
        label->setJustificationType(juce::Justification::centred);
        return label;
    }

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPosProportional, float rotaryStartAngle,
                          float rotaryEndAngle, juce::Slider& slider) override
    {
        auto bounds = juce::Rectangle<float>((float) x, (float) y, (float) width, (float) height).reduced(10.0f);
        auto size = juce::jmin(bounds.getWidth(), bounds.getHeight());
        auto radius = size * 0.5f;
        auto centre = bounds.getCentre();
        auto trackRadius = radius - 9.0f;
        auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
        auto zeroAngle = rotaryStartAngle + 0.5f * (rotaryEndAngle - rotaryStartAngle);

        for (int i = 0; i < 5; ++i)
        {
            auto alpha = 0.10f - (float) i * 0.015f;
            g.setColour(juce::Colours::black.withAlpha(alpha));
            g.fillEllipse(bounds.reduced((float) i * 2.0f).translated(0.0f, 7.0f + (float) i));
        }

        juce::ColourGradient knobGradient(juce::Colour(0xff373d49), centre.x, bounds.getY(),
                                          juce::Colour(0xff151820), centre.x, bounds.getBottom(), false);
        g.setGradientFill(knobGradient);
        g.fillEllipse(bounds.reduced(8.0f));

        g.setColour(juce::Colour(0xff59606c).withAlpha(0.65f));
        g.drawEllipse(bounds.reduced(8.0f), 1.0f);

        auto makeArc = [&] (float from, float to)
        {
            juce::Path path;
            path.addCentredArc(centre.x, centre.y, trackRadius, trackRadius, 0.0f, from, to, true);
            return path;
        };

        g.setColour(findColour(juce::Slider::rotarySliderOutlineColourId));
        g.strokePath(makeArc(rotaryStartAngle, rotaryEndAngle),
                     juce::PathStrokeType(7.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        auto fillStart = juce::jmin(zeroAngle, angle);
        auto fillEnd = juce::jmax(zeroAngle, angle);
        auto fillColour = slider.getValue() >= 0.0 ? juce::Colour(0xfff0a84a) : juce::Colour(0xff55c7ff);

        g.setColour(fillColour);
        g.strokePath(makeArc(fillStart, fillEnd),
                     juce::PathStrokeType(7.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        auto drawMarker = [&] (float markerAngle, float inner, float outer, juce::Colour colour, float thickness)
        {
            auto start = centre.getPointOnCircumference(trackRadius - inner, markerAngle);
            auto end = centre.getPointOnCircumference(trackRadius + outer, markerAngle);
            g.setColour(colour);
            g.drawLine(juce::Line<float>(start, end), thickness);
        };

        drawMarker(rotaryStartAngle, 3.0f, 3.0f, juce::Colour(0xff68707d), 1.5f);
        drawMarker(zeroAngle, 5.0f, 6.0f, juce::Colour(0xfff3f4f6), 2.0f);
        drawMarker(rotaryEndAngle, 3.0f, 3.0f, juce::Colour(0xff68707d), 1.5f);

        auto pointerEnd = centre.getPointOnCircumference(trackRadius - 18.0f, angle);
        g.setColour(juce::Colour(0xfff6f7fb));
        g.drawLine(juce::Line<float>(centre, pointerEnd), 3.0f);

        g.setColour(juce::Colour(0xff0f1117));
        g.fillEllipse(centre.x - 5.0f, centre.y - 5.0f, 10.0f, 10.0f);
        g.setColour(fillColour);
        g.drawEllipse(centre.x - 5.0f, centre.y - 5.0f, 10.0f, 10.0f, 1.5f);
    }
};

class MyFirstJucePluginAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit MyFirstJucePluginAudioProcessorEditor(MyFirstJucePluginAudioProcessor&);
    ~MyFirstJucePluginAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    MyFirstJucePluginAudioProcessor& processorRef;

    ModernLookAndFeel modernLAF;

    juce::Label titleLabel;
    juce::Label subtitleLabel;
    juce::Slider gainSlider;
    juce::Label gainLabel;
    juce::Label minLabel;
    juce::Label zeroLabel;
    juce::Label maxLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyFirstJucePluginAudioProcessorEditor)
};
