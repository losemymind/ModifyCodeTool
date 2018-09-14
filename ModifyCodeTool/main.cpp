#include <vector>
#include <string>
#include <atomic>
#include <thread>
#include <mutex>
#include <fstream>
#include <unordered_map>
#include <iterator>
#include "SimpleIni.h"
#include "FoundationKit/Base/string_builder.hpp"
#include "FoundationKit/Base/scope_guard.hpp"
#include "FoundationKit/Foundation/Path.hpp"
#include "FoundationKit/Foundation/File.hpp"
#include "FoundationKit/Foundation/Directory.hpp"
#include "FoundationKit/Foundation/StringUtils.hpp"
#include "FoundationKit/Foundation/DeltaTimer.hpp"
#include "FoundationKit/Foundation/TimeSpan.hpp"
#include "FoundationKit/Math/Math.hpp"

USING_NS_FK;

class ProgressBar
{
public:
    ProgressBar(size_t InTotalNum)
        : TotalNum(InTotalNum)
        , CurrentNum(0)
    {

    }
    ~ProgressBar()
    {

    }

    void Progress(size_t InNum)
    {
        CurrentNum += InNum;
        Report();
    }

    void Progress()
    {
        ++CurrentNum;
        Report();
    }

private:
    void Report()
    {
        double RunTimeValue = RunTime.Secondsf();
        TimeSpan RunSpan(RunTimeValue* Time::TicksPerSecond);
        double LeftTime = RunTimeValue / CurrentNum * TotalNum - RunTimeValue;
        TimeSpan LeftSpan(LeftTime* Time::TicksPerSecond);
        printf(">>> %.2lf%%, Run time :%d:%d:%d, Left time %d:%d:%d \r", CurrentNum * 100.0 / TotalNum, RunSpan.GetHours(), RunSpan.GetMinutes(), RunSpan.GetSeconds(), LeftSpan.GetHours(), LeftSpan.GetMinutes(), LeftSpan.GetSeconds());
        if (CurrentNum == TotalNum)
        {
            printf("\r\n");
        }
    }
    std::atomic_long TotalNum;
    std::atomic_long CurrentNum;
    DeltaTimer       RunTime;
};

class ScopeRunTime
{
public:
    ScopeRunTime(const std::string& InTag)
        :TAG(InTag)
    {

    }
    ~ScopeRunTime()
    {
        TimeSpan ts(dt.Secondsf()* Time::TicksPerSecond);
        FKLog(">>> %s run time:%d:%d:%d", TAG.c_str(), ts.GetHours(), ts.GetMinutes(), ts.GetSeconds());
    }
protected:
    DeltaTimer dt;
    std::string TAG;
};


struct ConfigDefinition
{
    std::vector<std::string> PrefixList;
    std::vector<std::string> SuffixList;
    std::string ProjectDir;               // 项目代码根目录
    int ReplaceFunctionCharacter;         // 方法名超过多少个字符才替换
    std::string WorkPath;
    std::string ResourcesPath;
};

bool LoadConfigDefinition(ConfigDefinition& Definition)
{
    ScopeRunTime SRT("Load and parse config");
    std::string ResourcesPath = Path::GetApplicationPath();
    Definition.WorkPath = Path::GetPath(ResourcesPath);
    Definition.ResourcesPath = Definition.WorkPath + "Resources\\";
    std::string ConfigFilePath = Definition.ResourcesPath + "ModifyCodeTool.ini";
    CSimpleIniA ModifyCodeToolIni(true, true);
    if (ModifyCodeToolIni.LoadFile(ConfigFilePath.c_str()) != SI_OK)
    {
        FKLog("Load ModifyCodeTool.ini file failed:%s", ConfigFilePath.c_str());
        return false;
    }
    Definition.ReplaceFunctionCharacter = ModifyCodeToolIni.GetLongValue("Main", "ReplaceFunctionCharacter");
    Definition.ProjectDir = ModifyCodeToolIni.GetValue("Main", "CodePath");
    std::string PrefixListPath = ModifyCodeToolIni.GetValue("Main", "PrefixFile");
    PrefixListPath = Definition.ResourcesPath + PrefixListPath;
    Definition.PrefixList = File::ReadAllLines(PrefixListPath);
    if (Definition.PrefixList.empty())
    {
        FKLog("Read file:%s failed.", PrefixListPath.c_str());
        return false;
    }
    else
    {
        std::sort(Definition.PrefixList.begin(), Definition.PrefixList.end());
        std::vector<std::string>::iterator iter = std::unique(Definition.PrefixList.begin(), Definition.PrefixList.end());
        Definition.PrefixList.erase(iter, Definition.PrefixList.end());
    }
    std::string SuffixListPath = ModifyCodeToolIni.GetValue("Main", "SuffixFile");
    SuffixListPath = Definition.ResourcesPath + SuffixListPath;
    Definition.SuffixList = File::ReadAllLines(SuffixListPath);
    if (Definition.SuffixList.empty())
    {
        FKLog("Read file:%s failed.", SuffixListPath.c_str());
        return false;
    }
    else
    {
        std::sort(Definition.SuffixList.begin(), Definition.SuffixList.end());
        std::vector<std::string>::iterator iter = std::unique(Definition.SuffixList.begin(), Definition.SuffixList.end());
        Definition.SuffixList.erase(iter, Definition.SuffixList.end());
    }
    return true;
}

