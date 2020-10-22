#include <cis-workflow-gen/quick-include.hpp>

int main() {
    #define DEFINE_ACTIVITY(name, entityName) \
        Activity name (#name, "FleetAGC.Activities.DelayActivity", entityName); \
        name.addInputSetting("Timespan", "60");
    
    DEFINE_ACTIVITY(GridManager, "M365Poc.SPO.GridManager")
    DEFINE_ACTIVITY(AuthAndProvision, "M365Poc.SPO.AuthProvisioning")
    DEFINE_ACTIVITY(FarmsAndTennats, "")
    DEFINE_ACTIVITY(CSC , "")
    DEFINE_ACTIVITY(FastSearch , "")
    DEFINE_ACTIVITY(Microservices, "M365Poc.SPO.MicroServices")
    DEFINE_ACTIVITY(MonitorAndAlert, "M365Poc.SPO.MonitoringAlerting")
    DEFINE_ACTIVITY(SPOTooling, "M365Poc.SPO.SPOTooling")
    DEFINE_ACTIVITY(UXCDNFailover, "M365Poc.SPO.UXCDNFailover")
    DEFINE_ACTIVITY(ODFBSyncClient, "M365Poc.SPO.ODFBSyncClient")
 
    auto block1 = GridManager >> FarmsAndTennats >> (CSC | FastSearch);
    auto block2 = Microservices | MonitorAndAlert;
    auto completeFlow = block1 | AuthAndProvision | block2;// | SPOTooling | UXCDNFailover | ODFBSyncClient;
 
    println(to_file("SPOBuildoutPOCWorkflow.xaml"), completeFlow.generateXaml("FleetAGC.Workflows.SPOBuildoutPOCWorkflow"));
}

