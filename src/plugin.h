#pragma once

#define LOFS_VERSION "0.0.1"

#include "LoFS.h"

#ifdef LOFS_PLUGIN_DIAGNOSTICS
#include "LofsTestModule.h"

/**
 * This plugin registers a Module with Meshtastic. Modules are the primary way
 * to extend Meshtastic with new functionality. Modules are registered with the
 * MPM_REGISTER_MESHTASTIC_MODULE comment directive below.
 */
// MPM_REGISTER_MESHTASTIC_MODULE: LofsTestModule, lofsTestModule, []
#endif

