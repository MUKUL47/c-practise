#include "../core/font_loader.h"
#include "entity.h"
void update_keystroke(char *k, char **destination);
FontInstance *new_font_instance(MyState *s, char *text_content, RGBA rgba);
TextPanel *new_text_panel_instance(MyState *s, int id, FontInstance *fi);
void free_text_panel(TextPanel *tp);
MyArray *split_text(char *text);
void normalize_panel_position(MyState *s, TextPanel *tp, FontInstance *fi,
                              TextPanel *last_panel, int width, int relative_x);
Square *get_active_quad(MyState *s);
void render_quad_links(SDL_Renderer *renderer, Square *active_quad);
int get_quad_on_active_cursor(MyState *s, int x, int y);
bool is_cursor_on_quad(int mouseX, int mouseY, SDL_Rect *quad);