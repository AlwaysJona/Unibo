#include "resonance_type.hpp"

double ResonanceType::GetWidth() const { return fWidth; }

void ResonanceType::Print() const
{
  ParticleType::Print();
  std::cout << "Width: " << fWidth << "\n";
}
