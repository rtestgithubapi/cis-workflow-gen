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

constexpr auto STD_XAML_HEAD = R"XAML(<?xml version='1.0' encoding='utf-8' standalone='yes'?>
<Activity
  xmlns="http://schemas.microsoft.com/netfx/2009/xaml/activities"
  xmlns:coordination="clr-namespace:Microsoft.WindowsAzure.Dependencies.Coordination;assembly=Microsoft.WindowsAzure.Dependencies.Coordination"
  xmlns:mc="http://schemas.openxmlformats.org/markup-compatibility/2006"
  xmlns:mca="clr-namespace:Microsoft.CSharp.Activities;assembly=System.Activities"
  xmlns:mwcwa="clr-namespace:Microsoft.WindowsAzure.Cis.Workflow.Activities;assembly=Microsoft.WindowsAzure.Cis.Workflow.Activities"
  xmlns:mwcwcs="clr-namespace:Microsoft.WindowsAzure.Cis.Workflow.Contracts.Settings;assembly=Microsoft.WindowsAzure.Cis.Workflow.Contracts"
  xmlns:sads="http://schemas.microsoft.com/netfx/2010/xaml/activities/debugger"
  xmlns:sap="http://schemas.microsoft.com/netfx/2009/xaml/activities/presentation"
  xmlns:sap2010="http://schemas.microsoft.com/netfx/2010/xaml/activities/presentation"
  xmlns:scg="clr-namespace:System.Collections.Generic;assembly=mscorlib"
  xmlns:sco="clr-namespace:System.Collections.ObjectModel;assembly=mscorlib"
  xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
  sap2010:ExpressionActivityEditor.ExpressionActivityEditor="C#"
  x:Class="__TEMPLATE_ARG_WorkflowClassName" 
__TEMPLATE_ARG_XtraShorthands  mc:Ignorable="sap sap2010 sads">
<TextExpression.NamespacesForImplementation>
  <sco:Collection x:TypeArguments="x:String">
    <x:String>System</x:String>
    <x:String>System.Collections.Generic</x:String>
    <x:String>System.Data</x:String>
    <x:String>System.Linq</x:String>
    <x:String>System.Text</x:String>
__TEMPLATE_ARG_XtraNamespaces  </sco:Collection>
</TextExpression.NamespacesForImplementation>
<TextExpression.ReferencesForImplementation>
  <sco:Collection x:TypeArguments="AssemblyReference">
    <AssemblyReference>Microsoft.CSharp</AssemblyReference>
    <AssemblyReference>Microsoft.WindowsAzure.Bootstrap.Common</AssemblyReference>
    <AssemblyReference>Microsoft.WindowsAzure.Bootstrap.ComponentBootstrap.Activities</AssemblyReference>
    <AssemblyReference>Microsoft.WindowsAzure.Bootstrap.ComponentBootstrap.Contracts</AssemblyReference>
    <AssemblyReference>Microsoft.WindowsAzure.Cis.Workflow.Activities</AssemblyReference>
    <AssemblyReference>Microsoft.WindowsAzure.Cis.Workflow.Activities.Design</AssemblyReference>
    <AssemblyReference>Microsoft.WindowsAzure.Cis.Workflow.Common</AssemblyReference>
    <AssemblyReference>Microsoft.WindowsAzure.Cis.Workflow.Contracts</AssemblyReference>
    <AssemblyReference>System</AssemblyReference>
    <AssemblyReference>System.Activities</AssemblyReference>
    <AssemblyReference>System.Core</AssemblyReference>
    <AssemblyReference>System.Data</AssemblyReference>
    <AssemblyReference>System.Runtime.Serialization</AssemblyReference>
    <AssemblyReference>System.ServiceModel</AssemblyReference>
    <AssemblyReference>System.ServiceModel.Activities</AssemblyReference>
    <AssemblyReference>System.Xaml</AssemblyReference>
    <AssemblyReference>System.Xml</AssemblyReference>
    <AssemblyReference>System.Xml.Linq</AssemblyReference>
    <AssemblyReference>mscorlib</AssemblyReference>
    <AssemblyReference>FleetAGC.Activities</AssemblyReference>
__TEMPLATE_ARG_XtraAssemblies  </sco:Collection>
</TextExpression.ReferencesForImplementation>
)XAML";
constexpr auto STD_XAML_TAIL = "</Activity>";

}
}


#endif

