
#include "ConstrainedHarmonicPeak.h"

#include <cmath>
#include <cstdio>

using std::cerr;
using std::endl;
using std::vector;

ConstrainedHarmonicPeak::ConstrainedHarmonicPeak(float inputSampleRate) :
    Plugin(inputSampleRate),
    m_blockSize(0),
    m_minFreq(0),
    m_maxFreq(inputSampleRate/2)
{
}

ConstrainedHarmonicPeak::~ConstrainedHarmonicPeak()
{
}

string
ConstrainedHarmonicPeak::getIdentifier() const
{
    return "constrainedharmonicpeak";
}

string
ConstrainedHarmonicPeak::getName() const
{
    return "Frequency-Constrained Harmonic Peak";
}

string
ConstrainedHarmonicPeak::getDescription() const
{
    //!!! Return something helpful here!
    return "";
}

string
ConstrainedHarmonicPeak::getMaker() const
{
    return "Queen Mary, University of London";
}

int
ConstrainedHarmonicPeak::getPluginVersion() const
{
    return 1;
}

string
ConstrainedHarmonicPeak::getCopyright() const
{
    return "GPL";
}

ConstrainedHarmonicPeak::InputDomain
ConstrainedHarmonicPeak::getInputDomain() const
{
    return FrequencyDomain;
}

size_t
ConstrainedHarmonicPeak::getPreferredBlockSize() const
{
    return 2048;
}

size_t 
ConstrainedHarmonicPeak::getPreferredStepSize() const
{
    return 512;
}

size_t
ConstrainedHarmonicPeak::getMinChannelCount() const
{
    return 1;
}

size_t
ConstrainedHarmonicPeak::getMaxChannelCount() const
{
    return 1;
}

ConstrainedHarmonicPeak::ParameterList
ConstrainedHarmonicPeak::getParameterDescriptors() const
{
    ParameterList list;

    ParameterDescriptor d;
    d.identifier = "minfreq";
    d.name = "Minimum frequency";
    d.description = "";
    d.unit = "Hz";
    d.minValue = 0;
    d.maxValue = m_inputSampleRate/2;
    d.defaultValue = 0;
    d.isQuantized = false;
    list.push_back(d);

    d.identifier = "maxfreq";
    d.name = "Maximum frequency";
    d.description = "";
    d.unit = "Hz";
    d.minValue = 0;
    d.maxValue = m_inputSampleRate/2;
    d.defaultValue = 0;
    d.isQuantized = false;
    list.push_back(d);

    return list;
}

float
ConstrainedHarmonicPeak::getParameter(string identifier) const
{
    if (identifier == "minfreq") {
	return m_minFreq;
    } else if (identifier == "maxfreq") {
	return m_maxFreq;
    }
    return 0;
}

void
ConstrainedHarmonicPeak::setParameter(string identifier, float value) 
{
    if (identifier == "minfreq") {
	m_minFreq = value;
    } else if (identifier == "maxfreq") {
	m_maxFreq = value;
    }
}

ConstrainedHarmonicPeak::ProgramList
ConstrainedHarmonicPeak::getPrograms() const
{
    ProgramList list;
    return list;
}

string
ConstrainedHarmonicPeak::getCurrentProgram() const
{
    return ""; // no programs
}

void
ConstrainedHarmonicPeak::selectProgram(string name)
{
}

ConstrainedHarmonicPeak::OutputList
ConstrainedHarmonicPeak::getOutputDescriptors() const
{
    OutputList list;

    OutputDescriptor d;
    d.identifier = "peak";
    d.name = "Peak frequency";
    d.description = "";
    d.unit = "Hz";
    d.sampleType = OutputDescriptor::OneSamplePerStep;
    d.hasDuration = false;
    list.push_back(d);

    return list;
}

bool
ConstrainedHarmonicPeak::initialise(size_t channels, size_t stepSize, size_t blockSize)
{
    if (channels < getMinChannelCount() ||
	channels > getMaxChannelCount()) {
	cerr << "ConstrainedHarmonicPeak::initialise: ERROR: channels " << channels
	     << " out of acceptable range " << getMinChannelCount()
	     << " -> " << getMaxChannelCount() << endl;
	return false;
    }

    m_blockSize = blockSize;

    return true;
}

void
ConstrainedHarmonicPeak::reset()
{
}

ConstrainedHarmonicPeak::FeatureSet
ConstrainedHarmonicPeak::process(const float *const *inputBuffers, Vamp::RealTime timestamp)
{
    FeatureSet fs;

    return fs;
}

ConstrainedHarmonicPeak::FeatureSet
ConstrainedHarmonicPeak::getRemainingFeatures()
{
    FeatureSet fs;

    return fs;
}

