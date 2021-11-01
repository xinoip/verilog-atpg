#ifndef VARIATIONMODEL_H
#define VARIATIONMODEL_H

#include<iostream>
#include<vector>
#include<cmath>
#include<random>

class VariationModel
{

private:
    //Par means
    std::vector<double> means;

    //Number of levels
    int numLevels;

    //A 2d array of random variables.
    std::vector<std::vector<double>> randomVariables;

    //Variable Names
    std::vector<std::string> parNames;

    //Standard Variation Amount for inter die variation.
    std::vector<double> stdDeviationInter;

    //Standard Variation Amounts. An array holding variation amounts of each level.
    std::vector<double> stdDeviationPerLevel;

    //3sigma/mu ratios for each parameters
    std::vector<double> varAmounts;



public:
    VariationModel(int levels, std::vector<double> variationRatios, std::vector<std::string> pars, std::vector<double> meanValues);
    void GenerateModelFiles(std::string fileLoc);
    void setRandomVariables(int numSamples);
    //void setDeviationPerLevel(std::vector<double> ratios);
};

#endif // VARIATIONMODEL_H
