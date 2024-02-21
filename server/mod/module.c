#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <dlfcn.h>
#include <string.h>
#include <errno.h>

#include <cjson/cJSON.h>
#include <yoghurt/yoghurt.h>

#include <mod/module.h>

static const char *module_paths[] = {
  "/usr/local/yoghurt/modules",
  "/usr/yoghurt/modules",
  "/usr/yoghurt"
};

/**
 * @brief Tries to open a module with the specified name.
 *
 * @param name  Simplified module name. Loading yoghurt_modcore.ymod
 *              would require this parameter to be just "core".
 */
struct yoghurt_module *module_tryopen(const char *name, const char **custom_paths)
{
  char *module_path = "<none>";
  int8_t found = 0;

  // is the module present?
  for (int i = 0; i < ARRAY_LENGTH(module_paths); i++) {
    char *path_tmp;
    sprintf(path_tmp, "%s/%s", module_paths[i], name);

    DIR *dir = opendir(path_tmp);
    if (dir) {
      //log_trace("Found module %s in %s\n", name, module_paths[i]);
      module_path = path_tmp;
      found = 1;
      closedir(dir);
      break;
    }
  }

  // if the module wasn't found, are there any custom paths to scan?
  if (found == 0) {
    if (strncmp(custom_paths[0], "<none>", strlen("<none>")) == 0) {
      printf("Module '%s' was not found!\n", name);
      return NULL;
    }

    // try scanning custom paths
    for (int i = 0; i < ARRAY_LENGTH(custom_paths); i++) {
      char *path_tmp;
      sprintf(path_tmp, "%s/%s", custom_paths[i], name);

      DIR *dir = opendir(path_tmp);
      if (dir) {
        //log_trace("Found module %s in %s\n", name, module_paths[i]);
        module_path = path_tmp;
        found = 1;
        closedir(dir);
        break;
      }
    }
  }

  if (found == 0) {
    printf("Module '%s' wasn't found!\n", name);
    return NULL;
  }

  // try to read manifest.json
  char *manifest_filepath;
  sprintf(manifest_filepath, "%s/manifest.json", module_path);
  FILE *manifest_file = fopen(manifest_filepath, "r");
  if (manifest_file == NULL) {
    printf("Failed to open manifest.json for module '%s': %s\n", name, strerror(errno));
    return NULL;
  }

  size_t manifest_size = 0;
  fseek(manifest_file, 0L, SEEK_END);
  manifest_size = ftell(manifest_file);
  rewind(manifest_file);

  if (manifest_size == 0) {
    printf("File manifest.json for module '%s' is empty!\n", name);
    return NULL;
  }

  char *manifest_content = malloc(manifest_size);

  size_t manifest_read_size = fread(manifest_content, 1, manifest_size, manifest_file);
  if (manifest_read_size != manifest_size) {
    printf("fread() read %zu bytes (instead of expected %zu bytes) for manifest.json of module '%s'.\n", manifest_read_size, manifest_size, name);

    free(manifest_content);
    fclose(manifest_file);
    return NULL;
  }

  fclose(manifest_file);

  // parse manifest.json
  cJSON *manifest = cJSON_Parse(manifest_content);

  free(manifest_content);

  // compose a dependency list
  char **dependency_list = {"<none>"};

  struct yoghurt_module *new = malloc(sizeof(struct yoghurt_module));
  if (new == NULL) {
    printf("Failed to allocate memory for module '%s'!\n", name);
    return NULL;
  }

  new->name = name;
  new->dependencies = dependency_list;
  return new;
}
