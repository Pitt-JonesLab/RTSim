#pragma once

namespace NVM {

/**
 * MemoryLayers hold simulation actors of the same physical layer (rank, bank,
 * subarray)
 *
 * The layer can send messages to actors, and send responses to higher or lower
 * layers
 */
class MemoryLayer {};

} // namespace NVM