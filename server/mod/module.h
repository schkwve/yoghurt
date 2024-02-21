#ifndef MOD_MODULE_H
#define MOD_MODULE_H

struct yoghurt_module {
  /* module handle */
  void *handle;

  /* module name */
  char *name;

  /* module version */
  char *version;

  /* module dependencies */
  char **dependencies;
};

struct yoghurt_module *module_tryopen(const char *name, const char **custom_paths);
void module_close(struct yoghurt_module *mod);

#endif /* MOD_MODULE_H */
