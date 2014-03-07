#ifndef CONSTRAINEDHARMONICPEAK_H
#define CONSTRAINEDHARMONICPEAK_H

#include <vamp-sdk/Plugin.h>

using std::string;

class ConstrainedHarmonicPeak : public Vamp::Plugin
{
public:
    ConstrainedHarmonicPeak(float inputSampleRate);
    virtual ~ConstrainedHarmonicPeak();

    string getIdentifier() const;
    string getName() const;
    string getDescription() const;
    string getMaker() const;
    int getPluginVersion() const;
    string getCopyright() const;

    InputDomain getInputDomain() const;
    size_t getPreferredBlockSize() const;
    size_t getPreferredStepSize() const;
    size_t getMinChannelCount() const;
    size_t getMaxChannelCount() const;

    ParameterList getParameterDescriptors() const;
    float getParameter(string identifier) const;
    void setParameter(string identifier, float value);

    ProgramList getPrograms() const;
    string getCurrentProgram() const;
    void selectProgram(string name);

    OutputList getOutputDescriptors() const;

    bool initialise(size_t channels, size_t stepSize, size_t blockSize);
    void reset();

    FeatureSet process(const float *const *inputBuffers,
                       Vamp::RealTime timestamp);

    FeatureSet getRemainingFeatures();

protected:
    int m_blockSize;
    float m_minFreq;
    float m_maxFreq;
    int m_harmonics;

    static double findInterpolatedPeak(const double *in, int peakbin, int bins);
};



#endif
