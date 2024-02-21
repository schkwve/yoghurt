#include <stdio.h>
#include <stdlib.h>

#include <yoghurt/yoghurt.h>
#include <mod/module.h>

int main(int argc, char **argv)
{
  char *custom_paths[2] = {
    "/Users/schkwve/yoghurt/build/modules",
    ""
};
  module_tryopen("core", custom_paths);
  return 0;
}
