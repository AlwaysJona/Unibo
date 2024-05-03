#include <cmath>
#include <vector>

#include "TFile.h"
#include "TH1F.h"
#include "TRandom.h"
#include "TCanvas.h"
#include "particle.hpp"
#include "particle_type.hpp"
#include "resonance_type.hpp"

int Main()
{
  TCanvas *c1 = new TCanvas("c1", "hist", 700, 500);
  Printf("Defining particle");
  gRandom->SetSeed();
  Particle::AddParticleType("pione+", 0.13957, 1);    // pione+
  Particle::AddParticleType("pione-", 0.13957, -1);   // pione-
  Particle::AddParticleType("Kaone+", 0.49367, 1);    // kaone+
  Particle::AddParticleType("Kaone-", 0.49367, -1);   // kaone-
  Particle::AddParticleType("protone+", 0.93827, 1);  // protone+
  Particle::AddParticleType("protone-", 0.93827, -1); // protone-
  Particle::AddParticleType("K*", 0.89166, 0, 0.050); // risonanza K*
  Printf("Particles are defined");
  int n = 10e5;
  std::vector<Particle> EventParticles;

  TH1F *hParticleTypes = new TH1F("hParticleTypes", "Partle Types", 7, 0, 6);
  TH1F *hPolar = new TH1F("h2", "Polar", 100, 0, M_PI);
  TH1F *hAzimuthal = new TH1F("h3", "Azimuthal", 100, 0, 2 * M_PI);
  TH1F *hImpulse = new TH1F("h4", "Impulse", 100, 0, 100);
  TH1F *hTrasverseP = new TH1F("h5", "Trasverse P", 100, 0, 100);
  TH1F *hEnergy = new TH1F("h6", "Energy", 100, 0, 1000000);
  TH1F *hInvariantMass = new TH1F("h7", "Invariant Mass", 100, 0, 1000000);

  TH1F *hInvMass1 = new TH1F("h8", "Invariant Mass Opposite Sign", 100, 0,
                             1000000); // segno discorde
  TH1F *hInvMass2 = new TH1F("h9", "Invariant Mass Same Sign", 100, 0,
                             1000000); // segno concorde
  TH1F *hInvMass3 =
      new TH1F("h10", "Invariant Mass pione+/Kaone- or pione-/Kaone+", 100, 0,
               1000000); // pione+/Kaone- or pione-/Kaone+
  TH1F *hInvMass4 =
      new TH1F("h11", "Invariant Mass pione+/Kaone+ or pione-/Kaone-", 100, 0,
               1000000); // pione+/Kaone+ or pione-/Kaone-
  TH1F *hInvMass5 = new TH1F("h12", "Invariant Mass Decay Particles", 100, 0,
                             1000000); // particelle di decadimento

  hParticleTypes->Sumw2();
  hPolar->Sumw2();
  hAzimuthal->Sumw2();
  hImpulse->Sumw2();
  hTrasverseP->Sumw2();
  hEnergy->Sumw2();
  hInvariantMass->Sumw2();
  hInvMass1->Sumw2();
  hInvMass2->Sumw2();
  hInvMass3->Sumw2();
  hInvMass4->Sumw2();
  hInvMass5->Sumw2();

  for (int i = 0; i < 100; i++)
  {
    Particle t_part;
    EventParticles.push_back(t_part);
  }

  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < 100; j++)
    {
      double phi = gRandom->Uniform(0, 2 * M_PI);
      double theta = gRandom->Uniform(0, M_PI);

      hPolar->Fill(theta);
      hAzimuthal->Fill(phi);

      double p = gRandom->Exp(1);

      hImpulse->Fill(p);

      double px = p * sin(theta) * cos(phi);
      double py = p * sin(theta) * sin(phi);
      double pz = p * cos(theta);

      double tr_imp = std::sqrt(px * px + py * py);
      hTrasverseP->Fill(tr_imp);
      hEnergy->Fill(EventParticles[j].TotalEnergy());

      EventParticles[j].SetP(px, py, pz);
      float rand = gRandom->Uniform(1, 100);
      if (rand <= 80)
      {
        if (rand <= 40)
        {
          EventParticles[j].SetIndex("pione+");
          hParticleTypes->Fill(EventParticles[j].FindParticle("pione+"));
        }
        else
        {
          EventParticles[j].SetIndex("pione-");
          hParticleTypes->Fill(EventParticles[j].FindParticle("pione-"));
        }
      }
      else if (rand <= 90)
      {
        if (rand <= 85)
        {
          EventParticles[j].SetIndex("Kaone+");
          hParticleTypes->Fill(EventParticles[j].FindParticle("Kaone+"));
        }
        else
        {
          EventParticles[j].SetIndex("Kaone-");
          hParticleTypes->Fill(EventParticles[j].FindParticle("Kaone-"));
        }
      }
      else if (rand <= 99)
      {
        if (rand <= 94.5)
        {
          EventParticles[j].SetIndex("protone+");
          hParticleTypes->Fill(EventParticles[j].FindParticle("protone+"));
        }
        else
        {
          EventParticles[j].SetIndex("protone-");
          hParticleTypes->Fill(EventParticles[j].FindParticle("protone-"));
        }
      }
      else
      {
        EventParticles[j].SetIndex("K*");
        int decay = std::rand() % 2;
        Particle p1, p2;

        if (decay)
        {
          p1.SetIndex("pione+");
          p2.SetIndex("Kaone-");
          EventParticles[j].Decay2body(p1, p2);
          hParticleTypes->Fill(EventParticles[j].FindParticle("pione+"));
          hParticleTypes->Fill(EventParticles[j].FindParticle("Kaone-"));
          EventParticles.push_back(p1);
          EventParticles.push_back(p2);
          hInvMass5->Fill(p1.InvMass(p2));
        }
        else
        {
          p1.SetIndex("pione-");
          p2.SetIndex("Kaone+");
          EventParticles[j].Decay2body(p1, p2);
          hParticleTypes->Fill(EventParticles[j].FindParticle("pione-"));
          hParticleTypes->Fill(EventParticles[j].FindParticle("Kaone+"));
          EventParticles.push_back(p1);
          EventParticles.push_back(p2);
          hInvMass5->Fill(p1.InvMass(p2));
        }
      }
    }

    for (int i = 0; i < (int)EventParticles.size(); ++i)
    {
      for (int j = i + 1; j < (int)EventParticles.size(); ++j)
      {
        hInvariantMass->Fill(EventParticles[i].InvMass(EventParticles[j]));
        int index1 = EventParticles[i].GetIndex();
        int index2 = EventParticles[j].GetIndex();
        if (index1 != 6 && index2 != 6)
        {
          if (index1 % 2 != index2 % 2) // segni discordi
          {
            hInvMass1->Fill(EventParticles[i].InvMass(EventParticles[j]));
          }
          else // segni concordi
          {
            hInvMass2->Fill(EventParticles[i].InvMass(EventParticles[j]));
          }
          if (index1 + index2 == 3)
          {
            if (index1 == 0 || index1 == 3) // pione+/Kaone-
            {
              hInvMass3->Fill(EventParticles[i].InvMass(EventParticles[j]));
            }
            else // pione-/Kaone+
            {
              hInvMass4->Fill(EventParticles[i].InvMass(EventParticles[j]));
            }
          }
        }
      }
    }
  }

  // creazione e riempimento ROOT file
  TFile *file = new TFile("histograms.root");
  hParticleTypes->Draw();
  hParticleTypes->Write();
  hPolar->Write();
  hAzimuthal->Write();
  hImpulse->Write();
  hTrasverseP->Write();
  hEnergy->Write();
  hInvariantMass->Write();
  hInvMass1->Write();
  hInvMass2->Write();
  hInvMass3->Write();
  hInvMass4->Write();
  hInvMass5->Write();

  return 0;
}
