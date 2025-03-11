#define MAX_KEYS = 256;

// TODO: map these to the correct platform-specific key codes within platform
// rather than hardcoding them here
typedef enum {
    INPUT_KEY_UP = 0x26,
    INPUT_KEY_DOWN = 0x28,
    INPUT_KEY_LEFT = 0x27,
    INPUT_KEY_RIGHT = 0x25,
    INPUT_KEY_CENTER = 0xC
} KeyMap;

bool get_key_state(KeyMap key);
void input_init();
