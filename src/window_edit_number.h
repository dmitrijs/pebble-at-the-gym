typedef void (*EditNumberCallback)(int key, int number);

void show_window_edit_number(int key, int number, EditNumberCallback);
void hide_window_edit_number(void);
