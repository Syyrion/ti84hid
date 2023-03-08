#include <Keyboard.h>
#include <Mouse.h>

#define CLOCK 0
#define DATA 1

#define MAPS 4

enum eventtype : unsigned short
{
    KEYBOARD,
    MOUSE_BUTTON = 0x100,
    MOUSE_MOVE = 0x200,
    MOUSE_WHEEL = 0x300,

    SLOW = 0x400,
    META = 0x500,
    ONCE = 0x8000
};

enum direction : char
{
    DOWN,
    LEFT,
    RIGHT,
    UP,
    MIDDLE
};

/*
    * Key Reference Chart

    ! Warning !
    There are no diodes on the buttons.
    If two buttons in the same column are held at the same time and a third button
    is pressed on a different column but on the same row as one of the held buttons,
    the button on intersection between the row of the other held button and the
    column of the third pressed button will also be pressed.

    e.g. if 3 and 1 are held, pressing 7 will also press 9.

    IX    0      1      2      3       4    5     6      7     < BIT
    0  - [Down ][Left ][Right][Up    ][   ][    ][     ][     ]
    1  - [Enter][+    ][-    ][*     ][/  ][^   ][Clear][     ]
    2  - [(-)  ][3    ][6    ][9     ][)  ][Tan ][Vars ][     ]
    3  - [.    ][2    ][5    ][8     ][(  ][Cos ][Prgm ][Stat ]
    4  - [0    ][1    ][4    ][7     ][,  ][Sin ][Apps ][XTOn ]
    5  - [     ][Sto  ][Ln   ][Log   ][x^2][x^-1][Math ][Alpha]
    6  - [Graph][Trace][Zoom ][Window][Y= ][2nd ][Mode ][Del  ]

    Set to 0 to unmap a key.

    Modifier keys reference:
    https://www.arduino.cc/reference/en/language/functions/usb/keyboard/keyboardmodifiers/

    ! The META key should appear in all keymaps on the same key
*/

/*
    Keycode
    oeeeeeeekkkkkkkk

    o = once
    e = event type
    k = key/direction
*/

// Mouse keycodes
#define MOUSE_BUTTON_LEFT (MOUSE_BUTTON | MOUSE_LEFT)
#define MOUSE_BUTTON_RIGHT (MOUSE_BUTTON | MOUSE_RIGHT)
#define MOUSE_BUTTON_MIDDLE (MOUSE_BUTTON | MOUSE_MIDDLE)

#define MOUSE_MOVE_DOWN (MOUSE_MOVE | DOWN)
#define MOUSE_MOVE_LEFT (MOUSE_MOVE | LEFT)
#define MOUSE_MOVE_RIGHT (MOUSE_MOVE | RIGHT)
#define MOUSE_MOVE_UP (MOUSE_MOVE | UP)

#define MOUSE_WHEEL_UP (MOUSE_WHEEL | UP)
#define MOUSE_WHEEL_DOWN (MOUSE_WHEEL | DOWN)

