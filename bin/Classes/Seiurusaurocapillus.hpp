
#ifndef MODIFYCODETOOL_Seiurusaurocapillus_HPP
#define MODIFYCODETOOL_Seiurusaurocapillus_HPP
#pragma once
#include <string>
class Seiurusaurocapillus
{
public:
    Seiurusaurocapillus();
    virtual ~Seiurusaurocapillus();
protected:
    void SetSeiurusaurocapillusName(const std::string& InName);
    std::string GetSeiurusaurocapillusName();
private:
    std::string MyName;

protected:
    std::string resin_LoaMHpI_Eremophilaalpestris(int arg0, float arg1);

    std::string rose_OMKvNTR_Bent_toedgecko(int arg0, float arg1);

    std::string rosewood_hhAkqd_Moreliaspilotesvariegata(int arg0, float arg1);
};

#endif // END OF MODIFYCODETOOL_Seiurusaurocapillus_HPP
