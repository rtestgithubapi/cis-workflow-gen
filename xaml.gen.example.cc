
// Define some handy c# types. 
auto dict = 'System.Collections.Generic.Dictionary<string, string>';

// Define activities. 
Activity SCS (ClassName='...', EntityName='...');
// SCS.AddRawArgument(Type=dict, CSharpCode='new Dictionary() {{"Message", "Doing SCS"}}');
SCS.AddInputSetting("Message", "Doing SCS");
Activity SearchAnalytics (......);
SearchAnalytics.AddArgument(......);
......;

// `->` means sequence, and `|` means parallel. 
auto block1 = SCS -> (SearchAnalytics | (SearchFarms -> (ClassisSearchUX | ModernSearch)));
auto block3 = Loki -> Yggdrasil -> OfficeGraph;
auto block4 = IC3Tooling -> (MonitoringSetup | (MicroServices -> DevelopmentValidation -> IntegrationTesting))
// TODO: How to represent controlled-if? 

(block1 | 3SConfigAndInterop | block3 | block4).GenerateXaml("/home/recolic/code/Azure/myWorkflow.xaml");


