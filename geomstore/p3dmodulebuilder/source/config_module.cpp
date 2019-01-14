
#include "config_module.h"

/*

Include all your dynamically typed classes here, e.g.
#include "my_dynamic_class.h"

*/
#include "dconfig.h"

Configure(config_geomstore);
NotifyCategoryDef(geomstore , "");

ConfigureFn(config_geomstore) {
  init_libgeomstore();
}

void
init_libgeomstore() {
  static bool initialized = false;
  if (initialized) {
    return;
  }
  initialized = true;
  // Init your dynamic types here, e.g.:
  // MyDynamicClass::init_type();

  return;
}

