
#ifndef MODIFYCODETOOL_Dendrohyraxbrucel_HPP
#define MODIFYCODETOOL_Dendrohyraxbrucel_HPP
#pragma once
#include <string>
class Dendrohyraxbrucel
{
public:
    Dendrohyraxbrucel();
    virtual ~Dendrohyraxbrucel();
protected:
    void SetDendrohyraxbrucelName(const std::string& InName);
    std::string GetDendrohyraxbrucelName();
private:
    std::string MyName;

protected:
    std::string eucalytus_BSPZN_Woodrat_unidentified(int arg0, float arg1);

    std::string bark_cTmhy_Hydrochoerushydrochaeris(int arg0, float arg1);

    std::string willow_AtsSz_Greatersagegrouse(int arg0, float arg1);

    std::string pine_nSmoCH_Axisaxis(int arg0, float arg1);
};

#endif // END OF MODIFYCODETOOL_Dendrohyraxbrucel_HPP