const unsigned short keymap[MAPS][7][8] = {
    {
        // DOWN | LEFT | RIGHT | UP | <unused> | <unused> | <unused> | <unused>
        {MOUSE_MOVE_DOWN, MOUSE_MOVE_LEFT, MOUSE_MOVE_RIGHT, MOUSE_MOVE_UP, 0, 0, 0, 0},

        // ENTER | + | - | * | / | ^ | CLEAR | <unused>
        {META, KEY_F10, 0, 0, 0, MOUSE_WHEEL_DOWN | ONCE, MOUSE_WHEEL_UP | ONCE, 0},

        // (-) | 3 | 6 | 9 | ) | TAN | VARS | <unused>
        {0, '3' | ONCE, '6' | ONCE, '9' | ONCE, ' ', KEY_LEFT_CTRL, KEY_LEFT_SHIFT, 0},

        // . | 2 | 5 | 8 | ( | COS | PRGM | STAT
        {0, '2' | ONCE, '5' | ONCE, '8' | ONCE, KEY_RIGHT_ARROW, 0, 'z', 'd'},

        // 0 | 1 | 4 | 7 | , | SIN | APPS | XTOn
        {'0' | ONCE, '1' | ONCE, '4' | ONCE, '7' | ONCE, KEY_DOWN_ARROW, KEY_UP_ARROW, 'f', 's'},

        // <unused> | STO> | LN | LOG | x^2 | x^-1 | MATH | ALPHA
        {0, 0, 0, 0, KEY_LEFT_ARROW, SLOW, 'c', 'a'},

        // GRAPH | TRACE | ZOOM | WINDOW | Y= | 2ND | MODE | DEL
        {MOUSE_BUTTON_RIGHT, MOUSE_BUTTON_LEFT, KEY_F5 | ONCE, KEY_F3 | ONCE, KEY_ESC | ONCE, 'q', 'w', 'e'},
    },
    {
        // DOWN | LEFT | RIGHT | UP | <unused> | <unused> | <unused> | <unused>
        {0, 0, 0, 0, 0, 0, 0, 0},

        // ENTER | + | - | * | / | ^ | CLEAR | <unused>
        {META, 0, 0, 0, 0, 0, 0, 0},

        // (-) | 3 | 6 | 9 | ) | TAN | VARS | <unused>
        {0, 0, 0, 0, 0, 0, 0, 0},

        // . | 2 | 5 | 8 | ( | COS | PRGM | STAT
        {0, 0, 0, 0, 0, 0, 0, 0},

        // 0 | 1 | 4 | 7 | , | SIN | APPS | XTOn
        {0, 0, 0, 0, 0, 0, 0, 0},

        // <unused> | STO> | LN | LOG | x^2 | x^-1 | MATH | ALPHA
        {0, 0, 0, 0, 0, 0, 0, 0},

        // GRAPH | TRACE | ZOOM | WINDOW | Y= | 2ND | MODE | DEL
        {0, 0, 0, 0, 0, 0, 0, 0},
    },
    {
        // DOWN | LEFT | RIGHT | UP | <unused> | <unused> | <unused> | <unused>
        {0, 0, 0, 0, 0, 0, 0, 0},

        // ENTER | + | - | * | / | ^ | CLEAR | <unused>
        {META, 0, 0, 0, 0, 0, 0, 0},

        // (-) | 3 | 6 | 9 | ) | TAN | VARS | <unused>
        {0, 0, 0, 0, 0, 0, 0, 0},

        // . | 2 | 5 | 8 | ( | COS | PRGM | STAT
        {0, 0, 0, 0, 0, 0, 0, 0},

        // 0 | 1 | 4 | 7 | , | SIN | APPS | XTOn
        {0, 0, 0, 0, 0, 0, 0, 0},

        // <unused> | STO> | LN | LOG | x^2 | x^-1 | MATH | ALPHA
        {0, 0, 0, 0, 0, 0, 0, 0},

        // GRAPH | TRACE | ZOOM | WINDOW | Y= | 2ND | MODE | DEL
        {0, 0, 0, 0, 0, 0, 0, 0},
    },
    {
        // DOWN | LEFT | RIGHT | UP | <unused> | <unused> | <unused> | <unused>
        {0, 0, 0, 0, 0, 0, 0, 0},

        // ENTER | + | - | * | / | ^ | CLEAR | <unused>
        {META, 0, 0, 0, 0, 0, 0, 0},

        // (-) | 3 | 6 | 9 | ) | TAN | VARS | <unused>
        {0, 0, 0, 0, 0, 0, 0, 0},

        // . | 2 | 5 | 8 | ( | COS | PRGM | STAT
        {0, 0, 0, 0, 0, 0, 0, 0},

        // 0 | 1 | 4 | 7 | , | SIN | APPS | XTOn
        {0, 0, 0, 0, 0, 0, 0, 0},

        // <unused> | STO> | LN | LOG | x^2 | x^-1 | MATH | ALPHA
        {0, 0, 0, 0, 0, 0, 0, 0},

        // GRAPH | TRACE | ZOOM | WINDOW | Y= | 2ND | MODE | DEL
        {0, 0, 0, 0, 0, 0, 0, 0},
    },
};

char mapindex = 0;

enum metatype : char
{
    NONE,
    NEXT,
    PREV,
    SLOWER,
    FASTER,
};

const char metamap[7][8] = {
    // DOWN | LEFT | RIGHT | UP | <unused> | <unused> | <unused> | <unused>
    {SLOWER, PREV, NEXT, FASTER, 0, 0, 0, 0},

    // ENTER | + | - | * | / | ^ | CLEAR | <unused>
    {0, 0, 0, 0, 0, 0, 0, 0},

    // (-) | 3 | 6 | 9 | ) | TAN | VARS | <unused>
    {0, 0, 0, 0, 0, 0, 0, 0},

    // . | 2 | 5 | 8 | ( | COS | PRGM | STAT
    {0, 0, 0, 0, 0, 0, 0, 0},

    // 0 | 1 | 4 | 7 | , | SIN | APPS | XTOn
    {0, 0, 0, 0, 0, 0, 0, 0},

    // <unused> | STO> | LN | LOG | x^2 | x^-1 | MATH | ALPHA
    {0, 0, 0, 0, 0, 0, 0, 0},

    // GRAPH | TRACE | ZOOM | WINDOW | Y= | 2ND | MODE | DEL
    {0, 0, 0, 0, 0, 0, 0, 0},
};

