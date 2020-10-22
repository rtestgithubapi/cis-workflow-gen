#ifndef CIS_WORKFLOW_GEN_ACTIVITY_HPP
#define CIS_WORKFLOW_GEN_ACTIVITY_HPP

#include <iterator>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <list>
#include <queue>
#include <algorithm>

#include "utility.hpp"
#include "xaml-template.hpp"
#include <rlib/string.hpp>
#include <rlib/require/cxx14>
#include <rlib/scope_guard.hpp>

namespace CIS {
    using std::string;
    using namespace rlib::literals;
    class Flow;
    class Metadata;

    class ActivityBase {
    private:
        friend Flow;
        virtual rlib::string generateXaml() const = 0;
    public:
        Flow operator>>(const Flow &seqNext) const;
        Flow operator|(const Flow &seqNext) const;
    };

    class Activity : public ActivityBase {
    public:
        // All `Name` should not contain QuotationMark(")
        Activity(string displayName, string className, string entityName = "")
            : displayName(Utility::HtmlEscapeString(displayName)), className(className), entityName(entityName), taskId(Utility::GenUUID()) {}
        
        void addInputSetting(string k, string v) {
            inputSettings[k] = v;
        }
        void explicitSetRawArgument(string argTypeInXaml, string argValueInCSharp) {
            throw std::runtime_error("NotSupported! Activity seems doesn't support custom type inputSettings at all. Refer to commit 7fd539d6d5f6b102337da9591217b781cb71ced9 if we get new info and want to support it again. ");
        }
    private:
        string displayName, className, entityName;
        string taskId;
        std::unordered_map<string, string> inputSettings;

        auto inputSettingsToCodelines() const {
            // Convert InputSettings Dictionary to C# code. 
            std::list<string> inputSettingStrings;
            std::transform(this->inputSettings.begin(), this->inputSettings.end(), std::back_inserter(inputSettingStrings), [](auto &&kv) {
                return "            {\"{}\", \"{}\"}"_rs.format(kv.first, kv.second);
            });
            auto inputSettingsString = ",\n"_rs.join(inputSettingStrings);
            return rlib::string(templates::ACTIVITY_DICT_TEMPLATE_UNESCAPED).replace_once("__TEMPLATE_ARG_DictLines", inputSettingsString);
        }
        virtual rlib::string generateXaml() const override {
            rlib::string xamlCode;

            if(inputSettings.empty()) {
                // Also no inputSettings. 
                xamlCode = templates::ACTIVITY_XAML_TEMPLATE_WITHOUT_INPUTSETTINGS;
            }
            else {
                // Generate inputSettings.
                xamlCode = templates::ACTIVITY_XAML_TEMPLATE;
                xamlCode.replace("__TEMPLATE_ARG_TypeName", templates::ACTIVITY_DICT_TYPENAME);
                xamlCode.replace_once("__TEMPLATE_ARG_TypeValue", Utility::HtmlEscapeString(inputSettingsToCodelines()));
            }

            xamlCode.replace_once("__TEMPLATE_ARG_ClassName", this->className);
            xamlCode.replace_once("__TEMPLATE_ARG_DisplayName", this->displayName);
            xamlCode.replace_once("__TEMPLATE_ARG_TaskId", this->taskId);

            auto entityXaml = this->entityName == "" ? "" : rlib::string(templates::ENTITY_DEF_TEMPLATE).replace("__TEMPLATE_ARG_EntityName", this->entityName);
            xamlCode.replace_once("__TEMPLATE_ARG_EntityDefPlaceholder", entityXaml);

            return xamlCode;
        }
    };

    class ManualOperation : public ActivityBase {
    public:
        explicit ManualOperation(string displayName, string message = "", string entityName = "")
            : displayName(Utility::HtmlEscapeString(displayName)), messageInCSharp(Utility::HtmlEscapeString("\"" + message + "\"")), entityName(entityName) {}
        
        ManualOperation &explicitSetMessageInCSharp(string messageInCSharp) {
            this->messageInCSharp = messageInCSharp;
            return *this;
        }

    private:
        string displayName, messageInCSharp, entityName;
        virtual rlib::string generateXaml() const override {
            rlib::string xamlCode = templates::MANUAL_OPERATION_XAML;
            xamlCode.replace_once("__TEMPLATE_ARG_DisplayName", displayName);
            xamlCode.replace_once("__TEMPLATE_ARG_CodeLines", messageInCSharp);

            auto entityXaml = this->entityName == "" ? "" : rlib::string(templates::ENTITY_DEF_TEMPLATE).replace("__TEMPLATE_ARG_EntityName", this->entityName);
            xamlCode.replace_once("__TEMPLATE_ARG_EntityDefPlaceholder", entityXaml);
            return xamlCode;
        }

    };