bool InsertRubbishCodeToClass(const ConfigDefinition& Definition, const std::string& ClassName, std::string& ClassHeaderContent, std::string& ClassSourceContent)
{

    size_t ClassEndPos = ClassHeaderContent.find("};");
    if (ClassEndPos == std::string::npos)
    {
        FKLog("The definition of %s is missing };", ClassName.c_str());
    }
   
    std::string FunctionsDeclaration = "\nprotected:";
    int InsertFuncNum = Math::Random(3, 10); // 每个类中随机插入3~10个方法
    for (int i = 0; i < InsertFuncNum; ++i)
    {
        FunctionsDeclaration += "\n    std::string ";
        //生成函数名
        //生成的方法的前缀和后缀颠倒是为了避免和要生成插入到游戏代码中的方法名可能存在的冲突。
        std::string FuncName = Definition.SuffixList[Math::Random(0u, Definition.SuffixList.size() - 1)];
        FuncName.push_back('_');
        int FuncCharNum = Math::Random(5, 8);
        for (int j = 0; j < FuncCharNum; j++)
        {
            bool bUpper = Math::RandBoolean();
            if (bUpper)
            {
                FuncName.push_back(FoundationKit::Math::Random((int)'A', (int)'Z'));
            }
            else
            {
                FuncName.push_back(FoundationKit::Math::Random((int)'a', (int)'z'));
            }
        }
        FuncName.push_back('_');
        FuncName += Definition.PrefixList[Math::Random(0u, Definition.PrefixList.size() - 1)];
        FunctionsDeclaration += FuncName;
        FunctionsDeclaration += "(int arg0, float arg1);\n";

        //生成函数定义和垃圾代码
        string_builder<char> FunctionsDefinition;
        FunctionsDefinition.append("\nstd::string ");
        FunctionsDefinition.append(ClassName);
        FunctionsDefinition.append("::");
        FunctionsDefinition.append(FuncName);
        FunctionsDefinition.append("(int arg0, float arg1)\n{\n    Set");
        FunctionsDefinition.append(ClassName);
        FunctionsDefinition.append("Name(\"");
        FunctionsDefinition.append(FuncName);
        FunctionsDefinition.append("\");\n    return Get");
        FunctionsDefinition.append(ClassName);
        FunctionsDefinition.append("Name();\n}");
        ClassSourceContent += FunctionsDefinition.to_string();
    }
    ClassHeaderContent.insert(ClassEndPos, FunctionsDeclaration);
    return true;
}

