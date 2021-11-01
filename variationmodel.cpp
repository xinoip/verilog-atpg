#include "variationmodel.h"

VariationModel::VariationModel(int levels, std::vector<double> variationRatios, std::vector<std::string> pars, std::vector<double> meanValues)
{
    numLevels = levels;
    parNames = pars;
    if(pars.size() != variationRatios.size() || pars.size() != meanValues.size())
    {
        throw std::invalid_argument("Error: Number of paramaters and variation rations do not match!!");
    }
    varAmounts = variationRatios;
    means = meanValues;

    for(size_t i = 0; i < parNames.size(); ++i)
    {
        double interStd, intraStd;
        double sigma = means[i] * variationRatios[i] / 3;
        interStd = sqrt(sigma * sigma / 2) ;
        intraStd = interStd;
        stdDeviationPerLevel[i] = std::sqrt(intraStd * intraStd / numLevels);
        stdDeviationInter[i] = interStd;
    }

}


void VariationModel::GenerateModelFiles(std::string fileLoc)
{

}

void VariationModel::setRandomVariables(int numSamples)
{
    std::default_random_engine generator_intra;
    std::default_random_engine generator_inter;
    double interValue;
    int numVars = parNames.size();

    int dimension = 2^numLevels;
    double parSum[numVars][numSamples][dimension][dimension];

    for(std::size_t var = 0; var < parNames.size(); ++var)
    {
        std::normal_distribution<double> distribution_intra(0, stdDeviationPerLevel[var]);
        std::normal_distribution<double> distribution_inter(0, stdDeviationInter[var]);

        for(int i = 0; i < numSamples; ++i)
        {
            interValue = distribution_inter(generator_inter);
            double parArray[numLevels][dimension][dimension];

            for(int j = 1; j <= numLevels; ++j)
            {
                for(int x = 0; x < (2^j); ++x)
                {
                    for(int y = 0; y < (2^j); ++y)
                    {
                        parArray[j][x][y] = distribution_intra(generator_intra);
                        if(j == numLevels)
                        {
                            parSum[var][i][x][y] = 0;
                            for(int lev = 1; lev <= numLevels; ++lev)
                            {
                                parSum[var][i][x][y] += parArray[lev][x/(dimension/(2^lev))][y/(dimension/(2^lev))];
                            }
                            parSum[var][i][x][y] += interValue + means[var];
                        }
                    }
                }
            }

        }
    }

}
