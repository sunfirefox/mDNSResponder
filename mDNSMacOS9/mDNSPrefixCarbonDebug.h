// Global options for the Mac OS Test Responder target.// Symbols defined here are available within all source files, like symbols// defined globally for a project using "-d SYMBOL=VALUE" in Unix Makefiles// For normal DeferredTask time execution, set __ONLYSYSTEMTASK__ to 0// For easier debugging, set __ONLYSYSTEMTASK__ to 1, and OT Notifier executions// will be deferred until SystemTask time#define TARGET_API_MAC_CARBON 1#define OTCARBONAPPLICATION 1#define __ONLYSYSTEMTASK__ 1#define MDNS_DEBUGMSGS 1