
#include <vamp/vamp.h>
#include <vamp-sdk/PluginAdapter.h>

#include "ConstrainedHarmonicPeak.h"

static Vamp::PluginAdapter<ConstrainedHarmonicPeak> chpAdapter;

const VampPluginDescriptor *
vampGetPluginDescriptor(unsigned int version, unsigned int index)
{
    if (version < 1) return 0;

    switch (index) {
    case  0: return chpAdapter.getDescriptor();
    default: return 0;
    }
}