    class Flow {
    public:
        Flow(const ActivityBase &activity) {
            xamlCode = activity.generateXaml();
        }
        explicit Flow(rlib::string xamlCode) : xamlCode(xamlCode) {}
        Flow(const Flow &another) : queuedOnRight(another.queuedOnRight), xamlCode(another.xamlCode), prevOperationIsSequential(another.prevOperationIsSequential) {}

        // Actually modify xamlCode on "OperationChange". 
        // for example, A >> B >> C >> D | E. ABCD should be merged into one sequential operation. 
        Flow operator>>(const Flow &seqNext) const {
            return binaryOperation(seqNext, true);
        }

        Flow operator|(const Flow &seqNext) const {
            return binaryOperation(seqNext, false);
        }

        auto generateXaml(Metadata) const;
        auto generateXaml(std::string className) const;

    private:
        bool prevOperationIsSequential = true;
        rlib::string xamlCode;

        // For a long expression A >> B >> C >> D, A is in xamlCode, and [B,C,D] are cached in queuedOnRight to wait for merge. 
        std::list<rlib::string> queuedOnRight;

        Flow binaryOperation(Flow seqNext, bool thisOperationIsSequential) const {
            Flow result = *this;
            result.reduceQueuedIfNecessary(thisOperationIsSequential);
            seqNext.reduceQueuedIfNecessary(thisOperationIsSequential);

            result.prevOperationIsSequential = thisOperationIsSequential;

            result.queuedOnRight.emplace_back(seqNext.xamlCode);
            for(auto &&item : seqNext.queuedOnRight)
                result.queuedOnRight.emplace_back(item);
            return result;
        }

        void reduceQueuedIfNecessary(bool thisOperationIsSequential) {
            if(thisOperationIsSequential == prevOperationIsSequential || queuedOnRight.empty()) return;
            rlib::string resultXaml = prevOperationIsSequential ? templates::SEQ_BEGIN : templates::PAR_BEGIN;

            resultXaml += xamlCode;
            for(auto &&item : queuedOnRight)
                resultXaml += item;

            resultXaml += prevOperationIsSequential ? templates::SEQ_END : templates::PAR_END;

            xamlCode = std::move(resultXaml);
            queuedOnRight.clear();
        }
    };

    inline Flow ActivityBase::operator>>(const Flow &seqNext) const {
        return Flow(*this) >> seqNext;
    }
    inline Flow ActivityBase::operator|(const Flow &seqNext) const {
        return Flow(*this) | seqNext;
    }

    struct Metadata {
        friend Flow;
        auto &setXtraShorthands(std::list<string> &&xtraShorthands) {this->xtraShorthands = std::move(xtraShorthands); return *this;}
        auto &setXtraNamespaces(std::list<string> &&xtraNamespaces) {this->xtraNamespaces = std::move(xtraNamespaces); return *this;}
        auto &setXtraAssemblies(std::list<string> &&xtraAssemblies) {this->xtraAssemblies = std::move(xtraAssemblies); return *this;}
        std::list<string> xtraShorthands;
        std::list<string> xtraNamespaces;
        std::list<string> xtraAssemblies;
        string className;
        explicit Metadata(string className) : className(className) {}
        Metadata() = delete;

    private:
        static auto doTransform(rlib::string template_, const std::list<string> &originBuf) {
            std::list<string> resultBuf;
            std::transform(originBuf.cbegin(), originBuf.cend(), std::back_inserter(resultBuf), [&](auto &item) {return template_.format(item);});
            return resultBuf;
        }
        auto generateXamlHead() const {
            rlib::string result = templates::STD_XAML_HEAD;
            result.replace_once("__TEMPLATE_ARG_XtraShorthands", ""_rs.join(doTransform("  {}\n", xtraShorthands)));
            result.replace_once("__TEMPLATE_ARG_XtraNamespaces", ""_rs.join(doTransform("    <x:String>{}</x:String>\n", xtraNamespaces)));
            result.replace_once("__TEMPLATE_ARG_XtraAssemblies", ""_rs.join(doTransform("    <AssemblyReference>{}</AssemblyReference>\n", xtraAssemblies)));
            result.replace_once("__TEMPLATE_ARG_WorkflowClassName", className);
            return result;
        }
        constexpr auto generateXamlTail() const {
            return templates::STD_XAML_TAIL;
        }
    };

    inline auto Flow::generateXaml(Metadata metadata) const {
        Flow finalized(*this);
        finalized.reduceQueuedIfNecessary(!finalized.prevOperationIsSequential); // Always necessary if queue is not empty.
        return metadata.generateXamlHead() + finalized.xamlCode + metadata.generateXamlTail();
    }
    inline auto Flow::generateXaml(std::string className) const {
        Metadata defaultMetadata(className);
        return generateXaml(std::move(defaultMetadata));
    }
}


#endif
