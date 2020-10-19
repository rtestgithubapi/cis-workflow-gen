#ifndef CIS_WORKFLOW_GEN_ACTIVITY_HPP
#define CIS_WORKFLOW_GEN_ACTIVITY_HPP

#include <string>
#include <map>
#include "uuid.hpp"
#include "xaml-template.hpp"
#include <rlib/string.hpp>

namespace CIS {
    using std::string;
    class Flow;

    class Activity {
    public:
        friend Flow;
        Activity(string displayName, string className, string entityName = "")
            : displayName(displayName), className(className), entityName(entityName), taskId(GenUUID()) {}
        
        Flow &operator>>(const Flow &seqNext) {
            return Flow(*this) >> seqNext;
        }
        

    private:
        string displayName, className, entityName;
        string taskId;
        std::map<string, string>
    };

    class Flow {
    public:
        Flow(const Activity &activity) {
            xamlCode = ACTIVITY_XAML_TEMPLATE;
            xamlCode.replace("__TEMPLATE_ARG_ClassName", activity.className);
            xamlCode.replace("__TEMPLATE_ARG_DisplayName", activity.displayName);
            auto entityXaml = activity.entityName == "" ? "" : rlib::string(ENTITY_DEF_TEMPLATE).replace("__TEMPLATE_ARG_EntityName", activity.entityName);
            xamlCode.replace("__TEMPLATE_ARG_EntityDefPlaceholder", entityXaml);

        }

        Flow &operator>>(const Flow &seqNext) {

        }

        rlib::string xamlCode;
    };

    struct Workflow {
        std::list<string> assemblyReferences;

    };
}


#endif
