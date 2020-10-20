#include <cis-workflow-gen/quick-include.hpp>

int main() {
    Activity SCS ("SCS", "FleetAGC.Activities.WriteTraceinfo", "PreRteg.InitiateBareMetalComplete");
    SCS.addInputSetting("Message", "Doing SCS");
    Activity SearchFarms ("Search Farms", "FleetAGC.Activities.WriteTraceinfo");
    SearchFarms.addInputSetting("Message", "fuck");
    SearchFarms.addInputSetting("AnotherMessage", "shit");

    rlib::println(((SCS >> SearchFarms) | SCS).generateXaml());
}

/*
Activity SearchAnalytics (......);
SearchAnalytics.AddArgument(......);
......;

// `->` means sequence, and `|` means parallel. 
auto block1 = SCS -> (SearchAnalytics | (SearchFarms -> (ClassisSearchUX | ModernSearch)));
auto block3 = Loki -> Yggdrasil -> OfficeGraph;
auto block4 = IC3Tooling -> (MonitoringSetup | (MicroServices -> DevelopmentValidation -> IntegrationTesting))
// TODO: How to represent controlled-if? 

(block1 | 3SConfigAndInterop | block3 | block4).GenerateXaml("/home/recolic/code/Azure/myWorkflow.xaml");


*/