/*
    Input
    Xiiiubbb

    X = don't care
    i = index
    u = up/down
    b = bit
*/

volatile bool available;
volatile char bit;
volatile char input;

void receive(void)
{
    if (available)
        return;
    input = (input << 1) | digitalRead(DATA);
    if (++bit == 8)
    {
        available = true;
        bit = 0;
    }
}

struct led
{
    const int TIME;
    int count;
    int timer;
    led(int t);
    void blink(int n);
    void update(int dt);
};

led::led(int t) : TIME(t) {}

void led::blink(int n)
{
    count = 2 * n;
    timer = 0;
    digitalWrite(LED_BUILTIN, 0);
}

void led::update(int dt)
{
    if (count == 0)
        return;

    timer -= dt;
    if (timer <= 0)
    {
        --count;
        timer += TIME;
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
}

struct led led(150);

struct repeating
{
    unsigned short dir[4];
    void release(void);
    void reset(void);
    char get(enum direction);
};

void repeating::release(void)
{
    for (int i = 0, j = sizeof(dir) / sizeof(*dir); i < j; ++i)
        if (dir[i] & ~1)
            dir[i] = 0;
}

void repeating::reset(void)
{
    memset(dir, 0, sizeof(dir));
}

char repeating::get(enum direction d)
{
    return dir[d] & 1;
}

struct repeating move;
struct repeating scroll;

char slow = 0;
char del = 0;
char meta = 0;

void reset(void)
{
    Keyboard.releaseAll();
    Mouse.release(MOUSE_ALL);
    move.reset();
    scroll.reset();
    available = bit = input = slow = 0;
}

bool metaevent(char index, char bit)
{
    char event = metamap[index][bit];
    if (!event)
        return false;
    reset();
    switch (event)
    {
    case PREV:
        if (--mapindex < 0)
            mapindex = 3;
        led.blink(mapindex + 1);
        break;
    case NEXT:
        if (++mapindex == MAPS)
            mapindex = 0;
        led.blink(mapindex + 1);
        break;
    case SLOWER:
        if (del < 2)
            ++del;
        led.blink(3 - del);
        break;
    case FASTER:
        if (del > 0)
            --del;
        led.blink(3 - del);
        break;
    }
    return true;
}

struct event
{
    unsigned short type;
    unsigned short once;
    char key;
    char down;
};

bool readinput(struct event &event)
{
    if (!available)
        return false;

    char index = input >> 4 & 7;
    char bit = input & 7;
    char down = input >> 3 & 1;

    if (meta && down && metaevent(index, bit))
        return false;

    unsigned short keycode = keymap[mapindex][index][bit];

    event.type = keycode & 0x7F00;
    event.key = keycode & 0xFF;

    event.once = keycode & ONCE;
    event.down = down;

    available = false;
    return true;
}

void setup(void)
{
    Mouse.begin();
    Keyboard.begin();
    attachInterrupt(digitalPinToInterrupt(CLOCK), receive, CHANGE);
    pinMode(DATA, INPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    led.blink(1);
    reset();
}

void loop(void)
{
    static unsigned long prevtime = 0;
    unsigned long currenttime = millis();
    int deltatime = currenttime - prevtime;
    prevtime = currenttime;

    led.update(deltatime);

    struct event event;
    if (readinput(event))
    {
        switch (event.type)
        {
        case KEYBOARD:
            if (event.down)
            {
                if (event.once)
                    Keyboard.write(event.key);
                else
                    Keyboard.press(event.key);
            }
            else
                Keyboard.release(event.key);
            break;
        case MOUSE_BUTTON:
            if (event.once && event.down)
                Mouse.click(event.key);
            else
            {
                if (event.down)
                    Mouse.press(event.key);
                else
                    Mouse.release(event.key);
            }
            break;
        case MOUSE_MOVE:
            move.dir[event.key] = event.once | event.down;
            break;
        case MOUSE_WHEEL:
            scroll.dir[event.key] = event.once | event.down;
            break;
        case SLOW:
            slow = event.down;
            break;
        case META:
            if (meta = event.down)
                reset();
            break;
        }
    }

    char dx = move.get(RIGHT) - move.get(LEFT);
    char dy = move.get(DOWN) - move.get(UP);
    char ds = scroll.get(UP) - scroll.get(DOWN);
    if (dx || dy || ds)
        Mouse.move(dx, dy, ds);
    move.release();
    scroll.release();

    delayMicroseconds(1500 * del + 3000 * slow);
}