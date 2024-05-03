#ifndef RESONANCE_TYPE_HPP
#define RESONANCE_TYPE_HPP

#include "particle_type.hpp"

class ResonanceType : public ParticleType
{
public:
  ResonanceType(const char *Name, const double Mass, const int Charge,
                const double Width)
      : ParticleType(Name, Mass, Charge), fWidth{Width} {}

  double GetWidth() const;

  void Print() const;

private:
  const double fWidth;
};

#endif
