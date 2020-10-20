#include <cis-workflow-gen/activity.hpp>
#include <cis-workflow-gen/quick-include.hpp>

int main() {

#define DEFINE_ACTIVITY(name) \
    Activity name(#name, "FleetAGC.Activities.DelayActivity", "PortalAdminCenterUseageReports." #name)

    DEFINE_ACTIVITY(AdminCenterApp);
    DEFINE_ACTIVITY(OfficeHome);
    DEFINE_ACTIVITY(ShellServices);
    DEFINE_ACTIVITY(PortalUseageReports);
    DEFINE_ACTIVITY(SvcHealthDashboard);
    DEFINE_ACTIVITY(CustomerLockboxUX);

    auto flow = AdminCenterApp | OfficeHome | ShellServices | PortalUseageReports | PortalUseageReports | SvcHealthDashboard | CustomerLockboxUX;
    println(to_file("BuildPortal.xaml"), flow.generateXaml("FleetAGC.Workflows.BuildPortal"));
}
