
void ReadHistograms(){
    TFile *file = new Tfile("istogrammi.root");

    TH1F *histo1=(TH1F*)file->Get("histo1");
    int expectedEntries = 10E7;
    int entries = h1->GetEntries();
    double ratio = entries/expectedEntries;
    cout << "\n\n******** Analisi numero di ingressi ********";
    cout << "\nExpected entries: " << expectedEntries;
    cout << "\nEntries: " << entries;
    cout << "\nRatio:" << histo1->GetNBins();

    if (ratio > 1.1){
        cout << "\nNumero di ingressi troppo alto";
    } else if (ratio < 0.9){
        cout << "\nNumero di ingressi troppo basso";            
    } else {
        cout << "\nNumero di ingresi atteso";
    }

    cout << "\n******** Analisi percentuali di particelle ********";

    TH1F *hParticleTypes=(TH1F*)file->Get("hParticleTypes");
    int expectedPercentages[7] = {0.4,0.4,0.05,0.05,0,0,0}; //allora levo anche getnbins sotto
    for (int i=0; i<hParticleTypes->GetNBins(); i++) {
        double upperBound = expectedPercentages[i] + hParticleTypes->GetBinError(i)/entries;
        double lowerBound = expectedPercentages[i] - hParticleTypes->GetBinError(i)/entries;
        double percentage = hParticleTypes->GetBinContent(i)/entries;
        cout<<"\nTipo di particella: " << i;
        cout<<"\nPercentuale sul totale: " << percentage << " +- " << hParticleTypes->GetBinError(i)/entries;
        if (percentage>upperBound){
            cout<<"\nLa percentuale è maggiore di quella attesa.";
        } else if (percentage<lowerBound){
            cout<<"\nLa percentuale è minore di quella attesa.";
        } else{
            cout << "\nLa percentuale è coerente con quella attesa.";
        }
    }

    cout << "\n**** Verifica consistenza distribuzione direzioni angolari con dist. uniforme*****";
    TF1 *polarFit = new TF1("polarFit","[0]",0, M_PI);
    TH1F *hPolar=(TH1F*)file->Get("hPolar");
    hPolar->Fit(polarFit);
    TF1 *polarFitFunc = hPolar->GetFit("polarFit");
    cout << "\nComponente polare:";
    cout << "\n    Parametro del fit: " << polarFitFunc->GetParameter(0);
    cout << "\n    Chi quadro: " << polarFitFunc->GetChiSquare();
    cout << "\n    Gradi di libertà: " << polarFitFunc->GetNDF();
    cout << "\n    Probabilità di ottenere il fit: " << 



    TF1 *azimuthalFit = new TF1("f1","[0]",0, 2*M_PI);
    TH1F *hAzimuthal=(TH1F*)file->Get("hAzimuthal");
    hAzimuthal->Fit(azimuthalFit);
    TF1 *azimuthalFitFunc = hAzimuthal->GetFit("azimuthalFit");
    cout << "\n\nComponente azimuthale:";
    cout << "\n    Parametro del fit: " << azimuthalFitFunc->GetParameter(0);
    cout << "\n    Chi quadro: " << azimuthalFitFunc->GetChiSquare();
    cout << "\n    Gradi di libertà: " << azimuthalFitFunc->GetNDF();
    cout << "\n    Probabilità di ottenere il fit: " << 




    

}

