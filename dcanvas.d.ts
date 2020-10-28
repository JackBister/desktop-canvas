
declare var console: Console;
declare var global: Global;

declare class Audio {
    constructor(src: string);

    pause(): void;
    play(): void;
}

declare interface Canvas {
    readonly width: number;
    readonly height: number;

    getContext(type: string): CanvasRenderingContext2D;
}

declare class OffscreenCanvas implements Canvas {
    constructor(width: number, height: number);
    readonly width: number;
    readonly height: number;

    getContext(type: string): CanvasRenderingContext2D;
}

declare interface CanvasRenderingContext2D {
    drawImage(image: ImageBitmap | OffscreenCanvas, dx: number, dy: number): void;
    drawImage(image: ImageBitmap | OffscreenCanvas, dx: number, dy: number, dWidth: number, dHeight: number): void;
    drawImage(image: ImageBitmap | OffscreenCanvas, sx: number, sy: number, sWidth: number, sHeight: number, dx: number, dy: number, dWidth: number, dHeight: number): void;

    fillRect(x: number, y: number, width: number, height: number): void;

    fillText(text: string, x: number, y: number): void;

    resetTransform(): void;
    rotate(angle: number): void;

    readonly canvas: Canvas;

    fillStyle: string;
    // font must be of the following form: '<n>pt <font>' where <n> is a number and <font> is the location of a TTF file relative to the js bundle
    font: string
}

declare interface Console {
    log(s: any);
}

declare interface Gamepad {
    readonly axes: number[]; // For Xbox pad: [ LX, LY, RX, RY ]
    readonly buttons: GamepadButton[]; // For Xbox pad: [A, B, X, Y, LB, RB, LT, RT, Select, Start, L3, R3, D-Up, D-Down, D-Left, D-Right]
    readonly id: string;
}

declare interface GamepadButton {
    pressed: boolean;
    value: number;
}

declare interface EventListenerTypes {
    'keydown': KeyboardEvent;
    'keyup': KeyboardEvent;

    'mousedown': MouseEvent;

    'touchstart': TouchEvent;
    'touchend': TouchEvent;
}

declare interface Global {
    init: (cv: CanvasRenderingContext2D) => void;
    loadState: (state) => void;
    stashState: () => any;
    tick: () => void;

    loadBitmap(url: string, onLoad: (bitmap: ImageBitmap) => void);

    addEventListener<K extends keyof EventListenerTypes>(eventName: K, handler: (evt: EventListenerTypes[K]) => void): void;
    removeEventListener<K extends keyof EventListenerTypes>(eventName: K, handler: (evt: EventListenerTypes[K]) => void): void;

    readonly navigator: Navigator;
}

declare interface ImageBitmap {
    readonly height: number;
    readonly width: number;

    close();
}

declare interface KeyboardEvent {
    readonly altKey: boolean;
    readonly code: string;
    readonly ctrlKey: boolean;
    readonly metaKey: boolean;
    readonly repeat: boolean;
    readonly shiftKey: boolean;
}

declare interface MessageEvent {
    data: string;
}

declare interface MouseEvent {
    readonly button: number;
    readonly clientX: number;
    readonly clientY: number;
}

declare interface Navigator {
    getGamepads(): Gamepad[];
}

declare interface ChangedTouch {
    readonly clientX: number;
    readonly clientY: number;
    readonly force: number;
}

declare interface TouchEvent {
    readonly type: 'touchstart' | 'touchend';
    readonly changedTouches: ChangedTouch[];
}

declare class WebSocket {
    constructor(url: string);

    readonly readyState: number;

    onmessage: (message: MessageEvent) => void;
    
    close();
    send(data: string);
}
