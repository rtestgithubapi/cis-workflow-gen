#ifndef XAML_TEMPLATE_HPP
#define XAML_TEMPLATE_HPP

#include <string>
#include <rlib/require/cxx14>

namespace CIS {
namespace templates {

// Rule: Every end-of-line should contains a \n

constexpr auto ACTIVITY_XAML_TEMPLATE = 
R"XAMLTL(    <mwcwa:ControlledActivity ClassName="__TEMPLATE_ARG_ClassName" DisplayName="__TEMPLATE_ARG_DisplayName" TaskId="__TEMPLATE_ARG_TaskId" __TEMPLATE_ARG_EntityDefPlaceholder>
      <mwcwa:ControlledActivity.InputSettings>
        <InArgument x:TypeArguments="scg:Dictionary(x:String, x:String)">
          <mca:CSharpValue x:TypeArguments="scg:Dictionary(x:String, x:String)" xml:space="preserve">
          new Dictionary&lt;string, string&gt;()
          {
            __TEMPLATE_ARG_DictLines
          }
          </mca:CSharpValue>
        </InArgument>
      </mwcwa:ControlledActivity.InputSettings>
    </mwcwa:ControlledActivity>
)XAMLTL";

constexpr auto ENTITY_DEF_TEMPLATE = R"(coordination:DependencyBinder.EntityName="__TEMPLATE_ARG_EntityName")";

constexpr auto SEQ_BEGIN = R"(  <mwcwa:ControlledSequence IsHiddenFromOperator="False">)" "\n";
constexpr auto SEQ_END   = R"(  </mwcwa:ControlledSequence>)" "\n";
constexpr auto PAR_BEGIN = R"(  <mwcwa:ControlledParallel IsHiddenFromOperator="False">)" "\n";
constexpr auto PAR_END   = R"(  </mwcwa:ControlledParallel>)" "\n";


}
}


#endif

