#ifndef XAML_TEMPLATE_HPP
#define XAML_TEMPLATE_HPP

#include <string>
namespace CIS::impl {

constexpr auto ACTIVITY_XAML_TEMPLATE = R"XAMLTL(
    <mwcwa:ControlledActivity ClassName="__TEMPLATE_ARG_ClassName" DisplayName="__TEMPLATE_ARG_DisplayName" TaskId="__TEMPLATE_ARG_TaskId" __TEMPLATE_ARG_EntityDefPlaceholder>
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
)XAMLTL"

constexpr auto ENTITY_DEF_TEMPLATE = R"XAMLTL(coordination:DependencyBinder.EntityName="__TEMPLATE_ARG_EntityName")XAMLTL";

}

#endif

