//
// Created by AswinM on 18/02/26.
//

#include "OliUI.h"

#define HASH_INITIAL 2166136261

uint32_t Hash(const void* data, int size) {
    uint32_t hash = HASH_INITIAL;
    const unsigned char* p = data;
    while (size--) {
        hash = (hash ^ *p++) * 16777619;
    }
    return hash;
}

void InsertElementIntoOliUIWindowContainerStack(
    OliUIMainContext* oliUIMainContext,
    OliUIWindowContainer oliUIWindowContainer
) {
    assert(oliUIMainContext->oliUIWindowContainerStack.index < MAX_OLIUI_WINDOW_CONTAINER_STACK_SIZE);
    oliUIMainContext->oliUIWindowContainerStack.oliUIWindowContainers[
        ++oliUIMainContext->oliUIWindowContainerStack.index
    ] = oliUIWindowContainer;
}

OliUIWindowContainer DeleteElementFromOliUIWindowContainerStack(
    OliUIMainContext* oliUIMainContext
) {
    assert(oliUIMainContext->oliUIWindowContainerStack.index >= 0);
    return oliUIMainContext->oliUIWindowContainerStack.oliUIWindowContainers[
        oliUIMainContext->oliUIWindowContainerStack.index--
    ];
}

OliUIWindowContainer GetTopElementFromOliUIWindowContainerStack(
    OliUIMainContext* oliUIMainContext
) {
    assert(
        oliUIMainContext->oliUIWindowContainerStack.index >= 0
        &&
        oliUIMainContext->oliUIWindowContainerStack.index < MAX_OLIUI_WINDOW_CONTAINER_STACK_SIZE
    );
    return oliUIMainContext->oliUIWindowContainerStack.oliUIWindowContainers[
        oliUIMainContext->oliUIWindowContainerStack.index
    ];
}

void InsertElementIntoOliUISubWindowContainerStack(
    OliUIMainContext* oliUIMainContext,
    OliUISubWindowContainer oliUISubWindowContainer
) {
    assert(oliUIMainContext->oliUISubWindowContainerStack.index < MAX_OLIUI_SUB_WINDOW_CONTAINER_STACK_SIZE);
    oliUIMainContext->oliUISubWindowContainerStack.oliUISubWindowContainers[
        ++oliUIMainContext->oliUISubWindowContainerStack.index
    ] = oliUISubWindowContainer;
}

OliUISubWindowContainer DeleteElementFromOliUISubWindowContainerStack(
    OliUIMainContext* oliUIMainContext
) {
    assert(oliUIMainContext->oliUISubWindowContainerStack.index >= 0);
    return oliUIMainContext->oliUISubWindowContainerStack.oliUISubWindowContainers[
        oliUIMainContext->oliUISubWindowContainerStack.index--
    ];
}

OliUISubWindowContainer GetTopElementFromOliUISubWindowContainerStack(
    OliUIMainContext* oliUIMainContext
) {
    assert(
        oliUIMainContext->oliUISubWindowContainerStack.index >= 0
        &&
        oliUIMainContext->oliUISubWindowContainerStack.index < MAX_OLIUI_SUB_WINDOW_CONTAINER_STACK_SIZE
    );
    return oliUIMainContext->oliUISubWindowContainerStack.oliUISubWindowContainers[
        oliUIMainContext->oliUISubWindowContainerStack.index
    ];
}

void InsertElementIntoOliUILayoutStack(
    OliUIMainContext* oliUIMainContext,
    OliUILayout oliUILayout
) {
    assert(oliUIMainContext->oliUILayoutStack.index < MAX_OLIUI_LAYOUT_STACK_SIZE);
    oliUIMainContext->oliUILayoutStack.oliUILayouts[
        ++oliUIMainContext->oliUILayoutStack.index
    ] = oliUILayout;
}

OliUILayout DeleteElementFromOliUILayoutStack(
    OliUIMainContext* oliUIMainContext
) {
    assert(oliUIMainContext->oliUILayoutStack.index >= 0);
    OliUILayout layout = oliUIMainContext->oliUILayoutStack.oliUILayouts[
        oliUIMainContext->oliUILayoutStack.index--
    ];
    return layout;
}

OliUILayout GetTopElementFromOliUILayoutStack(
    OliUIMainContext* oliUIMainContext
) {
    assert(
        oliUIMainContext->oliUILayoutStack.index >= 0
        &&
        oliUIMainContext->oliUILayoutStack.index < MAX_OLIUI_LAYOUT_STACK_SIZE
    );
    return oliUIMainContext->oliUILayoutStack.oliUILayouts[
        oliUIMainContext->oliUILayoutStack.index
    ];
}

