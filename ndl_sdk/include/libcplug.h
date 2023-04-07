int printf(char *format, ... );
int puts(const char* str);
int scanf(const char *format, ...);
void *malloc(size_t size);
void free(void *ptr);
void scanKeys(void);
uint32 keysDown(void);
char *load_plugin(char *filename );
void call_plug_main(char *imageBytes);
