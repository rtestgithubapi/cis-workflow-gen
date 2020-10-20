#ifndef CIS_WORKFLOW_GEN_ACTIVITY_HPP
#define CIS_WORKFLOW_GEN_ACTIVITY_HPP

#include <iterator>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <list>
#include <queue>
#include <algorithm>

#include "uuid.hpp"
#include "xaml-template.hpp"
#include <rlib/string.hpp>
#include <rlib/require/cxx14>
#include <rlib/scope_guard.hpp>

namespace CIS {
    using std::string;
    using namespace rlib::literals;
    class Flow;
    class Metadata;

    class Activity {
    public:
        friend Flow;
        Activity(string displayName, string className, string entityName = "")
            : displayName(displayName), className(className), entityName(entityName), taskId(GenUUID()) {}
        
        Flow operator>>(const Flow &seqNext) const;
        Flow operator|(const Flow &seqNext) const;
        void addInputSetting(string k, string v) {
            inputSettings[k] = v;
        }
        void addRawActivityArgument(string xamlTypeString, string csharpValueCode) {
            throw std::invalid_argument("Not implemented yet.");
        }
    private:
        string displayName, className, entityName;
        string taskId;
        std::unordered_map<string, string> inputSettings;
    };

    class Flow {
    public:
        Flow(const Activity &activity) {
            xamlCode = templates::ACTIVITY_XAML_TEMPLATE;
            xamlCode.replace_once("__TEMPLATE_ARG_ClassName", activity.className);
            xamlCode.replace_once("__TEMPLATE_ARG_DisplayName", activity.displayName);
            xamlCode.replace_once("__TEMPLATE_ARG_TaskId", activity.taskId);
            auto entityXaml = activity.entityName == "" ? "" : rlib::string(templates::ENTITY_DEF_TEMPLATE).replace("__TEMPLATE_ARG_EntityName", activity.entityName);
            xamlCode.replace_once("__TEMPLATE_ARG_EntityDefPlaceholder", entityXaml);

            std::list<string> inputSettingStrings;
            std::transform(activity.inputSettings.begin(), activity.inputSettings.end(), std::back_inserter(inputSettingStrings), [](auto &&kv) {
                return "{\"{}\", \"{}\"}"_rs.format(kv.first, kv.second);
            });
            auto inputSettingsString = ",\n            "_rs.join(inputSettingStrings);
            xamlCode.replace_once("__TEMPLATE_ARG_DictLines", inputSettingsString);
        }
        Flow(rlib::string xamlCode) : xamlCode(xamlCode) {}
        Flow(const Flow &another) : queued(another.queued), xamlCode(another.xamlCode), prevOperationIsSequential(another.prevOperationIsSequential) {}

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
        std::queue<rlib::string> queued;

        Flow binaryOperation(Flow seqNext, bool thisOperationIsSequential) const {
            Flow result = *this;
            result.reduceQueuedIfNecessary(thisOperationIsSequential);
            seqNext.reduceQueuedIfNecessary(thisOperationIsSequential);

            result.prevOperationIsSequential = thisOperationIsSequential;
            result.queued.push(seqNext.xamlCode);
            return result;
        }

        void reduceQueuedIfNecessary(bool thisOperationIsSequential) {
            if(thisOperationIsSequential == prevOperationIsSequential || queued.empty()) return;
            rlib::string resultXaml = prevOperationIsSequential ? templates::SEQ_BEGIN : templates::PAR_BEGIN;
            resultXaml += xamlCode;
            while(!queued.empty())
                resultXaml += queued.front(), queued.pop();
            resultXaml += prevOperationIsSequential ? templates::SEQ_END : templates::PAR_END;
            xamlCode = std::move(resultXaml);
        }
    };

    inline Flow Activity::operator>>(const Flow &seqNext) const {
        return Flow(*this) >> seqNext;
    }
    inline Flow Activity::operator|(const Flow &seqNext) const {
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
        Metadata(string className) : className(className) {}
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
