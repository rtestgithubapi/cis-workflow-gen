#include <cis-workflow-gen/quick-include.hpp>

int main() {
#define DEFINE_ACTIVITY(name) \
    Activity name(#name, "FleetAGC.Activities.DelayActivity", "M365Poc.PortalAdminCenterUseageReports." #name)

    DEFINE_ACTIVITY(AdminCenterApp);
    DEFINE_ACTIVITY(OfficeHome);
    DEFINE_ACTIVITY(ShellServices);
    DEFINE_ACTIVITY(PortalUseageReports);
    DEFINE_ACTIVITY(SvcHealthDashboard);
    DEFINE_ACTIVITY(CustomerLockboxUX);

    auto flow = AdminCenterApp | OfficeHome | ShellServices | PortalUseageReports | SvcHealthDashboard | CustomerLockboxUX;
    println(to_file("/home/recolic/code/Azure-Deployment-Builder/src/Microsoft/Azure/Workflows/M365FleetAGC/Workflows/E2EPOCBuildPortalWorkflow.xaml"), flow.generateXaml("FleetAGC.Workflows.E2EPOCBuildPortalWorkflow"));
}
