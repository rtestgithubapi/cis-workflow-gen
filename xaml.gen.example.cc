#include <cis-workflow-gen/quick-include.hpp>

auto simpleExample() {
    Activity SCS ("SCS", "FleetAGC.Activities.WriteTraceinfo", "PreRteg.InitiateBareMetalComplete");
    SCS.addInputSetting("Message", "Doing SCS");
    Activity SearchFarms ("Search Farms", "FleetAGC.Activities.WriteTraceinfo");
    SearchFarms.addInputSetting("Message", "fuck");
    SearchFarms.addInputSetting("AnotherMessage", "shit");

    println(((SCS >> SearchFarms) | SCS).generateXaml());
}

auto complexExample() {
    auto className = "FleetAGC.Activities.WriteTraceinfo";
    #define DEFINE_ACTIVITY(name, entityName) \
        Activity name (#name, className, entityName); \
        name.addInputSetting("Message", "Doing " #name);
    
    DEFINE_ACTIVITY(SCS, "PreRteg.InitiateBareMetalComplete")
    DEFINE_ACTIVITY(SearchAnalytics, "")
    DEFINE_ACTIVITY(SearchFarms , "")
    DEFINE_ACTIVITY(ClassisSearchUX , "")
    DEFINE_ACTIVITY(ModernSearch, "")
    DEFINE_ACTIVITY(Loki, "PreRteg.InitiateBareMetalComplete")
    DEFINE_ACTIVITY(Yggdrasil, "")
    DEFINE_ACTIVITY(OfficeGraph, "")
    DEFINE_ACTIVITY(IC3Tooling, "PreRteg.InitiateBareMetalComplete")
    DEFINE_ACTIVITY(MonitoringSetup, "")
    DEFINE_ACTIVITY(MicroServices, "")
    DEFINE_ACTIVITY(DevelopmentValidation, "")
    DEFINE_ACTIVITY(IntegrationTesting, "")
    DEFINE_ACTIVITY(TSConfigAndInterop, "PreRteg.InitiateBareMetalComplete")

    auto block1 = SCS >> (SearchAnalytics | (SearchFarms >> (ClassisSearchUX | ModernSearch)));
    auto block3 = Loki >> Yggdrasil >> OfficeGraph;
    auto block4 = IC3Tooling >> (MonitoringSetup | (MicroServices >> DevelopmentValidation >> IntegrationTesting));
    auto completeFlow = block1 | TSConfigAndInterop | block3 | block4;

    auto myMetadata = DefaultMetadata().setXtraAssemblies({"FleetAGC.Workflows"});
    println(to_file("BuildTeams.xaml"), completeFlow.generateXaml(myMetadata));
}

int main() {
    // simpleExample();
    complexExample();
}

/*
// `>>` means sequence, and `|` means parallel. 
auto block1 = SCS >> (SearchAnalytics | (SearchFarms >> (ClassisSearchUX | ModernSearch)));
auto block3 = Loki >> Yggdrasil >> OfficeGraph;
auto block4 = IC3Tooling >> (MonitoringSetup | (MicroServices >> DevelopmentValidation >> IntegrationTesting))



*/