bool GenerateRubbishClass(const ConfigDefinition& Definition)
{
    FKLog(">>> Generate rubbish class...");
    ScopeRunTime SRT("Generate rubbish class");
    auto  aaa = File::ReadAllBytes(Definition.ResourcesPath + "class_header.template");
    std::string ClassHeaderTemplateContent = File::ReadAllText(Definition.ResourcesPath + "class_header.template");
    std::string ClassSourceTempLateContent = File::ReadAllText(Definition.ResourcesPath + "class_source.template");
    std::string ClassesDir = Definition.WorkPath + "Classes\\";
    if (!Directory::Create(ClassesDir))
    {
        FKLog("Create %s directory failed.", ClassesDir.c_str());
    }
    ProgressBar pb(Definition.PrefixList.size() + Definition.SuffixList.size());


    std::mutex InsertRubbishClassMutex;
    auto WriteClassFun = [&](const std::string& ClassName) 
    {
        std::string ClassHeaderContent = ClassHeaderTemplateContent;
        StringUtils::ReplaceAll(ClassHeaderContent, "@ClassName@", ClassName);
        std::string ClassSourceContent = ClassSourceTempLateContent;
        StringUtils::ReplaceAll(ClassSourceContent, "@ClassName@", ClassName);
        InsertRubbishCodeToClass(Definition, ClassName, ClassHeaderContent, ClassSourceContent);
        File::WriteAllText(ClassesDir + ClassName + ".hpp", ClassHeaderContent);
        File::WriteAllText(ClassesDir + ClassName + ".cpp", ClassSourceContent);
        std::lock_guard<std::mutex> lock(InsertRubbishClassMutex);
        pb.Progress();
    };

    std::vector<std::thread> WorkThreadList;
    std::atomic_long PrefixListMaxNum = Definition.PrefixList.size();
    std::atomic_long PrefixListCurNum = 0;
    std::atomic_long SuffixListMaxNum = Definition.SuffixList.size();
    std::atomic_long SuffixListCurNum = 0;
    size_t ThreadNumMax = std::thread::hardware_concurrency();
    for (size_t i = 0; i < ThreadNumMax; ++i)
    {
        WorkThreadList.push_back(std::thread([&]() 
        {
            while (PrefixListCurNum < PrefixListMaxNum)
            {
                WriteClassFun(Definition.PrefixList[PrefixListCurNum++]);
            }

            while (SuffixListCurNum < SuffixListMaxNum)
            {
                WriteClassFun(Definition.SuffixList[SuffixListCurNum++]);
            }
        }));
    }

    for (auto& stdthread: WorkThreadList)
    {
        if (stdthread.joinable())
        {
            stdthread.join();
        }
    }
    return true;
}

void FindProjectCodeFiles(const ConfigDefinition& Definition, std::vector<std::string>& ProjectHeaderFiles, std::vector<std::string>& ProjectSourceFiles)
{
    ScopeRunTime SRT("Find project code files");
    Directory::GetFiles(Definition.ProjectDir, [&](const std::string& FileFullPath) 
    {
        std::string FileExtension = Path::GetExtension(FileFullPath);
        if (FileExtension == ".h" || FileExtension == ".hpp")
        {
            ProjectHeaderFiles.push_back(FileFullPath);
        }
        else if (FileExtension == ".cpp")
        {
            ProjectSourceFiles.push_back(FileFullPath);
        }
        return false;
    }, Directory::ESearchOption::AllDirectories);
}

void GenerateFunctionReplaceMap(const ConfigDefinition& Definition, std::vector<std::string>& ProjectSourceFiles, std::unordered_map<std::string, std::string>& FunctionReplaceMap)
{
    ProgressBar pb(ProjectSourceFiles.size());
    ScopeRunTime SRT("Generate function replace map");
    for (auto& FilePath : ProjectSourceFiles)
    {
        pb.Progress();
        FILE* SourceFile = fopen(FilePath.c_str(), "rb");
        if(!SourceFile) break;
        char szGetString[1024] = { 0 };
        std::string Line;
        for (; fgets(szGetString, 1024, SourceFile) != NULL && szGetString[0] != '\0'; )
        {
            if ( (szGetString[0] < 'a' || szGetString[0] > 'z') && (szGetString[0] <'A' || szGetString[0] >'Z'))
            {
                continue;
            }

            Line = szGetString;
            size_t FuncStartPos = Line.find("::");
            if (FuncStartPos != std::string::npos)
            {
                if (Line.find("=")  != std::string::npos ||
                    Line.find(".")  != std::string::npos ||
                    Line.find("->") != std::string::npos ||
                    Line.find(";")  != std::string::npos ||
                    Line.find("~")  != std::string::npos)
                {
                    //丢弃不处理
                    continue;
                }


                size_t FuncEndPos = Line.find('(', FuncStartPos);

                if (FuncEndPos != std::string::npos)
                {
                    // 这里加的2 是为了去除掉"::"
                    FuncStartPos += 2;
                    std::string FuncName = Line.substr(FuncStartPos, FuncEndPos - FuncStartPos);
                    if (FuncName.find("::") != std::string::npos ||
                        FuncName.find("<")  != std::string::npos ||
                        FuncName.find("&")  != std::string::npos ||
                        FuncName.find(")")  != std::string::npos ||
                        FuncName.find(" ") != std::string::npos ||
                        FuncName.find("*") != std::string::npos ||
                        Line.find(FuncName) < FuncStartPos || // 去除构造函数
                        FuncName.size() < Definition.ReplaceFunctionCharacter)
                    {
                        //丢弃不处理
                        continue;
                    }

                    std::string TrimFuncName = StringUtils::Trim(FuncName);
                    if (FunctionReplaceMap.find(TrimFuncName) != FunctionReplaceMap.end())
                    {
                        continue;
                    }
                    std::string NewFuncName = Definition.PrefixList[Math::Random(0u, Definition.PrefixList.size() - 1)]; 
                    NewFuncName.push_back('_');
                    NewFuncName += TrimFuncName;
                    NewFuncName.push_back('_');
                    NewFuncName += Definition.SuffixList[Math::Random(0u, Definition.SuffixList.size() - 1)];
                    FunctionReplaceMap.insert(std::make_pair(TrimFuncName, NewFuncName));
                }
            }
        }
        fclose(SourceFile);
    }
}

