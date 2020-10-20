#ifndef CIS_WORKFLOW_GEN_ACTIVITY_HPP
#define CIS_WORKFLOW_GEN_ACTIVITY_HPP

#include <iterator>
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
    private:
        string displayName, className, entityName;
        string taskId;
        std::unordered_map<string, string> inputSettings;
    };

    class Flow {
    public:
        Flow(const Activity &activity) {
            xamlCode = templates::ACTIVITY_XAML_TEMPLATE;
            xamlCode.replace("__TEMPLATE_ARG_ClassName", activity.className);
            xamlCode.replace("__TEMPLATE_ARG_DisplayName", activity.displayName);
            xamlCode.replace("__TEMPLATE_ARG_TaskId", activity.taskId);
            auto entityXaml = activity.entityName == "" ? "" : rlib::string(templates::ENTITY_DEF_TEMPLATE).replace("__TEMPLATE_ARG_EntityName", activity.entityName);
            xamlCode.replace("__TEMPLATE_ARG_EntityDefPlaceholder", entityXaml);

            std::list<string> inputSettingStrings;
            std::transform(activity.inputSettings.begin(), activity.inputSettings.end(), std::back_inserter(inputSettingStrings), [](auto &&kv) {
                return "{\"{}\", \"{}\"}"_rs.format(kv.first, kv.second);
            });
            auto inputSettingsString = ",\n            "_rs.join(inputSettingStrings);
            xamlCode.replace("__TEMPLATE_ARG_DictLines", inputSettingsString);
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

        rlib::string generateXaml() const {
            Flow finalized(*this);
            if(!finalized.queued.empty())
                finalized.reduceQueued();
            return finalized.xamlCode;
        }

    private:
        bool prevOperationIsSequential = false;
        rlib::string xamlCode;
        std::queue<rlib::string> queued;

        Flow binaryOperation(const Flow &seqNext, bool thisOperationIsSequential) const {
            Flow result = *this;
            if(thisOperationIsSequential != prevOperationIsSequential && !queued.empty()) {
                result.reduceQueued();
            }
            result.prevOperationIsSequential = thisOperationIsSequential;
            result.queued.push(seqNext.xamlCode);
            return result;
        }

        void reduceQueued() {
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
        return Flow(*this) >> seqNext;
    }

    struct Workflow {
        std::list<string> assemblyReferences;

    };
}


#endif
