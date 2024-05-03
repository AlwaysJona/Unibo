#include "particle_type.hpp"

const char *ParticleType::GetName() const { return fName; }

double ParticleType::GetMass() const { return fMass; }

int ParticleType::GetCharge() const { return fCharge; }

void ParticleType::Print() const
{
  std::cout << "Name: " << fName << "\n"
            << "Mass: " << fMass << "\n"
            << "Charge: " << fCharge << "\n \n";
}
