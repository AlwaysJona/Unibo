#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <map>
#include <vector>

#include "particle_type.hpp"
#include "resonance_type.hpp"

class Particle
{
public:
  Particle(const char *Name = 0, const double Px = 0, const double Py = 0,
           const double Pz = 0);

  int FindParticle(const char *name) const;

  void SetP(const double px, const double py, const double pz);

  int GetIndex() const;

  int GetPx() const;
  int GetPy() const;
  int GetPz() const;

  static void AddParticleType(const char *Name, const double Mass,
                              const int Charge, const double Width = 0);

  void SetIndex(const int &index);
  void SetIndex(const char *name);

  static void PrintParticleTypes();

  void PrintImpulse() const;

  int GetParticleMass() const;

  double TotalEnergy() const;

  double InvMass(const Particle &other) const;

  int Decay2body(Particle &dau1, Particle &dau2) const;

private:
  static std::vector<ParticleType *> fParticleType;
  int fIndex;

  double fPx;
  double fPy;
  double fPz;

  void Boost(double bx, double by, double bz);
};

inline double Length(const double x, const double y, const double z)
{
  return std::sqrt(x * x + y * y + z * z);
}
#endif
