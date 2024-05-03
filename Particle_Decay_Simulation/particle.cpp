#include <cmath>   // for M_PI
#include <cstdlib> //for RAND_MAX

#include "particle.hpp"

#include "particle_type.hpp"
#include "resonance_type.hpp"

std::vector<ParticleType *> Particle::fParticleType;

Particle::Particle(const char *name, const double Px, const double Py,
                   const double Pz) : fPx{Px}, fPy{Py}, fPz{Pz}
{
  int index = FindParticle(name);
  if (index < (int)fParticleType.size())
  {
    fIndex = index;
  }
  else
  {
    std::cerr << "Particle type not found \n";
  }
}

int Particle::FindParticle(const char *name) const
{
  for (auto it = fParticleType.begin(); it != fParticleType.end(); ++it)
  {
    if ((*it)->GetName() == name)
    {
      return (int)(it - fParticleType.begin());
    }
  }
  return fParticleType.size();
}

void Particle::SetP(const double px, const double py, const double pz)
{
  fPx = px;
  fPy = py;
  fPz = pz;
}

int Particle::GetIndex() const { return fIndex; }

int Particle::GetPx() const { return fPx; }
int Particle::GetPy() const { return fPy; }
int Particle::GetPz() const { return fPz; }

void Particle::AddParticleType(const char *name, const double mass,
                               const int charge, const double width)
{
  ParticleType *pt = new ResonanceType(name, mass, charge, width);
  fParticleType.push_back(pt);
}

void Particle::SetIndex(const int &index)
{
  if (index < (int)fParticleType.size())
  {
    fIndex = index;
  }
  else
  {
    std::cerr << "Index not found \n";
  }
}

void Particle::SetIndex(const char *name)
{
  auto it = fParticleType.begin();
  for (; it != fParticleType.end(); ++it)
  {
    if ((*it)->GetName() == name)
    {
      fIndex = (int)(it - fParticleType.begin());
      break;
    }
  }
  if (it == fParticleType.end())
  {
    std::cerr << "Particle name not found \n";
  }
}

void Particle::PrintParticleTypes()
{
  for (auto particleType : fParticleType)
  {
    particleType->Print();
    std::cout << '\n';
  }
}

void Particle::PrintImpulse() const
{
  std::cout << "Index: " << fIndex << '\n'
            << "Name: " << fParticleType[fIndex]->GetName() << '\n'
            << "Impulse (Px,Py,Pz): (" << fPx << ", " << fPy << ", " << fPz
            << ") \n";
}

int Particle::GetParticleMass() const
{
  return fParticleType[fIndex]->GetMass();
}

double Particle::TotalEnergy() const
{
  return std::sqrt(this->GetParticleMass() * this->GetParticleMass() +
                   Length(fPx, fPy, fPz) * Length(fPx, fPy, fPz));
}

double Particle::InvMass(const Particle &other) const
{
  return std::sqrt(
      (this->TotalEnergy() + other.TotalEnergy()) *
          (this->TotalEnergy() + other.TotalEnergy()) -
      Length(fPx + other.GetPx(), fPy + other.GetPy(), fPz + other.GetPz()) *
          Length(fPx + other.GetPx(), fPy + other.GetPy(),
                 fPz + other.GetPz()));
}

int Particle::Decay2body(Particle &dau1, Particle &dau2) const
{
  if (this->GetParticleMass() == 0.0)
  {
    printf("Decayment cannot be preformed if mass is zero\n");
    return 1;
  }

  double massMot = this->GetParticleMass();
  double massDau1 = dau1.GetParticleMass();
  double massDau2 = dau2.GetParticleMass();

  if (fIndex > -1)
  { // add width effect

    // gaussian random numbers

    float x1, x2, w, y1;

    double invnum = 1. / RAND_MAX;
    do
    {
      x1 = 2.0 * rand() * invnum - 1.0;
      x2 = 2.0 * rand() * invnum - 1.0;
      w = x1 * x1 + x2 * x2;
    } while (w >= 1.0);

    w = sqrt((-2.0 * log(w)) / w);
    y1 = x1 * w;

    massMot += fParticleType[fIndex]->GetWidth() * y1;
  }

  if (massMot < massDau1 + massDau2)
  {
    printf("Decayment cannot be preformed because mass is too low in this channel\n");
    return 2;
  }

  double pout = sqrt((massMot * massMot - (massDau1 + massDau2) * (massDau1 + massDau2)) * (massMot * massMot - (massDau1 - massDau2) * (massDau1 - massDau2))) / massMot * 0.5;

  double norm = 2 * M_PI / RAND_MAX;

  double phi = rand() * norm;
  double theta = rand() * norm * 0.5 - M_PI / 2.;
  dau1.SetP(pout * sin(theta) * cos(phi), pout * sin(theta) * sin(phi), pout * cos(theta));
  dau2.SetP(-pout * sin(theta) * cos(phi), -pout * sin(theta) * sin(phi), -pout * cos(theta));

  double energy = sqrt(fPx * fPx + fPy * fPy + fPz * fPz + massMot * massMot);

  double bx = fPx / energy;
  double by = fPy / energy;
  double bz = fPz / energy;

  dau1.Boost(bx, by, bz);
  dau2.Boost(bx, by, bz);

  return 0;
}

void Particle::Boost(double bx, double by, double bz)
{

  double energy = TotalEnergy();

  // Boost this Lorentz vector
  double b2 = bx * bx + by * by + bz * bz;
  double gamma = 1.0 / sqrt(1.0 - b2);
  double bp = bx * fPx + by * fPy + bz * fPz;
  double gamma2 = b2 > 0 ? (gamma - 1.0) / b2 : 0.0;

  fPx += gamma2 * bp * bx + gamma * bx * energy;
  fPy += gamma2 * bp * by + gamma * by * energy;
  fPz += gamma2 * bp * bz + gamma * bz * energy;
}