void OliUIInit(OliUIMainContext* oliUIMainContext) {
    oliUIMainContext->activeItem = 0;
}

void OliUIBegin(OliUIMainContext* oliUIMainContext) {
    assert(oliUIMainContext != NULL);
    oliUIMainContext->hotItem = 0;
    oliUIMainContext->oliUIDrawCommandIndex = 0;
    oliUIMainContext->oliUILayoutStack.index = -1;
    oliUIMainContext->oliUIWindowContainerStack.index = -1;
    oliUIMainContext->oliUISubWindowContainerStack.index = -1;
}

void OliUIEnd(OliUIMainContext* oliUIMainContext) {
}

void OliUIBeginWindowContainer(OliUIMainContext* oliUIMainContext, OliUIRect rect) {
    assert(
        rect.x >= 0
        &&
        rect.y >= 0
        &&
        rect.w * rect.h >= 0
    );
    OliUIWindowContainer oliUIWindowContainer;
    oliUIWindowContainer.totalBodySize = rect;
    InsertElementIntoOliUIWindowContainerStack(oliUIMainContext, oliUIWindowContainer);
}

void OliUIEndWindowContainer(OliUIMainContext* oliUIMainContext) {
    DeleteElementFromOliUIWindowContainerStack(oliUIMainContext);
}

void OliUIBeginLayout(
    OliUIMainContext* oliUIMainContext,
    int widths[],
    int numWidths,
    OliUILayoutType oliUILayoutType
) {
    OliUILayout oliUILayout;
    memset(&oliUILayout, 0, sizeof(OliUILayout));
    oliUILayout.oliUILayoutType = oliUILayoutType;
    oliUILayout.widthIndex = 0;
    assert(numWidths < MAX_LAYOUT_WIDTH_SIZE);
    for (int i = 0; i < numWidths; i++) {
        oliUILayout.widths[i] = widths[i];
    }
    InsertElementIntoOliUILayoutStack(oliUIMainContext, oliUILayout);
}

OliUIRect GetNextLayoutRect(OliUIMainContext* oliUIMainContext) {
    assert(oliUIMainContext->oliUILayoutStack.index >= 0);
    assert(oliUIMainContext->oliUIWindowContainerStack.index >= 0);
    OliUILayout* layout =
        &oliUIMainContext->oliUILayoutStack.oliUILayouts[oliUIMainContext->oliUILayoutStack.index];
    assert(layout->widthIndex < MAX_LAYOUT_WIDTH_SIZE);

    OliUIRect parentRect;
    if (oliUIMainContext->oliUISubWindowContainerStack.index >= 0) {
        parentRect = GetTopElementFromOliUISubWindowContainerStack(oliUIMainContext).totalContentSize;
    }
    else {
        parentRect = GetTopElementFromOliUIWindowContainerStack(oliUIMainContext).totalBodySize;
    }

    int offset = 0;
    for (int i = 0; i < layout->widthIndex; i++) {
        offset += layout->widths[i];
    }

    OliUIRect rect;
    int currentSize = layout->widths[layout->widthIndex];

    if (layout->oliUILayoutType == OliUILayoutHorizontal) {
        rect.x = parentRect.x + offset;
        rect.y = parentRect.y;
        rect.w = currentSize;
        rect.h = parentRect.h;
    }
    else {
        rect.x = parentRect.x;
        rect.y = parentRect.y + offset;
        rect.w = parentRect.w;
        rect.h = currentSize;
    }

    layout->widthIndex++;

    return rect;
}

void OliUIEndLayout(OliUIMainContext* oliUIMainContext) {
    DeleteElementFromOliUILayoutStack(oliUIMainContext);
}

OliUIMouseClickState OliUIBeginSubWindowContainer(
    OliUIMainContext* oliUIMainContext,
    const void* name,
    int size) {
    OliUISubWindowContainer oliUISubWindowContainer;
    oliUISubWindowContainer.totalContentSize = GetNextLayoutRect(oliUIMainContext);
    oliUISubWindowContainer.name = name;
    oliUISubWindowContainer.size = size;
    InsertElementIntoOliUISubWindowContainerStack(oliUIMainContext, oliUISubWindowContainer);

    return GetMouseClickStateForASubWindow(
        oliUIMainContext,
        oliUISubWindowContainer.totalContentSize,
        Hash(name, size));
}
void OliUIEndSubWindowContainer(OliUIMainContext* oliUIMainContext) {
    DeleteElementFromOliUISubWindowContainerStack(oliUIMainContext);
}

