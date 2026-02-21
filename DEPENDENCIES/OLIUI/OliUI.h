//
// Created by AswinM on 18/02/26.
//

#ifndef OLIUI_OLIUI_H
#define OLIUI_OLIUI_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef OLIUI_EXPORTS
#define OLIUI_API __declspec(dllexport)
#else
#define OLIUI_API __declspec(dllimport)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

#define MAX_OLIUI_WINDOW_CONTAINER_STACK_SIZE 100
#define MAX_OLIUI_SUB_WINDOW_CONTAINER_STACK_SIZE 100
#define MAX_OLIUI_LAYOUT_STACK_SIZE 100
#define MAX_LAYOUT_WIDTH_SIZE 10
#define MAX_OLIUI_DRAW_COMMANDS_ARRAY_SIZE 100

typedef struct OliUIRect {
    int x; //starting x-coordinate
    int y; //starting y-coordinate
    int w; //width
    int h; //height
}OliUIRect;

typedef struct OliUIColor {
    uint8_t r, g, b, a;
}OliUIColor;

typedef struct OliUITexture{
    const char* textureName;
}OliUITexture;

typedef struct OliUIWindowContainer {
    OliUIRect totalBodySize;
} OliUIWindowContainer;

typedef struct OliUISubWindowContainer {
    OliUIRect totalContentSize;
    const void* name; // For unique id generation.
    int size;
} OliUISubWindowContainer;

typedef enum OliUILayoutType {
    OliUILayoutHorizontal = 0,
    OliUILayoutVertical = 1
} OliUILayoutType;

typedef struct OliUILayout {
    OliUILayoutType oliUILayoutType;
    int widths[MAX_LAYOUT_WIDTH_SIZE];// If any of the widths value is zero, then we consider it as end.
    int widthIndex;
} OliUILayout;

typedef enum {
    OliUIDrawCommandRect = 0,
    OliUIDrawCommandTexturedRect = 1
}OliUIDrawCommandType;

typedef struct OliUIDrawCommand {
    OliUIDrawCommandType oliUIDrawCommandType;
    OliUIRect size;
    OliUIColor color;
    OliUITexture texture;
}OliUIDrawCommand;

typedef struct OliUIWindowContainerStack {
    int index;
    OliUIWindowContainer oliUIWindowContainers[MAX_OLIUI_WINDOW_CONTAINER_STACK_SIZE];
} OliUIWindowContainerStack;

typedef struct OliUISubWindowContainerStack {
    int index;
    OliUISubWindowContainer oliUISubWindowContainers[MAX_OLIUI_SUB_WINDOW_CONTAINER_STACK_SIZE];
} OliUISubWindowContainerStack;

typedef struct OliUILayoutStack {
    int index;
    OliUILayout oliUILayouts[MAX_OLIUI_LAYOUT_STACK_SIZE];
}OliUILayoutStack;

typedef enum OliUIMouseClickState {
    OliUINone = 0,
    OliUIJustClicked = 1 << 0,
    OliUIIsPressed = 1 << 1,
    OliUIJustReleased = 1 << 2,
    OliUIOnHover = 1 << 3
} OliUIMouseClickState;

typedef struct OliUIInput {
    // Mouse State
    double mouseX, mouseY;
    int mouseScrollDelta;
    bool mouseButtons[3];      // 0: Left, 1: Right, 2: Middle
    bool mouseButtonsPrev[3];

    // Keyboard State
    bool keys[512];            // Current frame
    bool keysPrev[512];        // Previous frame

    // Character Input (for text boxes)
    int textInputLength;
    unsigned int textInput[32]; // Unicode codepoints
} OliUIInput;

typedef struct OliUIMainContext {
    OliUIWindowContainerStack oliUIWindowContainerStack;
    OliUISubWindowContainerStack oliUISubWindowContainerStack;
    OliUILayoutStack oliUILayoutStack;
    OliUIDrawCommand oliUIDrawCommands[MAX_OLIUI_DRAW_COMMANDS_ARRAY_SIZE];
    int oliUIDrawCommandIndex;

    OliUIInput input;

    uint32_t hotItem;
    uint32_t activeItem;
    uint32_t keyboardFocusItem;
}OliUIMainContext;

void InsertElementIntoOliUIWindowContainerStack(
    OliUIMainContext* oliUIMainContext,
    OliUIWindowContainer oliUIWindowContainer
);

OliUIWindowContainer DeleteElementFromOliUIWindowContainerStack(
    OliUIMainContext* oliUIMainContext
);

OliUIWindowContainer GetTopElementFromOliUIWindowContainerStack(
    OliUIMainContext* oliUIMainContext
);

void InsertElementIntoOliUISubWindowContainerStack(
    OliUIMainContext* oliUIMainContext,
    OliUISubWindowContainer oliUISubWindowContainer
);

OliUISubWindowContainer DeleteElementFromOliUISubWindowContainerStack(
    OliUIMainContext* oliUIMainContext
);

OliUISubWindowContainer GetTopElementFromOliUISubWindowContainerStack(
    OliUIMainContext* oliUIMainContext
);

void InsertElementIntoOliUILayoutStack(
    OliUIMainContext* oliUIMainContext,
    OliUILayout oliUILayout
);

OliUILayout DeleteElementFromOliUILayoutStack(
    OliUIMainContext* oliUIMainContext
);

OliUILayout GetTopElementFromOliUILayoutStack(
    OliUIMainContext* oliUIMainContext
);

OLIUI_API void OliUIInit(OliUIMainContext* oliUIMainContext);

OLIUI_API void OliUIBegin(OliUIMainContext* oliUIMainContext);
OLIUI_API void OliUIEnd(OliUIMainContext* oliUIMainContext);

OLIUI_API void OliUIBeginWindowContainer(OliUIMainContext* oliUIMainContext, OliUIRect rect);
OLIUI_API void OliUIEndWindowContainer(OliUIMainContext* oliUIMainContext);

OLIUI_API void OliUIBeginLayout(
    OliUIMainContext* oliUIMainContext,
    int widths[],
    int numWidths,
    OliUILayoutType oliUILayoutType
);
OLIUI_API OliUIRect GetNextLayoutRect(OliUIMainContext* oliUIMainContext);
OLIUI_API void OliUIEndLayout(OliUIMainContext* oliUIMainContext);

OLIUI_API OliUIMouseClickState OliUIBeginSubWindowContainer(
    OliUIMainContext* oliUIMainContext,
    const void* name,
    int size);
OLIUI_API void OliUIEndSubWindowContainer(OliUIMainContext* oliUIMainContext);

uint32_t Hash(const void* data, int size);

OLIUI_API bool OliUIMouseJustPressed(OliUIMainContext* ctx, int button);
OLIUI_API bool OliUIMouseJustReleased(OliUIMainContext* ctx, int button);
OLIUI_API bool OliUIMouseIsOver(OliUIMainContext* ctx, OliUIRect rect);
OLIUI_API OliUIMouseClickState GetMouseClickStateForASubWindow(
    OliUIMainContext* oliUIMainContext,
    OliUIRect rect,
    uint32_t id);

OLIUI_API void OliUIDrawColoredRectange(OliUIMainContext* oliUIMainContext, OliUIColor color);

OLIUI_API void OliUIDrawColoredButton(OliUIMainContext* oliUIMainContext, OliUIColor color);

OLIUI_API void OliUIDrawTexturedRectangle(OliUIMainContext* oliUIMainContext, OliUITexture texture);

#ifdef __cplusplus
}
#endif

#endif //OLIUI_OLIUI_H