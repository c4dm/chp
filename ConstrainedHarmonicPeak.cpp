
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
    m_maxFreq(inputSampleRate/2),
    m_harmonics(5)
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

    d.identifier = "harmonics";
    d.name = "Harmonics";
    d.description = "Maximum number of harmonics to consider";
    d.unit = "";
    d.minValue = 1;
    d.maxValue = 20;
    d.defaultValue = 5;
    d.isQuantized = true;
    d.quantizeStep = 1;
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
    } else if (identifier == "harmonics") {
	return m_harmonics;
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
    } else if (identifier == "harmonics") {
	m_harmonics = int(round(value));
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

double
ConstrainedHarmonicPeak::findInterpolatedPeak(const double *in, 
					      int peakbin,
					      int bins)
{
    // duplicate with SimpleCepstrum plugin
    // after jos, 
    // https://ccrma.stanford.edu/~jos/sasp/Quadratic_Interpolation_Spectral_Peaks.html

    if (peakbin < 1 || peakbin > bins - 2) {
        return peakbin;
    }

    double alpha = in[peakbin-1];
    double beta  = in[peakbin];
    double gamma = in[peakbin+1];

    double denom = (alpha - 2*beta + gamma);

    if (denom == 0) {
        // flat
        return peakbin;
    }

    double p = ((alpha - gamma) / denom) / 2.0;

    return double(peakbin) + p;
}

ConstrainedHarmonicPeak::FeatureSet
ConstrainedHarmonicPeak::process(const float *const *inputBuffers, Vamp::RealTime timestamp)
{
    FeatureSet fs;

    int hs = m_blockSize/2;

    double *mags = new double[hs+1];
    for (int i = 0; i <= hs; ++i) {
	mags[i] = sqrtf(inputBuffers[0][i*2] * inputBuffers[0][i*2] +
			inputBuffers[0][i*2+1] * inputBuffers[0][i*2+1]);
    }

    // bin freq is bin * samplerate / fftsize

    int minbin = int(floor((m_minFreq * m_blockSize) / m_inputSampleRate));
    int maxbin = int(ceil((m_maxFreq * m_blockSize) / m_inputSampleRate));
    if (minbin > hs) minbin = hs;
    if (maxbin > hs) maxbin = hs;
    if (maxbin <= minbin) return fs;

    double *hps = new double[maxbin - minbin + 1];

    // HPS in dB after MzHarmonicSpectrum

    for (int bin = minbin; bin <= maxbin; ++bin) {

	int i = bin - minbin;
	hps[i] = 1.0;

	int contributing = 0;

	for (int j = 1; j <= m_harmonics; ++j) {
	    if (j * bin > hs) break;
	    hps[i] *= mags[j * bin];
	    ++contributing;
	}

	if (hps[i] <= 0.0) {
	    hps[i] = -120.0;
	} else {
	    hps[i] = 20.0 / contributing * log10(hps[i]);
	}
    }

    double maxdb = -120.0;
    int maxidx = 0;
    for (int i = 0; i <= maxbin - minbin; ++i) {
	if (hps[i] > maxdb) {
	    maxdb = hps[i];
	    maxidx = i;
	}
    }

    double interpolated = findInterpolatedPeak(hps, maxidx, maxbin - minbin + 1);
    interpolated = interpolated + minbin;

    double freq = interpolated * m_inputSampleRate / m_blockSize;

    Feature f;
    f.values.push_back(freq);
    fs[0].push_back(f);

    return fs;
}

ConstrainedHarmonicPeak::FeatureSet
ConstrainedHarmonicPeak::getRemainingFeatures()
{
    FeatureSet fs;
    return fs;
}