bool OliUIMouseJustPressed(OliUIMainContext* oliUIMainContext, int button) {
    return
        oliUIMainContext->input.mouseButtons[button]
        &&
        !oliUIMainContext->input.mouseButtonsPrev[button];
}

bool OliUIMouseJustReleased(OliUIMainContext* oliUIMainContext, int button) {
    return
        !oliUIMainContext->input.mouseButtons[button]
        &&
        oliUIMainContext->input.mouseButtonsPrev[button];
}

bool OliUIMouseIsOver(OliUIMainContext* oliUIMainContext, OliUIRect rect) {
    return
        oliUIMainContext->input.mouseX >= rect.x
        &&
        oliUIMainContext->input.mouseX < rect.x + rect.w
        &&
        oliUIMainContext->input.mouseY >= rect.y
        &&
        oliUIMainContext->input.mouseY < rect.y + rect.h;
}

OliUIMouseClickState GetMouseClickStateForASubWindow(
    OliUIMainContext* oliUIMainContext,
    OliUIRect rect,
    uint32_t id
) {
    bool isHovered = OliUIMouseIsOver(oliUIMainContext, rect);

    // 1. Hover Logic
    if (isHovered) {
        oliUIMainContext->hotItem = id;
    }

    // 2. Interaction Logic
    if (oliUIMainContext->hotItem == id && OliUIMouseJustPressed(oliUIMainContext, 0)) {
        oliUIMainContext->activeItem = id;
    }

    OliUIMouseClickState state = OliUINone;
    if (isHovered) state |= OliUIOnHover;

    if (oliUIMainContext->activeItem == id) {
        state |= OliUIIsPressed;
        if (OliUIMouseJustReleased(oliUIMainContext, 0)) {
            if (isHovered) state |= OliUIJustClicked;
            oliUIMainContext->activeItem = 0; // Release focus
        }
    }

    return state;
}

void OliUIDrawColoredRectange(OliUIMainContext* oliUIMainContext, OliUIColor color) {
    assert(oliUIMainContext->oliUISubWindowContainerStack.index >= 0);
    assert(oliUIMainContext->oliUIDrawCommandIndex < MAX_OLIUI_DRAW_COMMANDS_ARRAY_SIZE);

    OliUIDrawCommand oliUIDrawCommand;
    oliUIDrawCommand.oliUIDrawCommandType = OliUIDrawCommandRect;
    oliUIDrawCommand.size = GetTopElementFromOliUISubWindowContainerStack(
        oliUIMainContext
    ).totalContentSize;
    oliUIDrawCommand.color = color;

    oliUIMainContext->oliUIDrawCommands[oliUIMainContext->oliUIDrawCommandIndex++] = oliUIDrawCommand;
}

void OliUIDrawColoredButton(
    OliUIMainContext* oliUIMainContext,
    OliUIColor color)
{
    assert(oliUIMainContext->oliUISubWindowContainerStack.index >= 0);
    assert(oliUIMainContext->oliUIDrawCommandIndex < MAX_OLIUI_DRAW_COMMANDS_ARRAY_SIZE);

    OliUIRect rect =
        GetTopElementFromOliUISubWindowContainerStack(oliUIMainContext).totalContentSize;

    // ---------- Draw ----------
    OliUIDrawCommand cmd;
    cmd.oliUIDrawCommandType = OliUIDrawCommandRect;
    cmd.size = rect;
    cmd.color = color;

    oliUIMainContext->oliUIDrawCommands[oliUIMainContext->oliUIDrawCommandIndex++] = cmd;
}

void OliUIDrawTexturedRectangle(OliUIMainContext* oliUIMainContext, OliUITexture texture){
    assert(oliUIMainContext->oliUISubWindowContainerStack.index >= 0);
    assert(oliUIMainContext->oliUIDrawCommandIndex < MAX_OLIUI_DRAW_COMMANDS_ARRAY_SIZE);

    OliUIRect rect =
        GetTopElementFromOliUISubWindowContainerStack(oliUIMainContext).totalContentSize;

    // ---------- Draw ----------
    OliUIDrawCommand cmd;
    cmd.oliUIDrawCommandType = OliUIDrawCommandTexturedRect;
    cmd.size = rect;
    cmd.texture = texture;

    oliUIMainContext->oliUIDrawCommands[oliUIMainContext->oliUIDrawCommandIndex++] = cmd;
}