void EraseWhiteListFunctions(const ConfigDefinition& Definition, std::unordered_map<std::string, std::string>& FunctionReplaceMap)
{
    ScopeRunTime SRT("Erase white list functions");
    std::vector<std::string> WhiteListFunctions = File::ReadAllLines(Definition.ResourcesPath + "WhiteList.ini");
    for (auto& func: WhiteListFunctions)
    {
        auto iter = FunctionReplaceMap.find(func);
        if (iter != FunctionReplaceMap.end())
        {
            FunctionReplaceMap.erase(iter);
        }
    }
}

void FunctionReplace(const ConfigDefinition& Definition,
    const std::unordered_map<std::string, std::string>& FunctionReplaceMap,
    const std::vector<std::string>& ProjectHeaderFiles,
    const std::vector<std::string>& ProjectSourceFiles)
{
    ScopeRunTime SRT("Function replace");
    std::vector<std::thread> WorkThreadList;
    size_t ThreadNumMax = std::thread::hardware_concurrency() - 2;
    std::vector<std::string> AllProjectFiles = ProjectSourceFiles;
    AllProjectFiles.insert(AllProjectFiles.end(), ProjectHeaderFiles.begin(), ProjectHeaderFiles.end());
    size_t EveryThreadWorks = Math::CeilToInt(AllProjectFiles.size()*1.0f / ThreadNumMax);
    ProgressBar pb(AllProjectFiles.size());
    for (int i = 0; i < ThreadNumMax; ++i)
    {
        size_t ThreadWorkStart = i*EveryThreadWorks;
        size_t ThreadWorkEnd = ThreadWorkStart + EveryThreadWorks;
        if (i ==(ThreadNumMax-1))
        {
            ThreadWorkEnd = AllProjectFiles.size();
        }
        WorkThreadList.push_back(std::thread([&](size_t StartIndex, size_t EndIndex)
        {
            for (size_t i = StartIndex; i < EndIndex; ++i)
            {
                std::string FileContent;
                do
                {
                    std::string FilePath = AllProjectFiles[i];
                    int64 FileSize = File::GetSize(FilePath);
                    BREAK_IF(FileSize <= 0);
                    FILE* FileHandle = nullptr;
                    FileHandle = File::Open(FilePath, "rb+");
                    BREAK_IF(!FileHandle);
                    FileContent.resize(static_cast<size_t>(FileSize));
                    size_t ReadCount = 0;
                    while (ReadCount < FileSize)
                    {
                        size_t NumRead = fread(((char*)FileContent.data() + ReadCount), sizeof(uint8), 1024, FileHandle);
                        if (NumRead == 0)
                        {
                            if (errno == EINTR)
                                continue;
                            if (feof(FileHandle) == 0) // read file error.
                            {
                                FKLog("Error while read %s: %s\n", FilePath.c_str(), strerror(errno));
                            }
                            break;
                        }
                        ReadCount += NumRead;
                    }

                    size_t ReplaceCount = 0;
                    for (auto iter : FunctionReplaceMap)
                    {
                        ReplaceCount += StringUtils::ReplaceAll(FileContent, iter.first, iter.second);
                    }

                    if (ReplaceCount > 0)
                    {
                        fseek(FileHandle, 0, SEEK_SET);
                        fwrite(FileContent.c_str(), 1, FileContent.size(), FileHandle);
                    }
                    fclose(FileHandle);
                } while (false);

                pb.Progress();
            }
        }, ThreadWorkStart, ThreadWorkEnd));
    }

    for (auto& stdthread : WorkThreadList)
    {
        if (stdthread.joinable())
        {
            stdthread.join();
        }
    }
}


