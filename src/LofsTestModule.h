#pragma once

#include "plugin.h"
#include "MeshModule.h"

/**
 * LofsTest Module
 *
 * A Meshtastic firmware plugin that runs comprehensive filesystem tests.
 */
class LofsTestModule : public MeshModule
{
  public:
    LofsTestModule();

  protected:
    /**
     * This module doesn't listen for packets, so always return false
     */
    virtual bool wantPacket(const meshtastic_MeshPacket *p) override { return false; }

  private:
};

extern LofsTestModule *lofsTestModule;

