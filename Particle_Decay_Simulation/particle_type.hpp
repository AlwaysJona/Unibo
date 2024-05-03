#ifndef PARTICLETYPE_HPP
#define PARTICLETYPE_HPP

#include <iostream>

class ParticleType
{
public:
  ParticleType(const char *Name, const double Mass, const int Charge)
      : fName{Name}, fMass{Mass}, fCharge{Charge} {}

  virtual double GetWidth() { return 0; }
  const char *GetName() const;
  double GetMass() const;
  int GetCharge() const;

  void Print() const;

private:
  const char *fName;
  const double fMass;
  const int fCharge;
};

#endif