void InsertRubbishCodeToGameClass(const ConfigDefinition& Definition,const std::vector<std::string>& ProjectHeaderFiles, const std::vector<std::string>& ProjectSourceFiles)
{
    ScopeRunTime SRT("Insert rubbish code to game");
    ProgressBar pb(ProjectHeaderFiles.size());
    for (auto& HeaderFilePath : ProjectHeaderFiles)
    {
        pb.Progress();
        std::string SourceFilePath = Path::ChangeExtension(HeaderFilePath, ".cpp");
        if (!File::IsExists(SourceFilePath))
        {
            continue;
        }

        auto GenerateRubbishCode = [](const ConfigDefinition& Definition,
                                      const std::string& ClassName,
                                      std::string& FunctionsDeclaration,
                                      std::string& FunctionsDefinition)
        {
            FunctionsDeclaration = "\n";
            string_builder<char> FunctionsDefinitionBuilder;
            int InsertFuncNum = Math::Random(5, 15); // 每个类中随机插入5~15个方法
            for (int i = 0; i < InsertFuncNum; ++i)
            {
                FunctionsDeclaration += "\n    int ";
                //生成函数名
                //生成的方法的前缀和后缀颠倒是为了避免和要生成插入到游戏代码中的方法名可能存在的冲突。
                std::string FuncName = Definition.SuffixList[Math::Random(0u, Definition.SuffixList.size() - 1)];
                FuncName.push_back('_');
                int FuncCharNum = Math::Random(5, 8);
                for (int j = 0; j < FuncCharNum; j++)
                {
                    bool bUpper = Math::RandBoolean();
                    if (bUpper)
                    {
                        FuncName.push_back(FoundationKit::Math::Random((int)'A', (int)'Z'));
                    }
                    else
                    {
                        FuncName.push_back(FoundationKit::Math::Random((int)'a', (int)'z'));
                    }
                }
                FuncName.push_back('_');
                FuncName += Definition.PrefixList[Math::Random(0u, Definition.PrefixList.size() - 1)];
                FunctionsDeclaration += FuncName;
                FunctionsDeclaration += "(int arg0, float arg1);\n";

                //生成函数定义和垃圾代码
                FunctionsDefinitionBuilder.append("\nint ");
                FunctionsDefinitionBuilder.append(ClassName);
                FunctionsDefinitionBuilder.append("::");
                FunctionsDefinitionBuilder.append(FuncName);
                FunctionsDefinitionBuilder.append("(int arg0, float arg1)\n{\n");
                FunctionsDefinitionBuilder.append("    long result = static_cast<int>(arg0 * arg1) % arg0;\n");
                FunctionsDefinitionBuilder.append("    result += strlen(\"");
                FunctionsDefinitionBuilder.append(FuncName);
                FunctionsDefinitionBuilder.append("\");\n");
                FunctionsDefinitionBuilder.append("    return result;\n");
                FunctionsDefinitionBuilder.append("}\n");
            }
            FunctionsDefinition = FunctionsDefinitionBuilder.to_string();
        };


        auto ParseClassName = [](const std::string& InClassContent) 
        {
            std::stringstream ss;
            ss << InClassContent;
            std::string Line;
            std::string ClassName;
            for (;std::getline(ss, Line);)
            {
                Line = StringUtils::LTrim(Line);
                size_t ClassNamePos = Line.find("class");
                // Found class
                if (ClassNamePos == 0)
                {
                    // 删除多余的空格
                    std::string Line2;
                    std::unique_copy(Line.begin(), Line.end(), std::back_inserter(Line2), [](char c1, char c2)
                    {
                        return c1 == ' ' && c2 == ' ';
                    });
                    // 这里的6 = strlen("class ");
                    ClassName = Line2.substr(6, Line2.find(' ', 6) - 6);
                    break;
                }
            }
            return ClassName;
        };

        std::string ClassContent;
        std::string ClassName;
        std::string FunctionsDeclaration;
        std::string FunctionsDefinition;
        do
        {
            int64 FileSize = File::GetSize(HeaderFilePath);
            BREAK_IF(FileSize <= 0);
            FILE* FileHandle = nullptr;
            FileHandle = File::Open(HeaderFilePath, "rb+");
            BREAK_IF(!FileHandle);
            SCOPE_EXIT{ fclose(FileHandle); };
            ClassContent.resize(static_cast<size_t>(FileSize));
            size_t ReadCount = 0;
            while (ReadCount < FileSize)
            {
                size_t NumRead = fread(((char*)ClassContent.data() + ReadCount), sizeof(uint8), 1024, FileHandle);
                if (NumRead == 0)
                {
                    if (errno == EINTR)
                        continue;
                    if (feof(FileHandle) == 0) // read file error.
                    {
                        FKLog("Error while read %s: %s\n", HeaderFilePath.c_str(), strerror(errno));
                    }
                    break;
                }
                ReadCount += NumRead;
            }
            ClassName = ParseClassName(ClassContent);
            GenerateRubbishCode(Definition, ClassName, FunctionsDeclaration, FunctionsDefinition);
            std::string ClassDestructorName = "~" + ClassName + "();";
            size_t DestructorPos = ClassContent.find(ClassDestructorName);
            if (DestructorPos == std::string::npos)
            {
                break;
            }
            ClassContent.insert(DestructorPos + ClassDestructorName.size(), FunctionsDeclaration);
            fseek(FileHandle, 0, SEEK_SET);
            fwrite(ClassContent.c_str(), 1, ClassContent.size(), FileHandle);
            
        } while (false);

        do
        {
            int64 FileSize = File::GetSize(SourceFilePath);
            BREAK_IF(FileSize <= 0);
            FILE* FileHandle = nullptr;
            FileHandle = File::Open(SourceFilePath, "rb+");
            BREAK_IF(!FileHandle);
            SCOPE_EXIT{ fclose(FileHandle); };
            ClassContent.clear();
            ClassContent.resize(static_cast<size_t>(FileSize));
            size_t ReadCount = 0;
            while (ReadCount < FileSize)
            {
                size_t NumRead = fread(((char*)ClassContent.data() + ReadCount), sizeof(uint8), 1024, FileHandle);
                if (NumRead == 0)
                {
                    if (errno == EINTR)
                        continue;
                    if (feof(FileHandle) == 0) // read file error.
                    {
                        FKLog("Error while read %s: %s\n", SourceFilePath.c_str(), strerror(errno));
                    }
                    break;
                }
                ReadCount += NumRead;
            }

            std::string ClassDestructorName = ClassName+ "::~" + ClassName + "()";
            size_t DestructorPos = ClassContent.find(ClassDestructorName);
            if (DestructorPos == std::string::npos)
            {
                break;
            }
            ClassContent.insert(DestructorPos, FunctionsDefinition);
            fseek(FileHandle, 0, SEEK_SET);
            fwrite(ClassContent.c_str(), 1, ClassContent.size(), FileHandle);
        } while (false);
    }
}


int wmain()
{
    {
        ScopeRunTime AllRunTime("Total");
        ConfigDefinition Definition;
        if (!LoadConfigDefinition(Definition))
            return 0;
        GenerateRubbishClass(Definition);
        std::vector<std::string> ProjectHeaderFiles;
        std::vector<std::string> ProjectSourceFiles;
        FindProjectCodeFiles(Definition, ProjectHeaderFiles, ProjectSourceFiles);
        std::unordered_map<std::string, std::string> FunctionReplaceMap;
        GenerateFunctionReplaceMap(Definition, ProjectSourceFiles, FunctionReplaceMap);
        EraseWhiteListFunctions(Definition, FunctionReplaceMap);
        FunctionReplace(Definition, FunctionReplaceMap, ProjectHeaderFiles, ProjectSourceFiles);
        InsertRubbishCodeToGameClass(Definition, ProjectHeaderFiles, ProjectSourceFiles);
    }
    system("pause");
    return 0;
}
