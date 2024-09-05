#include "../core/font_loader.h"
#include "entity.h"
void update_keystroke(char *k, char **destination);
FontInstance *new_font_instance(MyState *s, char *text_content, RGBA rgba);
TextPanel *new_text_panel_instance(MyState *s, int id, FontInstance *fi);
void free_text_panel(TextPanel *tp);